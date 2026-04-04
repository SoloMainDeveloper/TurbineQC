#include "curve/pch.h"

#include "algorithms.h"
#include "ymaxdiameter.h"

YMaxDiameter::YMaxDiameter(double nominal, double upperTolerance, double lowerTolerance, QString extraParam1, QString extraParam2)
    : ITurbineProfileParameter(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2)
{
    _type = Type::MaxWidthY;
}

void YMaxDiameter::calculateNominal(const QString& nominalProfileName, const Function18Params& params)
{
    CircleFigure circle = Algorithms::getMaxCircle(nominalProfileName, params);
    double yMaxDiameter = circle.center().y;

    _nominal = yMaxDiameter;
}

void YMaxDiameter::createMeasured(const QString& nominalProfileName, const QString& measuredProfileName, const Function18Params& params)
{
    auto circle = Algorithms::getMaxCircle(measuredProfileName, params);
    double yMaxDiameter = circle.center().y;

    _measured = yMaxDiameter;
    _dimensionName = _dimensionNameTemplate.arg(nominalProfileName);

    Project* project = &Project::instance();

    auto circleName = _parameterNameTemplate.arg(measuredProfileName);
    auto circleFigure = new CircleFigure(circle);
    circleFigure->setName(circleName);
    project->safeInsert(circleName, circleFigure);

    double labelX = circleFigure->center().x;
    double labelY = circleFigure->center().y + circleFigure->radius() + 1;
    double labelZ = 0.0;
    auto labelPoint = Point(labelX, labelY, labelZ);

    auto dimFigure = new DimFigure(_dimensionName, labelPoint, circleName);
    dimFigure->setDimType(DimFigure::DimType::Position);

    auto value = DimFigure::Value(DimFigure::ValueType::Radius);
    value.nominal = _nominal;
    value.measurement = _measured;
    value.upperTolerance = _upperTolerance;
    value.lowerTolerance = _lowerTolerance;

    dimFigure->addValue(value);

    project->safeInsert(_dimensionName, dimFigure);
}

QString YMaxDiameter::reportType() const
{
    return "Y";
}

QString YMaxDiameter::dimensionName() const
{
    return _dimensionName;
}
