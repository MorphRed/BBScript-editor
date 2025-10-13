#include <iostream>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "core/Editor.h"
#include "core/Parser.h"

std::string debug(const std::vector<std::string>& arguments)
{
    std::string result;
    for (const auto& argument : arguments)
    {
        result += " " + argument;
    }
    return result;
};

std::string debug(const std::vector<std::vector<std::string>>& arguments)
{
    std::string result;
    for (const auto& argument : arguments)
        result += "\n" + debug(argument);
    return result;
};

int main(int argc, char* argv[])
{
    const QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("bbscript_editor", "Main");

    return QGuiApplication::exec();
    std::string in{"../scr_tm.bin"};
    
    const std::string game{"BBCF"};
    // Parser parser{game};
    // Editor editor{parser};
    // editor.parse(in);
    // std::vector<Command> test_cmd = {{parser.cmd_id_db.at(4), {0, 0, 0, 0, 0, 0, 0, 0}}};
    //
    // editor.insert(1, test_cmd);
    // editor.insert(3, test_cmd);
    // editor.insert(10, test_cmd);
    // auto line = editor.getText(0, 20);
    // auto output = debug(line);
    // std::cout << output << std::endl;
    
    // QApplication a(argc, argv);
    // QPushButton button("Hello world!", nullptr);
    // button.resize(200, 100);
    // button.show();
    // return QApplication::exec();
}
