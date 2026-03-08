#include "curve/pch.h"

#include "radiuscorrectioncommand.h"
#include "algorithms.h"

RadiusCorrectionCommand::RadiusCorrectionCommand(QString figureName, QString newFigureName, const Function3Params* params) {
    _figureName = figureName;
    _newFigureName = newFigureName;

    if(params) {
        Function3Params* nonConstParams = const_cast<Function3Params*>(params);
        auto* paramsMap = new QMap<QString, QString>(nonConstParams->toQMap());
        _params = new Function3Params(paramsMap);
    }
}

void RadiusCorrectionCommand::run() {
    Algorithms::makeRadiusCorrection(_figureName, _newFigureName, _params);
}

CommandType RadiusCorrectionCommand::getType() const {
    return CommandType::RadiusCorrection;
}

QMap<QString, QVariant> RadiusCorrectionCommand::getParameters() const {
    QMap<QString, QVariant> result;
    auto log = _params->toQMap();
    for(auto [key, value] : log.asKeyValueRange()) {
        result.insert(key, value);
    }
    result.insert({
        { "figureName", _figureName },
        { "newFigureName", _newFigureName },
        });
    return result;
}

void RadiusCorrectionCommand::setParameters(QMap<QString, QVariant> params) {
    _figureName = params["figureName"].toString();
    _newFigureName = params["newFigureName"].toString();

    auto functionParams = new QMap<QString, QString>();
    for(auto [key, value] : params.asKeyValueRange()) {
        functionParams->insert(key, value.toString());
    }
    _params = new Function3Params(functionParams);
}

QString RadiusCorrectionCommand::getName() const {
    return "Радиус-коррекция";
}

QString RadiusCorrectionCommand::getDescription() const {
    return "Radius correction";
}
