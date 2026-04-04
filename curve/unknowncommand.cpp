#include "curve/pch.h"

#include "unknowncommand.h"

void UnknownCommand::run()
{
}

CommandType UnknownCommand::getType() const
{
    return CommandType::Unknown;
}

QMap<QString, QVariant> UnknownCommand::getParameters() const
{
    return QMap<QString, QVariant>();
}

void UnknownCommand::setParameters(QMap<QString, QVariant> params)
{
}

QString UnknownCommand::getName() const
{
    return tr("Unknown Command");
}

QString UnknownCommand::getDescription() const
{
    return "Unknown Command";
}
