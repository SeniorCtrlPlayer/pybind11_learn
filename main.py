import sys
import os
if __name__ == "__main__":
    current_abs_path = os.getcwd()
    # print(os.path.join(current_abs_path, "build/Student"))
    sys.path.append(os.path.join(current_abs_path, "build/Student"))
    from stu import Student
    a = Student(9638)
    a.print()