cmake_minimum_required(VERSION 3.15)
project(GA)
# set(pybind11_DIR /home/admin/software/miniconda3/lib/python3.12/site-packages/pybind11/share/cmake/pybind11)
# set(PYTHON_EXECUTABLE "/home/admin/software/miniconda3/bin/python")

message(STATUS "lwk " ${CMAKE_CURRENT_SOURCE_DIR})

# 必须要加上PARENT_SCOPE，否则，父CMakeLists.txt拿不到student_src变量
# SET(student_src ${CMAKE_CURRENT_SOURCE_DIR}/Student.cpp PARENT_SCOPE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb")

set(ga_src 
    ${CMAKE_CURRENT_SOURCE_DIR}/GA.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/organism.cc
)
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
add_executable(ga ${ga_src})
target_include_directories(ga PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})

# interface for pybind11
# find_package(pybind11 REQUIRED)
# pybind11_add_module(stu ${CMAKE_CURRENT_SOURCE_DIR}/PyStudent.cpp)
# target_link_libraries(stu PRIVATE student)