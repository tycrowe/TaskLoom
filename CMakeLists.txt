cmake_minimum_required(VERSION 3.16)
project(task-loom VERSION 1.0 LANGUAGES CXX)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(lib/sqlite)
