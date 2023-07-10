#include <iostream>
using std::cout;

template <int n>
struct Obj {
    static const int val = 2 * Obj<n - 1>::val;
};

template<>
struct Obj <0> {
    static const int val = 1;
};

int main() {
    cout << Obj<10>::val;
    return 0;
}
