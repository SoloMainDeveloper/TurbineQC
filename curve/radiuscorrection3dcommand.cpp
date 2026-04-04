#include "curve/pch.h"

#include "algorithms.h"
#include "radiuscorrection3dcommand.h"

RadiusCorrection3DCommand::RadiusCorrection3DCommand(const QString& nominalCurveName, const QString& measuredCurveName,
    const QString& resultCurveName, const Function42Params* params, double radiusCorrection)
{
    _nominalCurveName = nominalCurveName;
    _measuredCurveName = measuredCurveName;
    _resultCurveName = resultCurveName;
    _params = params;
    _radiusCorrection = radiusCorrection;
}

void RadiusCorrection3DCommand::run()
{
    Algorithms::calculateCurveUsing3DVectorsTest(_nominalCurveName, _measuredCurveName, _resultCurveName, _params, _radiusCorrection);
}

CommandType RadiusCorrection3DCommand::getType() const
{
    return CommandType::RadiusCorrection3D;
}

QMap<QString, QVariant> RadiusCorrection3DCommand::getParameters() const
{
    QMap<QString, QVariant> result;
    for(auto [key, value] : _params->toQMap().asKeyValueRange()) {
        result.insert(key, value);
    }

    result.insert({ { "nominalName", _nominalCurveName },
        { "measuredName", _measuredCurveName },
        { "resultName", _resultCurveName },
        { "radiusCorrection", _radiusCorrection } });
    return result;
}

void RadiusCorrection3DCommand::setParameters(QMap<QString, QVariant> params)
{
    _nominalCurveName = params["nominalName"].toString();
    _measuredCurveName = params["measuredName"].toString();
    _resultCurveName = params["resultName"].toString();
    _radiusCorrection = params["radiusCorrection"].toDouble();

    auto functionParams = new QMap<QString, QString>();
    for(auto [key, value] : params.asKeyValueRange()) {
        functionParams->insert(key, value.toString());
    }
    _params = new Function42Params(functionParams);
}

QString RadiusCorrection3DCommand::getName() const
{
    return tr("Radius Correction 3D");
}

QString RadiusCorrection3DCommand::getDescription() const
{
    return "Radius Correction 3D";
}
