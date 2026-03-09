#include "curve/pch.h"

#include "insertbestfitpositioncommand.h"
#include "algorithms.h"

InsertBestFitPositionCommand::InsertBestFitPositionCommand(QString figureName, QString parentName, double x, double y, double z, bool isShowX, bool isShowY, bool isShowR) {
    _figureName = figureName;
    _parentName = parentName;
    _x = x;
    _y = y;
    _z = z;
    _isShowX = isShowX;
    _isShowY = isShowY;
    _isShowR = isShowR;
}

void InsertBestFitPositionCommand::run() {
    Algorithms::insertBestFitDimension(_figureName, _parentName, _x, _y, _z, _isShowX, _isShowY, _isShowR);
}

CommandType InsertBestFitPositionCommand::getType() const {
    return CommandType::InsertBestFitPosition;
}

QMap<QString, QVariant> InsertBestFitPositionCommand::getParameters() const {
    QMap<QString, QVariant> result;

    result["figureName"] = _figureName;
    result["parentName"] = _parentName;
    result["x"] = _x;
    result["y"] = _y;
    result["z"] = _z;
    result["isShowX"] = _isShowX;
    result["isShowY"] = _isShowY;
    result["isShowR"] = _isShowR;

    return result;
}

void InsertBestFitPositionCommand::setParameters(QMap<QString, QVariant> params) {
    _figureName = params["figureName"].toString();
    _parentName = params["parentName"].toString();
    _x = params["x"].toDouble();
    _y = params["y"].toDouble();
    _z = params["z"].toDouble();
    _isShowX = params["isShowX"].toBool();
    _isShowY = params["isShowY"].toBool();
    _isShowR = params["isShowR"].toBool();
}

QString InsertBestFitPositionCommand::getName() const {
    return "Вставка BestFitPosition";
}

QString InsertBestFitPositionCommand::getDescription() const {
    return "Insert BestFit Position";
}
