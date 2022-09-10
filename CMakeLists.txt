cmake_minimum_required(VERSION 3.16)

project(wmm VERSION 0.1 LANGUAGES CXX) # Writing Materials Manager (WMM)

# basic compilation options
set(CMAKE_INCLUDE_CURRENT_DIR ON)   # Automatically add the current source and build directories to the include path (without subdirectories).
set(CMAKE_AUTOMOC ON)               # CMake will handle the Qt Meta-Object Compiler (MOC) automatically, i.e. without having to use commands like QT4_WRAP_CPP(), QT5_WRAP_CPP(), etc.
set(CMAKE_AUTOUIC ON)               # CMake will handle the Qt User Interface Compiler (UIC) automatically, i.e. without having to use commands like QT4_WRAP_UI(), QT5_WRAP_UI(), etc.
set(CMAKE_AUTORCC ON)               # CMake will handle the Qt Resource Compiler (RCC) automatically, i.e. without having to use commands like QT4_ADD_RESOURCES(), QT5_ADD_RESOURCES(), etc.
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# print system info (both host and target platform)
message(STATUS "Host System: ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_HOST_SYSTEM_VERSION}")
message(STATUS "Target System: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION}")

# print messages of compilers used
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C Compiler ABI: ${CMAKE_C_COMPILER_ABI}")
message(STATUS "C Compiler Architecture ID: ${CMAKE_C_COMPILER_ARCHITECTURE_ID}")
message(STATUS "C Compiler ID: ${CMAKE_C_COMPILER_ID}")
message(STATUS "C Compiler Version: ${CMAKE_C_COMPILER_VERSION}")

message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Compiler ABI: ${CMAKE_CXX_COMPILER_ABI}")
message(STATUS "C++ Compiler Architecture ID: ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

# list files to be compiled
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.c*
    ${PROJECT_SOURCE_DIR}/test/*.c*
)
file(GLOB_RECURSE HEADERS
    ${PROJECT_SOURCE_DIR}/src/*.h*
    ${PROJECT_SOURCE_DIR}/test/*.h*
)
file(GLOB_RECURSE QT_WIDGETS_UIS
    ${PROJECT_SOURCE_DIR}/src/*.ui
    ${PROJECT_SOURCE_DIR}/test/*.ui
)

# set variables
set(CODE_FILES
    ${SOURCES}
    ${HEADERS}
    ${QT_WIDGETS_UIS}
)
set(TS_FILES # translation files (Qt)
    wmm_en_US.ts
)
set(DEP_ROOT "3rd/install/${CMAKE_CXX_COMPILER_ID}")

# includes
include_directories(appwmm PUBLIC "src")

# dependencies: Qt
find_package(Qt6 6.3 COMPONENTS Core Gui Widgets Core5Compat PdfWidgets REQUIRED)
set(Qt6_ROOT "${Qt6_DIR}/../../..")

qt_add_executable(appwmm
    ${CODE_FILES}
)

set_target_properties(appwmm PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER sw.abr.top
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(appwmm PUBLIC $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(appwmm PUBLIC Qt6::Core Qt6::Widgets Qt6::Core5Compat Qt6::PdfWidgets)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(appwmm)
endif ()

# dependency: mongocxx
list(APPEND MONGO_LIB_DIR "${DEP_ROOT}/mongo-c-driver-1.21.0/${CMAKE_BUILD_TYPE}/bin")
list(APPEND MONGO_LIB_DIR "${DEP_ROOT}/mongo-c-driver-1.21.0/${CMAKE_BUILD_TYPE}/lib")
list(APPEND MONGO_LIB_DIR "${DEP_ROOT}/mongo-cxx-driver-r3.6.6/${CMAKE_BUILD_TYPE}/bin")
list(APPEND MONGO_LIB_DIR "${DEP_ROOT}/mongo-cxx-driver-r3.6.6/${CMAKE_BUILD_TYPE}/lib")
list(APPEND MONGO_INCLUDE_DIR "${DEP_ROOT}/mongo-cxx-driver-r3.6.6/${CMAKE_BUILD_TYPE}/include/bsoncxx/v_noabi")
list(APPEND MONGO_INCLUDE_DIR "${DEP_ROOT}/mongo-cxx-driver-r3.6.6/${CMAKE_BUILD_TYPE}/include/mongocxx/v_noabi")
find_library(BSON bson-1.0 ${MONGO_LIB_DIR})
find_library(MONGOC mongoc-1.0 ${MONGO_LIB_DIR})
find_library(BSONCXX bsoncxx ${MONGO_LIB_DIR})
find_library(MONGOCXX mongocxx ${MONGO_LIB_DIR})
list(APPEND MONGO_LIBS ${BSON})
list(APPEND MONGO_LIBS ${MONGOC})
list(APPEND MONGO_LIBS ${BSONCXX})
list(APPEND MONGO_LIBS ${MONGOCXX})
target_link_libraries(appwmm LINK_PUBLIC ${MONGO_LIBS})
include_directories(appwmm PUBLIC ${MONGO_INCLUDE_DIR})
message(STATUS "Found MongoDB dependencies: ${MONGO_LIBS}")

# dependency: JSON libs
find_library(SIMDJSON simdjson "${DEP_ROOT}/simdjson-1.0.2/${CMAKE_BUILD_TYPE}/lib")
list(APPEND JSON_INCLUDE_DIR "${DEP_ROOT}/simdjson-1.0.2/${CMAKE_BUILD_TYPE}/include")
list(APPEND JSON_INCLUDE_DIR "${DEP_ROOT}/rapidjson-1.1.0/${CMAKE_BUILD_TYPE}/include")
list(APPEND JSON_LIBS ${SIMDJSON})
target_link_libraries(appwmm LINK_PUBLIC ${JSON_LIBS})
include_directories(appwmm PUBLIC ${JSON_INCLUDE_DIR})
message(STATUS "Found JSON dependencies: ${JSON_LIBS}")

# dependency: DuckX
find_library(DUCKX duckx "${DEP_ROOT}/duckx-1.2.2/${CMAKE_BUILD_TYPE}/lib")
target_link_libraries(appwmm LINK_PUBLIC ${DUCKX})
include_directories(appwmm PUBLIC ${DEP_ROOT}/duckx-1.2.2/${CMAKE_BUILD_TYPE}/include)
message(STATUS "Found DOCX dependencies: ${DUCKX}")

# post-build
# if((${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows") AND (${CMAKE_SYSTEM_NAME} STREQUAL "Windows"))
#     add_custom_command(TARGET appwmm POST_BUILD
#         COMMAND "./post-build" -QtBinaryDir ${Qt6_ROOT}/bin -BuildDir ${PROJECT_BINARY_DIR} -BuildType ${CMAKE_BUILD_TYPE}
#         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
#     )
# elseif((${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux") AND (${CMAKE_SYSTEM_NAME} STREQUAL "Linux"))
#     # TODO
# else()
#     message(WARNING "You're building on unsupported platforms. At present you should compile this software directly on the target machine. That is, cross-compilation is not supported yet.")
# endif()
message(STATUS "Note: Remember to run post-build script at the root directory of this software.")
