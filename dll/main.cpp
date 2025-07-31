#include <iostream>
#include "LibClass.h"
using std::cout;

int main() {

    LibClass lc(10);
    lc.SetValue(19);
    cout << lc.GetValue();

    cout << "foo:" << foo(7);

    LibClass* instance = CreateInstance();
    delete instance;

    return 0;
}
