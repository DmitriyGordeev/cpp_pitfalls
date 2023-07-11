#include <iostream>
#include <memory>
using std::cout;
using std::endl;

using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;


class A {
public:
    A(int value) : m_value(value) {}
    int m_value;
};

class B {
public:
    B() = default;

    void action1() {
        if (pA.expired())
            cout << "B::action1() - expired" << endl;
        else
            pA.lock()->m_value -= 1;
    }

    void action2() {
        if (pA.expired()) {
            cout << "B::action2() - expired" << endl;
        }
        else
            cout << "Calculated value = " << pA.lock()->m_value << endl;
    }

    // B doesn't need ownership of A,
    // it's just should know how to locate an object if it's still in memory,
    // hence we use weak_ptr instead of shared_ptr
    // in other words - B doesn't control lifetime
    // of A (it's controlled by something else that B is not aware of) but sometimes B needs to access it
    weak_ptr<A> pA;
};


// C does have control over A's lifetime via shared_ptr
class C {
public:
    void foo(B& b) {
        shared_ptr<A> pA(new A(1));

        b.pA = pA;

        b.action1();

        if (pA->m_value == 0)
            pA.reset(new A(1));

        b.action2();
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