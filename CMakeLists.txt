cmake_minimum_required(VERSION 3.15)
project(main)
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb")
ADD_SUBDIRECTORY(Student)

# message(STATUS "lwk " ${student_src})
add_executable(main main.cpp)
target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Student
    ${CMAKE_CURRENT_SOURCE_DIR}/inc)
# message(STATUS "lwk " ${CMAKE_CURRENT_SOURCE_DIR}/Student)
target_link_libraries(main PRIVATE student)