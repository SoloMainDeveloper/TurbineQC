#include "curve/pch.h"

#include "algorithms.h"
#include "minx.h"

MinX::MinX(double nominal, double upperTolerance, double lowerTolerance, QString extraParam1, QString extraParam2)
    : ITurbineProfileParameter(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2)
{
    _type = Type::MinX;
}

void MinX::calculateNominal(const QString& nominalProfileName, const Function18Params& params)
{
    double minX = Algorithms::getMinX(nominalProfileName, params);

    _nominal = minX;
}

void MinX::createMeasured(const QString& nominalProfileName,
    const QString& measuredProfileName, const Function18Params& params)
{
    double minX = Algorithms::getMinX(measuredProfileName, params);

    _measured = minX;
    _dimensionName = _dimensionNameTemplate.arg(nominalProfileName);

    Project* project = &Project::instance();

    QString pointName = _parameterNameTemplate.arg(measuredProfileName);
    auto minXPoint = CurvePoint(_measured, 0.0, 0.0);
    auto pointFigure = new PointFigure(pointName, minXPoint);
    project->safeInsert(pointName, pointFigure);

    double labelX = minXPoint.x + 1;
    double labelY = minXPoint.y;
    double labelZ = 0.0;
    auto labelPoint = Point(labelX, labelY, labelZ);

    auto dimFigure = new DimFigure(_dimensionName, labelPoint, pointName);
    dimFigure->setDimType(DimFigure::DimType::Position);

    auto value = DimFigure::Value(DimFigure::ValueType::X);
    value.nominal = _nominal;
    value.measurement = _measured;
    value.upperTolerance = _upperTolerance;
    value.lowerTolerance = _lowerTolerance;

    dimFigure->addValue(value);

    project->safeInsert(dimFigure->name(), dimFigure);
}

QString MinX::reportType() const
{
    return "X";
}

QString MinX::dimensionName() const
{
    return _dimensionName;
}
