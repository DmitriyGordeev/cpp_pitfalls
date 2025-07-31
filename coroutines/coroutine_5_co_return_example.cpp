#include <concepts>
#include <coroutine>
#include <exception>
#include <iostream>
using std::cout;

struct Task {
    struct promise_type {
        unsigned value_;

        ~promise_type() {
            std::cout << "promise_type destroyed" << std::endl;
        }
        Task get_return_object() {
            return {
                    .h_ = std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() {}
        std::suspend_always yield_value(unsigned value) {
            value_ = value;
            return {};
        }

        void return_value(int i) {
            cout << "promise_type::return_value()\n";
            value_ = i;
        }
    };

    std::coroutine_handle<promise_type> h_;
};

Task CoroutineFunction()
{
    for (unsigned i = 0; i < 3; ++i)
        co_yield i;
    co_return 18;
}

int main()
{
    auto h = CoroutineFunction().h_;
    auto &promise = h.promise();
    while (!h.done()) {
        std::cout << "counter5: " << promise.value_ << std::endl;
        h();
    }

    cout << "promise->value = " << promise.value_ << "\n";
    h.destroy();

    return 0;
}