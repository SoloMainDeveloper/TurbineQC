#include "curve/pch.h"

#include "removefigurecommand.h"

RemoveFigureCommand::RemoveFigureCommand(QString figureName) {
    _figureName = figureName;
}

void RemoveFigureCommand::run() {
    Project::instance().removeFigure(_figureName);
}

CommandType RemoveFigureCommand::getType() const {
    return CommandType::RemoveFigure;
}

QMap<QString, QVariant> RemoveFigureCommand::getParameters() const {
    return {
        { "figureName", _figureName }
    };
}

void RemoveFigureCommand::setParameters(QMap<QString, QVariant> params) {
    _figureName = params["figureName"].toString();
}

QString RemoveFigureCommand::getName() const {
    return "Удаление фигуры";
}

QString RemoveFigureCommand::getDescription() const {
    return "Remove figure";
}
