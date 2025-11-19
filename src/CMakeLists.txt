# Create library target (STATIC or SHARED based on BUILD_SHARED_LIBS)
add_library(iniconfig iniConfig.cpp)

# Add namespaced alias for consistent usage
add_library(iniconfig::iniconfig ALIAS iniconfig)

# Set target properties
set_target_properties(iniconfig PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER ${PROJECT_SOURCE_DIR}/include/iniConfig.hpp
)

# Configure include directories
target_include_directories(iniconfig
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Install library, headers, and export targets
install(TARGETS iniconfig
    EXPORT iniconfigTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install export targets
install(EXPORT iniconfigTargets
    FILE iniconfigTargets.cmake
    NAMESPACE iniconfig::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/iniconfig
)

# Create and install package config files
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/iniconfigConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/iniconfigConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/iniconfig
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/iniconfigConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/iniconfigConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/iniconfigConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/iniconfig
)