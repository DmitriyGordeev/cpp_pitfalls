#include <iostream>
using std::cout;
using std::endl;

// Simple example of Singleton using static variable and reference
class Singleton {
public:
    static Singleton& get(int value) {
        static Singleton instance(value);
        return instance;
    }

    virtual ~Singleton() {
        cout << "destructor" << endl;
    }

protected:
    // TODO: thread safe
    Singleton(int value) : m_Value(value) {
        cout << "Created" << endl;
    }

    int m_Value;
};


class Object {
public:
    Object() { cout << "Object created" << endl; }
};


void foo() {
    static Object object;

}



int main() {

    foo();
    foo();
    foo();
    foo();

    return 0;
}
