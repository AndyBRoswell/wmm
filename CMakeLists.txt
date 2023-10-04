cmake_minimum_required(VERSION 3.16)

# basic compilation options
set(CMAKE_INCLUDE_CURRENT_DIR ON)   # Automatically add the current source and build directories to the include path (without subdirectories).
set(CMAKE_AUTOMOC ON)               # CMake will handle the Qt Meta-Object Compiler (MOC) automatically, i.e. without having to use commands like QT4_WRAP_CPP(), QT5_WRAP_CPP(), etc.
set(CMAKE_AUTOUIC ON)               # CMake will handle the Qt User Interface Compiler (UIC) automatically, i.e. without having to use commands like QT4_WRAP_UI(), QT5_WRAP_UI(), etc.
set(CMAKE_AUTORCC ON)               # CMake will handle the Qt Resource Compiler (RCC) automatically, i.e. without having to use commands like QT4_ADD_RESOURCES(), QT5_ADD_RESOURCES(), etc.
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1)

# If language is enabled by project() or enable_language(), CMAKE_<LANG>_COMPILER_LOADED is 1.
project(wmm VERSION 0.1 LANGUAGES CXX)		# Writing Materials Manager (WMM)
project(wmmgtest VERSION 0.1 LANGUAGES CXX)	# Tests for WMM using Google Test
# project(wmmqtest VERSION 0.1 LANGUAGES CXX) # Tests for WMM using Qt Test

# print system info (both host and target platform)
message(STATUS "Host System: ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_HOST_SYSTEM_VERSION}")
message(STATUS "Target System: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION}")

# print messages of compilers used
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C Compiler ABI: ${CMAKE_C_COMPILER_ABI}")
message(STATUS "C Compiler Architecture ID: ${CMAKE_C_COMPILER_ARCHITECTURE_ID}")
message(STATUS "C Compiler ID: ${CMAKE_C_COMPILER_ID}")
message(STATUS "C Compiler Version: ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "C Compiler Version (Internal): ${CMAKE_C_COMPILER_VERSION_INTERNAL}")

message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Compiler ABI: ${CMAKE_CXX_COMPILER_ABI}")
message(STATUS "C++ Compiler Architecture ID: ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ Compiler Version (Internal): ${CMAKE_CXX_COMPILER_VERSION_INTERNAL}")

message(STATUS "Source Dir: ${CMAKE_SOURCE_DIR}")
message(STATUS "Binary Dir: ${CMAKE_BINARY_DIR}")

# set common variables
set(wmm_root ${CMAKE_CURRENT_SOURCE_DIR})
set(dep_root "${wmm_root}/3rd/install/${CMAKE_CXX_COMPILER_ID}")

# common dependency: Qt
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets Core5Compat PdfWidgets)

# common dependency: JSON libs
list(APPEND JSON_inc_dir "${dep_root}/rapidjson-1.1.0/${CMAKE_BUILD_TYPE}/include")

# common dependency: mongocxx
list(APPEND mongo_inc_dir "${dep_root}/mongo-cxx-driver-r3.7.0/${CMAKE_BUILD_TYPE}/include/bsoncxx/v_noabi")
list(APPEND mongo_inc_dir "${dep_root}/mongo-cxx-driver-r3.7.0/${CMAKE_BUILD_TYPE}/include/mongocxx/v_noabi")
list(APPEND mongo_lib_dir "${dep_root}/mongo-c-driver-1.22.2/${CMAKE_BUILD_TYPE}/bin")
list(APPEND mongo_lib_dir "${dep_root}/mongo-c-driver-1.22.2/${CMAKE_BUILD_TYPE}/lib")
list(APPEND mongo_lib_dir "${dep_root}/mongo-cxx-driver-r3.7.0/${CMAKE_BUILD_TYPE}/bin")
list(APPEND mongo_lib_dir "${dep_root}/mongo-cxx-driver-r3.7.0/${CMAKE_BUILD_TYPE}/lib")
find_library(bson bson-1.0 ${mongo_lib_dir})
find_library(mongoc mongoc-1.0 ${mongo_lib_dir})
find_library(bsoncxx bsoncxx ${mongo_lib_dir})
find_library(mongocxx mongocxx ${mongo_lib_dir})
list(APPEND mongo_libs ${bson})
list(APPEND mongo_libs ${mongoc})
list(APPEND mongo_libs ${bsoncxx})
list(APPEND mongo_libs ${mongocxx})

# common dependency: DuckX
set(duckx_inc_dir ${dep_root}/duckx-1.2.2/${CMAKE_BUILD_TYPE}/include)
find_library(DuckX duckx "${dep_root}/duckx-1.2.2/${CMAKE_BUILD_TYPE}/lib")

add_subdirectory(src)
add_subdirectory(test/google)
add_subdirectory(test/qt)

message(STATUS "Note: Remember to run post-build script at the root directory of this software.")
