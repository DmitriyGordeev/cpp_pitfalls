#include <iostream>
#include <variant>
using std::cout;


class A {
public:
    A() {
        cout << "A constructor\n";
    }

    void print() const {
        cout << "A\n";
    }
};


class B {
public:
    B() {
        cout << "B constructor\n";
    }

    void print() const {
        cout << "B\n";
    }
};


class Invoker {
public:
    void operator()(const A& a) {
        a.print();
    }
    void operator()(const B& b) {
        b.print();
    }
};


void foo(const A& a) {
    cout << "foo() A\n";
}

void foo(const B& b) {
    cout << "foo() B\n";
}



int main() {

    std::variant<A, B> var;

    Invoker inv;

    auto lambda = [](const auto& obj) {
        obj.print();
    };

    std::visit(lambda, var);

    return 0;
}