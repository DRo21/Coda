# CMake version
cmake_minimum_required(VERSION 3.16)

# Project settings
project(Coda VERSION 0.1 LANGUAGES CXX)

# C++ Standard settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable automatic MOC generation for Qt
set(CMAKE_AUTOMOC ON)

# Find the required Qt6 modules
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Define source files for the project
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/EditorWidget.cpp
)

# Define header files for clarity
set(HEADERS
    src/MainWindow.h
    src/EditorWidget.h
)

# Add the executable target
add_executable(Coda ${SOURCES} ${HEADERS})

# Link the executable with the Qt6 Widgets module
target_link_libraries(Coda PRIVATE Qt6::Widgets)