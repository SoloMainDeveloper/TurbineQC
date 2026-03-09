#include "curve/pch.h"

#include "bestfitcommand.h"
#include "algorithms.h"

BestFitCommand::BestFitCommand(QString nominalCurveName, QString measuredCurveName, QString resultCurveName,
    QString bestFitLineName, const Function6Params* params) {
    _nominalCurveName = nominalCurveName;
    _measuredCurveName = measuredCurveName;
    _resultCurveName = resultCurveName;
    _bestFitLineName = bestFitLineName;

    if(params) {
        Function6Params* nonConstParams = const_cast<Function6Params*>(params);
        auto* paramsMap = new QMap<QString, QString>(nonConstParams->toQMap());
        _params = new Function6Params(paramsMap);
    }
}

void BestFitCommand::run() {
    Algorithms::calculateBestFit(_nominalCurveName, _measuredCurveName, _resultCurveName, _bestFitLineName, _params);
}

CommandType BestFitCommand::getType() const {
    return CommandType::BestFit;
}

QMap<QString, QVariant> BestFitCommand::getParameters() const {
    QMap<QString, QVariant> result;
    auto log = const_cast<Function6Params*>(_params)->toQMap();
    for(auto [key, value] : log.asKeyValueRange()) {
        result.insert(key, value);
    }
    result.insert({
        { "nominal", _nominalCurveName },
        { "measured", _measuredCurveName },
        { "resultName", _resultCurveName },
        { "bestFitLineName", _bestFitLineName }
        });
    return result;
}

void BestFitCommand::setParameters(QMap<QString, QVariant> params) {
    _nominalCurveName = params["nominal"].toString();
    _measuredCurveName = params["measured"].toString();
    _resultCurveName = params["resultName"].toString();
    _bestFitLineName = params["bestFitLineName"].toString();

    auto functionParams = new QMap<QString, QString>();
    for(auto [key, value] : params.asKeyValueRange()) {
        functionParams->insert(key, value.toString());
    }
    _params = new Function6Params(functionParams);
}

QString BestFitCommand::getName() const {
    return "Подгонка кривых";
}

QString BestFitCommand::getDescription() const {
    return "BestFit";
}
