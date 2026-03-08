#include "curve/pch.h"

#include "saveprojectcommand.h"
#include "filesystem.h"

SaveProjectCommand::SaveProjectCommand(QString directory, QString projectName, bool needCRV) {
    _directory = directory;
    _projectName = projectName;
    _needCRV = needCRV;
}

void SaveProjectCommand::run() {
    FileSystem::saveProject(_directory, _projectName, _needCRV);
}

CommandType SaveProjectCommand::getType() const {
    return CommandType::SaveProject;
}

QMap<QString, QVariant> SaveProjectCommand::getParameters() const {
    return {
        { "directoryName", _directory },
        { "projectName", _projectName },
        { "needCRV", _needCRV }
    };
}

void SaveProjectCommand::setParameters(QMap<QString, QVariant> params) {
    _directory = params["directoryName"].toString();
    _projectName = params["projectName"].toString();
    _needCRV = params["needCRV"].toBool();
}

QString SaveProjectCommand::getName() const {
    return "Сохранение проекта";
}

QString SaveProjectCommand::getDescription() const {
    return "Save project";
}
