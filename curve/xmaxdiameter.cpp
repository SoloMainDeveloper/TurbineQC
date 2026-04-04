#include "curve/pch.h"

#include "algorithms.h"
#include "xmaxdiameter.h"

XMaxDiameter::XMaxDiameter(double nominal, double upperTolerance, double lowerTolerance, QString extraParam1, QString extraParam2)
    : ITurbineProfileParameter(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2)
{
    _type = Type::MaxWidthX;
}

void XMaxDiameter::calculateNominal(const QString& nominalProfileName, const Function18Params& params)
{
    CircleFigure circle = Algorithms::getMaxCircle(nominalProfileName, params);
    double xMaxDiameter = circle.center().x;

    _nominal = xMaxDiameter;
}

void XMaxDiameter::createMeasured(const QString& nominalProfileName, const QString& measuredProfileName, const Function18Params& params)
{
    auto circle = Algorithms::getMaxCircle(measuredProfileName, params);
    double xMaxDiameter = circle.center().x;

    _measured = xMaxDiameter;
    _dimensionName = _dimensionNameTemplate.arg(nominalProfileName);

    Project* project = &Project::instance();

    auto circleName = _parameterNameTemplate.arg(measuredProfileName);
    auto circleFigure = new CircleFigure(circle);
    circleFigure->setName(circleName);
    project->safeInsert(circleName, circleFigure);

    double labelX = circleFigure->center().x;
    double labelY = circleFigure->center().y - circleFigure->radius() - 1;
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

QString XMaxDiameter::reportType() const
{
    return "X";
}

QString XMaxDiameter::dimensionName() const
{
    return _dimensionName;
}
