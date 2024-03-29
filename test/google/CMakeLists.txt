# list files to be compiled
file(GLOB_RECURSE sources
    *.c*
)
file(GLOB_RECURSE headers
    *.h*
)
file(GLOB modules_to_be_tested
    ${wmm_root}/src/Algorithm.cpp
    ${wmm_root}/src/FileSystemAccessor.cpp
    ${wmm_root}/src/JSONFormatter.cpp
    ${wmm_root}/src/MongoDBAccessor.cpp
)

# set variables
set(code_files
    ${sources}
    ${headers}
    ${modules_to_be_tested}
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# dependency: GoogleTest
list(APPEND CMAKE_PREFIX_PATH "${dep_root}/googletest-main/${CMAKE_BUILD_TYPE}")
find_package(GTest CONFIG REQUIRED)

enable_testing()

qt_add_executable(wmmgtest
    ${code_files}
)

# target configuration
set_target_properties(wmmgtest PROPERTIES
    # MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" # C/C++ > Code Generation > Runtime Library = /MTd instead of default /MDd
    MACOSX_BUNDLE_GUI_IDENTIFIER sw.abr.top
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    # WIN32_EXECUTABLE TRUE
)
target_compile_definitions(wmmgtest PUBLIC $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_include_directories(wmmgtest PUBLIC
    ${wmm_root}
    ${wmm_root}/test
    ${JSON_inc_dir}
    ${mongo_inc_dir}    # dependency: mongo libs
)
target_link_libraries(wmmgtest PUBLIC
    ${mongo_libs}
    GTest::gtest_main
    Qt6::Core Qt6::Core5Compat
)

include(GoogleTest)
# gtest_discover_tests(wmmgtest)
