#include "curve/pch.h"

#include "renamefigurecommand.h"
#include "project.h"

RenameFigureCommand::RenameFigureCommand(QString name, QString newName) {
    _name = name;
    _newName = newName;
}

void RenameFigureCommand::run() {
    Project::instance().renameFigure(_name, _newName);
}

CommandType RenameFigureCommand::getType() const {
    return CommandType::RenameFigure;
}

QMap<QString, QVariant> RenameFigureCommand::getParameters() const {
    return {
        { "figureName", _name },
        { "newName", _newName }
    };
}

void RenameFigureCommand::setParameters(QMap<QString, QVariant> params) {
    _name = params["figureName"].toString();
    _newName = params["newName"].toString();
}

QString RenameFigureCommand::getName() const {
    return "Rename figure";
}

QString RenameFigureCommand::getDescription() const {
    return "Context";
}
