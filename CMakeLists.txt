cmake_minimum_required(VERSION 3.16)

project(project41 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

qt_add_executable(project41 main.cpp
    mainwindow.h mainwindow.cpp mainwindow.ui
)

target_link_libraries(project41 PRIVATE Qt6::Widgets)
