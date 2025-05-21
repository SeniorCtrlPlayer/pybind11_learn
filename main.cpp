#include "Student.h"
#include <fstream>
#include <stack>

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

enum last_type {
    PRIMITIVE,
    OBJECT_START,
    OBJECT_END,
    ARRAY_START,
    ARRAY_END,
    KEY
};

class SaxWriter {
public:
    // SaxWriter(const string& file_name, int max_depth_ = 10) {
    //     o = std::ofstream(file_name);
    //     if (!o.is_open()) {
    //         cerr << "open file failed" << endl;
    //         return;
    //     }
    //     max_depth = max_depth_;
    // }
    SaxWriter(ostream& out, int max_depth_ = 10) : o(out), max_depth(max_depth_) {
        // o = out;
        // max_depth = max_depth_;
    }
    // SaxWriter(std::ostream& out) : o(out) {}
    ~SaxWriter() {
        // o.close();
        // assert(last_value.empty());
    }

    bool null() {
        if (last_state == PRIMITIVE) o << ',';
        o << "null"; return true;}
    bool boolean(bool value) {
        if (last_state == PRIMITIVE) o << ',';
        o << (value ? "true" : "false"); last_state=PRIMITIVE; return true;}
    bool number_integer(json::number_integer_t value) {
        if (last_state == PRIMITIVE) o << ',';
        o << value; last_state=PRIMITIVE; return true;}
    bool number_unsigned(json::number_unsigned_t value) {
        if (last_state == PRIMITIVE) o << ',';
        o << value; last_state=PRIMITIVE; return true;}
    bool number_float(json::number_float_t value, const std::string & /*unused*/) {
        if (last_state == PRIMITIVE) o << ',';
        o << value; last_state=PRIMITIVE; return true;}
    bool string(const std::string &value) {
        if (last_state == PRIMITIVE) o << ',';
        o << "\"" << value << "\""; last_state=PRIMITIVE; return true;}
    bool binary(std::vector<std::uint8_t> & /*unused*/) {last_state=PRIMITIVE; return true;}

    bool start_object() {
        // depth++;
        switch (last_state)
        {
        case OBJECT_END:
            // o << ',';
            print_comma();
            print_tab();
            break;
        case ARRAY_END:
            print_comma();
            print_tab();
            // o << ',';
            break;
        case ARRAY_START:
            // print_comma();
            print_tab();
            break;
        case KEY:
            break;
        default:
            break;
        }
        o << "{";
        // o << depth;
        last_state = last_type::OBJECT_START;
        return true;
    }

    bool key(const std::string &key) {
        // if (last_state>PRIMITIVE)
        //     depth++;
        switch (last_state)
        {
        case PRIMITIVE:
            print_comma();
            print_tab();
            break;
        case OBJECT_START:
            depth++;
            print_tab();
            break;
        case ARRAY_END:
            print_comma();
            print_tab();
            break;
        case OBJECT_END:
            print_comma();
            print_tab();
            break;
        default:
            break;
        }
        // print_comma();
        // print_tab();
        o << "\"" << key << "\": ";
        last_state = KEY;
        return true;
    }

    bool end_object() {
        if (depth == max_depth) {
            print_tab();
            depth--;
        } else {
            depth--;
            print_tab();
        }
        o << "}";
        last_state = last_type::OBJECT_END;
        return true;
    }

    bool start_array(bool tab = false) {
        switch (last_state)
        {
        case ARRAY_END:
            print_comma();
            depth++;
            print_tab();
            break;
        case ARRAY_START:
            depth++;
            print_tab();
            break;
        case KEY:
            depth++;
            break;
        default:
            break;
        }
        o << "[";
        // depth++;
        last_state = last_type::ARRAY_START;
        return true;
    }

    bool end_array(bool tab = false) {
        depth--;
        switch (last_state)
        {
        case ARRAY_END:
            print_tab();
            break;
        case OBJECT_END:
            // print_comma();
            print_tab();
            break;
        default:
            break;
        }
        o << "]";
        // last_value.back() = last_type::ARRAY_END;
        last_state = last_type::ARRAY_END;
        return true;
    }

    bool parse_error(std::size_t /*unused*/, const std::string & /*unused*/, const json::exception & /*unused*/) {
        return false;
    }

private:
    std::vector<last_type> last_value = {last_type::OBJECT_START}; // 标记每步值是否为基础类型或者非基础类型
    int max_depth;
    int depth = 0;
    std::ostream& o;
    last_type last_state = last_type::OBJECT_START;

public:
    // 是否要打印 [',', ' ', '\n'] 由下个字符决定

    void print_comma() {
        o << ',';
    }

    void print_tab() {
        if (depth >= max_depth)
            return;
        o << std::endl;
        for (int i=0;i<depth; i++) {
            o << "  ";
        }
    }
};

// 定义栈帧结构体
struct Frame {
    const json* node;   // 当前 JSON 节点
    bool visited;       // 是否已访问过
    std::string key;    // 对象成员的键名，数组元素或根节点为空
};

