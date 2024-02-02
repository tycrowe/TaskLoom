//
// Created by me on 2/2/2024.
//

#ifndef DATABASECLIENT_H
#define DATABASECLIENT_H

#include <string>
#include <nlohmann/json.hpp>

class DatabaseClient {
public:
    // Constructor: Optionally initializes the database file path
    explicit DatabaseClient(const std::string& dbPath = "");

    // Checks if the database file exists at the given path or the member path
    bool checkIfDatabaseExists(const std::string& dbPath = "") const;

    // Creates a database at the given path or the member path, and initializes a table
    int createDatabase(const std::string& dbPath = "");

    // Inserts a new todo into the database
    int insertTodo(const nlohmann::json& json);

    // Gets all todos from the database
    nlohmann::json getTodos();

    // Gets a single todo from the database
    nlohmann::json getTodoById(int id);

    // Gets a single todo from the database by title
    nlohmann::json getTodoByTitle(const std::string& title);

private:
    std::string dbFilePath; // Path to the database file
};

#endif // DATABASECLIENT_H

