#include "Student.h"
#include <fstream>

class class_room {
    public:
        string teacher_name;
        std::vector<Student> student_list;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(class_room, teacher_name, student_list)
};

void json_test() {
    std::string file_path = "./test.json";

    json root;
    std::ifstream config_file(file_path);
    if (!config_file.is_open()) {
        cout << file_path << " not exist" << endl;
        // root = R"({"json_id": 9638})"_json;
        root = R"(
            {
                "teacher_name": "lwk",
                "student_list": [
                    {"id": 9638},
                    {"id": 96380}
                ]
            }
        )"_json;
    } else {
        try {
            config_file >> root;
        } catch (exception& e) {
            cout << e.what() << endl;
            config_file.close();
            return;
        }
    }
    config_file.close();
    ofstream os(file_path);
    if (!os.is_open()) {
        cerr << "open file failed" << endl;
        return;
    }
    os << root;
    os.close();

    // Student s(9638);
    // to_json(root, s);
    // root = s;
    cout << root << endl;
    class_room s1 = root.template get<class_room>();
    for (Student& stu : s1.student_list) {
        stu.print();
    }
    return;
}

int main(int argc, char** argv) {
    Student* s = new Student(1);
    s->print();
    delete s;

    json_test();
    return 0;
}