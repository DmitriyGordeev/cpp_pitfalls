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
    cout << "print() A\n";
}

void foo(const B& b) {
    cout << "print() B\n";
}


int bar() {
    throw 42;
}


int main() {

    std::variant<A, B> var;
    Invoker inv;
    auto lambda = [](const auto& obj) {
        obj.print();
    };
    std::visit(lambda, var);



    std::variant<float, int> var2 {10.2f};
    var2.emplace<int>(10);
    cout << "holds_alternative = " << std::holds_alternative<int>(var2) << "\n";
    cout << std::get<1>(var2);

    return 0;
}