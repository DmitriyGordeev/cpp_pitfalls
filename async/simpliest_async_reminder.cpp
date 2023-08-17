#include <iostream>
#include <future>
#include <chrono>

// TODO: сделать флаг на Linux/Windows:
#include <windows.h>
#include <processthreadsapi.h>     // GetCurrentThreadId()

using std::cout;
using std::endl;


int foo() {
    cout << "foo start" << endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    cout << "foo ends" << endl;

    cout << "Async thread id = " << GetCurrentThreadId() << endl;

    return 20;
}

int main() {

    // TODO: сделать пример с вложенными callback, как в js Promise->then->then ...

    std::future<int> f = std::async(std::launch::async, foo);

    // TODO: есть ли более простой способ ждать future ?
    while(f.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
        cout << "main() continues | main thread id = " << GetCurrentThreadId() << endl;
    }

    cout << "result of future = " << f.get() << endl;

    return 0;
}