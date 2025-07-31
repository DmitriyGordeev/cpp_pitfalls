#include <coroutine>
#include <iostream>
using std::cout;


struct Task {
    struct promise_type {
        Task get_return_object() {
            cout << "Task::get_return_object()\n";
            return {};
        }

        // контролирует, нужно ли приостанавливать сразу после создания
        // или приостановка  происходит потом
        std::suspend_never initial_suspend() {
            cout << "Task::initial_suspend()\n";
            return {};
        }

        std::suspend_never final_suspend() noexcept {
            cout << "Task::final_suspend()\n";
            return {};
        }

        void unhandled_exception() {}
    };
};

struct Awaiter {
    std::coroutine_handle<> *hp_;

    bool await_ready() const noexcept {
        cout << "Awaiter::await_ready()\n";
        return false;
    }

    void await_suspend(std::coroutine_handle<> h) {
        cout << "Awaiter::await_suspend()\n";
        *hp_ = h;
    }

    int await_resume() const noexcept {
        cout << "Awaiter::await_resume()\n";
        return -19;
    }
};

Task CoroutineFunction(std::coroutine_handle<> *continuation_out) {
    Awaiter a {continuation_out};
    int value = co_await a;
    cout << "value = " << value << ", CoroutineFunction() ends\n";
}

int main() {
    std::coroutine_handle<> h;

    CoroutineFunction(&h);
    cout << "main func after coroutine\n";
    h();

    // h.destroy()
    // h.destroy() - кидает SegFault, если final_suspend()
    // возвращает стандартный awaitable std::suspend_never
    // (или любой другой awaitable у которого await_ready() возвращает true),
    // так как объект корутины уже удалился. Если возвращается std::suspend_always,
    // то корутина все еще в памяти и к ней еще можно обратиться

    return 0;
}