#include <iostream>
#include <memory>
using std::cout;
using std::endl;
using std::shared_ptr;
using std::weak_ptr;

// Example of Singleton using smart pointers

class Singleton {
public:
    static weak_ptr<Singleton> get(int value) {
        if (!m_Ptr) {
            shared_ptr<Singleton> p(new Singleton(value));
            m_Ptr = p;
        }

        return m_Ptr;
    }

    virtual ~Singleton() {
        cout << "destructor" << endl;
    }

    static shared_ptr<Singleton> m_Ptr;

protected:
    Singleton(int value) : m_Value(value) {
        cout << "Singleton create()" << endl;
    }

    int m_Value;
};

// TODO: test inheritance of Singleton
// TODO: test thread safety

shared_ptr<Singleton> Singleton::m_Ptr = nullptr;


void foo() {
    auto p1 =  Singleton::get(10);
    cout << "foo(): singleton address = " << p1.lock().get() << endl;
}



int main() {

    foo();

    auto p = Singleton::get(10);

    cout << p.lock().get() << endl;

    return 0;
}
