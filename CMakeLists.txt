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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

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

message(STATUS "Source Dir: ${CMAKE_SOURCE_DIR}")
message(STATUS "Binary Dir: ${CMAKE_BINARY_DIR}")

# dependencies: Qt
find_package(Qt6 6.3 COMPONENTS Core Gui Widgets Core5Compat PdfWidgets REQUIRED)

add_subdirectory(src)

message(STATUS "Note: Remember to run post-build script at the root directory of this software.")
