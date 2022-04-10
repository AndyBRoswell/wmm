#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "duckx::duckx" for configuration "Release"
set_property(TARGET duckx::duckx APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(duckx::duckx PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/duckx.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS duckx::duckx )
list(APPEND _IMPORT_CHECK_FILES_FOR_duckx::duckx "${_IMPORT_PREFIX}/lib/duckx.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
