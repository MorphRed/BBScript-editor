cmake_minimum_required(VERSION 4.0)
project(bbscript-editor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(QT_FORCE_CMP0156_TO_VALUE NEW)

find_package(Qt6 COMPONENTS REQUIRED 
        Core
        Gui
        Widgets
        Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(app-bbscript-editor src/main.cpp
        src/core/Parser.cpp
        src/core/Parser.h
        src/core/Command.cpp
        src/core/Command.h
        src/core/Editor.cpp
        src/core/Editor.h
        src/core/PieceTable.cpp
        src/core/PieceTable.h
        depends/whereami/whereami.c
        src/gui/Main.qml
        src/gui/CommandQML.cpp
        src/gui/CommandQML.h
)

qt_add_qml_module(app-bbscript-editor
    URI bbscript_editor
    VERSION 1.0
    QML_FILES
        src/gui/Main.qml
    SOURCES
        src/gui/CommandQML.cpp src/gui/CommandQML.h
        
)

target_include_directories(app-bbscript-editor PRIVATE depends src/core src/gui)

target_link_libraries(app-bbscript-editor PRIVATE 
        Qt6::Core
        Qt6::Widgets
        Qt6::Quick
)

file(COPY res/static_db DESTINATION ${CMAKE_BINARY_DIR})


