# Task Loom

Task Loom is a super lightweight and basic task creation and management system designed to provide a straightforward interface for managing tasks efficiently. Built in C++, it leverages a modular architecture comprising a backend database client and a command-line interface for user interactions.

## Features

- **Modular Design**: Split into backend and CLI components for ease of maintenance and extension.
- **Database Integration**: Utilizes a custom database client for task management.
- **Interactive CLI**: Offers an intuitive command-line interface for task operations.

## Installation

To install Task Loom, ensure you have CMake installed on your system for building the project. Follow these steps:

1. Clone the repository to your local machine.
2. Navigate to the project directory.
3. Run `cmake .` to generate the build system.
4. Build the project with `make`.

## Usage

After installation, you can run Task Loom by executing the built application. When running the application, 
the user needs to provide a directory for the `.db` file to ensure proper database functionality. The CLI 
provides various commands to create and view tasks directly from your terminal.

Refer to the CLI help for more details on available commands.

## Contributing

Contributions to Task Loom are welcome! If you have suggestions for improvements or bug fixes, please feel free to fork the repository, make changes, and submit a pull request.

## License

This project is open-source and available under the MIT License. See the LICENSE file in the repository for more details.
