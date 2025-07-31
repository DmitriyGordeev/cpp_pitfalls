#include "LibClass.h"

#define DLLSPEC

int LibClass::GetValue() const {
    return value;
}

void LibClass::SetValue(int newX) {
    value = newX;
}

float foo(int a) { return 12.3f + a; }

LibClass* CreateInstance() {
    return new LibClass(20);
}