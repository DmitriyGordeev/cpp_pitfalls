#include <future>
#include <chrono>
#include <iostream>
using std::cout;
using std::endl;


void sleep(int sec) {
    std::this_thread::sleep_for(std::chrono::seconds(sec));
}

int loadSomething(int a) {
    sleep(2);
    cout << "loadSomething(" << a << ")" << endl;
    return a;
}

void callback_hell_example() {
    bool finished = false;
    std::future f1 = std::async(std::launch::async, [&finished](){
        loadSomething(0);
        std::future f2 = std::async(std::launch::async, [&finished]() {
            loadSomething(1);
            finished = true;
            cout << "All resources loaded" << endl;
        });
    });
}

// this is basic alternative to javascript's Promise.then().then()...,
// when we need to do multiple async operations sequentially and get rid of callback hell.
void async_then_example() {
    std::promise p = std::promise<int>();
    std::thread t1 = std::thread([&]() {
        loadSomething(0);
        cout << "t1: first resource loaded " << endl;
        p.set_value(0);
    });

    std::promise p2 = std::promise<int>();  // todo: можно ли обойтись одним promise ?
    std::thread t2 = std::thread([&]() {
        int result = p.get_future().get();
        cout << "t2: start loading second resource" << endl;
        loadSomething(1);
        p2.set_value(1);
    });

    std::thread t3 = std::thread([&]() {
        int result = p2.get_future().get();
        cout << "All resources loaded" << endl;
    });

    cout << "Started async chain operation" << endl;
    t1.join();
    t2.join();
    t3.join();
}

int main() {

    callback_hell_example();
    cout << "-----------------------------------" << endl;
    async_then_example();

    return 0;
}