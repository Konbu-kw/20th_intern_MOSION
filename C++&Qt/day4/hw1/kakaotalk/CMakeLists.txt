cmake_minimum_required(VERSION 3.16)
project(KakaoUDPChat VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

# 소스 및 헤더
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
)

set(HEADERS
    include/mainwindow.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include 경로
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Qt 라이브러리 링크
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)
