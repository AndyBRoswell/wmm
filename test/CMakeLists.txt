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
