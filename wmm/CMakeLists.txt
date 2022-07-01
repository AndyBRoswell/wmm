cmake_minimum_required(VERSION 3.16)

project(wmm VERSION 0.1 LANGUAGES CXX)

# basic options
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

find_package(Qt6 6.3 COMPONENTS Core Gui Quick QuickControls2 Widgets Core5Compat PdfWidgets REQUIRED)

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

set(CODE_FILES
        ${SOURCES}
        ${HEADERS}
        ${QT_WIDGETS_UIS}
        )

set(TS_FILES
        wmm_en_US.ts
        )

qt_add_executable(appwmm
        ${CODE_FILES}
        )

qt_add_qml_module(appwmm
        URI wmm
        VERSION 1.0
        QML_FILES src/main.qml
        )

set_target_properties(appwmm PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER sw.abr.top
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

target_compile_definitions(appwmm PUBLIC $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(appwmm PUBLIC Qt6::Core Qt6::Quick Qt6::QuickControls2 Qt6::Widgets Qt6::Core5Compat Qt6::PdfWidgets)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(appwmm)
    qt_finalize_executable(appwmm)
endif ()

# includes
include_directories(appwmm PUBLIC "src")

# root paths
set(DEP_ROOT "3rd/install/${CMAKE_CXX_COMPILER_ID}")

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

# dependency: JSON libs
find_library(SIMDJSON simdjson "${DEP_ROOT}/simdjson-1.0.2/${CMAKE_BUILD_TYPE}/lib")
list(APPEND JSON_INCLUDE_DIR "${DEP_ROOT}/simdjson-1.0.2/${CMAKE_BUILD_TYPE}/include")
list(APPEND JSON_INCLUDE_DIR "${DEP_ROOT}/rapidjson-1.1.0/${CMAKE_BUILD_TYPE}/include")
list(APPEND JSON_LIBS ${SIMDJSON})
target_link_libraries(appwmm LINK_PUBLIC ${JSON_LIBS})
include_directories(appwmm PUBLIC ${JSON_INCLUDE_DIR})

# dependency: DuckX
find_library(DUCKX duckx "${DEP_ROOT}/duckx-1.2.2/${CMAKE_BUILD_TYPE}/lib")
target_link_libraries(appwmm LINK_PUBLIC ${DUCKX})
include_directories(appwmm PUBLIC ${DEP_ROOT}/duckx-1.2.2/${CMAKE_BUILD_TYPE}/include)

# copy essential data
file(COPY "py" DESTINATION "${PROJECT_BINARY_DIR}")
file(COPY "src/main.qml" DESTINATION "${PROJECT_BINARY_DIR}/src")
file(COPY "src/thm" DESTINATION "${PROJECT_BINARY_DIR}/src")
file(COPY "test/data" DESTINATION "${PROJECT_BINARY_DIR}/test")
file(COPY "test/test-data" DESTINATION "${PROJECT_BINARY_DIR}/test")
