# pybind11_learn

## cmake记录

- `` 添加 SAX event-based printer，自定义打印格式，对于由基本元素组成的数组实现不换行打印。
- `9fbfb92` json 嵌套对象与 c++ 嵌套对象的转换。
- `6d0fd74` 使用 nlohmann json 自带的宏，进行 json 对象与 c++ 对象的互转，包含私有成员及公共成员，因为 nlohmann json 的 template get 方法调用的是无参构造方法，然后再通过宏生成的友元函数 from_json 进行成员变量的赋值，所以必须要有 default 无参构造方法(自定义也可, 但不建议)。
- `4d3fff6` 添加 nlohmann json 库，json 对象与 c++ 对象互转示例，不包含私有成员变量。
- `dd372ec` 将 Student 类与 pybind11 绑定，wrapper 成 python 的接口。需注意 Student 编译成静态库时，需要加上 -fPIC 参数。
- `4b74b3d` 使用生成动态库的嵌套 cmake 项目。
- `f26f89e` 使用生成静态库的嵌套 cmake 项目。
- `475d19d` 创建了一个包含嵌套 cmake 的项目，与之前大致相同，子cmake 将变量传递给 父cmake，并没有生成静态或动态库。
- `2e1c60d` 使用 cmake 创建了一个基本的项目，类及函数库与main函数在同一级目录，没有区分。