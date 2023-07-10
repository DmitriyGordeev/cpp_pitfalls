#include <iostream>
using std::cout;

class Int {
public:
    explicit Int(int a) : m_Value(a) {}

    [[nodiscard]] int get() const { return m_Value; }

    // Overloading cast operator Int -> int:
    // explicit requires 'static_cast<int>(Int)', but if its not marked with 'explicit',
    // implicit will be used which means:
    // 'foo(Int(0))' is a legit call if foo defined as: 'void foo(int a)'
    // does automatic (implicit) casting from Int to int
    explicit operator int () const {
        cout << "Int -> int cast operator\n";
        return m_Value;
    }

    explicit operator float () const {
        cout << "Int -> float cast operator\n";
        return static_cast<float>(m_Value);
    }

protected:
    int m_Value;
};

void foo(int a) {
    cout << "print() = " << a << "\n";
}

int main() {

    Int I(0);

    foo(static_cast<int>(I));
    foo(static_cast<float>(I));

    // TODO: if we didn't mark casting operators with 'explicit',
    // TODO: which of casting functions will be invoked in this case ?
    // foo(I);

    return 0;
}
