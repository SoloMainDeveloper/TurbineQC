#include "curve/pch.h"

#include "saveprojectcommand.h"
#include "filesystem.h"

SaveProjectCommand::SaveProjectCommand(QString fileNameWithoutExtension) {
    _fileNameWithoutExtension = fileNameWithoutExtension;
}

void SaveProjectCommand::run() {
    FileSystem::saveProject(_fileNameWithoutExtension);
}

CommandType SaveProjectCommand::getType() const {
    return CommandType::SaveProject;
}

QMap<QString, QVariant> SaveProjectCommand::getParameters() const {
    return {
        { "fileNameWithoutExtension", _fileNameWithoutExtension },
    };
}

void SaveProjectCommand::setParameters(QMap<QString, QVariant> params) {
    _fileNameWithoutExtension = params["fileNameWithoutExtension"].toString();
}

QString SaveProjectCommand::getName() const {
    return "Сохранение проекта";
}

QString SaveProjectCommand::getDescription() const {
    return "Save project";
}
