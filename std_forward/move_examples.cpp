#include <iostream>
using std::cout;
using std::string;


//void foo(const string& s) {
//    cout << s << "\n";
//}


void foo(string&& s) {
    cout << s << "\n";
}


int main() {

    std::string s = "Hello";

    foo((string&&)s);


    return 0;
}