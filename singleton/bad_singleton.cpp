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

    Singleton(const Singleton& other) {
        cout << "Copy constructor" << endl;
    }

    virtual ~Singleton() {
        cout << "destructor" << endl;
    }

protected:
    Singleton(int value) : m_Value(value) {
        cout << "Created" << endl;
    }

    int m_Value;
};

int main() {
    Singleton s = Singleton::get(10);

    // TODO: деструктор вызывается 2 раза: 1ый раз при удалении s из стека main
    //  второй раз, удаляя статическую переменную при выходе из процесса,
    //  это может вызывать проблемы при удалении динамической памяти в деструкторе в первый раз

    return 0;
}
