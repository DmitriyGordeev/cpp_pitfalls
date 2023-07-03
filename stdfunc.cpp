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

int main() {

    std::function<int(int, int)> f = foo;
    f(3, 4);

    f = bar;
    f(3, 4);

    return 0;
}
