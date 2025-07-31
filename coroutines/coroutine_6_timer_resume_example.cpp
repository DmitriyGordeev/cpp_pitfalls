#include <iostream>
#include <coroutine>
#include <thread>
using std::cout;
using std::string;

// thread является глобальным чисто для упрощения примера,
// в реальных проектах можно завернуть в некий класс
std::thread* thread_ptr = nullptr;


struct Task {

    struct promise_type {

        int value {0};

        Task get_return_object() {
            return {
                std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }

        std::suspend_never initial_suspend() {
            return {};
        }

        std::suspend_always final_suspend() noexcept {
            return {};
        }

        void return_void() {
            cout << "return_void\n";
        }

        void unhandled_exception() {}
    };

    std::coroutine_handle<promise_type> handle;


};


struct Awaiter {

    std::coroutine_handle<Task::promise_type> _handle;

    bool await_ready() noexcept {
        return false;
    }

    bool await_suspend(std::coroutine_handle<Task::promise_type> handle) noexcept {
        cout << "await starts\n";
        _handle = handle;
        _handle.promise().value++;

        // создаем таймер, который по истечении времени возобновит корутину,
        // перехватив при этом работающий caller, в данном случае main()
        thread_ptr = new std::thread([this]() {
                std::this_thread::sleep_for(std::chrono::seconds(3));
                cout << "thread has finished, resuming...\n";
                this->_handle.resume();
        });
        return true;
    }

    int await_resume() const noexcept {
        Task::promise_type p = _handle.promise();
        return p.value;
    }
};


Task coroutine() {
    int v = co_await Awaiter {};
    cout << "print " << v << "\n";
    co_return;
}


int main() {
    Task task = coroutine();

    // В какой-то момент этот цикл прервется на возвобновляющуюся корутину,
    // когда thread проснется после таймера и вызовет handle.resume()
    // и завершившись, то есть дойдя до co_return (return_void())
    // функция main продолжит этот цикл дальше, как обычно
    for (int i = 0; i < 20000; i++) {
        cout << "main " << i << "\n";
    };

    thread_ptr->join();
    delete thread_ptr;
    thread_ptr = nullptr;

    return 0;
}