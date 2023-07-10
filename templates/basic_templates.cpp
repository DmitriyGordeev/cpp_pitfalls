#include <iostream>
using std::cout;
using std::string;

// ------------------------------
// template with default type
template <class T = int>
class Object {
public:
    Object() {}

    void print() const {
        cout << m_Value << "\n";
    }

    T m_Value;
};

// ------------------------------
// template with const
template <class T, int limit>
class ObjectWithConst {
public:
    ObjectWithConst() {}

    void print() const {
        for (int i = 0; i < limit; i++)
            cout << "ObjectWithConst printing " << i << "\n";
    }
};

// ------------------------------
// template specialization for functions:
template <class T>
void foo(T a) {
    cout << a << "\n";
}

template <>
void foo(const string& a) {
    cout << a.size() << " : " << a << "\n";
}

// template specialization for classes:
template <class T>
class TestObj {
public:
    TestObj() {
        cout << "Constructed with general template\n";
    }
};

template<>
class TestObj<int> {
public:
    TestObj() {
        cout << "Constructed with specialized <int> template\n";
    }
};


int main() {
    // example 1. template with default type
    // deduces to int because it's default and we didn't specify type T
    Object object;
    object.print();

    // deduces to float
    Object<float> object2;
    object2.print();

    cout << "------------------------------\n";

    // example 2. template with const
    ObjectWithConst<float, 10> objectWithConst;
    objectWithConst.print();

    cout << "------------------------------\n";
    // Specialization for functions
    foo(-19);
    foo(12.f);

    string s = "string";
    foo<const string&>(s);

    cout << "------------------------------\n";
    // Specialization for classes
    TestObj<float> testObj_float;
    TestObj<int> testObj_int;


    return 0;
}