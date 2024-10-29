#include "Student.h"
#include <fstream>

void json_test() {
    std::string file_path = "./test.json";

    json root;
    std::ifstream config_file(file_path);
    if (!config_file.is_open()) {
        cout << file_path << " not exist" << endl;
        root = R"(
            {
                "json_id": 9638
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

    Student s(9638);
    s.json_id = 960308;
    // to_json(root, s);
    root = s;
    cout << root << endl;
    Student s1 = root.template get<Student>();
    cout << "json id: " << s1.json_id << endl;
    return;
}

int main(int argc, char** argv) {
    Student* s = new Student(1);
    s->print();
    delete s;

    json_test();
    return 0;
}