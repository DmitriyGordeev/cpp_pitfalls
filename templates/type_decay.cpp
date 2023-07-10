#include <iostream>
using std::cout;


template <class T>
void foo(T x) {
    cout << x;
}

template <class T>
void bar(T& x) {
    cout << x;
}

template <class T>
std::pair<T, T> make_pair_by_value(T a1, T a2) {
    return std::pair<T, T>(a1, a2);
}

template <class T>
std::pair<T, T> make_pair_by_ref(T& a1, T& a2) {
    return std::pair<T, T>(a1, a2);
}


int main() {
    int a = 10;
    const int& ref = a;

    // example 1:
    foo(ref);      // this will cause type decay from 'const int&' to 'int' (deduced to 'int' in compile time)

    // we can still force 'const int&'
    foo<const int &>(ref);

    // bar() avoids type decay to 'int' because argument is 'T& x' instead of 'T x'
    bar(a);     // deduces to int&
    bar(ref);   // deduces to const int&


    // example 2:
    const char str[7] = "hello!";
    foo(str);   // deduces to 'const char*' (type decay is happening)
    bar(str);   // deduces to 'const char(&)[7]' - avoids type decay

    const char str1[7] = "hello!";
    const char str2[10] = "something";

    // type decay to 'const char*'
    auto p1 = make_pair_by_value(str1, str2);

    // no type decay: input type deduced as 'const char(&)[7]' instead of 'const char*',
    // because of pass-by-reference 'T& arg' in function make_pair_by_ref
    // auto p2 = make_pair_by_ref(str1, str2);

    return 0;
}