#include <iostream>
#include <memory>
using std::cout;
using std::endl;

using std::shared_ptr;
//using std::unique_ptr;
//using std::weak_ptr;

class Object {
public:
    Object() : m_value(100) {
        cout << "constructor" << endl;
    }

    virtual ~Object() {
        m_value = 0;
        cout << "destructor" << endl;
    }

    void print() {
        cout << m_value << endl;
    }

    int m_value;
};


void with_shared(Object* obj) {
    shared_ptr<Object> sp(obj);
    // Object will be destroyed here !
}



int main() {

    Object* pObj = new Object();

    with_shared(pObj);

    // MEMORY LEAK !
    pObj->print();

    return 0;
}