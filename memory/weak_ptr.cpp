#include <iostream>
#include <memory>
using std::cout;
using std::endl;

using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;


class Object {
public:
    Object() : m_Value(0) { cout << "default constructor" << endl; }
    Object(int value) : m_Value(value) { cout << "param constructor" << endl; }
    ~Object() { cout << "destructor" << endl; }
    void print() const { cout << "print() " << m_Value << endl; }
    int m_Value;
};


class B;

class A {
public:
    A() : pB(nullptr) { }
    A(const shared_ptr<B>& sp) : pB(sp) {
        cout << "A constructor" << endl;
    }
    ~A() {
        cout << "A destructor" << endl;
    }

    shared_ptr<B> pB;
};

class B {
public:
    B() : pA(nullptr) {}
    B(const shared_ptr<A>& sp) : pA(sp) {
        cout << "B constructor" << endl;
    }
    ~B() {
        cout << "B destructor" << endl;
    }

    shared_ptr<A> pA;
};


void TEST1_weak_ptr_use_count() {
    shared_ptr<Object> s1(new Object(10));
    auto s2 = s1;

    // both s1 and s2 point to the same data and the same counter object
    cout << "Shared use count = " << s1.use_count() << endl;

    // w1 points to the same counter object that s1 and s2
    weak_ptr<Object> w1 = s1;

    // deleting pointers
    s1.~shared_ptr<Object>();
    cout << "Shared use count = " << s1.use_count() << endl;

    s2.~shared_ptr<Object>();
    cout << "Shared use count = " << s2.use_count() << endl;

    // Counter object is still alive, and use_count() prints 0.
    // Without weak_ptr counter's use_count() would yield some random number
    cout << "Weak use count = " << s1.use_count() << endl;
    cout << "Weak expired = " << w1.expired() << endl;

    // Counter object will be deleted finally when weak ptr is deleted
}


int main() {

    shared_ptr<A> pA(new A());
    shared_ptr<B> pB(new B());

    pA->pB = pB;
    pB->pA = pA;

    return 0;
}