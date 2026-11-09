cmake_minimum_required(VERSION 3.16)

project(project41 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

qt_add_executable(project41
    src/main.cpp

    src/ui/mainwindow.h
    src/ui/mainwindow.cpp
    src/ui/mainwindow.ui

    src/ui/algorithmpreviewrow.h
    src/ui/algorithmpreviewrow.cpp
    src/ui/algorithmpreviewrow.ui

    src/ui/frameview.h
    src/ui/frameview.cpp

    src/processing/blendinterpolator.h
    src/processing/blendinterpolator.cpp

    src/runners/riferunner.h
    src/runners/riferunner.cpp

    src/ui/videoinputdialog.h
    src/ui/videoinputdialog.cpp
    src/ui/videoinputdialog.ui
)

target_include_directories(project41 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processing
)

target_link_libraries(project41 PRIVATE
    Qt6::Widgets
)