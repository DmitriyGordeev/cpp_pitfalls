// TODO: добавить коменты

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

enum EThreadStatus {
    PENDING,
    RUNNING,
    FINISHED
};

struct ThreadItem {
//    ThreadItem(const std::function<void(int)>& threadLambda, EThreadStatus status, int index)
//        : m_status(status), m_index(index) {
//        m_worker = std::thread(threadLambda, index);
//    }

    ThreadItem(const std::function<void(int)>& threadLambda, EThreadStatus status, int index)
        : m_status(status), m_index(index), m_process(threadLambda) {
    }

    void run() {
        m_worker = std::thread(m_process, m_index);
    }

    EThreadStatus m_status {PENDING};
    size_t m_index;
    std::thread m_worker;
    std::function<void(int)> m_process;
};




template <class Out, class ...Args>
class QueuedThreadPool {
public:
    explicit QueuedThreadPool(size_t size) : m_size(size) {}

    virtual ~QueuedThreadPool() {
        join();
    }

    void join() {
        for(auto & threadItem : m_threads) {
            if (threadItem.m_worker.joinable())
                threadItem.m_worker.join();
        }

        cleanupScheduler();
    }

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
        }

        m_schedulerRunning = false;
    }

    void run(size_t index, std::function<Out(Args... args)> functor, Args... args) {
        // todo: mutex
        m_threads[index].m_status = RUNNING;

        Out result = functor(std::forward<Args...>(args...));
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        cout << "result = " << result << " | thread id = " << std::this_thread::get_id() << endl;

        // todo: подумать про data-race
        m_threads[index].m_status = FINISHED;
    }

    void consume(std::function<Out(Args... args)> functor, Args... args) {
        if (m_threads.size() < m_size) {
            int index = m_threads.size();

            auto& inserted_element = m_threads.emplace_back([&, functor = functor](int index) {
                run(index, functor, std::forward<Args...>(args...));
            }, PENDING, index);

            inserted_element.run();
            return;
        }

        // todo: extract tasks from queue if there are any
        for (int i = 0; i < m_threads.size(); i++) {
            // once we found finished thread we create another one and finish the loop
            if (m_threads[i].m_status == FINISHED) {
                m_threads[i].m_worker.join();

                auto& inserted_element = m_threads.emplace_back([&, functor = functor](int index) {
                    run(i, functor, std::forward<Args...>(args...));
                }, PENDING, i);

                inserted_element.run();
                return;
            }
        }

        // if no process was marked as FINISHED, we stash action into queue
        // saving as a lambda to pass it later to thread
        m_taskQueue.emplace_back([&, functor = functor](int index) {
            run(index, functor, std::forward<Args...>(args...));
        }, PENDING, -1);


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

protected:
    size_t m_size {0};
    std::vector<ThreadItem> m_threads;
    std::deque<ThreadItem> m_taskQueue;
    std::thread* m_schedulerThread {nullptr};
    bool m_schedulerRunning {false};
};


int foo(int a) {
    cout << "foo(" << a << ")\n";
    return a + 2;
}

int main() {

    // todo: пофиксить отдачу одинаковых аргументов
    QueuedThreadPool<int, int> queuedThreadPool(3);
    queuedThreadPool.consume(foo, 1);
    queuedThreadPool.consume(foo, 2);
    queuedThreadPool.consume(foo, 3);
    queuedThreadPool.consume(foo, 4);

    return 0;
}