// TODO: добавить коменты

#include <iostream>
#include <thread>
#include <functional>
#include <vector>
#include <deque>
#include <mutex>
#include <chrono>

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
    ThreadItem(const std::function<void()>& threadLambda, EThreadStatus status) : m_status(status) {
        m_worker = std::thread(threadLambda);
    }
    std::thread m_worker;
    EThreadStatus m_status {PENDING};
};


template <class Out, class ...Args>
class QueuedThreadPool {
public:
    explicit QueuedThreadPool(size_t size) : m_size(size) {}

    virtual ~QueuedThreadPool() {
        join();
    }

    void join() {
        for(int i = 0; i < m_threads.size(); i++) {
            if (m_threads[i].m_worker.joinable())
                m_threads[i].m_worker.join();
        }
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

            m_threads.emplace_back([&, index=index, functor = functor]() {
                run(index, functor, std::forward<Args...>(args...));
            }, PENDING);

        }
        else {
            for (int i = 0; i < m_threads.size(); i++) {
                // once we found finished thread we create another one and finish the loop
                if (m_threads[i].m_status == FINISHED) {
                    m_threads[i].m_worker.join();

                    m_threads.emplace_back([&, i=i, functor = functor]() {
                        run(i, functor, std::forward<Args...>(args...));
                    }, PENDING);
                    break;
                }
            }
        }


    }

protected:
    size_t m_size {0};
    std::vector<ThreadItem> m_threads;
//    std::deque<std::function<Out(Args... args)>, Args...> m_taskQueue;
};


int foo(int a) {
    cout << "foo(" << a << ")\n";
    return a + 2;
}


int main() {

    QueuedThreadPool<int, int> queuedThreadPool(3);
    queuedThreadPool.consume(foo, 1);



    return 0;
}