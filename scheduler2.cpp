#include <iostream> // For standard input and output
#include <vector>   // For dynamic arrays (task storage)
#include <string>   // For handling text (task titles, dates, and times)
#include <cstdlib>  // For system() to execute Notifu commands
#include <thread>   // For running reminders in the background
#include <chrono>   // For delays (checking tasks periodically)
#include <ctime>    // For getting the current system date and time
#include <regex>    // For validating time format
#include <fstream>  // For file logging and saving/loading tasks
#include <algorithm> // For sorting tasks

// Task structure to hold individual task details
struct Task {
    std::string title; // Task name or description
    std::string date;  // Task date in DD/MM/YYYY format
    std::string time;  // Task time in HH:MM (24-hour format)

    // Comparator for sorting tasks
    bool operator<(const Task& other) const {
        if (date == other.date) {
            return time < other.time;
        }
        return date < other.date;
    }
};

// Main Scheduler class
class Scheduler {
private:
    std::vector<Task> tasks;      // Stores all added tasks

    // Function to convert date format from DD/MM/YYYY to YYYY/MM/DD for sorting
    std::string convertDateForSorting(const std::string& date) const {
        return date.substr(6, 4) + "/" + date.substr(3, 2) + "/" + date.substr(0, 2);
    }

    // Sort tasks chronologically by date and time
    void sortTasks() {
        std::sort(tasks.begin(), tasks.end(), [this](const Task& a, const Task& b) {
            std::string aDate = convertDateForSorting(a.date);
            std::string bDate = convertDateForSorting(b.date);
            if (aDate == bDate) {
                return a.time < b.time;
            }
            return aDate < bDate;
        });
    }

    // Save tasks to a file
    void saveTasksToFile() const {
        std::ofstream outFile("tasks.txt");
        if (!outFile) {
            std::cerr << "Error: Could not open tasks.txt for writing.\n";
            return;
        }
        for (const auto& task : tasks) {
            outFile << task.title << "," << task.date << "," << task.time << "\n";
        }
        outFile.close();
    }

    // Load tasks from a file
    void loadTasksFromFile() {
        std::ifstream inFile("tasks.txt");
        if (!inFile) {
            std::cerr << "Warning: Could not open tasks.txt. Starting with no tasks.\n";
            return;
        }
        tasks.clear();
        std::string line;
        while (std::getline(inFile, line)) {
            size_t firstComma = line.find(',');
            size_t secondComma = line.find(',', firstComma + 1);
            if (firstComma != std::string::npos && secondComma != std::string::npos) {
                std::string title = line.substr(0, firstComma);
                std::string date = line.substr(firstComma + 1, secondComma - firstComma - 1);
                std::string time = line.substr(secondComma + 1);
                tasks.push_back({title, date, time});
            }
        }
        inFile.close();
        sortTasks();
    }

    // Function to send a desktop notification using Notifu
    void sendNotification(const std::string& title, const std::string& message) const {
        // Command to call Notifu
        std::string command = "notifu /m \"" + message + "\" /p \"" + title + "\" /d 5";

        // Execute the Notifu command
        system(command.c_str());
    }

    // Real-time reminder system to check tasks periodically
    void startRealTimeReminder() {
        std::thread([this]() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::seconds(30)); // Check every minute

                // Get current date and time
                time_t now = time(0);
                tm* localTime = localtime(&now);
                char dateBuffer[11], timeBuffer[6];
                strftime(dateBuffer, sizeof(dateBuffer), "%d/%m/%Y", localTime);
                strftime(timeBuffer, sizeof(timeBuffer), "%H:%M", localTime);

                std::string currentDate = dateBuffer;
                std::string currentTime = timeBuffer;

                // Check tasks
                for (const auto& task : tasks) {
                    if (task.date == currentDate && task.time == currentTime) {
                        sendNotification(task.title, "Scheduled for now!");
                    }
                }
            }
        }).detach(); // Run this thread in the background
    }

public:
    // Constructor to load tasks from file and start the reminder system
    Scheduler() {
        loadTasksFromFile();
        startRealTimeReminder();
    }

    // Destructor to save tasks to file before exiting
    ~Scheduler() {
        saveTasksToFile();
    }

    // Function to add a new task to the task list
    void addTask() {
        std::cin.ignore(); // Clear the input buffer

        // Prompt user for task details
        std::string title, date, time;
        std::cout << "Enter task title: ";
        std::getline(std::cin, title);
        std::cout << "Enter task date (DD/MM/YYYY): ";
        std::getline(std::cin, date);
        time = promptValidTime(); // Prompt user until valid time is entered

        tasks.push_back({title, date, time}); // Add task to the vector
        sortTasks(); // Sort tasks after adding a new one
        saveTasksToFile(); // Save updated tasks to file
        std::cout << "\nTask added successfully!\n";
    }

    // Function to delete a task by its index
    void deleteTask(size_t index) {
        if (index == 0 || index > tasks.size()) {
            std::cout << "\nError: Invalid task index.\n";
            return;
        }

        tasks.erase(tasks.begin() + (index - 1)); // Remove the task from the vector
        saveTasksToFile(); // Save updated tasks to file
        std::cout << "\nTask deleted successfully.\n";
    }

    // Function to display all stored tasks
    void displayTasks() const {
        if (tasks.empty()) { // Check if no tasks are stored
            std::cout << "\nNo tasks scheduled.\n";
            return;
        }

        std::cout << "\nScheduled Tasks:\n";
        for (size_t i = 0; i < tasks.size(); ++i) {
            std::cout << i + 1 << ". Title: " << tasks[i].title
                      << ", Date: " << tasks[i].date
                      << ", Time: " << tasks[i].time << "\n";
        }
    }

    // Function to prompt user to re-enter time until valid
    std::string promptValidTime() const {
        std::string time;
        while (true) {
            std::cout << "Enter task time (HH:MM): ";
            std::getline(std::cin, time);
            if (isValidTime(time)) {
                break;
            } else {
                std::cout << "Error: Invalid time format. Please use HH:MM (24-hour format).\n";
            }
        }
        return time;
    }

    // Function to validate time format (HH:MM)
    bool isValidTime(const std::string& time) const {
        std::regex timePattern("^([01]?[0-9]|2[0-3]):[0-5][0-9]$");
        return std::regex_match(time, timePattern);
    }
};

int main() {
    Scheduler scheduler; // Create an instance of the Scheduler class
    int choice;          // User's menu choice

    do {
        // Display the menu
        std::cout << "\rScheduler Menu:\n"
                  << "1. Add Task\n"
                  << "2. View Tasks\n"
                  << "3. Delete Task\n"
                  << "4. Exit\n"
                  << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            scheduler.addTask(); // Add a new task
            break;
        case 2:
            scheduler.displayTasks(); // Display all tasks
            break;
        case 3: {
            scheduler.displayTasks(); // Show tasks to help the user choose
            std::cout << "Enter the task number to delete: ";

            std::cout << "(Enter 0 to return to the main menu)\n";
            size_t index;
            std::cin >> index;

            if (index == 0) {
                std::cout << "Returning to the main menu...\n";
                break;
            }

            scheduler.deleteTask(index); // Delete the specified task
            break;
        }
        case 4:
            std::cout << "\nExiting the program. Goodbye!\n";
            return 0; // Exit the program
        default:
            std::cout << "\nInvalid choice. Please try again.\n";
        }
    } while (true); // Loop until the user chooses to exit
}
