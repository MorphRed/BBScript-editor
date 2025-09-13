#include <iostream>
#include <QApplication>
#include <QPushButton>

#include "parser/Parser.h"

int main(int argc, char* argv[])
{
    std::ifstream in{"../scr_tm.bin", std::ios::in | std::ios::binary};
    if (!in)
    {
        // Print an error and exit
        std::cerr << "could not be opened for reading!\n";
        return 1;
    }
    
    const std::string game{"BBCF"};
    Parser parser{in, game};
    parser.decode();
    QApplication a(argc, argv);
    QPushButton button("Hello world!", nullptr);
    button.resize(200, 100);
    button.show();
    return QApplication::exec();
}
