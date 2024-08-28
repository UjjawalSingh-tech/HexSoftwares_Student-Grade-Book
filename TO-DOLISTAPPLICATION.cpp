#include <iostream>
#include <vector>
#include <fstream>
#include <string>

class ToDoList {
private:
    std::vector<std::pair<std::string, bool>> tasks; // Task and its completion status

public:
    // Add a new task
    void addTask(const std::string& task) {
        tasks.push_back(std::make_pair(task, false)); // New task is not completed
    }

    // Display all tasks
    void displayTasks() const {
        if (tasks.empty()) {
            std::cout << "No tasks in the list.\n";
            return;
        }

        std::cout << "TO-DO LIST:\n";
        for (size_t i = 0; i < tasks.size(); ++i) {
            std::cout << i + 1 << ". " << tasks[i].first << (tasks[i].second ? " [DONE]" : "") << "\n";
        }
    }

    // Mark a task as done
    void markTaskDone(int index) {
        if (index > 0 && index <= tasks.size()) {
            tasks[index - 1].second = true;
            std::cout << "Task " << index << " marked as done.\n";
        } else {
            std::cout << "Invalid task number.\n";
        }
    }

    // Delete a task
    void deleteTask(int index) {
        if (index > 0 && index <= tasks.size()) {
            tasks.erase(tasks.begin() + index - 1);
            std::cout << "Task " << index << " deleted.\n";
        } else {
            std::cout << "Invalid task number.\n";
        }
    }

    // Save tasks to a file
    void saveToFile(const std::string& filename) const {
        std::ofstream ofs(filename);
        for (const auto& task : tasks) {
            ofs << task.first << "\n" << task.second << "\n";
        }
        std::cout << "Tasks saved to " << filename << ".\n";
    }

    // Load tasks from a file
    void loadFromFile(const std::string& filename) {
        std::ifstream ifs(filename);
        if (!ifs.is_open()) {
            std::cout << "No saved tasks found.\n";
            return;
        }

        tasks.clear();
        std::string task;
        bool isDone;
        while (std::getline(ifs, task)) {
            ifs >> isDone;
            ifs.ignore(); // Ignore the newline after reading isDone
            tasks.push_back(std::make_pair(task, isDone));
        }
        std::cout << "Tasks loaded from " << filename << ".\n";
    }
};

int main() {
    ToDoList todoList;
    std::string filename = "tasks.txt";

    // Load tasks from file if available
    todoList.loadFromFile(filename);

    int choice;
    while (true) {
        std::cout << "\n1. Add Task\n2. Display Tasks\n3. Mark Task as Done\n4. Delete Task\n5. Save and Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        if (choice == 1) {
            std::cin.ignore(); // Ignore the newline left by std::cin
            std::string task;
            std::cout << "Enter task: ";
            std::getline(std::cin, task);
            todoList.addTask(task);

        } else if (choice == 2) {
            todoList.displayTasks();

        } else if (choice == 3) {
            int taskNumber;
            std::cout << "Enter task number to mark as done: ";
            std::cin >> taskNumber;
            todoList.markTaskDone(taskNumber);

        } else if (choice == 4) {
            int taskNumber;
            std::cout << "Enter task number to delete: ";
            std::cin >> taskNumber;
            todoList.deleteTask(taskNumber);

        } else if (choice == 5) {
            todoList.saveToFile(filename);
            break;

        } else {
            std::cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
