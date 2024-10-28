# pybind11_learn

## cmake记录

- `8568e27` 将 Student 类与 pybind11 绑定，wrapper 成 python 的接口。需注意 Student 编译成静态库时，需要加上 -fPIC 参数。
- `2fb3fd7` 使用生成动态库的嵌套 cmake 项目。
- `f26f89e` 使用生成静态库的嵌套 cmake 项目。
- `475d19d` 创建了一个包含嵌套 cmake 的项目，与之前大致相同，子cmake 将变量传递给 父cmake，并没有生成静态或动态库。
- `2e1c60d` 使用 cmake 创建了一个基本的项目，类及函数库与main函数在同一级目录，没有区分。