# Add the executable
add_executable(task-loom main.cpp backend/DatabaseClient.cpp
        cli/TaskLoomInterface.h
        cli/TaskLoomInterface.cpp)

# Include directories
include_directories(include)


# Link libraries if needed (e.g., SQLite3)
target_link_libraries(task-loom PRIVATE unofficial::sqlite3::sqlite3)
