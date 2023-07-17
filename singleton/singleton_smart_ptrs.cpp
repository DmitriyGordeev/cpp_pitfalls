#include <iostream>
#include <memory>
using std::cout;
using std::endl;
using std::shared_ptr;
using std::weak_ptr;

// Example of Singleton using smart pointers

class Singleton {
public:

    // TODO: используем weak_ptr, потому что нам не нужен ownership объекта,
    //  мы просто хотим получить доступ к нему, если он есть памяти.
    //  Подумать, есть ли тут какие-то серьезные проблемы и
    //  добавить другие интересные случаи - например thread safe
    static weak_ptr<Singleton> get(int value) {
        if (!m_Ptr) {
            shared_ptr<Singleton> p(new Singleton(value));
            m_Ptr = p;
        }

        return m_Ptr;
    }

    void setValue(int value) {
        m_Value = value;
    }

    int getValue() const { return m_Value; }

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

shared_ptr<Singleton> Singleton::m_Ptr = nullptr;


void foo() {
    auto p1 =  Singleton::get(10);

    p1.lock()->setValue(20);

    cout << "foo(): singleton address = " << p1.lock().get() << endl;
}



int main() {

    foo();

    auto p = Singleton::get(10);
    if (!p.expired()) {
        auto ps = p.lock();
        cout << "main(): Singleton address: " << ps.get() << endl;
        cout << "Singleton value = " << ps->getValue() << endl;
    }

    return 0;
}
