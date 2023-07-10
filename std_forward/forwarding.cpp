#include <iostream>
#include <memory>
using std::cout;
using std::string;

struct Object {
    Object(int arg) : value(arg) {}
    int value;
};


template <class T, class A1>
std::shared_ptr<T> factory_bad(A1& a1) {
    return std::shared_ptr<T>(new T(a1));
}

template <class T, class A1>
std::shared_ptr<T> factory_good(A1&& a1) {
    return std::shared_ptr<T>(new T(std::forward<A1>(a1)));
}


int main() {

    // auto p1 = factory_bad<Object>(10);   // error
    auto p2 = factory_bad<Object, const int&>(10); // ok but need to specify 'const int&'

    // Good: works either with lvalue and rvalue - forwards originally specified types
    int a = 10;
    auto p3 = factory_good<Object>(a);
    auto p4 = factory_good<Object>(10);

    return 0;
}