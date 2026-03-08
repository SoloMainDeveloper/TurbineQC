#include "curve/pch.h"

#include "mergescanscommand.h"
#include "algorithms.h"

MergeScansCommand::MergeScansCommand(QString firstCurveName, QString secondCurveName, QString resultName, double threshold, bool needSorted) {
    _firstCurveName = firstCurveName;
    _secondCurveName = secondCurveName;
    _resultName = resultName;
    _threshold = threshold;
    _needSorted = needSorted;
}

void MergeScansCommand::run() {
    Algorithms::tryMergePointClouds(_firstCurveName, _secondCurveName, _resultName, _threshold, _needSorted);
}

CommandType MergeScansCommand::getType() const {
    return CommandType::MergeScans;
}

QMap<QString, QVariant> MergeScansCommand::getParameters() const {
    return {
        { "firstName", _firstCurveName },
        { "secondName", _secondCurveName },
        { "resultName", _resultName },
        { "threshold", _threshold },
        { "needSorted", _needSorted }
    };
}

void MergeScansCommand::setParameters(QMap<QString, QVariant> params) {
    _firstCurveName = params["firstName"].toString();
    _secondCurveName = params["secondName"].toString();
    _resultName = params["resultName"].toString();
    _threshold = params["threshold"].toDouble();
    _needSorted = params["needSorted"].toBool();
}

QString MergeScansCommand::getName() const {
    return "Объединение сканов";
}

QString MergeScansCommand::getDescription() const {
    return "Merge Scans";
}
