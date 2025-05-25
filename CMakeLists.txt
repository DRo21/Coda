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

# Find packages
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Lua REQUIRED)
find_package(KF5SyntaxHighlighting REQUIRED)

# Add include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/syntax
    ${LUA_INCLUDE_DIR}
)

# Add sol2 (header-only library)
include(FetchContent)
FetchContent_Declare(
    sol2
    GIT_REPOSITORY https://github.com/ThePhD/sol2.git
    GIT_TAG        v3.3.0
)
FetchContent_MakeAvailable(sol2)

# Source files
set(SOURCES
    src/main.cpp
    src/core/MainWindow.cpp
    src/core/EditorWidget.cpp
    src/core/ScriptingEngine.cpp
    src/syntax/KSyntaxHighlightingAdapter.cpp
    src/core/PluginManager.cpp
)

# Header files (for clarity)
set(HEADERS
    src/core/MainWindow.h
    src/core/EditorWidget.h
    src/core/ScriptingEngine.h
    src/syntax/KSyntaxHighlightingAdapter.h
    src/core/PluginManager.h
    include/IPlugin.h
    include/ISyntaxHighlighter.h
)

# Add the executable
add_executable(Coda ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(Coda
    PRIVATE
        Qt5::Widgets
        sol2::sol2
        ${LUA_LIBRARIES}
        KF5::SyntaxHighlighting
)

# For Linux: Add pthread if needed
if(UNIX)
    target_link_libraries(Coda PRIVATE pthread)
endif()

# Install the Coda binary
install(TARGETS Coda DESTINATION bin)

# Install the plugins.json and plugins folder
install(FILES plugins.json DESTINATION share/coda)

install(DIRECTORY plugins/ DESTINATION share/coda)
