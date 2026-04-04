#include "curve/pch.h"

#include "algorithms.h"
#include "leadingedgeradius.h"

LeadingEdgeRadius::LeadingEdgeRadius(double nominal, double upperTolerance,
    double lowerTolerance, QString extraParam1, QString extraParam2)
    : ITurbineProfileParameter(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2)
{
    _type = Type::LERadius;
}

void LeadingEdgeRadius::calculateNominal(const QString& nominalProfileName, const Function18Params& params)
{
    CircleFigure circle = Algorithms::getRadiusOfLeadingEdge(nominalProfileName, params, _extraParam1.toDouble());
    double leadingEdgeRadius = circle.radius();

    _nominal = leadingEdgeRadius;
}

void LeadingEdgeRadius::createMeasured(const QString& nominalProfileName,
    const QString& measuredProfileName, const Function18Params& params)
{
    CircleFigure circle = Algorithms::getRadiusOfLeadingEdge(measuredProfileName, params, _extraParam1.toDouble());
    double leadingEdgeRadius = circle.radius();

    _measured = leadingEdgeRadius;
    _dimensionName = _dimensionNameTemplate.arg(nominalProfileName);

    Project* project = &Project::instance();

    QString circleName = _parameterNameTemplate.arg(measuredProfileName);
    auto circleFigure = new CircleFigure(circle);
    circleFigure->setName(circleName);
    project->safeInsert(circleName, circleFigure);

    double labelX = circleFigure->center().x + circleFigure->radius() + 1;
    double labelY = circleFigure->center().y;
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

QString LeadingEdgeRadius::reportType() const
{
    return "Radius";
}

QString LeadingEdgeRadius::dimensionName() const
{
    return _dimensionName;
}
