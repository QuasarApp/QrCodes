
cmake_minimum_required(VERSION 3.1)

project(SCodes LANGUAGES CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    option (BUILD_SHARED_LIBS "Build and link as shared library" OFF)
else()
    option (BUILD_SHARED_LIBS "Build and link as shared library" ON)
endif()

option (BUILD_EXAMPLES "Build the example barcode reader/writer applicatons" OFF)
option (BUILD_BLACKBOX_TESTS "Build the black box reader/writer tests" OFF)
option (BUILD_UNIT_TESTS "Build the unit tests (don't enable for production builds)" OFF)
add_subdirectory(zxing-cpp)

find_package(QT NAMES Qt5 Qt6 COMPONENTS Core Multimedia Concurrent Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Multimedia Concurrent Quick REQUIRED)

file(GLOB SOURCE_CPP
    "*.cpp"
)

#add_definitions(-DQML_IMPORT_NAME com.scythestudio.scodes)
#add_definitions(-DQML_IMPORT_MAJOR_VERSION 1)


add_library(${PROJECT_NAME} ${SOURCE_CPP})

set_target_properties(${PROJECT_NAME} PROPERTIES
    QT_QML_MODULE_VERSION 1.0
    QT_QML_MODULE_URI     com.scythestudio.scodes
)

target_link_libraries(${PROJECT_NAME} PUBLIC ZXing Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Multimedia Qt${QT_VERSION_MAJOR}::Concurrent Qt${QT_VERSION_MAJOR}::Quick)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/zxing-cpp/thirdparty/stb)

if (ANDROID)

    if (${QT_VERSION} VERSION_LESS 6.0)
        find_package(Qt${QT_VERSION_MAJOR} COMPONENTS AndroidExtras REQUIRED)
        target_link_libraries(${PROJECT_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::AndroidExtras)
    endif()


    target_link_libraries(${PROJECT_NAME} PUBLIC EGL GLESv2)

endif()

