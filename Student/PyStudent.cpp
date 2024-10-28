#include <pybind11/pybind11.h>
#include "Student.h"

namespace py = pybind11;

PYBIND11_MODULE(stu, m) {
    py::class_<Student>(m, "Student")
        .def(py::init<int>())
        .def("setId", &Student::setId)
        .def("print", &Student::print);
}