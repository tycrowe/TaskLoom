//
// Created by me on 2/2/2024.
//

#ifndef COMMANDLINEINTERFACE_H
#define COMMANDLINEINTERFACE_H
#include <map>
#include <string>
#include <nlohmann/json.hpp> // Include this for nlohmann::json
#include <iostream>
#include "../backend/DatabaseClient.h"

class TaskLoomInterface {
    std::map<std::string, std::string> commands;
    DatabaseClient& dbClient; // Reference to a DatabaseClient instance

public:
    // Constructor takes a reference to DatabaseClient
    explicit TaskLoomInterface(DatabaseClient& dbClient) : dbClient(dbClient) {
        commands = {
                {"help", "Display this help message"},
                {"create", "Create a new todo"},
                {"list", "List all todos"},
                {"get", "Get a todo by title or id"},
                {"update", "Update a todo"},
                {"delete", "Delete a todo"},
                {"exit", "Exit the program"}
        };
        std::cout << "Welcome, please enter a command or use 'help' to get started." << std::endl;
    }

    [[nodiscard]] int getCommand() const;

private:
    void createTodo(const nlohmann::json& todoData) const; // Ensure the parameter is passed correctly

    void listTodos() const;

    void getTodo(const std::string& title, const std::string id) const;
};


#endif //COMMANDLINEINTERFACE_H
