#include <iostream>
using std::cout;

class Experiment {
public:
    explicit Experiment(bool condition) : bCondition(condition) {
        // we can pick necessary behavior on construct time once
        action_ptr = &Experiment::action_False;
        if (condition)
            action_ptr = &Experiment::action_True;
    }

    void action_True() {
        cout << "Action True\n";
    }

    void action_False() {
        cout << "Action False\n";
    }

    void action() {
        cout << "action()\n";
        (this->*action_ptr)();
    }

    void (Experiment::*action_ptr)();

protected:
    bool bCondition;
};

int main() {

    Experiment* e1 = new Experiment(false);
    Experiment* e2 = new Experiment(true);

    delete e1;
    delete e2;
    return 0;
}
