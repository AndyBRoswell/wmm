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
# message(STATUS "code_files in src: ${code_files}")
set(Qt_ts_Files # Qt translation files
    ts/*.ts
)
set(dep_root "../3rd/install/${CMAKE_CXX_COMPILER_ID}")

# dependencies: Qt
set(Qt6_root "${Qt6_DIR}/../../..")

qt_add_executable(wmm
    ${code_files}
)

set_target_properties(wmm PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER sw.abr.top
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(wmm PUBLIC $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(wmm PUBLIC Qt6::Core Qt6::Widgets Qt6::Core5Compat Qt6::PdfWidgets)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(wmm)
endif ()

# dependency: mongocxx
list(APPEND mongo_inc_dir "${dep_root}/mongo-cxx-driver-r3.6.7/${CMAKE_BUILD_TYPE}/include/bsoncxx/v_noabi")
list(APPEND mongo_inc_dir "${dep_root}/mongo-cxx-driver-r3.6.7/${CMAKE_BUILD_TYPE}/include/mongocxx/v_noabi")
target_include_directories(wmm PUBLIC ${mongo_inc_dir})

list(APPEND mongo_lib_dir "${dep_root}/mongo-c-driver-1.22.1/${CMAKE_BUILD_TYPE}/bin")
list(APPEND mongo_lib_dir "${dep_root}/mongo-c-driver-1.22.1/${CMAKE_BUILD_TYPE}/lib")
list(APPEND mongo_lib_dir "${dep_root}/mongo-cxx-driver-r3.6.7/${CMAKE_BUILD_TYPE}/bin")
list(APPEND mongo_lib_dir "${dep_root}/mongo-cxx-driver-r3.6.7/${CMAKE_BUILD_TYPE}/lib")
find_library(bson bson-1.0 ${mongo_lib_dir})
find_library(mongoc mongoc-1.0 ${mongo_lib_dir})
find_library(bsoncxx bsoncxx ${mongo_lib_dir})
find_library(mongocxx mongocxx ${mongo_lib_dir})
list(APPEND mongo_libs ${bson})
list(APPEND mongo_libs ${mongoc})
list(APPEND mongo_libs ${bsoncxx})
list(APPEND mongo_libs ${mongocxx})
target_link_libraries(wmm LINK_PUBLIC ${mongo_libs})
message(STATUS "Found MongoDB libraries: ${mongo_libs}")

# dependency: JSON libs
list(APPEND JSON_inc_dir "${dep_root}/rapidjson-1.1.0/${CMAKE_BUILD_TYPE}/include")
# list(APPEND JSON_inc_dir "${dep_root}/simdjson-1.0.2/${CMAKE_BUILD_TYPE}/include")
target_include_directories(wmm PUBLIC ${JSON_inc_dir})

# find_library(simdjson simdjson "${dep_root}/simdjson-1.0.2/${CMAKE_BUILD_TYPE}/lib")
list(APPEND JSON_libs ${simdjson})
target_link_libraries(wmm LINK_PUBLIC ${JSON_libs})
message(STATUS "Found JSON libraries: ${JSON_libs}")

# dependency: DuckX
target_include_directories(wmm PUBLIC ${dep_root}/duckx-1.2.2/${CMAKE_BUILD_TYPE}/include)

find_library(DuckX duckx "${dep_root}/duckx-1.2.2/${CMAKE_BUILD_TYPE}/lib")
target_link_libraries(wmm LINK_PUBLIC ${DuckX})
message(STATUS "Found DOCX libraries: ${DuckX}")

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
