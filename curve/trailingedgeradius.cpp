#include "curve/pch.h"

#include "algorithms.h"
#include "trailingedgeradius.h"

TrailingEdgeRadius::TrailingEdgeRadius(double nominal, double upperTolerance,
    double lowerTolerance, QString extraParam1, QString extraParam2)
    : ITurbineProfileParameter(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2)
{
    _type = Type::TERadius;
}

void TrailingEdgeRadius::calculateNominal(const QString& nominalProfileName, const Function18Params& params)
{
    auto circle = Algorithms::getRadiusOfTrailingEdge(nominalProfileName, params, _extraParam1.toDouble());
    double trailingEdgeRadius = circle.radius();

    _nominal = trailingEdgeRadius;
}

void TrailingEdgeRadius::createMeasured(const QString& nominalProfileName,
    const QString& measuredProfileName, const Function18Params& params)
{
    auto circle = Algorithms::getRadiusOfTrailingEdge(measuredProfileName, params, _extraParam1.toDouble());
    double trailingEdgeRadius = circle.radius();

    _measured = trailingEdgeRadius;
    _dimensionName = _dimensionNameTemplate.arg(nominalProfileName);

    Project* project = &Project::instance();

    QString circleName = _parameterNameTemplate.arg(measuredProfileName);
    auto circleFigure = new CircleFigure(circle);
    circleFigure->setName(circleName);
    project->safeInsert(circleName, circleFigure);

    double labelX = circle.center().x + circle.radius() + 1;
    double labelY = circle.center().y;
    double labelZ = 0.0;
    auto labelPoint = Point(labelX, labelY, labelZ);

    auto dimFigure = new DimFigure(_dimensionName, labelPoint, circleName);
    dimFigure->setDimType(DimFigure::DimType::Radius);

    auto value = DimFigure::Value(DimFigure::ValueType::Radius);
    value.nominal = _nominal;
    value.measurement = _measured;
    value.upperTolerance = _upperTolerance;
    value.lowerTolerance = _lowerTolerance;

    dimFigure->addValue(value);

    project->safeInsert(_dimensionName, dimFigure);
}

QString TrailingEdgeRadius::reportType() const
{
    return "Radius";
}

QString TrailingEdgeRadius::dimensionName() const
{
    return _dimensionName;
}
