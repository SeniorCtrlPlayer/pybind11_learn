#include <iostream>
#include "json.hpp"
using json = nlohmann::json;
using namespace std;

class Student {
    private:
        int id;
    public:
        Student() = default;
        Student(int id);
        ~Student();
        void setId(int id);
        void print();
        static void print1();

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Student, id)
};