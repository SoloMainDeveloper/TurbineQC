#include "curve/pch.h"

#include "loadcloudcommand.h"
#include "filesystem.h"

LoadCloudCommand::LoadCloudCommand(QString filePath, QString name, QString sep, int skipStart,
    int skipAfter, QString columnNames, QString columnNumbers, QString decimal) {
    _filePath = filePath;
    _name = name;
    _sep = sep;
    _skipStart = skipStart;
    _skipAfter = skipAfter;
    _columnNames = columnNames;
    _columnNumbers = columnNumbers;
    _decimal = decimal;
}

void LoadCloudCommand::run() {
    FileSystem::loadCloud(
        _filePath, _name, _sep, _skipStart,
        _skipAfter, _columnNames, _columnNumbers, _decimal
    );
}

CommandType LoadCloudCommand::getType() const {
    return CommandType::LoadCloud;
}

QMap<QString, QVariant> LoadCloudCommand::getParameters() const {
    return {
        { "filepath", _filePath },
        { "name", _name },
        { "separator", _sep },
        { "skipStart", _skipStart },
        { "skipAfter", _skipAfter },
        { "columnNames", _columnNames },
        { "columnNumbers", _columnNumbers },
        { "decimal", _decimal }
    };
}

void LoadCloudCommand::setParameters(QMap<QString, QVariant> params) {
    _filePath = params["filepath"].toString();
    _name = params["name"].toString();
    _sep = params["separator"].toString();
    _skipStart = params["skipStart"].toInt();
    _skipAfter = params["skipAfter"].toInt();
    _columnNames = params["columnNames"].toString();
    _columnNumbers = params["columnNumbers"].toString();
    _decimal = params["decimal"].toString();
}

QString LoadCloudCommand::getName() const {
    return "Загрузка точек";
}

QString LoadCloudCommand::getDescription() const {
    return "Load cloud";
}
