#include <concepts>
#include <coroutine>
#include <exception>
#include <iostream>
using std::cout;

template<typename PromiseType>
struct Awaiter_GetPromise {

    PromiseType *p_;

    bool await_ready() {
        cout << "Awaiter_GetPromise::await_ready()\n";
        return false;
    }

    bool await_suspend(std::coroutine_handle<PromiseType> h) {
        cout << "Awaiter_GetPromise::await_suspend()\n";
        p_ = &h.promise();
        return false;
    }

    PromiseType *await_resume() {
        cout << "Awaiter_GetPromise::await_resume()\n";
        return p_;
    }

};

struct Task {
    struct promise_type {
        unsigned value_;

        Task get_return_object() {
            cout << "Task::get_return_object()\n";
            return Task {
                    .h_ = std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void unhandled_exception() {}
    };

    std::coroutine_handle<promise_type> h_;
    operator std::coroutine_handle<promise_type>() const { return h_; }
};

Task CoroutineFunction()
{
    // Этот co_await не передает управление main(), потому что у его awaiter-класса
    // функция await_suspend() возвращает false
    auto pp = co_await Awaiter_GetPromise<Task::promise_type>{};
    cout << "CoroutineFunction() cached promise object pointer\n";

    for (unsigned i = 0;; ++i) {
        pp->value_ = i;

        cout << "CoroutineFunction(): i = " << i << "\n";

        // Здесь передается управление main(), так как мы используем стандартный
        // awaiter-класс std::suspend_always() у которого await_ready() -> false
        co_await std::suspend_always{};
    }
}

int main()
{
    std::coroutine_handle<Task::promise_type> h = CoroutineFunction();
    cout << "main(): after getting h\n";
    Task::promise_type &promise = h.promise();
    for (int i = 0; i < 3; ++i) {
        std::cout << "CoroutineFunction: " << promise.value_ << std::endl;
        h();
    }
    h.destroy();

    return 0;
}