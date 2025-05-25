# Coda

**Coda** is a lightweight, cross-platform text editor written in modern C++ and Qt. It aims to provide a clean, fast, and extensible editing experience with a focus on code and plain text.

---

## Features

- Open, edit, and save text files
- Clean Qt-based GUI
- Cross-platform: Linux, macOS, Windows (via Qt)
- Written in C++20 with a modular, extensible architecture

---

## Build Instructions

### Prerequisites

- **C++20 compiler** (e.g., GCC 11+, Clang 12+, MSVC 2019+)
- **CMake 3.16+**
- **Qt 5** (Widgets module)

### Build Steps

```bash
# Clone the repository
git clone https://github.com/DRo21/Coda.git
cd Coda

# Create a build directory
mkdir build
cd build

# Configure and build
cmake ..
make

# Run the application
./Coda
