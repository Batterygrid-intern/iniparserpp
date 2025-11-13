#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "iniconfig::iniconfig" for configuration ""
set_property(TARGET iniconfig::iniconfig APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(iniconfig::iniconfig PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libiniconfig.a"
  )

list(APPEND _cmake_import_check_targets iniconfig::iniconfig )
list(APPEND _cmake_import_check_files_for_iniconfig::iniconfig "${_IMPORT_PREFIX}/lib/libiniconfig.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
