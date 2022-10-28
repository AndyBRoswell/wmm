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
file(GLOB_RECURSE cat3-sources
    cat3/*.c*
)
file(GLOB_RECURSE cat3-headers
    cat3/*.h*
)
file(GLOB cat1-modules-to-be-tested
    ${wmm_root}/src/global.cpp
    ${wmm_root}/src/TextArea.cpp
    ${wmm_root}/src/TreeView.cpp
)
file(GLOB cat2-modules-to-be-tested
    ${wmm_root}/src/Algorithm.cpp
    ${wmm_root}/src/FileSystemAccessor.cpp
    ${wmm_root}/src/global.cpp
    ${wmm_root}/src/JSONFormatter.cpp
    ${wmm_root}/src/JSONHighlighter.cpp
    ${wmm_root}/src/QtTreeModel.cpp
    ${wmm_root}/src/TextArea.cpp
    ${wmm_root}/src/TextFormatter.cpp
    ${wmm_root}/src/TextHighlighter.cpp
    ${wmm_root}/src/TreeEditor.cpp
    ${wmm_root}/src/TreeView.cpp
)
file(GLOB cat3-modules-to-be-tested
    ${wmm_root}/src/Algorithm.cpp
    ${wmm_root}/src/DatabaseConsole.cpp
    ${wmm_root}/src/FileSystemAccessor.cpp
    ${wmm_root}/src/global.cpp
    ${wmm_root}/src/JSONFormatter.cpp
    ${wmm_root}/src/JSONHighlighter.cpp
    ${wmm_root}/src/MongoDBConsole.cpp
    ${wmm_root}/src/QtTreeModel.cpp
    ${wmm_root}/src/TextArea.cpp
    ${wmm_root}/src/TextFormatter.cpp
    ${wmm_root}/src/TextHighlighter.cpp
    ${wmm_root}/src/TreeEditor.cpp
    ${wmm_root}/src/TreeView.cpp
)

# dependency: Qt
find_package(Qt6 6.4 COMPONENTS Test REQUIRED)

qt_add_executable(wmmqtest-cat1
    ${cat1-sources}
    ${cat1-headers}
    ${cat1-modules-to-be-tested}
)
qt_add_executable(wmmqtest-cat2
    ${cat2-sources}
    ${cat2-headers}
    ${cat2-modules-to-be-tested}
)
qt_add_executable(wmmqtest-cat3
    ${cat3-sources}
    ${cat3-headers}
    ${cat3-modules-to-be-tested}
)

set_target_properties(wmmqtest-cat1 PROPERTIES
    # WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
target_compile_definitions(wmmqtest-cat1 PUBLIC $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_include_directories(wmmqtest-cat1 PUBLIC
    ${wmm_root}
    ${wmm_root}/src
    ${wmm_root}/test
    ${JSON_inc_dir}     # dependency: JSON libs
    ${mongo_inc_dir}    # dependency: mongo libs
)
target_link_libraries(wmmqtest-cat1 PUBLIC
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Test
)

set_target_properties(wmmqtest-cat2 PROPERTIES
    # WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
target_compile_definitions(wmmqtest-cat2 PUBLIC $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_include_directories(wmmqtest-cat2 PUBLIC
    ${wmm_root}
    ${wmm_root}/src
    ${wmm_root}/test
    ${JSON_inc_dir}     # dependency: JSON libs
    ${mongo_inc_dir}    # dependency: mongo libs
)
target_link_libraries(wmmqtest-cat2 PUBLIC
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Test Qt6::Core5Compat
)

set_target_properties(wmmqtest-cat3 PROPERTIES
    # WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
target_compile_definitions(wmmqtest-cat3 PUBLIC $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_include_directories(wmmqtest-cat3 PUBLIC
    ${wmm_root}
    ${wmm_root}/src
    ${wmm_root}/test
    ${JSON_inc_dir}     # dependency: JSON libs
    ${mongo_inc_dir}    # dependency: mongo libs
)
target_link_libraries(wmmqtest-cat3 PUBLIC
    ${mongo_libs}
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Test Qt6::Core5Compat
)
