#include "curve/pch.h"

#include "changefigurevisibilitycommand.h"

ChangeFigureVisibilityCommand::ChangeFigureVisibilityCommand(QString figureName, bool isVisible) {
	_figureName = figureName;
	_isVisible = isVisible;
}

void ChangeFigureVisibilityCommand::run() {
	Project::instance().changeFigureVisibility(_figureName, _isVisible);
}

CommandType ChangeFigureVisibilityCommand::getType() const {
	return CommandType::ChangeFigureVisibility;
}

QMap<QString, QVariant> ChangeFigureVisibilityCommand::getParameters() const {
	return {
		{ "figureName", _figureName },
		{ "isVisible", _isVisible }
	};
}

void ChangeFigureVisibilityCommand::setParameters(QMap<QString, QVariant> params) {
	_figureName = params["figureName"].toString();
	_isVisible = params["isVisible"].toBool();
}

QString ChangeFigureVisibilityCommand::getName() const {
	return "Изменить видимость фигуры";
}

QString ChangeFigureVisibilityCommand::getDescription() const {
	return "Change figure visibility";
}
