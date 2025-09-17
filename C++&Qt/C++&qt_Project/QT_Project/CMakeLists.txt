
cmake_minimum_required(VERSION 3.5)
project(qt_template VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT Qt6_FOUND)
    find_package(Qt6 COMPONENTS Widgets Network REQUIRED)
else()
    find_package(Qt5 COMPONENTS Widgets Network REQUIRED)
endif()

set(SOURCES src/main.cpp src/mainwindow.cpp)
set(HEADERS include/mainwindow.h)
set(UI_FILES ui/mainwindow.ui)

qt6_wrap_cpp(MOC_SOURCES ${HEADERS})
qt6_wrap_ui(UIC_SOURCES ${UI_FILES})

add_executable(qt_template ${SOURCES} ${MOC_SOURCES} ${UIC_SOURCES})

target_include_directories(qt_template PRIVATE include ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(qt_template Qt6::Widgets Qt6::Network)
