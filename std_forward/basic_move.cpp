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
    size_t m_size {0};
    int* m_data {nullptr};
};



class Consumer {
public:
    Consumer(Object obj) : m_Object(std::move(obj)) {}

protected:
    Object m_Object;
};



int main() {

    // 1. Copying obj to temporary object and then moves from temporary object to Consumer
    Object obj(10);
    Consumer consumer(obj);

    // 2. Consumes temporary object (moves) immediately
    Consumer consumer2(Object(20));

    // 3. steal from original value
    Object obj2(30);
    Consumer consumer3(std::move(obj2));

    // obj2 may be unusable because its m_data = nullptr

    return 0;
}
