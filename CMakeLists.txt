cmake_minimum_required(VERSION 3.15)
project(main)
ADD_SUBDIRECTORY(Student)

# message(STATUS "lwk " ${student_src})
add_executable(main main.cpp ${student_src})
target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Student)
# message(STATUS "lwk " ${CMAKE_CURRENT_SOURCE_DIR}/Student)