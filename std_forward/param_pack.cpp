#include <iostream>
using std::cout;
using std::endl;
using std::string;


class Object {
public:
    Object() { cout << "constructor" << endl;}
    Object(const Object& object) { cout << "copy constructor" << endl; }
    Object(Object&& object) noexcept { cout << "move constructor" << endl; }

    void print() const { cout << "Object.print\n"; }
};


template <typename T>
void func(T t) {
    // do something
}

template<typename T, typename... Args>
void func(T t, Args... args) {
    std::cout << t << "\n";
    func(std::forward<Args>(args)...);
}

// Unpacking variadic params
template <typename ...Args>
void bar(Args... args) {
    int index = 0;
    ([&]{
        cout << "param index[" << index << "] : " << args << endl;
        index++;
    } (), ...);
}


int main() {

    // variable number of arguments

    Object obj;
    func(1.12f, obj);       // copies
    func(1.00f, Object());  // moves

    bar(1, 3, -2.1f);


    return 0;
}
