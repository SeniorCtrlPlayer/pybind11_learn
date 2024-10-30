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

class SaxWriter {
public:
    SaxWriter(const string& file_name) {
        o = std::ofstream(file_name);
        if (!o.is_open()) {
            cerr << "open file failed" << endl;
            return;
        }
    }
    // SaxWriter(std::ostream& out) : o(out) {}
    ~SaxWriter() {
        o.close();
        assert(last_value.empty());
    }

    bool null() {
        print_comma();
        o << "null";
        return true;
    }

    bool boolean(bool value) {
        print_comma();
        o << (value ? "true" : "false");
        return true;
    }

    bool number_integer(json::number_integer_t value) {
        print_comma();
        o << value;
        return true;
    }

    bool number_unsigned(json::number_unsigned_t value) {
        print_comma();
        o << value;
        return true;
    }

    bool number_float(json::number_float_t value, const std::string & /*unused*/) {
        print_comma();
        o << value;
        return true;
    }

    bool string(const std::string &value) {
        print_comma();
        o << "\"" << value << "\"";
        return true;
    }

    bool binary(std::vector<std::uint8_t> & /*unused*/) {
        return true;
    }

    bool start_object(std::size_t /*unused*/) {
        print_comma();
        o << "{";
        last_value.push_back(1);
        // print_tab();
        return true;
    }

    bool key(const std::string &key) {
        print_comma();
        last_value.back() = 1; // do not print a comma when printing the value following this key
        print_tab();
        o << "\"" << key << "\": ";
        return true;
    }

    bool end_object() {
        last_value.pop_back();
        print_tab();
        o << "}";
        return true;
    }

    bool start_array(bool tab = false) {
        print_comma();
        o << "[";
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
        o << "]";
        return true;
    }

    bool parse_error(std::size_t /*unused*/, const std::string & /*unused*/, const json::exception & /*unused*/) {
        return false;
    }

private:
    // std::vector<bool> first_value;
    std::vector<int> last_value;
    std::ofstream o;
    // std::ostream& o;

public:
    void print_tab() {
        o << endl;
        for (int i=0;i<last_value.size();i++) {
            o << "  ";
        }
    }

    void print_comma() {
        if (!last_value.empty()) {
            switch (last_value.back())
            {
                case 0:
                    o << ",";
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
void sax_event_creator_nr(const json &j, Sax *sax) {
    auto handle_primitive_value = [](json::const_iterator it, Sax *sax) {
        switch (it.m_object->type()) {
            case nlohmann::json::value_t::number_integer: {
                sax->number_integer(it->template get<json::number_integer_t>());
                break;
            }

            case nlohmann::detail::value_t::number_unsigned: {
                sax->number_unsigned(it->template get<json::number_unsigned_t>());
                break;
            }

            case nlohmann::detail::value_t::number_float: {
                sax->number_float(it->template get<json::number_float_t>(), "");
                break;
            }

            case nlohmann::detail::value_t::null: {
                sax->null();
                break;
            }

            case nlohmann::detail::value_t::string: {
                sax->string(it->template get_ref<const json::string_t &>());
                break;
            }

            case nlohmann::detail::value_t::boolean: {
                sax->boolean(it->template get<const json::boolean_t>());
                break;
            }

            default:
                break;
        }
    };

    if (j.is_primitive()) {
        handle_primitive_value(j.cbegin(), sax);
        return;
    }

    std::vector<json::const_iterator> m_stack;
    m_stack.push_back(j.cbegin());

    while (!m_stack.empty()) {
        auto &it = m_stack.back();
        assert(it.m_object->is_structured());
        const auto is_object = it.m_object->is_object();
        std::size_t size = false;

        // start before the first value
        if (it == it.m_object->cbegin()) {
            size = it.m_object->size();
            // is_object ? sax->start_object(size) : sax->start_array(size);
            if (is_object) {
                sax->start_object(size);
            } else {
                bool tab = size && (it.m_object->cbegin()->is_object() || it.m_object->cbegin()->is_array());
                sax->start_array(tab);
            }
        }

        // end after the last value
        if (it == it.m_object->end()) {
            size = it.m_object->size();
            if (is_object) {
                sax->end_object();
            } else {
                bool tab = size && (it.m_object->cbegin()->is_object() || it.m_object->cbegin()->is_array());
                sax->end_array(tab);
            }
            m_stack.pop_back();
            continue;
        }

        // handle the current value
        if (is_object) {
            sax->key(it.key());
        }
        auto it_current = it++->cbegin();
        if (it_current.m_object->is_primitive()) {
            handle_primitive_value(it_current, sax);
        } else {
            m_stack.push_back(it_current);
        }
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

    
    // SaxWriter p(std::cout);
    SaxWriter p("sax.json");

    sax_event_creator_nr(j, &p);
    cout << endl;
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