#include <iostream>
#include <map>
#include <string>
#include <numeric>

class Student {
private:
    std::string name;
    int id;
    std::map<std::string, int> grades; // Course name as key, grade as value

public:
    // Constructor
    Student(std::string name, int id) : name(name), id(id) {}

    // Add a grade for a specific course
    void addGrade(const std::string& courseName, int grade) {
        grades[courseName] = grade;
    }

    // Get grades for the student
    const std::map<std::string, int>& getGrades() const {
        return grades;
    }

    // Calculate the average grade
    double calculateAverage() const {
        if (grades.empty()) return 0.0;
        int total = std::accumulate(grades.begin(), grades.end(), 0,
            [](int sum, const std::pair<std::string, int>& p) { return sum + p.second; });
        return static_cast<double>(total) / grades.size();
    }

    // Getters
    std::string getName() const { return name; }
    int getId() const { return id; }

    // Serialization method (saving to file)
    friend std::ostream& operator<<(std::ostream& os, const Student& student) {
        os << student.name << "\n" << student.id << "\n";
        os << student.grades.size() << "\n";
        for (const auto& grade : student.grades) {
            os << grade.first << " " << grade.second << "\n";
        }
        return os;
    }

    // Deserialization method (loading from file)
    friend std::istream& operator>>(std::istream& is, Student& student) {
        int numGrades;
        is >> student.name >> student.id >> numGrades;
        student.grades.clear();
        for (int i = 0; i < numGrades; ++i) {
            std::string courseName;
            int grade;
            is >> courseName >> grade;
            student.grades[courseName] = grade;
        }
        return is;
    }
};


#include <vector>
#include <fstream>

class GradeBook {
private:
    std::vector<Student> students;

public:
    // Add a new student
    void addStudent(const std::string& name, int id) {
        students.emplace_back(name, id);
    }

    // Find a student by ID
    Student* findStudentById(int id) {
        for (auto& student : students) {
            if (student.getId() == id) {
                return &student;
            }
        }
        return nullptr;
    }

    // Save the student data to a file
    void saveToFile(const std::string& filename) {
        std::ofstream ofs(filename);
        ofs << students.size() << "\n";
        for (const auto& student : students) {
            ofs << student;
        }
    }

    // Load the student data from a file
    void loadFromFile(const std::string& filename) {
        std::ifstream ifs(filename);
        int numStudents;
        ifs >> numStudents;
        students.clear();
        for (int i = 0; i < numStudents; ++i) {
            Student student("", 0);
            ifs >> student;
            students.push_back(student);
        }
    }

    // Display all students and their grades
    void displayAllStudents() const {
        for (const auto& student : students) {
            std::cout << "Student: " << student.getName() << ", ID: " << student.getId() << "\n";
            std::cout << "Grades: \n";
            for (const auto& grade : student.getGrades()) {
                std::cout << grade.first << ": " << grade.second << "\n";
            }
            std::cout << "Average Grade: " << student.calculateAverage() << "\n\n";
        }
    }
};

#include <iostream>
#include <string>

int main() {
    GradeBook gradeBook;
    std::string filename = "students.dat";

    // Load student data from file if available
    std::ifstream infile(filename);
    if (infile.good()) {
        gradeBook.loadFromFile(filename);
    } else {
        std::cout << "No previous data found. Starting fresh.\n";
    }

    int choice;
    while (true) {
        std::cout << "1. Add Student\n";
        std::cout << "2. Add Grade\n";
        std::cout << "3. Display Grades\n";
        std::cout << "4. Calculate Average\n";
        std::cout << "5. Save and Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        if (choice == 1) {
            std::string name;
            int id;
            std::cout << "Enter student name: ";
            std::cin.ignore(); // consume newline
            std::getline(std::cin, name);
            std::cout << "Enter student ID: ";
            std::cin >> id;
            gradeBook.addStudent(name, id);

        } else if (choice == 2) {
            int id;
            std::cout << "Enter student ID: ";
            std::cin >> id;
            Student* student = gradeBook.findStudentById(id);
            if (student) {
                std::string courseName;
                int grade;
                std::cout << "Enter course name: ";
                std::cin.ignore(); // consume newline
                std::getline(std::cin, courseName);
                std::cout << "Enter grade for " << courseName << ": ";
                std::cin >> grade;
                student->addGrade(courseName, grade);
            } else {
                std::cout << "Student not found.\n";
            }

        } else if (choice == 3) {
            gradeBook.displayAllStudents();

        } else if (choice == 4) {
            int id;
            std::cout << "Enter student ID: ";
            std::cin >> id;
            Student* student = gradeBook.findStudentById(id);
            if (student) {
                std::cout << "Average Grade: " << student->calculateAverage() << "\n";
            } else {
                std::cout << "Student not found.\n";
            }

        } else if (choice == 5) {
            gradeBook.saveToFile(filename);
            std::cout << "Data saved. Exiting.\n";
            break;

        } else {
            std::cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}
