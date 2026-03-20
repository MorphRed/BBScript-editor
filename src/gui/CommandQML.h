//
// Created by morphred on 10/15/25.
//

#ifndef BBSCRIPT_EDITOR_COMMANDQML_H
#define BBSCRIPT_EDITOR_COMMANDQML_H

#include <QObject>
#include <qqmlintegration.h>

#include "Editor.h"

class CommandQML : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QStringList commandQML READ commandQML WRITE setCommandQML NOTIFY commandQMLChanged)

public:
    explicit CommandQML(QObject* parent = nullptr);
    QStringList commandQML();
    void setCommandQML(const std::vector<std::string>& commandQML);
    void setCommandQML(const QStringList& commandQML);
    
private:
    QStringList command_data;

signals:
    void commandQMLChanged();
};


#endif //BBSCRIPT_EDITOR_COMMANDQML_H
