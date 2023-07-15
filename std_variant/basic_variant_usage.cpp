#include <iostream>
#include <variant>
using std::cout;
using std::string;


class A {
public:
    A() {
        cout << "A constructor\n";
    }

    void print() const {
        cout << "A\n";
    }
};


class ObjectWithoutCopyConstructor {
public:
    ObjectWithoutCopyConstructor() {
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
    void operator()(const ObjectWithoutCopyConstructor& b) {
        b.print();
    }
};


void foo(const A& a) {
    cout << "print() A\n";
}

void foo(const ObjectWithoutCopyConstructor& b) {
    cout << "print() B\n";
}


std::variant<string, bool> getStringIfAvailable(bool available) {
    std::variant<string, bool> result;
    if (available)
        result.emplace<string>("name");
    else
        result.emplace<bool>(false);

    return result;
}


int main() {

    std::variant<A, ObjectWithoutCopyConstructor> var;
    Invoker inv;
    auto lambda = [](const auto& obj) {
        obj.print();
    };
    std::visit(lambda, var);



    std::variant<float, int> var2 {10.2f};
    var2.emplace<int>(10);
    cout << "holds_alternative = " << std::holds_alternative<int>(var2) << "\n";
    cout << std::get<1>(var2);


    auto r_true = getStringIfAvailable(true);
    auto r_false = getStringIfAvailable(false);

    return 0;
}