template<class Sax>
void sax_event_creator_nr(const json &root, Sax *sax) {
    std::stack<Frame> st;
    // 将根节点入栈，标记为“未访问”
    st.push({&root, false, ""});
    // st.emplace(&root, false, std::string(""));

    while (!st.empty()) {
        // pop 出栈顶帧
        Frame frame = st.top();  
        st.pop();
        const json* node = frame.node;

        // 如果是原子值（非 object/array），且未访问，则直接分发一次
        if (!node->is_object() && !node->is_array()) {
            if (!frame.key.empty()) sax->key(frame.key);

            if (node->is_number_integer()) sax->number_integer(node->get<json::number_integer_t>());
            else if (node->is_number_unsigned()) sax->number_unsigned(node->get<json::number_unsigned_t>());
            else if (node->is_number_float())    sax->number_float(node->get<json::number_float_t>(), "");
            else if (node->is_string())          sax->string(node->get_ref<const json::string_t&>());
            else if (node->is_boolean())         sax->boolean(node->get<bool>());
            else /* null */                      sax->null();
            continue;
        }

        // 容器类型（object 或 array）
        bool is_obj = node->is_object();

        if (!frame.visited) {
            // 首次访问：触发 start_*
            if (!frame.key.empty()) sax->key(frame.key);
            if (is_obj)       sax->start_object();
            else /* array */  sax->start_array(false);

            // 将自己标记为“已访问”重新入栈，以便稍后触发 end_*
            st.push(Frame{frame.node, true, frame.key});

            // 以深度优先顺序反序压入所有子节点
            if (is_obj) {
                for (json::const_reverse_iterator rit = node->rbegin(); rit != node->rend(); ++rit) {
                    st.push(Frame{&*rit, false, rit.key()});
                }
            } else {
                // 数组元素：倒序入栈
                for (json::const_reverse_iterator rit = node->rbegin(); rit != node->rend(); ++rit) {
                    st.push(Frame{&*rit, false, ""});
                }
            }
        }
        else {
            // 二次访问：触发 end_*
            if (is_obj)       sax->end_object();
            else /* array */  sax->end_array(false);
        }
    }
}

class RouteLog {
public:
    int log_id;
    std::vector<std::vector<unsigned int>> multi_fanout_path;
    RouteLog() = default;
    RouteLog(int log_id_, std::vector<std::vector<unsigned int>>&& multi_fanout_path_) : log_id(log_id_), multi_fanout_path(std::move(multi_fanout_path_)) {}
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(RouteLog, log_id, multi_fanout_path)
};
class RouteNet {
public:
    std::string net_name;
    std::vector<RouteLog> route_logs;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(RouteNet, net_name, route_logs)
};
class RouteNetLogs {
public:
    std::map<std::string, RouteNet> route_net_map;
    std::map<int, std::string> comments;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(RouteNetLogs, route_net_map, comments)
};

void sax_test() {
    json j;
    j["foo"][2] = false;
    j["bar"]["number"] = 10;
    j["bar"]["null"] = nullptr;
    j["booltest"] = true;
    j["bar1"] = R"(
        [{
            "a": "b"
        },{
            "b": 1
        }]
    )"_json;
    j["2d"] = R"(
        [[1,2,3],
         [2,3,4]]
    )"_json;

    // RouteLog rlog1 = RouteLog(1, {{1, 2, 3}, {2,3,4}});
    // RouteLog rlog2 = RouteLog(2, {{1, 2, 3}, {2,3,4}});
    // RouteLog rlog3 = RouteLog(3, {{1, 2, 3}, {2,3,4}});
    auto rpath1 = std::vector<std::vector<unsigned int>>({{1, 2, 3}, {2,3,4}});
    auto rpath2 = std::vector<std::vector<unsigned int>>({{1, 2, 3}, {2,3,4}});
    auto rpath3 = std::vector<std::vector<unsigned int>>({{1, 2, 3}, {2,3,4}});
    
    RouteNet rnet1;
    rnet1.net_name = "r1";
    rnet1.route_logs.emplace_back(1, std::move(rpath1));
    rnet1.route_logs.emplace_back(2, std::move(rpath2));
    RouteNet rnet2;
    rnet2.net_name = "r2";
    rnet2.route_logs.emplace_back(3, std::move(rpath3));
    RouteNetLogs r;
    r.route_net_map["r1"] = rnet1;
    r.route_net_map["r2"] = rnet2;
    json rjson;
    rjson["checkpoint"] = r;

    // SaxWriter p(std::cout);
    // SaxWriter p("sax.json", 8);
    std::cout << std::unitbuf;
    SaxWriter p(std::cout, 6);

    sax_event_creator_nr(rjson, &p);
    cout << endl;
}

int main(int argc, char** argv) {
    // Student* s = new Student(1);
    // s->print();
    // delete s;

    // json_test();

    sax_test();
    return 0;
}