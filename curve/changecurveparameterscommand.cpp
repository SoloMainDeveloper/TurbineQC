#include "curve/pch.h"

#include "changecurveparameterscommand.h"

ChangeCurveParametersCommand::ChangeCurveParametersCommand(const QString &figureName, bool showPoints, bool connectPoints,
    bool showVectors, bool closed, bool showNumbering, int numberingInterval, double amplification,
    bool showTolerances, bool showDeviations, bool connectDeviations, bool highLightOut) {
    _figureName = figureName;
    _showPoints = showPoints;
    _connectPoints = connectPoints;
    _showVectors = showVectors;
    _closed = closed;
    _showNumbering = showNumbering;
    _numberingInterval = numberingInterval;
    _amplification = amplification;
    _showTolerances = showTolerances;
    _showDeviations = showDeviations;
    _connectDeviations = connectDeviations;
    _highLightOut = highLightOut;
}

void ChangeCurveParametersCommand::run() {
    Project::instance().changeCurveParameters(
        _figureName,
        _showPoints,
        _connectPoints,
        _showVectors,
        _closed,
        _showNumbering,
        _numberingInterval,
        _amplification,
        _showTolerances,
        _showDeviations,
        _connectDeviations,
        _highLightOut
    );
}

CommandType ChangeCurveParametersCommand::getType() const {
    return CommandType::ChangeCurveParameters;
}

QMap<QString, QVariant> ChangeCurveParametersCommand::getParameters() const {
    QMap<QString, QVariant> result;

    result["figureName"] = _figureName;
    result["showPoints"] = _showPoints;
    result["connectPoints"] = _connectPoints;
    result["showVectors"] = _showVectors;
    result["closed"] = _closed;
    result["showNumbering"] = _showNumbering;
    result["numberingInterval"] = _numberingInterval;
    result["amplification"] = _amplification;
    result["showTolerances"] = _showTolerances;
    result["showDeviations"] = _showDeviations;
    result["connectDeviations"] = _connectDeviations;
    result["highLightOut"] = _highLightOut;

    return result;
}

void ChangeCurveParametersCommand::setParameters(QMap<QString, QVariant> params) {
    _figureName = params.take("figureName").toString();
    _showPoints = params["showPoints"].toBool();
    _connectPoints = params["connectPoints"].toBool();
    _showVectors = params["showVectors"].toBool();
    _closed = params["closed"].toBool();
    _showNumbering = params["showNumbering"].toBool();
    _numberingInterval = params["numberingInterval"].toInt();
    _amplification = params["amplification"].toDouble();
    _showTolerances = params["showTolerances"].toBool();
    _showDeviations = params["showDeviations"].toBool();
    _connectDeviations = params["connectDeviations"].toBool();
    _highLightOut = params["highLightOut"].toBool();
}

QString ChangeCurveParametersCommand::getName() const {
    return "Изменение параметров кривой";
}

QString ChangeCurveParametersCommand::getDescription() const {
    return "Change curve parameters";
}

//else {
//    auto isVisible = _params["isVisible"].toBool();
//    project.changeFigureVisibility(_figureName, isVisible);
//    }