#include <iostream>
using std::cout;

template <int n>
struct Fun {
    static const int val = 2 * Fun<n-1>::val;
};

template<>
struct Fun <0> {
    static const int val = 1;
};

int main() {

    cout << Fun<10>::val;

    return 0;
}
