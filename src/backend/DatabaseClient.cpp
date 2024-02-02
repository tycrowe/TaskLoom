// File: DatabaseClient.cpp
/***
 * Create by Tyler on 2/2/2024
 *
 * Purpose: This file contains the implementation of the DatabaseClient class. It is responsible for interacting with
 * the SQLite database. It will create the database if it does not exist, and insert data into the database.
 */
#include "DatabaseClient.h"
#include <fstream>
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <utility>
#include <nlohmann/json.hpp>

// Constructor
DatabaseClient::DatabaseClient(const std::string& dbPath) : dbFilePath(dbPath) {}

// Check if database exists
bool DatabaseClient::checkIfDatabaseExists(const std::string& dbPath) const {
    std::string pathToCheck = dbPath.empty() ? dbFilePath : dbPath;
    std::cout << "Checking if database exists at: " << pathToCheck << std::endl;
    std::ifstream file(pathToCheck);
    return file.good();
}

// Create the database
int DatabaseClient::createDatabase(const std::string& dbPath) {
    std::string pathToUse = dbPath.empty() ? dbFilePath : dbPath;
    if (!pathToUse.empty()) {
        dbFilePath = pathToUse;
    }

    sqlite3* db;
    int rc = sqlite3_open(dbFilePath.c_str(), &db);

    if (rc != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db); // Ensure database is closed
        return rc;
    }
    std::cout << "Opened database successfully" << std::endl;

    const char* sql = "CREATE TABLE IF NOT EXISTS todos("
                      "id INTEGER PRIMARY KEY,"
                      "title TEXT NOT NULL,"
                      "description TEXT,"
                      "due_date TEXT,"
                      "completed INTEGER"
                      ");";
    char* zErrMsg = nullptr;
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Table created successfully" << std::endl;
    }

    return sqlite3_close(db);
}

int DatabaseClient::insertTodo(const nlohmann::json &json) {
    sqlite3* db;
    int rc = sqlite3_open(dbFilePath.c_str(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return -1; // Indicate failure
    }

    // Prepare the SQL statement
    const char* sql = "INSERT INTO todos (title, description, due_date, completed) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db); // Ensure the database is closed
        return -1;
    }

    // Bind the values to the SQL statement
    if (sqlite3_bind_text(stmt, 1, json["title"].get<std::string>().c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }
    if (sqlite3_bind_text(stmt, 2, json["description"].get<std::string>().c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }
    if (sqlite3_bind_text(stmt, 3, json["due_date"].get<std::string>().c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }
    if (sqlite3_bind_int(stmt, 4, json["completed"].get<bool>()) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }

    // Execute the SQL statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt); // Cleanup
        sqlite3_close(db); // Ensure the database is closed
        return -1;
    }

    sqlite3_finalize(stmt); // Cleanup
    sqlite3_close(db); // Close the database
    return 0;
}

nlohmann::json DatabaseClient::getTodos() {
    sqlite3* db;
    int rc = sqlite3_open(dbFilePath.c_str(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return {}; // Return an empty JSON object
    }

    const char* sql = "SELECT * FROM todos;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db); // Ensure the database is closed
        return {}; // Return an empty JSON object
    }

    nlohmann::json todos = nlohmann::json::array();
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        nlohmann::json todo;
        todo["id"] = sqlite3_column_int(stmt, 0);
        todo["title"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        todo["description"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        todo["due_date"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        todo["completed"] = sqlite3_column_int(stmt, 4);
        todos.push_back(todo);
    }

    sqlite3_finalize(stmt); // Cleanup
    sqlite3_close(db); // Close the database
    return todos;
}

nlohmann::json DatabaseClient::getTodoById(int id) {
    sqlite3* db;
    int rc = sqlite3_open(dbFilePath.c_str(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return {}; // Return an empty JSON object
    }

    const char* sql = "SELECT * FROM todos WHERE id = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db); // Ensure the database is closed
        return {}; // Return an empty JSON object
    }

    if (sqlite3_bind_int(stmt, 1, id) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt); // Cleanup
        sqlite3_close(db); // Ensure the database is closed
        return {}; // Return an empty JSON object
    }

    nlohmann::json todo;
    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        todo["id"] = sqlite3_column_int(stmt, 0);
        todo["title"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        todo["description"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        todo["due_date"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        todo["completed"] = sqlite3_column_int(stmt, 4);
    }

    sqlite3_finalize(stmt); // Cleanup
    sqlite3_close(db); // Close the database
    return todo;
}

nlohmann::json DatabaseClient::getTodoByTitle(const std::string &title) {
    sqlite3* db;
    int rc = sqlite3_open(dbFilePath.c_str(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return {}; // Return an empty JSON object
    }

    const char* sql = "SELECT * FROM todos WHERE title = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db); // Ensure the database is closed
        return {}; // Return an empty JSON object
    }

    if (sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt); // Cleanup
        sqlite3_close(db); // Ensure the database is closed
        return {}; // Return an empty JSON object
    }

    nlohmann::json todo;
    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        todo["id"] = sqlite3_column_int(stmt, 0);
        todo["title"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        todo["description"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        todo["due_date"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        todo["completed"] = sqlite3_column_int(stmt, 4);
    }

    sqlite3_finalize(stmt); // Cleanup
    sqlite3_close(db); // Close the database
    return todo;
}
