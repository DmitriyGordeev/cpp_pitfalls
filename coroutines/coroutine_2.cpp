#include <concepts>
#include <coroutine>
#include <exception>
#include <iostream>
using std::cout;

struct Task {
    struct promise_type {
        Task get_return_object() {
            return {
                    .h_ = std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void unhandled_exception() {}
    };

    std::coroutine_handle<promise_type> h_;
    operator std::coroutine_handle<promise_type>() const { return h_; }
    operator std::coroutine_handle<>() const { return h_; }
};

Task CoroutineFunction()
{
    for (unsigned i = 0;; ++i) {
        co_await std::suspend_always{};
        std::cout << "CoroutineFunction: " << i << std::endl;
    }
}

int main()
{
    // здесь вызывается оператор conversion
    // из Task в std::coroutine_handle<>(),
    // то есть мы сохраняем "указатель-хэндл на объект корутины (сохраненный где-то в heap)
    // но сам объект Task уничтожается, так как он rvalue
    // Если не сохранить хэндл в h - мы теряем референс на объект корутины
    // то есть происходит memory-leak !
    std::coroutine_handle<> h = CoroutineFunction();

    for (int i = 0; i < 3; ++i) {
        std::cout << "In main2 function\n";
        h();
    }
    h.destroy();
    return 0;
}