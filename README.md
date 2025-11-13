# iniparserpp

A lightweight, modern C++17 INI file parser library with a simple and forgiving API.

## Features

- **Simple API**: Load INI files and retrieve values with just two methods
- **Forgiving Parser**: Skips malformed lines and continues parsing
- **Comment Support**: Handles full-line and inline comments (`;` and `#`)
- **Header-Only**: Single header and implementation file
- **Modern CMake**: Full CMake package support with `find_package()`
- **Flexible Build**: Build as static or shared library

## INI Format

The parser supports standard INI format:

```ini
; This is a comment
# This is also a comment

key1=value1  ; inline comment

[section1]
key2=value2
key3=value with spaces  # another inline comment

[section2]
host=localhost
port=8080
```

## Installation

### Build and Install

```bash
# Configure
cmake -B build -DCMAKE_INSTALL_PREFIX=/usr/local

# Build
cmake --build build

# Install (may require sudo)
cmake --install build
```

### Build Options

- **`BUILD_SHARED_LIBS`**: Build as shared library (default: OFF for static)
  ```bash
  cmake -B build -DBUILD_SHARED_LIBS=ON
  ```

## Usage

### Basic Example

```cpp
#include <iniConfig.hpp>
#include <iostream>
#include <string>

int main() {
    Config config;
    std::string err;
    
    // Load INI file
    if (!config.loadFromFile("config.ini", err)) {
        std::cerr << "Error: " << err << std::endl;
        return 1;
    }
    
    // Get values with defaults
    std::string host = config.get("database", "host", "localhost");
    std::string port = config.get("database", "port", "5432");
    std::string name = config.get("database", "name", "mydb");
    
    std::cout << "Database: " << name << " at " << host << ":" << port << std::endl;
    
    return 0;
}
```

### Using with CMake (Installed Library)

```cmake
cmake_minimum_required(VERSION 3.16)
project(myapp)

find_package(iniconfig 1.0 REQUIRED)

add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE iniconfig::iniconfig)
```

### Using with CMake (Subdirectory)

```cmake
add_subdirectory(iniparserpp)

add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE iniconfig::iniconfig)
```

## API Reference

### `class Config`

#### Methods

##### `bool loadFromFile(const std::string &path, std::string &err)`

Loads and parses an INI file.

- **Parameters:**
  - `path`: Path to the INI file
  - `err`: Output parameter for error message on failure
- **Returns:** `true` on success, `false` on failure (file not found, cannot open, etc.)
- **Behavior:**
  - Clears any previously loaded configuration
  - Skips empty lines and comment lines (starting with `;` or `#`)
  - Strips inline comments from values
  - Ignores malformed lines without `=`
  - Sections start with `[section_name]`
  - Keys before any section go to an empty-named section

##### `std::string get(const std::string &section, const std::string &key, const std::string &default_val = "") const`

Retrieves a configuration value.

- **Parameters:**
  - `section`: Section name (use empty string `""` for top-level keys)
  - `key`: Key name within the section
  - `default_val`: Value to return if key/section not found (default: empty string)
- **Returns:** The configuration value or `default_val` if not found

## Parser Behavior

- **Whitespace**: Leading and trailing whitespace is trimmed from sections, keys, and values
- **Comments**: 
  - Lines starting with `;` or `#` are ignored
  - Inline comments (`;` or `#` after values) are stripped
- **Sections**: Case-sensitive, defined with `[section_name]`
- **Keys**: Case-sensitive
- **Error Handling**: Parser is forgiving - malformed lines are skipped without failing the entire parse
- **Encoding**: Assumes UTF-8 or ASCII compatible encoding

## Requirements

- **C++17** or later
- **CMake 3.16** or later (for building)

## License

[Add your license here]

## Contributing

[Add contribution guidelines here]

## Version

Current version: **1.0.0**
