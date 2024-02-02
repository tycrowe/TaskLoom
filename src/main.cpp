#include <iostream>
#include <map>

#include "backend/DatabaseClient.h"
#include "cli/TaskLoomInterface.h"

/**
 * \brief This program is a CLI for a database for managing basic TODOs.
 * - The program will use the command line interface to interact with the user.
 * - And then use the DatabaseClient to interact with the database, returning the results to the user.
 * - The program runs using a main loop that will continue to prompt the user for input until the user exits the program.
 * - Data will be stored in an SQLite database, found in the data/ directory.
 * \return The exit code of the program.
 */
int main(int argc, char* argv[]) {
    const char* big_text[] = {
            "TTTTT    A     SSS   K   K  L       OOO    OOO   M   M  ",
            "  T     A A   S      K  K   L      O   O  O   O  MM MM  ",
            "  T    AAAAA   SSS   KKK    L      O   O  O   O  M M M  ",
            "  T    A   A      S  K  K   L      O   O  O   O  M   M  ",
            "  T    A   A  SSSS   K   K  LLLLL   OOO    OOO   M   M  "
    };

    // Print each line of the "Big Text"
    for (auto & i : big_text) {
        std::cout << i << std::endl;
    }

    std::string databasePath = argv[1];

    // Create an instance of the database client
    auto* dbClient = new DatabaseClient(databasePath);

    if (dbClient->checkIfDatabaseExists()) {
        dbClient->createDatabase();
    }

    auto* taskCli = new TaskLoomInterface(*dbClient);

    while (true) {
        const int rc = taskCli->getCommand();
    }


    return 0;
}