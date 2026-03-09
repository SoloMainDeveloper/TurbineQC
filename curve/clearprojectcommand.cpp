#include "curve/pch.h"

#include "clearprojectcommand.h"

void ClearProjectCommand::run() {
    Project::instance().clear();
}

CommandType ClearProjectCommand::getType() const {
    return CommandType::ClearProject;
}

QMap<QString, QVariant> ClearProjectCommand::getParameters() const {
    return QMap<QString, QVariant>();
}

void ClearProjectCommand::setParameters(QMap<QString, QVariant> params) {
}

QString ClearProjectCommand::getName() const {
    return "Очистка проекта";
}

QString ClearProjectCommand::getDescription() const {
    return "Clear Project";
}
