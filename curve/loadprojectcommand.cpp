#include "curve/pch.h"

#include "loadprojectcommand.h"
#include "filesystem.h"

LoadProjectCommand::LoadProjectCommand(QString filePath) {
    _filePath = filePath;
}

void LoadProjectCommand::run() {
    FileSystem::loadProject(_filePath);
}

CommandType LoadProjectCommand::getType() const {
    return CommandType::LoadProject;
}

QMap<QString, QVariant> LoadProjectCommand::getParameters() const {
    return { { "filePath", _filePath } };
}

void LoadProjectCommand::setParameters(QMap<QString, QVariant> params) {
    _filePath = params["filePath"].toString();
}

QString LoadProjectCommand::getName() const {
    return "Загрузка проекта";
}

QString LoadProjectCommand::getDescription() const {
    return "Load project from CRV";
}
