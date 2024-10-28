cmake_minimum_required(VERSION 3.15)
project(Student)
message(STATUS "lwk " ${CMAKE_CURRENT_SOURCE_DIR})

# 必须要加上PARENT_SCOPE，否则，父CMakeLists.txt拿不到student_src变量
# SET(student_src ${CMAKE_CURRENT_SOURCE_DIR}/Student.cpp PARENT_SCOPE)

set(student_src ${CMAKE_CURRENT_SOURCE_DIR}/Student.cpp)
add_library(student STATIC ${student_src})
target_include_directories(student PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})