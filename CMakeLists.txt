cmake_minimum_required(VERSION 4.0)
project(BBScript_editor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(depends)
# I got no idea how cmake works lol
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(BBScript_editor src/main.cpp
        src/core/Parser.cpp
        src/core/Parser.h
        src/core/Struct.cpp
        src/core/Struct.h
        src/core/Command.cpp
        src/core/Command.h
        src/core/Editor.cpp
        src/core/Editor.h
        src/core/PieceTable.cpp
        src/core/PieceTable.h
        depends/whereami/whereami.c
)

target_include_directories(BBScript_editor PRIVATE depends)

target_link_libraries(BBScript_editor
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

file(COPY res/static_db DESTINATION ${CMAKE_BINARY_DIR})


