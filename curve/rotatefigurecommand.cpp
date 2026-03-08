#include "curve/pch.h"

#include "rotatefigurecommand.h"

RotateFigureCommand::RotateFigureCommand(QString figureName, double angle, QString x, QString y, QString z) {
    _figureName = figureName;
    _angle = angle;
    _x = x;
    _y = y;
    _z = z;
}

void RotateFigureCommand::run() {
    Project::instance().rotateFigure(_figureName, _angle, _x, _y, _z);
}

CommandType RotateFigureCommand::getType() const {
    return CommandType::RotateFigure;
}

QMap<QString, QVariant> RotateFigureCommand::getParameters() const {
    return {
        { "figureName", _figureName },
        { "angle", _angle },
        { "x", _x },
        { "y", _y },
        { "z", _z }
    };
}

void RotateFigureCommand::setParameters(QMap<QString, QVariant> params) {
    _figureName = params["figureName"].toString();
    _angle = params["angle"].toDouble();
    _x = params["x"].toString();
    _y = params["y"].toString();
    _z = params["z"].toString();
}

QString RotateFigureCommand::getName() const {
    return "Поворот фигуры";
}

QString RotateFigureCommand::getDescription() const {
    return "Rotate figure";
}
