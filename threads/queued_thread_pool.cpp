// Simple thread pool with consume() method scheduling a template-specified
// functor in a separate thread. Destructor joins all scheduler thread

#include <iostream>
#include <thread>
#include <functional>
#include <vector>
#include <deque>
#include <mutex>
#include <chrono>
#include <variant>

using std::cout;
using std::endl;
using std::string;
using std::vector;

std::mutex mut;

enum EThreadStatus {
    PENDING,
    RUNNING,
    FINISHED
};

struct ThreadItem {
    ThreadItem(std::function<void(size_t)> threadLambda, EThreadStatus status, size_t index)
            : m_status(status), m_index(index), m_process(std::move(threadLambda)) {
    }

    ThreadItem(ThreadItem&& another) noexcept {
        m_status = another.m_status;
        m_index = another.m_index;
        m_worker = std::move(another.m_worker);
        m_process = another.m_process;
    }

    ThreadItem& operator = (ThreadItem&& another)  noexcept {
        m_status = another.m_status;
        m_index = another.m_index;
        m_worker = std::move(another.m_worker);
        m_process = another.m_process;
        return *this;
    }

    void run() {
        m_worker = std::thread(m_process, m_index);
    }

    EThreadStatus m_status {PENDING};
    size_t m_index;
    std::thread m_worker;
    std::function<void(size_t)> m_process;
};



class QueuedThreadPool {
public:
    explicit QueuedThreadPool(size_t size) : m_size(size) {}

    virtual ~QueuedThreadPool() {
        join_all();
    }

    void join_all() {
        cleanupScheduler();
        for(auto & threadItem : m_threads) {
            if (threadItem.m_worker.joinable())
                threadItem.m_worker.join();
        }
    }

    /** Runs in a separate thread to check whether any task finished to schedule a new one
     * we previously stashed into queue (if any). Loops with specified frequency time
     * */
    void scheduleQueue() {
        m_schedulerRunning = true;
        if (m_taskQueue.empty()) {
            m_schedulerRunning = false;
            return;
        }

        bool running = true;
        while(running) {
            for(int i = 0; i < m_threads.size(); i++) {
                if (m_taskQueue.empty()) {
                    running = false;
                    break;
                }

                if (m_threads[i].m_status == FINISHED) {
                    if (!m_threads[i].m_worker.joinable())
                        continue;
                    m_threads[i].m_worker.join();

                    // moving ownership from queue to vector of tasks and deleting empty entry in queue
                    m_threads[i] = std::move(m_taskQueue.front());
                    m_taskQueue.pop_front();

                    // assign correct index to ThreadItem
                    m_threads[i].m_index = i;

                    // start thread
                    m_threads[i].run();
                }

            }
            std::this_thread::sleep_for(std::chrono::milliseconds(m_schedulerUpdateFreqMs));
        }

        m_schedulerRunning = false;
    }


    /** Runs a worker with specified template arguments (functor) and out value
     * */
    // todo: надо подумать куда записывать результаты, сейчас просто принтятся
    template <class Out, class ...Args>
    void run(size_t index, std::function<Out(Args... args)> functor, Args... args) {
        m_threads[index].m_status = RUNNING;

        Out result = functor(std::forward<Args>(args)...);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        cout << "result = " << result << " | thread id = " << std::this_thread::get_id() << endl;

        m_threads[index].m_status = FINISHED;
    }


    /** Adds a task to worker and either runs it (if any free worker persists)
     * or stashes into queue to retrieve and
     * run later when any worker finished its previous task */
    template <class Out, class ...Args>
    void consume(std::function<Out(Args... args)> functor, Args... args) {
        if (m_threads.size() < m_size) {
            size_t index = m_threads.size();

            auto& inserted_element = m_threads.emplace_back([=, this, functor = functor](size_t index) {
                run(index, functor, args...);
            }, PENDING, index);

            inserted_element.run();
            return;
        }


        // if some of the workers already finished we can reassign a task
        for (size_t i = 0; i < m_threads.size(); i++) {
            // once we found finished thread we create another one and finish the loop
            if (m_threads[i].m_status == FINISHED) {
                if (!m_threads[i].m_worker.joinable())
                    continue;

                // lock to prevent other threads schedule at the same index
                // (in case .consume() is called from different threads)
                m_mutex.lock();
                m_threads[i].m_worker.join();
                m_threads[i] = std::move(
                        ThreadItem(
                                [=, this, functor = functor](size_t index) {
                                    run(i, functor, args...);
                                },
                                PENDING,
                                i)
                );
                m_threads[i].run();
                m_mutex.unlock();
                return;
            }
        }

        // if no process was marked as FINISHED, we stash action into queue
        m_taskQueue.emplace_back([=, this, functor = functor](size_t index) {
            run(index, functor, args...);
        }, PENDING, -1);


        // Start scheduler to schedule queued task when any of the running tasks was finished
        if (!m_schedulerRunning) {
            cleanupScheduler();
            m_schedulerThread = new std::thread(&QueuedThreadPool::scheduleQueue, this);
        }

    }

    void cleanupScheduler() {
        if (m_schedulerThread && m_schedulerThread->joinable()) {
            m_schedulerThread->join();
            delete m_schedulerThread;
            m_schedulerThread = nullptr;
        }
    }

    /** Changes scheduler's frequency for polling workers - if any finished or still running */
    void setSchedulerUpdateFreqMs(int ms) {
        m_schedulerUpdateFreqMs = ms < 0 ? 0 : ms;
        m_schedulerUpdateFreqMs = ms > 10000 ? 10000 : ms;
    }


protected:
    size_t m_size {0};
    std::vector<ThreadItem> m_threads;
    std::deque<ThreadItem> m_taskQueue;
    std::thread* m_schedulerThread {nullptr};
    bool m_schedulerRunning {false};
    int m_schedulerUpdateFreqMs {250};
    std::mutex m_mutex;
};


int foo(int a) {
    cout << "foo(" << a << ")\n";
    return a * 2;
}

float bar(float a, float c) {
    cout << "bar(" << a << ", " << c << ")\n";
    return a / c;
}

int main() {

    // Create a thread pool with max 3 workers and schedule 10 different tasks
    // which will be processed by batches of 3
    QueuedThreadPool queuedThreadPool(5);
    for (int i = 0; i < 10; i++) {
        queuedThreadPool.consume<int, int>(foo, i);
        queuedThreadPool.consume<float, float, float>(bar, (i + 1) * 1.1f, (i + 1) * 0.8f);
    }

    return 0;
}