#include <iostream>
using namespace std;

class Student {
    public:
        Student(int id);
        ~Student();
        void setId(int id);
        void print();
        static void print1();
        int json_id;
    private:
        int id;
};