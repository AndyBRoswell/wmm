#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "duckx::duckx" for configuration "RelWithDebInfo"
set_property(TARGET duckx::duckx APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(duckx::duckx PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "C;CXX"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/duckx.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS duckx::duckx )
list(APPEND _IMPORT_CHECK_FILES_FOR_duckx::duckx "${_IMPORT_PREFIX}/lib/duckx.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
