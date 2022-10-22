# list files to be compiled
file(GLOB_RECURSE sources
    *.c*
)
file(GLOB_RECURSE headers
    *.h*
)
file(GLOB_RECURSE Qt_Widgets_UIs
    *.ui
)

# set variables
set(code_files
    ${sources}
    ${headers}
    ${Qt_Widgets_UIs}
)
set(Qt_ts_Files # Qt translation files
    ts/*.ts
)

# dependency: Qt
find_package(Qt6 6.3 COMPONENTS Gui Widgets Core5Compat PdfWidgets REQUIRED)

qt_add_executable(wmm
    ${code_files}
)

# target configuration
set_target_properties(wmm PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER sw.abr.top
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
target_compile_definitions(wmm PUBLIC $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(wmm)
endif ()

# dependency: DuckX
find_library(DuckX duckx "${dep_root}/duckx-1.2.2/${CMAKE_BUILD_TYPE}/lib")

target_include_directories(wmm PUBLIC
    ${JSON_inc_dir}                                     # dependency: JSON libs
    ${mongo_inc_dir}                                    # dependency: mongo libs
    ${dep_root}/duckx-1.2.2/${CMAKE_BUILD_TYPE}/include # dependency: docx
)
target_link_libraries(wmm PUBLIC
    ${mongo_libs}
    ${DuckX}
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Core5Compat Qt6::PdfWidgets
)

# post-build
# if((${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows") AND (${CMAKE_SYSTEM_NAME} STREQUAL "Windows"))
#     add_custom_command(TARGET wmm POST_BUILD
#         COMMAND "./post-build" -QtBinaryDir ${Qt6_root}/bin -BuildDir ${PROJECT_BINARY_DIR} -BuildType ${CMAKE_BUILD_TYPE}
#         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
#     )
# elseif((${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux") AND (${CMAKE_SYSTEM_NAME} STREQUAL "Linux"))
#     # TODO
# else()
#     message(WARNING "You're building on unsupported platforms. At present you should compile this software directly on the target machine. That is, cross-compilation is not supported yet.")
# endif()
