#include "curve/pch.h"

#include "shiftfigurecommand.h"

ShiftFigureCommand::ShiftFigureCommand(QString figureName, QString x, QString y, QString z) {
    _figureName = figureName;
    _x = x;
    _y = y;
    _z = z;
}

void ShiftFigureCommand::run() {
    Project::instance().shiftFigure(_figureName, _x, _y, _z);
}

CommandType ShiftFigureCommand::getType() const {
    return CommandType::ShiftFigure;
}

QMap<QString, QVariant> ShiftFigureCommand::getParameters() const {
    return {
        { "figureName", _figureName },
        { "x", _x },
        { "y", _y },
        { "z", _z },
    };
}

void ShiftFigureCommand::setParameters(QMap<QString, QVariant> params) {
    _figureName = params["figureName"].toString();
    _x = params["x"].toString();
    _y = params["y"].toString();
    _z = params["z"].toString();
}

QString ShiftFigureCommand::getName() const {
    return "Смещение фигуры";
}

QString ShiftFigureCommand::getDescription() const {
    return "Shift figure";
}
