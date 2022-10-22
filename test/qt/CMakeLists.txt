# list files to be compiled
file(GLOB_RECURSE sources
    *.c*
)
file(GLOB_RECURSE headers
    *.h*
)
file(GLOB modules_to_be_tested

)

set(code_files
    ${sources}
    ${headers}
    ${modules_to_be_tested}
)

