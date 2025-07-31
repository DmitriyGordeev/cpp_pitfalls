#ifndef ALGORITHMS_LIBCLASS_H
#define ALGORITHMS_LIBCLASS_H

#ifdef DLLSPEC
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif


class API LibClass {
public:
    LibClass(int x) : value(x) {}

    int GetValue() const;

    void SetValue(int newX);

    const char* GetLongMessage() const;

// protected:
public:
    int value {0};
};




// Объявления ниже нужны для примера runtime_load.cpp,
// где dll загружаются на рантайме, и нужно предотвратить name-mangling,
// чтобы вызвать эти функции из загружающей exe
extern "C" float foo(int a);

extern "C" LibClass* CreateInstance();

#endif //ALGORITHMS_LIBCLASS_H
