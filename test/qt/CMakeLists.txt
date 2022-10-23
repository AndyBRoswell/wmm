# list files to be compiled
file(GLOB_RECURSE cat1-sources
    cat1/*.c*
)
file(GLOB_RECURSE cat1-headers
    cat1/*.h*
)
file(GLOB_RECURSE cat2-sources
    cat2/*.c*
)
file(GLOB_RECURSE cat2-headers
    cat2/*.h*
)
file(GLOB modules_to_be_tested
    ${wmm_root}/src/TreeView.cpp
)

# dependency: Qt
find_package(Qt6 6.3 COMPONENTS Test REQUIRED)

qt_add_executable(wmmqtest-cat1
    ${cat1-sources}
    ${cat1-headers}
    ${modules_to_be_tested}
)
qt_add_executable(wmmqtest-cat2
    ${cat2-sources}
    ${cat2-headers}
    ${modules_to_be_tested}
)

set_target_properties(wmmqtest-cat1 PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
target_compile_definitions(wmmqtest-cat1 PUBLIC $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_include_directories(wmmqtest-cat1 PUBLIC
    ${wmm_root}
    ${JSON_inc_dir}     # dependency: JSON libs
    ${mongo_inc_dir}    # dependency: mongo libs
)
target_link_libraries(wmmqtest-cat1 PUBLIC
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Test
)

set_target_properties(wmmqtest-cat2 PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
target_compile_definitions(wmmqtest-cat2 PUBLIC $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_include_directories(wmmqtest-cat2 PUBLIC
    ${wmm_root}
    ${JSON_inc_dir}     # dependency: JSON libs
    ${mongo_inc_dir}    # dependency: mongo libs
)
target_link_libraries(wmmqtest-cat2 PUBLIC
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Test
)
