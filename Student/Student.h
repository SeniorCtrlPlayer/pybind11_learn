#include <iostream>
using namespace std;

class Student {
    public:
        Student(int id);
        ~Student();
        void setId(int id);
        void print();
        static void print1();
    private:
        int id;
};