#include <iostream>
#include <memory>
using std::cout;
using std::endl;

using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;

class Object {
public:
    Object() : m_Value(0) {
        cout << "constructor" << endl;
    }

    ~Object() {
        cout << "destructor" << endl;
    }

    void print() const { cout << "print() " << m_Value << endl; }

    int m_Value;
};


// This increases use_count because shared_ptr creates a copy to the local stack
void foo_value(shared_ptr<Object> pObj) {
    cout << "foo_value() pObj.use_count() = " << pObj.use_count() << endl;
    if (pObj)
        pObj->print();
}

void foo_ref(const shared_ptr<Object>& pObj) {
    cout << "foo_ref() pObj.use_count() = " << pObj.use_count() << endl;
    if (pObj)
        pObj->print();
}

// if shared_ptr is a temporary object it will be moved and destroyed in the end of
// function's stack
void foo_rvalue(shared_ptr<Object>&& pObj) {
    cout << "foo_rvalue() pObj.use_count() = " << pObj.use_count() << endl;
    if (pObj)
        pObj->print();
}


int main() {

    shared_ptr<Object> p1(new Object());
    cout << "use_count = " << p1.use_count() << endl;

    foo_ref(p1);

    foo_rvalue(std::make_shared<Object>());

    cout << "before return\n";
    return 0;
}