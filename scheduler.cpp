#include <iostream> // For standard input and output
#include <vector>   // For dynamic arrays (task storage)
#include <string>   // For handling text (task titles, dates, and times)
#include <cstdlib>  // For system() to execute Notifu commands
#include <thread>   // For running reminders in the background
#include <chrono>   // For delays (checking tasks periodically)
#include <ctime>    // For getting the current system date and time

// Task structure to hold individual task details
struct Task {
    std::string title; // Task name or description
    std::string date;  // Task date in YYYY-MM-DD format
    std::string time;  // Task time in HH:MM (24-hour) format
};

// Main Scheduler class
class Scheduler {
private:
    std::vector<Task> tasks;      // Stores all added tasks
    bool reminderRunning = false; // Tracks whether the reminder system is active

    // Function to get the current time in HH:MM format
    std::string getCurrentTime() const {
        time_t now = time(0);          // Get the current system time
        tm* localTime = localtime(&now); // Convert to local time format (tm structure)

        char buffer[6];               // Buffer to store formatted time
        strftime(buffer, sizeof(buffer), "%H:%M", localTime); // Format time as HH:MM
        return std::string(buffer);   // Return time as a string
    }

    // Function to get the current date in YYYY-MM-DD format
    std::string getCurrentDate() const {
        time_t now = time(0);          // Get the current system time
        tm* localTime = localtime(&now); // Convert to local time format (tm structure)

        char buffer[11];              // Buffer to store formatted date
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", localTime); // Format date as YYYY-MM-DD
        return std::string(buffer);   // Return date as a string
    }

    // Function to send a desktop notification using Notifu
    void sendNotification(const std::string& title, const std::string& message) const {
    // Command to call Notifu
    std::string command = "notifu /m \"" + message + "\" /p \"" + title + "\" /d 5";

    // Execute the Notifu command
    system(command.c_str());

    // Inform that the notification was sent (optional)
    std::cout << "Notification sent successfully.\n";
}


    // Real-time reminder system running in the background
    void startRealTimeReminder() {
        if (reminderRunning) return; // Prevent multiple threads
        reminderRunning = true;

        // Run the reminder system in a separate thread
        std::thread([this]() {
            while (reminderRunning) {
                std::string currentDate = getCurrentDate(); // Get current system date
                std::string currentTime = getCurrentTime(); // Get current system time

                // Check each task to see if it matches the current date and time
                for (const auto& task : tasks) {
                    if (task.date == currentDate && task.time == currentTime) {
                        // Notify the user about the task
                        sendNotification(task.title, "Scheduled for now!");
                        std::cout << "\n Reminder: " << task.title
                                  << " is scheduled now (" << task.date << " " << task.time << ").\n";

                        // Prevent duplicate notifications for the same task
                        std::this_thread::sleep_for(std::chrono::minutes(1));
                    }
                }

                // Check tasks every 30 seconds
                std::this_thread::sleep_for(std::chrono::seconds(30));
            }
        }).detach(); // Detach the thread so it runs independently
    }

public:
    // Function to add a new task to the task list
    void addTask(const std::string& title, const std::string& date, const std::string& time) {
        tasks.push_back({title, date, time}); // Add task to the vector
        std::cout << "Task added successfully!\n";

        // Automatically start the reminder system if it's not running
        if (!reminderRunning) {
            startRealTimeReminder();
        }
    }

    // Function to display all stored tasks
    void displayTasks() const {
        if (tasks.empty()) { // Check if no tasks are stored
            std::cout << "No tasks scheduled.\n";
            return;
        }

        std::cout << "Scheduled Tasks:\n";
        for (size_t i = 0; i < tasks.size(); ++i) {
            std::cout << i + 1 << ". Title: " << tasks[i].title
                      << ", Date: " << tasks[i].date
                      << ", Time: " << tasks[i].time << "\n";
        }
    }
};

int main() {
    Scheduler scheduler; // Create an instance of the Scheduler class
    int choice;          // User's menu choice

    do {
        // Display the menu
        std::cout << "\nScheduler Menu:\n"
                  << "1. Add Task\n"
                  << "2. View Tasks\n"
                  << "3. Exit\n"
                  << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::cin.ignore(); // Clear the input buffer
            std::string title, date, time;
            std::cout << "Enter task title: ";
            std::getline(std::cin, title);
            std::cout << "Enter task date (YYYY-MM-DD): ";
            std::getline(std::cin, date);
            std::cout << "Enter task time (HH:MM): ";
            std::getline(std::cin, time);
            scheduler.addTask(title, date, time); // Add the new task
            break;
        }
        case 2:
            scheduler.displayTasks(); // Display all tasks
            break;
        case 3:
            std::cout << "Exiting the program. Goodbye!\n";
            return 0; // Exit the program
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (true); // Loop until the user chooses to exit
}
