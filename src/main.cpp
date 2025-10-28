#include <iostream>
#include <QApplication>
#include <QQmlApplicationEngine>

#include "CommandQML.h"
#include "Editor.h"
#include "Parser.h"

int main(int argc, char* argv[])
{
    const QApplication app(argc, argv);
    CommandQML command;

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("bbscript_editor", "Main");

    // return QApplication::exec();
    std::string in{"../scr_tm.bin"};
    
    const std::string game{"BBCF"};
    Parser parser{game};
    Editor editor{parser};
    editor.parse(in);
    std::vector<Command> test1 = {{parser.cmd_id_db.at(4), {"1", "1"}}};
    std::vector<Command> test2 = {{parser.cmd_id_db.at(4), {"2", "2"}}};
    std::vector<Command> test3 = {{parser.cmd_id_db.at(4), {"3", "3"}}};
    
    editor.insert(1, test1);
    editor.insert(3, test2);
    editor.insert(10, test3);
    editor.insert(10, test3);
    
    // auto line = editor.getText(0, 20);
    // auto output = debug_parser(line);
    // std::cout << output << std::endl;

    editor.piece_table.debug();
}
