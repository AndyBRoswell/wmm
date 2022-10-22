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

# dependency: Qt
find_package(Qt6 6.3 COMPONENTS Test REQUIRED)

qt_add_executable(wmmqtest
    ${code_files}
)

set_target_properties(wmmqtest PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
target_compile_definitions(wmmqtest PUBLIC $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(wmmqtest)
endif ()

target_include_directories(wmmqtest PUBLIC
    ${wmm_root}
    ${JSON_inc_dir}     # dependency: JSON libs
    ${mongo_inc_dir}    # dependency: mongo libs
)
target_link_libraries(wmmqtest PUBLIC
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Test
)
