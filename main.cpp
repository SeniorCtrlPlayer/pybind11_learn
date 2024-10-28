#include "Student.h"

int main(int argc, char** argv) {
    Student* s = new Student(1);
    s->print();
    delete s;
    return 0;
}