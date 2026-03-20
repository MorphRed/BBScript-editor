//
// Created by morphred on 10/23/25.
//

#include "CommandQML.h"

#include <Command.h>

CommandQML::CommandQML(QObject* parent) : QObject{parent}
{
}

QStringList CommandQML::commandQML()
{
    return command_data;
}

void CommandQML::setCommandQML(const std::vector<std::string>& commandQML)
{
    command_data.reserve(commandQML.size());
    std::ranges::transform(commandQML, std::back_inserter(command_data),
                           [](const std::string& v) { return QString::fromStdString(v); });
}

void CommandQML::setCommandQML(const QStringList& commandQML)
{
    command_data = commandQML;
}
