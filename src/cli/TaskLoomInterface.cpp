// File: TaskLoomInterface.cpp
/***
* Create by Tyler on 2/2/2024
 *
 * Purpose: This file contains the implementation of the DatabaseClient class. It is responsible for interacting with
 * the SQLite database. It will create the database if it does not exist, and insert data into the database.
 */
#include "TaskLoomInterface.h"

#include <iostream>
#include <nlohmann/json.hpp>

// Now, we want to create a function that will prompt the user for input and return the input.
int TaskLoomInterface::getCommand() const {
    std::string input;
    std::getline(std::cin, input); // Use getline to read the full line

    // Convert input to lowercase
    std::transform(input.begin(), input.end(), input.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    // Determine if the command is valid using the 'commands' dictionary.
    if (commands.contains(input)) {
        if (input == "exit"){
            std::cout << "Exiting program..." << std::endl;
            exit(0);
        }

        if (input == "help"){
            std::cout << "Commands: " << std::endl;
            for (auto & command : commands) {
                std::cout << command.first << " - " << command.second << std::endl;
            }
        }

        if (input == "create"){
            TaskLoomInterface::createTodo(nlohmann::json());
            return 0;
        }

        if (input == "list"){
            TaskLoomInterface::listTodos();
            return 0;
        }

        if (input == "get"){
            // Ask the user if they know the id or title
            std::string title;
            std::string id;

            std::cout << "Do you know the title of the todo? (yes/no): ";
            std::string knowTitle;
            std::getline(std::cin, knowTitle);
            bool knowsTitle = knowTitle == "yes" || knowTitle == "Yes" || knowTitle == "y" || knowTitle == "Y" || knowTitle == "1";

            if (knowsTitle) {
                std::cout << "Enter the title of the todo: ";
                std::getline(std::cin, title);
            } else {
                std::cout << "Enter the id of the todo: ";
                std::getline(std::cin, id);
            }

            TaskLoomInterface::getTodo(title, id);

            return 0;
        }
    } else {
        std::cout << "Invalid command. Type 'help' for a list of commands." << std::endl;
    }

    return -1;
}

void TaskLoomInterface::createTodo(const nlohmann::json& todoData) const {
    std::string title;
    std::string description;
    std::string due_date;
    std::string completed;

    std::cout << "Enter the title of the todo: ";
    std::getline(std::cin, title); // Use getline for input with spaces
    std::cout << "Enter the description of the todo: ";
    std::getline(std::cin, description);
    std::cout << "Enter the due date of the todo: ";
    std::getline(std::cin, due_date);
    std::cout << "Is the todo completed? (true/false): ";
    std::getline(std::cin, completed);

    // Convert the string to a boolean
    bool isCompleted = completed == "true" || completed == "True" || completed == "t" || completed == "T" || completed == "1";

    nlohmann::json j;
    j["title"] = title.empty() ? "null" : title;
    j["description"] = description.empty() ? "null" : description;
    j["due_date"] = due_date.empty() ? "null" : due_date;
    j["completed"] = isCompleted;

    // Insert the item into the database
    int rc = dbClient.insertTodo(j);

    if (rc == 0) {
        std::cout << "Todo created successfully!" << std::endl << std::endl << std::endl;
    } else {
        std::cerr << "Error creating todo." << std::endl << std::endl << std::endl;
    }
}

void TaskLoomInterface::listTodos() const {
    nlohmann::json todos = dbClient.getTodos();

    if (todos.empty()) {
        std::cout << "No todos found." << std::endl;
    } else {
        std::cout << "Todos: " << std::endl;
        for (auto & todo : todos) {
            std::cout << "ID: " << todo["id"] << std::endl;
            std::cout << "Title: " << todo["title"] << std::endl;
            std::cout << "Description: " << todo["description"] << std::endl;
            std::cout << "Due Date: " << todo["due_date"] << std::endl;
            std::cout << "Completed: " << todo["completed"] << std::endl;
            std::cout << std::endl;
        }
    }
}

void TaskLoomInterface::getTodo(const std::string& title, const std::string id) const {
    nlohmann::json todo;

    if (!title.empty()) {
        todo = dbClient.getTodoByTitle(title);
    } else if (!id.empty()) {
        todo = dbClient.getTodoById(std::stoi(id));
    }

    if (todo.empty()) {
        std::cout << "No todo found." << std::endl;
    } else {
        std::cout << "Todo: " << std::endl;
        std::cout << "ID: " << todo["id"] << std::endl;
        std::cout << "Title: " << todo["title"] << std::endl;
        std::cout << "Description: " << todo["description"] << std::endl;
        std::cout << "Due Date: " << todo["due_date"] << std::endl;
        std::cout << "Completed: " << todo["completed"] << std::endl << std::endl << std::endl;
    }
}
