#include <iostream>
#include <fstream>

class A {
public:
    int x() {
        return 1;
    }
};

class B {
public:
    operator A() const {
        return A();
    }
};

int main() {
    B b;
    b.operator A().x();
    return 0;
}