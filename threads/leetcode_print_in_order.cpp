#include <iostream>
#include <atomic>
#include <thread>
#include <functional>
using std::cout;
using std::endl;


class Foo {
public:
    Foo() {}

    void first(std::function<void()> printFirst) {
        printFirst();
        value = 1;
    }
    void second(std::function<void()> printSecond) {
        while(value != 1);
        printSecond();
        value = 2;
    }
    void third(std::function<void()> printThird) {
        while(value != 2);
        printThird();
    }

protected:
    std::atomic<int> value {0};
};


int main() {

    Foo f;

    auto l1 = [&f]() {
        f.first([]() {cout << "first\n";});
    };

    auto l2 = [&f]() {
        f.second([]() {cout << "second\n";});
    };

    auto l3 = [&f]() {
        f.third([]() {cout << "third\n";});
    };

    std::thread t1(l1);
    std::thread t2(l2);
    std::thread t3(l3);

    t1.join();
    t2.join();
    t3.join();
    return 0;
}
