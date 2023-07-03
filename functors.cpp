#include <iostream>
using std::cout;


class Object {
public:
    Object() {
        cout << "Object constructor\n";
    }

    virtual ~Object() = default;

    virtual void operator () () {
        cout << "Functor call\n";
    }
};

class DerObject : public Object {
public:
    DerObject() : Object() {
        cout << "DerObject construction\n";
    }

    void operator () () override {
        Object::operator()();
        cout << "DerObject functor call\n";
    }
};

int main() {

    Object obj;
    obj();

    DerObject dobj;
    dobj();

    return 0;
}
