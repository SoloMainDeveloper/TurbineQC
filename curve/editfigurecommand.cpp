#include "curve/pch.h"

#include "editfigurecommand.h"

EditFigureCommand::EditFigureCommand(QString figureName, QMap<QString, QString> &paramsChanged) {
    _figureName = figureName;
    _paramsChanged = paramsChanged;
}

void EditFigureCommand::run() {
    Project::instance().editFigure(_figureName, _paramsChanged);
}

CommandType EditFigureCommand::getType() const {
    return CommandType::EditFigure;
}

QMap<QString, QVariant> EditFigureCommand::getParameters() const {
    QMap<QString, QVariant> result;

    result["figureName"] = _figureName;
    for(auto [key, value] : _paramsChanged.asKeyValueRange()) {
        result[key] = value;
    }

    return result;
}

void EditFigureCommand::setParameters(QMap<QString, QVariant> params) {
    for(auto [key, value] : params.asKeyValueRange()) {
        _paramsChanged[key] = value.toString();
    }

    _figureName = _paramsChanged.take("figureName");
}

QString EditFigureCommand::getName() const {
    return "Редактирование фигуры";
}

QString EditFigureCommand::getDescription() const {
    return "Edit figure";
}
