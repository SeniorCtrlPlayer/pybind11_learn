#include "Student.h"

Student::Student(int id) {
    this->id = id;
}

Student::~Student() {
    cout << "destructor" << endl;
}

void Student::setId(int id) {
    this->id = id;
}

void Student::print() {
    cout << "id: " << this->id << endl;
    this->setId(111);
    cout << "id: " << this->id << endl;
}