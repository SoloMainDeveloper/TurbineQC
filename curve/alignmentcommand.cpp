#include "curve/pch.h"

#include "alignmentcommand.h"

AlignmentCommand::AlignmentCommand(QString angle, QString axis, QString offsetX, QString offsetY) {
    _angle = angle;
    _axis = axis;
    _offsetX = offsetX;
    _offsetY = offsetY;
}

void AlignmentCommand::run() {
    Project::instance().alignment(_angle, _axis, _offsetX, _offsetY);
}

CommandType AlignmentCommand::getType() const {
    return CommandType::Alignment;
}

QMap<QString, QVariant> AlignmentCommand::getParameters() const {
    return {
        { "angle", _angle },
        { "axis", _axis },
        { "offsetX", _offsetX },
        { "offsetY", _offsetY }
    };
}

void AlignmentCommand::setParameters(QMap<QString, QVariant> params) {
    _angle = params["angle"].toString();
    _axis = params["axis"].toString();
    _offsetX = params["offsetX"].toString();
    _offsetY = params["offsetY"].toString();
}

QString AlignmentCommand::getName() const {
    return "Выравнивание";
}

QString AlignmentCommand::getDescription() const {
    return "Alignment";
}
