#include <iostream>
#include <variant>
using std::cout;


class A {
public:
    A() : m_Value(1), m_Name{"Hello"} {
        cout << "Constructor A\n";
    }

    A(const A& copy) {
        m_Value = copy.m_Value;
        m_Name = copy.m_Name;
        cout << "Copy constructor\n";
    }

    A(A&& a) {
        m_Value = a.m_Value;
        m_Name = std::move(a.m_Name);
        cout << "Move constructor\n";
    }

    ~A() {
        cout << "Destructor A\n";
    }

protected:
    int m_Value;
    std::string m_Name;
};


int main() {

    std::variant<int, float, double, A> v{10};
    cout << std::holds_alternative<A>(v) << "\n";

    // variant uses move constructor if A is r-value
    v.emplace<A>(A());

    cout << "before finish\n";

    cout << "sizeof A = " << sizeof(A) << "\n";
    cout << "sizeof variant = " << sizeof(std::variant<int, float, double, A>) << "\n";

    return 0;
}
