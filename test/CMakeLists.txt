# list files to be compiled
file(GLOB_RECURSE sources
    *.c*
)
file(GLOB_RECURSE headers
    *.h*
)

# set variables
set(code_files
    ${sources}
    ${headers}
)
list(FILTER code_files EXCLUDE REGEX "/legacy/")
# message(STATUS "code_files in test: ${code_files}")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# dependency: GoogleTest
list(APPEND CMAKE_PREFIX_PATH "../3rd/install/${CMAKE_CXX_COMPILER_ID}/googletest-release-1.12.1/${CMAKE_BUILD_TYPE}")
find_package(GTest CONFIG REQUIRED)

enable_testing()

add_executable(wmmtest
    ${code_files}
)
set_target_properties(wmmtest PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" # C/C++ > Code Generation > Runtime Library = /MTd instead of default /MDd
)

target_link_libraries(wmmtest PUBLIC GTest::gtest_main Qt6::Core)

include(GoogleTest)
gtest_discover_tests(wmmtest)
