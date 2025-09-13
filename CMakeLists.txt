cmake_minimum_required(VERSION 4.0)
project(BBScript_editor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(BBScript_editor main.cpp
        parser/Parser.cpp
        parser/Parser.h
        parser/Struct.cpp
        parser/Struct.h
        parser/Command.cpp
        parser/Command.h
)

target_include_directories(BBScript_editor PRIVATE parser/include)

target_link_libraries(BBScript_editor
        Qt::Core
        Qt::Gui
        Qt::Widgets
)


