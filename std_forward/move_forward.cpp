#include <iostream>
using std::cout;
using std::endl;

class Object {
public:
    Object(size_t size) : m_size(size) {
        m_data = new int[size];
        for(size_t i = 0; i < size; i++)
            m_data[i] = 1;
    }

    Object(const Object& obj) {
        m_data = new int[obj.m_size];
        for(size_t i = 0; i < obj.m_size; i++)
            m_data[i] = 1;
        cout << "Copy constructor" << endl;
    }

    Object(Object&& obj) {
        delete[] m_data;
        m_data = obj.m_data;
        obj.m_data = nullptr;
        cout << "Move constructor" << endl;
    }

    virtual ~Object() {
        delete[] m_data;
        m_data = nullptr;
        m_size = 0;
    }

    void print() const {
        cout << m_size << endl;
    }

protected:
    int* m_data {nullptr};
    size_t m_size {0};
};


template <class T>
class Consumer {
public:
    Consumer(T obj) : m_Object(std::move(obj)) {
        cout << "Consumer() begins" << endl;
        int a = 1000;
        // [!] obj may be unusable here, because we moved it from local obj var to m_Object()
    }

protected:
    T m_Object;
};


template <class T>
Consumer<T> foo(T arg) {
    cout << "foo()" << endl;
    Consumer c = Consumer(std::forward<T>(arg));
    // [!] arg may be unusable here, because we moved it to Consumer
    return c;
}


int main() {

    // Example 1.
    // copies obj to foo()'s stack frame
    // then moves from local obj variable to Consumer() constructor
    // then moves from Consumers's local var to Object
    // one copy happens
    Object obj(10);
    foo(obj);

    cout << "-------------------------------" << endl;

    // Example 2. move constructor is called several times -
    // moving data from original temporary r-value to Consumer::m_Object
    // no copy happens
    // TODO: Move принтуется 3 раза, но должно быть 4? (move elision?)
    foo(Object(10));

    return 0;
}
