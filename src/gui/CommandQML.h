//
// Created by morphred on 10/15/25.
//

#ifndef BBSCRIPT_EDITOR_COMMANDQML_H
#define BBSCRIPT_EDITOR_COMMANDQML_H

#include <QObject>
#include <QtQml>

class CommandQML : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit CommandQML(QObject *parent = nullptr);
    
signals:
    
};


#endif //BBSCRIPT_EDITOR_COMMANDQML_H
