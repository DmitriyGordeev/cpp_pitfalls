#include <functional>
#include <iostream>
using std::cout;

int foo(int* v) {
    // if we know that function doesn't change underlying value of v
    cout << v;
    return 0;
}

int main() {

    const int* a = new const int(10);

    foo(const_cast<int*>(a));

    return 0;
}