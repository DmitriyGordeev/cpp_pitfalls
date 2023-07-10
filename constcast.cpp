#include <functional>
#include <iostream>
using std::cout;


int write(int* in) {
    *in = -10;
    return *in;
}

// if we know that function doesn't change underlying value of v but accepts
// non-const type (like in old libs) - we can specify const_cast<>
int readonly(int* v) {
    cout << *v;
    return 0;
}

int main() {

    const int a = 0;

    // readonly(&a); - error because of wrong input type

    // but we can use const_cast if we are sure that function is read-only
    readonly(const_cast<int*>(&a));

    // [!] possible undefined behavior:
    write(const_cast<int*>(&a));

    return 0;
}