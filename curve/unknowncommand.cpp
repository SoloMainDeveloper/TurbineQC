#include "curve/pch.h"

#include "unknowncommand.h"

UnknownCommand::UnknownCommand(QString name)
{
    _name = name;
}

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
    auto base = tr("Unknown Command");

    return _name.isEmpty() ? base : base.append(": " + _name);
}

QString UnknownCommand::getDescription() const
{
    return "Unknown Command";
}
