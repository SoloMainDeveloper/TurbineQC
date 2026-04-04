#include "curve/pch.h"

#include "algorithms.h"
#include "trailingedgewidth.h"

TrailingEdgeWidth::TrailingEdgeWidth(double nominal, double upperTolerance,
    double lowerTolerance, QString extraParam1, QString extraParam2)
    : ITurbineProfileParameter(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2)
{
    _type = Type::TEWidth;
}

void TrailingEdgeWidth::calculateNominal(const QString& nominalProfileName, const Function18Params& params)
{
    QPair<CurvePoint, CurvePoint> pointsPair = Algorithms::getWidthOfTrailingEdge(nominalProfileName, params, _extraParam1.toDouble());
    auto firstPoint = Point(pointsPair.first);
    auto secondPoint = Point(pointsPair.second);
    double trailingEdgeWidth = Algorithms::getDistanceBetweenPoints(firstPoint, secondPoint);

    _nominal = trailingEdgeWidth;
}

void TrailingEdgeWidth::createMeasured(const QString& nominalProfileName,
    const QString& measuredProfileName, const Function18Params& params)
{
    QPair<CurvePoint, CurvePoint> pointsPair = Algorithms::getWidthOfTrailingEdge(measuredProfileName, params, _extraParam1.toDouble());
    auto [firstCurvePoint, secondCurvePoint] = pointsPair;
    auto firstPoint = Point(firstCurvePoint);
    auto secondPoint = Point(secondCurvePoint);
    double trailingEdgeWidth = Algorithms::getDistanceBetweenPoints(firstPoint, secondPoint);

    _measured = trailingEdgeWidth;

    Project* project = &Project::instance();

    _dimensionName = project->getFreeName(_dimensionNameTemplate.arg(nominalProfileName), true);

    QString pointName = _parameterNameTemplate.arg(measuredProfileName);
    QString firstFreeName = project->getFreeName(pointName);
    auto firstPointFigure = new PointFigure(firstFreeName, firstCurvePoint);
    project->safeInsert(firstFreeName, firstPointFigure);

    QString secondFreeName = project->getFreeName(pointName);
    auto secondPointFigure = new PointFigure(secondFreeName, secondCurvePoint);
    project->safeInsert(secondFreeName, secondPointFigure);

    Point middlePoint = Algorithms::getMiddlePoint(firstPoint, secondPoint);

    auto dimFigure = new DimFigure(_dimensionName, middlePoint, firstFreeName, secondFreeName);
    dimFigure->setDimType(DimFigure::DimType::Distance); // TODO: Need DistanceBetweenPoints

    auto value = DimFigure::Value(DimFigure::ValueType::Length);
    value.nominal = _nominal;
    value.measurement = _measured;
    value.upperTolerance = _upperTolerance;
    value.lowerTolerance = _lowerTolerance;

    dimFigure->addValue(value);

    project->safeInsert(_dimensionName, dimFigure);

    // auto log = QMap<QString, QString>(const_cast<Function18Params*>(&params)->toQMap());
    // log.insert({ { "figureName", measuredCurve }, { "distanceTE", extraParam1 } });
    // MacrosManager::log(MacrosManager::CreateWidthOfTE, log);
}

QString TrailingEdgeWidth::reportType() const
{
    return "Distance 2D";
}

QString TrailingEdgeWidth::dimensionName() const
{
    return _dimensionName;
}
