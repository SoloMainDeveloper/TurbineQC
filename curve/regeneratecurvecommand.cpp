#include "curve/pch.h"

#include "regeneratecurvecommand.h"
#include "algorithms.h"

RegenerateCurveCommand::RegenerateCurveCommand(QString figureName, QString newFigureName, const Function19Params* params) {
    _figureName = figureName;
    _newFigureName = newFigureName;
    _params = params;
}

void RegenerateCurveCommand::run() {
    Algorithms::regenerateCurve(_figureName, _newFigureName, _params);
}

CommandType RegenerateCurveCommand::getType() const {
    return CommandType::RegenerateCurve;
}

QMap<QString, QVariant> RegenerateCurveCommand::getParameters() const {
    QMap<QString, QVariant> result;
    auto log = const_cast<Function19Params*>(_params)->toQMap();
    for(auto [key, value] : log.asKeyValueRange()) {
        result.insert(key, value);
    }
    result.insert({ { "figureName", _figureName }, { "newFigureName", _newFigureName } });
    return result;
}

void RegenerateCurveCommand::setParameters(QMap<QString, QVariant> params) {
    _figureName = params["figureName"].toString();
    _newFigureName = params["newFigureName"].toString();

    auto functionParams = new QMap<QString, QString>();
    for(auto [key, value] : params.asKeyValueRange()) {
        functionParams->insert(key, value.toString());
    }
    _params = new Function19Params(functionParams);
}

QString RegenerateCurveCommand::getName() const {
    return "Регенерация кривой";
}

QString RegenerateCurveCommand::getDescription() const {
    return "Regenerate Curve";
}
