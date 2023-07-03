#include <iostream>
using std::cout;

class Int {
public:
    explicit Int(int a) : m_Value(a) {}

    [[nodiscard]] int get() const { return m_Value; }

    // Overloading cast operator Int -> int
    // explicit requires 'static_cast<int>(Int)', while if not marked with explicit,
    // implicit will be used which means:
    // 'void foo(int a)' makes possible 'foo(Int(0))' - will automatically cast from Int to int
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
    cout << "foo() = " << a << "\n";
}

int main() {

    Int I(0);

    foo(static_cast<int>(I));
    foo(static_cast<float>(I));

    // if we didn't mark casting operators with 'explicit',
    // which of casting functions will be invoked in this case ?
    // foo(I);

    return 0;
}
