#include <iostream>
#include <memory>
using std::cout;
using std::endl;

using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;


class A {
public:
    A() = default;
};

class B {
public:
    B() = default;

    void action() {
        if (pA.expired()) {
            cout << "expired" << endl;
        }
    }

    weak_ptr<A> pA;
};

class C {
public:
    void foo(B& b) {
        shared_ptr<A> pA(new A());
        b.pA = pA;
        pA.reset(new A());

        b.action();
    }
};


int main() {

    // todo: если у нас уже есть объект,
    //  который приходит по обычному указателю, как свести его к smart ptr ?

    B b;
    C c;

    c.foo(b);


    return 0;
}