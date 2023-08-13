#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>
#include <memory>
#include <functional>

using std::cout;
using std::function;
using std::endl;


std::mutex mut;

class FooBar {
private:
    int n;
    std::atomic<bool> shouldExecuteFoo{true};

public:
    FooBar(int n) {
        this->n = n;
    }

    void foo(function<void()> printFoo) {
        for (int i = 0; i < n; i++) {
            while(!shouldExecuteFoo);
            printFoo();

            // если следать shouldExecuteFoo = false будет то же самое, но memory_order_relaxed -
            // дает меньше рестрикций по memory order, тут нам не важен порядок (надо потестить подробнее)
            // так как у нас есть жесткий while вначале (todo: можно ли без while?)
            shouldExecuteFoo.store(false, std::memory_order::memory_order_relaxed);
        }
    }

    void bar(function<void()> printBar) {
        for (int i = 0; i < n; i++) {
            while(shouldExecuteFoo);
            printBar();

            // если следать shouldExecuteFoo = true будет то же самое, но с memory_order_relaxed -
            // дает меньше рестрикций по memory order, тут нам не важен порядок (надо потестить подробнее)
            // так как у нас есть жесткий while вначале (todo: можно ли без while?)
            shouldExecuteFoo.store(true, std::memory_order::memory_order_relaxed);
        }
    }
};


int main() {

    FooBar foobar(1000);

    std::thread t1([&foobar]() {
        foobar.foo([]() { cout << "foo"; });
    });
    std::thread t2([&foobar]() {
        foobar.bar([]() { cout << "bar\n"; });
    });


    t1.join();
    t2.join();

    return 0;
}