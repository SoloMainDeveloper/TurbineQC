#include "curve/pch.h"

#include "algorithms.h"
#include "maxdiameter.h"

MaxDiameter::MaxDiameter(double nominal, double upperTolerance, double lowerTolerance, QString extraParam1, QString extraParam2)
    : ITurbineProfileParameter(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2)
{
    _type = Type::MaxWidth;
}

void MaxDiameter::calculateNominal(const QString& nominalProfileName, const Function18Params& params)
{
    CircleFigure circle = Algorithms::getMaxCircle(nominalProfileName, params);
    double maxDiameter = circle.radius() * 2;

    _nominal = maxDiameter;
}

void MaxDiameter::createMeasured(const QString& nominalProfileName, const QString& measuredProfileName, const Function18Params& params)
{
    CircleFigure circle = Algorithms::getMaxCircle(measuredProfileName, params);
    double maxDiameter = circle.radius() * 2;

    _measured = maxDiameter;
    _dimensionName = _dimensionNameTemplate.arg(nominalProfileName);

    Project* project = &Project::instance();

    QString circleName = _parameterNameTemplate.arg(measuredProfileName);
    auto circleFigure = new CircleFigure(circle);
    circleFigure->setName(circleName);
    project->safeInsert(circleName, circleFigure);

    double labelX = circleFigure->center().x + circleFigure->radius() + 1;
    double labelY = circleFigure->center().y;
    double lableZ = 0.0;
    auto labelPoint = Point(labelX, labelY, lableZ);

    auto dimFigure = new DimFigure(_dimensionName, labelPoint, circleName);
    dimFigure->setDimType(DimFigure::DimType::Diameter);

    auto value = DimFigure::Value(DimFigure::ValueType::Diameter);
    value.nominal = _nominal;
    value.measurement = _measured;
    value.upperTolerance = _upperTolerance;
    value.lowerTolerance = _lowerTolerance;

    dimFigure->addValue(value);

    project->safeInsert(_dimensionName, dimFigure);
}

QString MaxDiameter::reportType() const
{
    return "Diameter";
}

QString MaxDiameter::dimensionName() const
{
    return _dimensionName;
}
