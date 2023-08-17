#include <iostream>
#include <variant>
#include <vector>
using std::cout;

class A {
public:
    A() = default;
    void foo() { cout << "A::foo\n";}
};

class B {
public:
    B() = default;
    void foo() { cout << "B::foo\n";}
};

class C {
public:
    C() = default;
    void foo() { cout << "C::foo\n";}
};


int main() {

    auto l = [](auto& v) {
        v.foo();
    };

    std::vector<std::variant<A, B, C>> vec;
    vec.emplace_back(A{});
    vec.emplace_back(B{});
    vec.emplace_back(C{});

    for(auto& v : vec)
        std::visit(l, v);

    return 0;
}