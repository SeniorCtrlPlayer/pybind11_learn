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
    os << root.dump(2);
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

class SaxPrinter {
public:
    ~SaxPrinter() {
        assert(last_value.empty());
    }

    bool null() {
        print_comma();
        std::cout << "null";
        return true;
    }

    bool boolean(bool value) {
        print_comma();
        std::cout << (value ? "true" : "false");
        return true;
    }

    bool number_integer(json::number_integer_t value) {
        print_comma();
        std::cout << value;
        return true;
    }

    bool number_unsigned(json::number_unsigned_t value) {
        print_comma();
        std::cout << value;
        return true;
    }

    bool number_float(json::number_float_t value, const std::string & /*unused*/) {
        print_comma();
        std::cout << value;
        return true;
    }

    bool string(const std::string &value) {
        print_comma();
        std::cout << "\"" << value << "\"";
        return true;
    }

    bool binary(std::vector<std::uint8_t> & /*unused*/) {
        return true;
    }

    bool start_object(std::size_t /*unused*/) {
        print_comma();
        std::cout << "{";
        last_value.push_back(1);
        // print_tab();
        return true;
    }

    bool key(const std::string &key) {
        print_comma();
        last_value.back() = 1; // do not print a comma when printing the value following this key
        print_tab();
        std::cout << "\"" << key << "\": ";
        return true;
    }

    bool end_object() {
        last_value.pop_back();
        print_tab();
        std::cout << "}";
        return true;
    }

    bool start_array(bool tab = false) {
        print_comma();
        std::cout << "[";
        last_value.push_back(1);
        if (tab) {
            print_tab();
        }
        return true;
    }

    bool end_array(bool tab = false) {
        last_value.pop_back();
        if (tab) {
            print_tab();
        }
        std::cout << "]";
        return true;
    }

    bool parse_error(std::size_t /*unused*/, const std::string & /*unused*/, const json::exception & /*unused*/) {
        return false;
    }

    std::vector<bool> first_value;
    std::vector<int> last_value;

public:
    void print_tab() {
        cout << endl;
        for (int i=0;i<last_value.size();i++) {
            cout << "  ";
        }
    }

    void print_comma() {
        if (!last_value.empty()) {
            switch (last_value.back())
            {
                case 0:
                    cout << ",";
                    break;
                case 1:
                    last_value.back() = 0;
                    break;
                default:
                    break;
            }
        }
    }
};

template<class Sax>
void sax_event_creator(const json &j, Sax *sax) {
    switch (j.type()) {
        case nlohmann::detail::value_t::null: {
            sax->null();
            break;
        }

        case nlohmann::detail::value_t::object: {
            sax->start_object(j.size());
            for (auto &element: j.items()) {
                sax->key(element.key());
                sax_event_creator(element.value(), sax);
            }
            sax->end_object();
            break;
        }

        case nlohmann::detail::value_t::array: {
            bool tab = j.size() && (j[0].is_object() || j[0].is_array());
            sax->start_array(tab);
            for (auto &element: j) {
                sax_event_creator(element, sax);
            }
            sax->end_array(tab);
            break;
        }

        case nlohmann::detail::value_t::string: {
            sax->string(j.get_ref<const json::string_t&>());
            break;
        }

        case nlohmann::detail::value_t::boolean: {
            sax->boolean(j.get<json::boolean_t>());
            break;
        }

        case nlohmann::detail::value_t::number_integer: {
            sax->number_integer(j.get<json::number_integer_t>());
            break;
        }

        case nlohmann::detail::value_t::number_unsigned: {
            sax->number_unsigned(j.get<json::number_unsigned_t>());
            break;
        }

        case nlohmann::detail::value_t::number_float: {
            sax->number_float(j.get<json::number_float_t>(), "");
            break;
        }

        case nlohmann::detail::value_t::binary:
        case nlohmann::detail::value_t::discarded:
            break;
    }
}

void sax_test() {
    json j;
    j["foo"][2] = false;
    j["bar"]["number"] = 10;
    j["bar"]["null"] = nullptr;
    j["bar1"] = R"(
        [{
            "a": "b"
        },{
            "b": 1
        }]
    )"_json;

    // std::cout << j << std::endl;
    SaxPrinter p;

    sax_event_creator(j, &p);
    std::cout << std::endl;

    // json::sax_parse(R"([[],[{},{}],{}])", &p);
    // std::cout << std::endl;
}


int main(int argc, char** argv) {
    Student* s = new Student(1);
    s->print();
    delete s;

    json_test();

    sax_test();
    return 0;
}