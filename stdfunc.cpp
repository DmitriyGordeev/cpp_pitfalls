#include <functional>
#include <iostream>
using std::cout;


int foo(int a, int b) {
    int value = a + b;
    cout << "bar():" << a << ", " << b << ", result = " << value << "\n";
    return value;
}

int bar(int a, int b) {
    int value = a * b;
    cout << "bar():" << a << ", " << b << ", result = " << value << "\n";
    return value;
}

// ======================================================================================
void action(int b) {
    cout << "action(" << b << ")\n";
}

class Object {
public:
    Object() {}
    void operator () (int arg) {
        cout << "functor invocation, arg = " << arg << "\n";
    }
};

void acceptStdFunctional(std::function<void(int)> f) {
    f(0);
}


// =====================================================================================
void longRunningRequest(std::function<void()> responseCallback) {
    cout << "longRunningRequest() : start processing\n";
    int counter = 0;
    for(int i = 0; i < 100000; i++) {
        for (int j = 0; j < 100000; j++)
            counter = i + j - 1000;
        counter = 0;
    }
    responseCallback();
}

void onResult() {
    cout << "onResult\n";
}

int main() {

    std::function<int(int, int)> f = foo;
    f(3, 4);

    f = bar;
    f(3, 4);

    // lambda is converted to std::function
    acceptStdFunctional([](int v) -> void {
        cout << "acceptStdFunctional with lambda\n";
    });

    // function pointer is converted to std::function
    void (*fptr)(int) = &action;
    acceptStdFunctional(fptr);

    Object functorObject;
    acceptStdFunctional(functorObject);

    longRunningRequest(onResult);

    return 0;
}
