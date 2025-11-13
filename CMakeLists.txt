cmake_minimum_required(VERSION 3.16)
project(iniconfig VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure installation directories
include(GNUInstallDirs)

# Option to build shared or static library
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Option to build tests
option(BUILD_TESTING "Build tests" ON)

add_subdirectory(src)

# Add tests if enabled
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()