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

        // !!! Если этой функции нет в promise_type и корутина просто
        // завершается (без co_return) то будет undefined behaviour
        void return_void() {
            cout << "promise_type::return_void()\n";
        }
    };

    std::coroutine_handle<promise_type> h_;
};

Task CoroutineFunction()
{
    for (unsigned i = 0; i < 3; ++i)
        co_yield i;
}

int main()
{
    auto h = CoroutineFunction().h_;
    auto &promise = h.promise();
    while (!h.done()) { // <- произведет SegFault если promise_type::final_suspend() возвращает std::suspend_never
        std::cout << "counter5: " << promise.value_ << std::endl;
        h();

        // если promise_type::final_suspend() возвращает std::suspend_never,
        // здесь после последнего co_yield объект корутины уничтожится сам, то есть
        // на следующей итерации h будет невалиден, а значит h.done() произведет segfault
    }
    h.destroy();

    return 0;
}