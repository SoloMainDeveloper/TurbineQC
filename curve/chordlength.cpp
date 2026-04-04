#include "curve/pch.h"

#include "algorithms.h"
#include "chordlength.h"

ChordLength::ChordLength(double nominal, double upperTolerance, double lowerTolerance, QString extraParam1, QString extraParam2)
    : ITurbineProfileParameter(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2)
{
    _type = Type::ChordLength;
}

void ChordLength::calculateNominal(const QString& nominalProfileName, const Function18Params& params)
{
    QPair<PointFigure, PointFigure> chord = Algorithms::getChord(nominalProfileName, params);
    Point firstPoint = Point(chord.first.point());
    Point secondPoint = Point(chord.second.point());
    double chordLength = Algorithms::getDistanceBetweenPoints(firstPoint, secondPoint);

    _nominal = chordLength;
}

void ChordLength::createMeasured(const QString& nominalProfileName, const QString& measuredProfileName, const Function18Params& params)
{
    QPair<PointFigure, PointFigure> chord = Algorithms::getChord(measuredProfileName, params);
    auto [firstFigure, secondFigure] = chord;
    Point firstPoint = Point(firstFigure.point());
    Point secondPoint = Point(secondFigure.point());
    double chordLength = Algorithms::getDistanceBetweenPoints(firstPoint, secondPoint);

    _measured = chordLength;
    _dimensionName = _dimensionNameTemplate.arg(nominalProfileName);

    Project* project = &Project::instance();

    QString firstPointName = _parameter1NameTemplate.arg(measuredProfileName);
    auto firstPointFigure = new PointFigure(firstFigure);
    firstPointFigure->setName(firstPointName);
    project->safeInsert(firstPointName, firstPointFigure);

    QString secondPointName = _parameter2NameTemplate.arg(measuredProfileName);
    auto secondPointFigure = new PointFigure(secondFigure);
    secondPointFigure->setName(secondPointName);
    project->safeInsert(secondPointName, secondPointFigure);

    Point middlePoint = Algorithms::getMiddlePoint(firstPoint, secondPoint);

    auto dimFigure = new DimFigure(_dimensionName, middlePoint, firstPointName, secondPointName);
    dimFigure->setDimType(DimFigure::DimType::Distance); // TODO: Need DistanceBetweenPoints

    auto value = DimFigure::Value(DimFigure::ValueType::Length);
    value.nominal = _nominal;
    value.measurement = _measured;
    value.upperTolerance = _upperTolerance;
    value.lowerTolerance = _lowerTolerance;

    dimFigure->addValue(value);

    project->safeInsert(_dimensionName, dimFigure);
}

QString ChordLength::reportType() const
{
    return "Distance 2D";
}

QString ChordLength::dimensionName() const
{
    return _dimensionName;
}
