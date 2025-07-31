// Пример загрузки dll библиотеки,
// которая не была слинкована с exe, но все равно из нее можно вызвать функции,
// зная их имена и сигнатуры (например через утилиту Dependecy walker)


#include <iostream>
#include <windows.h>
#include "LibClass.h"
using std::cout;

typedef float (*MyFooPtr)(int); // Define a function pointer type
typedef char* (*MyCreateInstance)();

int main() {

    HMODULE hDll = LoadLibrary("libdll_producer.dll");
    if (hDll == NULL) {
        cout << "Couldn't load dll\n";
        return 1;
    }


    // ===== Пример загрузки отдельной функции из библиотеки ===========
    MyFooPtr myFunction = (MyFooPtr)GetProcAddress(hDll, "foo");
    if (myFunction == NULL) {
        cout << "function wasn't found\n";
    } else {
        // Call the function
        float result = myFunction(10);
        cout << "result = " << result << "\n";
    }

    // ===== Пример загрузки функции, которая создает класс, описанный в dll и
    // возвращает указатель на созданный инстанс (своего рода фактори)  ==========
    LibClass* instance = nullptr;
    MyCreateInstance myInstanceCreator = (MyCreateInstance)GetProcAddress(hDll, "CreateInstance");
    if (myInstanceCreator == NULL) {
        cout << "function wasn't found\n";
    } else {
        // Call the function
        instance = reinterpret_cast<LibClass *>(myInstanceCreator());
        cout << "instance value = " << instance->value << "\n";
    }


    bool result = FreeLibrary(hDll);
    cout << "\nLib deallocated:" << result << "\n";
    return 0;
}