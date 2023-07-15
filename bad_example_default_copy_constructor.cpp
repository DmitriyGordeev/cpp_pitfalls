#include <iostream>
using std::cout;
using std::endl;


// Example of class without proper copy constructor - leads to memory leak
class ObjectWithoutCopyConstructor {
public:
    ObjectWithoutCopyConstructor(int value) : m_Value(value) {
        cout << "constructor\n";
        if (m_Value > 0) {
            m_Array = new int[value];
            for (int i = 0; i < value; i++)
                m_Array[i] = 0;
        }
        else
            m_Array = nullptr;
    }

    virtual ~ObjectWithoutCopyConstructor() {
        cout << "destructor" << endl;
        if (m_Value > 0 && m_Array) {
            delete[] m_Array;
            m_Array = nullptr;
        }
    }

    int m_Value;
    int* m_Array;
};


int main() {
    ObjectWithoutCopyConstructor b1(3);

    // makes shallow copy of m_Value but not deep copy of m_Array!
    ObjectWithoutCopyConstructor b2 = b1;

    // destroying b1 - invalidates memory
    b1.~ObjectWithoutCopyConstructor();

    // b2 points to bad memory and causes error
    cout << b2.m_Array[0];

    return 0;
}