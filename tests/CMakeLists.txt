# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Create test executable
add_executable(iniconfig_tests
    test_iniconfig.cpp
)

# Link against the library and Google Test
target_link_libraries(iniconfig_tests
    PRIVATE
        iniconfig::iniconfig
        GTest::gtest_main
)

# Copy test INI files to build directory
configure_file(test_valid.ini test_valid.ini COPYONLY)
configure_file(test_comments.ini test_comments.ini COPYONLY)
configure_file(test_whitespace.ini test_whitespace.ini COPYONLY)
configure_file(test_malformed.ini test_malformed.ini COPYONLY)

# Discover tests
include(GoogleTest)
gtest_discover_tests(iniconfig_tests)
