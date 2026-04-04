#include "curve/pch.h"

#include "markupcreator.h"
#include "turbineparameter.h"

TurbineParameter::TurbineParameter(double nominal, double UT, double LT, QString extraParam1, QString extraParam2)
{
    this->nominal = nominal;
    this->measured = 0;
    this->UT = UT;
    this->LT = LT;
    this->extraParam1 = extraParam1;
    this->extraParam2 = extraParam2;
    this->_dimName = nullptr;
}

QMap<QString, QString> TurbineParameter::toQMap(const TurbineParameter* param, int indexFromList)
{
    QString paramTemplate = "Type:%1,Nom:%2,UT:%3,LT:%4,ex1:%5,ex2:%6";
    auto result = QMap<QString, QString>();
    auto index = static_cast<int>(param->type);
    auto key = QString("Param %1 %2").arg(QString::number(index), QString::number(indexFromList));
    auto paramStr = paramTemplate.arg(turbineParamTypeToQString(param->type))
                        .arg(QString::number(param->nominal))
                        .arg(QString::number(param->UT))
                        .arg(QString::number(param->LT))
                        .arg(param->extraParam1)
                        .arg(param->extraParam2);
    result.insert(key, paramStr);
    return result;
}

QMap<QString, QString> TurbineParameter::toQMapFromCRM(const QStringList& params)
{
    if(params[2] == "0") {
        return QMap<QString, QString>();
    }
    QMap<QString, QString> paramsOfSameType;
    auto index = params[1].mid(3).toInt();
    auto type = static_cast<TurbineParameter::Type>(index);
    auto nominals = params[3].split(";");
    auto extra1 = params[6].split(";");
    auto paramTemplate = QString("Type:%1,Nom:%2,UT:%3,LT:%4,ex1:%5,ex2:%6");
    for(auto j = 0; j < nominals.size(); j++) {
        auto key = QString("Param %1 %2").arg(QString::number(index), QString::number(j));
        auto parameter = paramTemplate.arg(TurbineParameter::turbineParamTypeToQString(type))
                             .arg(nominals[j])
                             .arg(params[4])
                             .arg(params[5])
                             .arg(extra1[j])
                             .arg(params[7]);
        paramsOfSameType.insert(key, parameter);
    }
    return paramsOfSameType;
}

TurbineParameter* TurbineParameter::toTurbineParameter(const QString& turbineParam)
{
    auto paramPairs = QMap<QString, QString>();
    auto paramsStr = turbineParam.split(",");
    for(auto& param : paramsStr) {
        auto pair = param.split(":");
        paramPairs[pair[0]] = pair[1];
    }

    auto turbineParamType = turbineParamTypeFromQString(paramPairs["Type"]);
    switch(turbineParamType) {
        case Type::MaxWidth:
            return new MaxDiameter(paramPairs["Nom"].toDouble(), paramPairs["UT"].toDouble(),
                paramPairs["LT"].toDouble(), paramPairs["ex1"], paramPairs["ex2"]);
        case Type::MaxWidthX:
            return new XMaxDiameter(paramPairs["Nom"].toDouble(), paramPairs["UT"].toDouble(),
                paramPairs["LT"].toDouble(), paramPairs["ex1"], paramPairs["ex2"]);
        case Type::MaxWidthY:
            return new YMaxDiameter(paramPairs["Nom"].toDouble(), paramPairs["UT"].toDouble(),
                paramPairs["LT"].toDouble(), paramPairs["ex1"], paramPairs["ex2"]);
        case Type::ChordLength:
            return new ChordLength(paramPairs["Nom"].toDouble(), paramPairs["UT"].toDouble(),
                paramPairs["LT"].toDouble(), paramPairs["ex1"], paramPairs["ex2"]);
        case Type::LEWidth:
            return new WidthLE(paramPairs["Nom"].toDouble(), paramPairs["UT"].toDouble(),
                paramPairs["LT"].toDouble(), paramPairs["ex1"], paramPairs["ex2"]);
        case Type::TEWidth:
            return new WidthTE(paramPairs["Nom"].toDouble(), paramPairs["UT"].toDouble(),
                paramPairs["LT"].toDouble(), paramPairs["ex1"], paramPairs["ex2"]);
        case Type::LERadius:
            return new RadiusLE(paramPairs["Nom"].toDouble(), paramPairs["UT"].toDouble(),
                paramPairs["LT"].toDouble(), paramPairs["ex1"], paramPairs["ex2"]);
        case Type::TERadius:
            return new RadiusTE(paramPairs["Nom"].toDouble(), paramPairs["UT"].toDouble(),
                paramPairs["LT"].toDouble(), paramPairs["ex1"], paramPairs["ex2"]);
        case Type::ShiftX:
            return new ShiftX(paramPairs["Nom"].toDouble(), paramPairs["UT"].toDouble(),
                paramPairs["LT"].toDouble(), paramPairs["ex1"], paramPairs["ex2"]);
        case Type::ShiftY:
            return new ShiftY(paramPairs["Nom"].toDouble(), paramPairs["UT"].toDouble(),
                paramPairs["LT"].toDouble(), paramPairs["ex1"], paramPairs["ex2"]);
        case Type::Turn:
            return new Turn(paramPairs["Nom"].toDouble(), paramPairs["UT"].toDouble(),
                paramPairs["LT"].toDouble(), paramPairs["ex1"], paramPairs["ex2"]);
        case Type::MinX:
            return new MinX(paramPairs["Nom"].toDouble(), paramPairs["UT"].toDouble(),
                paramPairs["LT"].toDouble(), paramPairs["ex1"], paramPairs["ex2"]);
        default:
            return nullptr;
    }
}

QString TurbineParameter::turbineParamTypeToQString(TurbineParameter::Type type)
{
    return QMetaEnum::fromType<TurbineParameter::Type>().valueToKey(static_cast<int>(type));
}

TurbineParameter::Type TurbineParameter::turbineParamTypeFromQString(const QString& name)
{
    return static_cast<TurbineParameter::Type>(QMetaEnum::fromType<TurbineParameter::Type>().keyToValue(name.toLatin1()));
}

QString TurbineParameter::createParameterMarkup() const
{
    auto& tableRowTemplate = MarkupCreator::tableRowTemplate;
    auto title = _dimName;
    auto type = getMarkupType();
    auto oot = QString("<td></td>");
    auto deviation = measured - nominal;
    if(nominal == 0 && UT == 0 && LT == 0) {
        return tableRowTemplate
            .arg(title)
            .arg(type)
            .arg("")
            .arg("")
            .arg("")
            .arg(QString::number(measured, 'f', 3))
            .arg("")
            .arg(oot);
    }
    else if(UT == 0 && LT == 0) {
        return tableRowTemplate
            .arg(title)
            .arg(type)
            .arg(QString::number(nominal, 'f', 3))
            .arg("")
            .arg("")
            .arg(QString::number(measured, 'f', 3))
            .arg(QString::number(deviation, 'f', 3))
            .arg(oot);
    }
    else {
        oot = MarkupCreator::getOOTMarkup(UT, LT, deviation);
        return tableRowTemplate
            .arg(title)
            .arg(type)
            .arg(QString::number(nominal, 'f', 3))
            .arg(QString::number(UT, 'f', 3))
            .arg(QString::number(LT, 'f', 3))
            .arg(QString::number(measured, 'f', 3))
            .arg(QString::number(deviation, 'f', 3))
            .arg(oot);
    }
}

MaxDiameter::MaxDiameter(double nominal, double UT, double LT, QString extraParam1, QString extraParam2)
    : TurbineParameter(nominal, UT, LT, extraParam1, extraParam2)
{
    type = TurbineParameter::Type::MaxWidth;
}

void MaxDiameter::calculateNominal(const QString& nominalCurve, const Function18Params& params, Project* project)
{
    nominal = Algorithms::getMaxCircle(nominalCurve, &params, project).radius() * 2;
}

void MaxDiameter::createMeasured(const QString& nominalCurve, const QString& measuredCurve, const Function18Params& params, Project* project)
{
    auto measMaxDia = Algorithms::getMaxCircle(measuredCurve, &params, project);
    measured = measMaxDia.radius() * 2;

    auto circleName = QString("%1_CMaxDia").arg(measuredCurve);
    auto circleFigure = new CircleFigure(circleName, measMaxDia.center(), Point(0, 0, 1), measMaxDia.radius());
    project->safeInsert(circleName, circleFigure);

    auto value = DimFigure::Value(DimFigure::ValueType::Diameter);
    value.nominal = nominal;
    value.measurement = measured;
    value.upperTolerance = UT;
    value.lowerTolerance = LT;

    auto dimName = QString("%1 Maximum Diameter").arg(nominalCurve);
    auto labelPoint = Point(circleFigure->center().x + circleFigure->radius() + 1, circleFigure->center().y);
    auto dimFigure = new DimFigure(dimName, labelPoint, circleName);
    dimFigure->setDimType(DimFigure::DimType::Diameter);
    dimFigure->addValue(value);
    project->safeInsert(dimName, dimFigure);

    _dimName = dimName;
}

QString MaxDiameter::getMarkupType() const
{
    return QString("Diameter");
}

XMaxDiameter::XMaxDiameter(double nominal, double UT, double LT, QString extraParam1, QString extraParam2)
    : TurbineParameter(nominal, UT, LT, extraParam1, extraParam2)
{
    type = TurbineParameter::Type::MaxWidthX;
}

void XMaxDiameter::calculateNominal(const QString& nominalCurve, const Function18Params& params, Project* project)
{
    nominal = Algorithms::getMaxCircle(nominalCurve, &params, project).center().x;
}

void XMaxDiameter::createMeasured(const QString& nominalCurve, const QString& measuredCurve, const Function18Params& params, Project* project)
{
    auto measMaxDia = Algorithms::getMaxCircle(measuredCurve, &params, project);
    measured = measMaxDia.center().x;

    auto circleName = QString("%1_CMaxDia").arg(measuredCurve);
    auto circleFigure = new CircleFigure(circleName, measMaxDia.center(), Point(0, 0, 1), measMaxDia.radius());
    project->safeInsert(circleName, circleFigure);

    auto value = DimFigure::Value(DimFigure::ValueType::Radius);
    value.nominal = nominal;
    value.measurement = measured;
    value.upperTolerance = UT;
    value.lowerTolerance = LT;

    auto dimName = QString("%1_XD").arg(nominalCurve);
    auto labelPoint = Point(circleFigure->center().x, circleFigure->center().y - circleFigure->radius() - 1);
    auto dimFigure = new DimFigure(dimName, labelPoint, circleName);
    dimFigure->setDimType(DimFigure::DimType::Position);
    dimFigure->addValue(value);
    project->safeInsert(dimName, dimFigure);

    this->_dimName = dimName;
}

QString XMaxDiameter::getMarkupType() const
{
    return QString("X");
}

YMaxDiameter::YMaxDiameter(double nominal, double UT, double LT, QString extraParam1, QString extraParam2)
    : TurbineParameter(nominal, UT, LT, extraParam1, extraParam2)
{
    type = TurbineParameter::Type::MaxWidthY;
}

void YMaxDiameter::calculateNominal(const QString& nominalCurve, const Function18Params& params, Project* project)
{
    nominal = Algorithms::getMaxCircle(nominalCurve, &params, project).center().y;
}

void YMaxDiameter::createMeasured(const QString& nominalCurve, const QString& measuredCurve, const Function18Params& params, Project* project)
{
    auto measMaxDia = Algorithms::getMaxCircle(measuredCurve, &params, project);
    measured = measMaxDia.center().y;

    auto circleName = QString("%1_CMaxDia").arg(measuredCurve);
    auto circleFigure = new CircleFigure(circleName, measMaxDia.center(), Point(0, 0, 1), measMaxDia.radius());
    project->safeInsert(circleName, circleFigure);

    auto value = DimFigure::Value(DimFigure::ValueType::Radius);
    value.nominal = nominal;
    value.measurement = measured;
    value.upperTolerance = UT;
    value.lowerTolerance = LT;

    auto dimName = QString("%1_YD").arg(nominalCurve);
    auto labelPoint = Point(circleFigure->center().x, circleFigure->center().y + circleFigure->radius() + 1);
    auto dimFigure = new DimFigure(dimName, labelPoint, circleName);
    dimFigure->setDimType(DimFigure::DimType::Position);
    dimFigure->addValue(value);
    project->safeInsert(dimName, dimFigure);

    _dimName = dimName;
}

QString YMaxDiameter::getMarkupType() const
{
    return QString("Y");
}

ChordLength::ChordLength(double nominal, double UT, double LT, QString extraParam1, QString extraParam2)
    : TurbineParameter(nominal, UT, LT, extraParam1, extraParam2)
{
    type = TurbineParameter::Type::ChordLength;
}

void ChordLength::calculateNominal(const QString& nominalCurve, const Function18Params& params, Project* project)
{
    auto nomChord = Algorithms::getChord(nominalCurve, &params, project);
    auto [first, second] = nomChord;
    nominal = Algorithms::getDistanceBetweenPoints(Point(first.point()), Point(second.point()));
}

void ChordLength::createMeasured(const QString& nominalCurve, const QString& measuredCurve, const Function18Params& params, Project* project)
{
    auto measChord = Algorithms::getChord(measuredCurve, &params, project);
    auto [first, second] = measChord;
    measured = Algorithms::getDistanceBetweenPoints(Point(first.point()), Point(second.point()));

    auto firstPointName = QString("%1_Chord_P1").arg(measuredCurve);
    auto firstPointFigure = new PointFigure(firstPointName, first.point());
    project->safeInsert(firstPointName, firstPointFigure);
    auto secondPointName = QString("%1_Chord_P2").arg(measuredCurve);
    auto secondPointFigure = new PointFigure(secondPointName, second.point());
    project->safeInsert(secondPointName, secondPointFigure);

    auto value = DimFigure::Value(DimFigure::ValueType::Length);
    value.nominal = nominal;
    value.measurement = measured;
    value.upperTolerance = UT;
    value.lowerTolerance = LT;

    auto dimName = QString("%1_Chord").arg(nominalCurve);
    auto middlePoint = Algorithms::getMiddlePoint(Point(firstPointFigure->point()), Point(secondPointFigure->point()));
    auto dimFigure = new DimFigure(dimName, middlePoint, firstPointName, secondPointName);
    dimFigure->setDimType(DimFigure::DimType::Distance); // TODO: Need DistanceBetweenPoints
    dimFigure->addValue(value);
    project->safeInsert(dimName, dimFigure);

    _dimName = dimName;
}

QString ChordLength::getMarkupType() const
{
    return QString("Distance 2D");
}

WidthLE::WidthLE(double nominal, double UT, double LT, QString extraParam1, QString extraParam2)
    : TurbineParameter(nominal, UT, LT, extraParam1, extraParam2)
{
    type = TurbineParameter::Type::LEWidth;
}

void WidthLE::calculateNominal(const QString& nominalCurve, const Function18Params& params, Project* project)
{
    auto nomWidthLE = Algorithms::getWidthOfLeadingEdge(nominalCurve, &params, extraParam1.toDouble(), project);
    auto [firstPoint, secondPoint] = nomWidthLE;
    nominal = Algorithms::getDistanceBetweenPoints(Point(firstPoint), Point(secondPoint));
}

void WidthLE::createMeasured(const QString& nominalCurve, const QString& measuredCurve, const Function18Params& params, Project* project)
{
    auto measWidthLE = Algorithms::getWidthOfLeadingEdge(measuredCurve, &params, extraParam1.toDouble(), project);
    auto [firstPoint, secondPoint] = measWidthLE;
    measured = Algorithms::getDistanceBetweenPoints(Point(firstPoint), Point(secondPoint));

    auto pointName = QString("%1_LE_Width_P").arg(measuredCurve);
    auto firstPointName = project->getFreeName(pointName);
    auto firstPointFigure = new PointFigure(firstPointName, firstPoint);
    project->safeInsert(firstPointName, firstPointFigure);
    auto secondPointName = project->getFreeName(pointName);
    auto secondPointFigure = new PointFigure(secondPointName, secondPoint);
    project->safeInsert(secondPointName, secondPointFigure);

    auto value = DimFigure::Value(DimFigure::ValueType::Length);
    value.nominal = nominal;
    value.measurement = measured;
    value.upperTolerance = UT;
    value.lowerTolerance = LT;

    auto dimName = project->getFreeName(QString("%1_LE_Thck").arg(nominalCurve), true);
    auto middlePoint = Algorithms::getMiddlePoint(Point(firstPointFigure->point()), Point(firstPointFigure->point()));
    auto dimFigure = new DimFigure(dimName, middlePoint, firstPointName, secondPointName);
    dimFigure->setDimType(DimFigure::DimType::Distance); // TODO: Need DistanceBetweenPoints
    dimFigure->addValue(value);
    project->safeInsert(dimName, dimFigure);

    _dimName = dimName;

    auto log = QMap<QString, QString>(const_cast<Function18Params*>(&params)->toQMap());
    log.insert({ { "figureName", measuredCurve }, { "distanceLE", extraParam1 } });
    //MacrosManager::log(MacrosManager::CreateWidthOfLE, log);
}

QString WidthLE::getMarkupType() const
{
    return QString("Distance 2D");
}

WidthTE::WidthTE(double nominal, double UT, double LT, QString extraParam1, QString extraParam2)
    : TurbineParameter(nominal, UT, LT, extraParam1, extraParam2)
{
    type = TurbineParameter::Type::TEWidth;
}

void WidthTE::calculateNominal(const QString& nominalCurve, const Function18Params& params, Project* project)
{
    auto nomWidthTE = Algorithms::getWidthOfTrailingEdge(nominalCurve, &params, extraParam1.toDouble(), project);
    auto [firstPoint, secondPoint] = nomWidthTE;
    nominal = Algorithms::getDistanceBetweenPoints(Point(firstPoint), Point(secondPoint));
}

void WidthTE::createMeasured(const QString& nominalCurve, const QString& measuredCurve, const Function18Params& params, Project* project)
{
    auto measWidthTE = Algorithms::getWidthOfTrailingEdge(measuredCurve, &params, extraParam1.toDouble(), project);
    auto [firstPoint, secondPoint] = measWidthTE;
    measured = Algorithms::getDistanceBetweenPoints(Point(firstPoint), Point(secondPoint));

    auto pointName = QString("%1_TE_Width_P").arg(measuredCurve);
    auto firstPointName = project->getFreeName(pointName);
    auto firstPointFigure = new PointFigure(firstPointName, firstPoint);
    project->safeInsert(firstPointName, firstPointFigure);
    auto secondPointName = project->getFreeName(pointName);
    auto secondPointFigure = new PointFigure(secondPointName, secondPoint);
    project->safeInsert(secondPointName, secondPointFigure);

    auto value = DimFigure::Value(DimFigure::ValueType::Length);
    value.nominal = nominal;
    value.measurement = measured;
    value.upperTolerance = UT;
    value.lowerTolerance = LT;

    auto dimName = project->getFreeName(QString("%1_TE_Thck").arg(nominalCurve), true);
    auto middlePoint = Algorithms::getMiddlePoint(Point(firstPointFigure->point()), Point(firstPointFigure->point()));
    auto dimFigure = new DimFigure(dimName, middlePoint, firstPointName, secondPointName);
    dimFigure->setDimType(DimFigure::DimType::Distance); // TODO: Need DistanceBetweenPoints
    dimFigure->addValue(value);
    project->safeInsert(dimName, dimFigure);

    _dimName = dimName;

    auto log = QMap<QString, QString>(const_cast<Function18Params*>(&params)->toQMap());
    log.insert({ { "figureName", measuredCurve }, { "distanceTE", extraParam1 } });
    //MacrosManager::log(MacrosManager::CreateWidthOfTE, log);
}

QString WidthTE::getMarkupType() const
{
    return QString("Distance 2D");
}

RadiusLE::RadiusLE(double nominal, double UT, double LT, QString extraParam1, QString extraParam2)
    : TurbineParameter(nominal, UT, LT, extraParam1, extraParam2)
{
    type = TurbineParameter::Type::LERadius;
}

void RadiusLE::calculateNominal(const QString& nominalCurve, const Function18Params& params, Project* project)
{
    nominal = Algorithms::getRadiusOfLeadingEdge(nominalCurve, &params, extraParam1.toDouble(), project).radius();
}

void RadiusLE::createMeasured(const QString& nominalCurve, const QString& measuredCurve, const Function18Params& params, Project* project)
{
    auto circle = Algorithms::getRadiusOfLeadingEdge(measuredCurve, &params, extraParam1.toDouble(), project);
    measured = circle.radius();

    auto circleName = QString("%1_LE_CRadius").arg(measuredCurve);
    auto circleFigure = new CircleFigure(circleName, circle.center(), circle.normal(), circle.radius());
    project->safeInsert(circleName, circleFigure);

    auto value = DimFigure::Value(DimFigure::ValueType::Radius);
    value.nominal = nominal;
    value.measurement = measured;
    value.upperTolerance = UT;
    value.lowerTolerance = LT;

    auto dimName = QString("%1_LE_Radius").arg(nominalCurve);
    auto labelPoint = Point(circleFigure->center().x + circleFigure->radius() + 1, circleFigure->center().y);
    auto dimFigure = new DimFigure(dimName, labelPoint, circleName);
    dimFigure->setDimType(DimFigure::DimType::Radius);
    dimFigure->addValue(value);
    project->safeInsert(dimName, dimFigure);

    _dimName = dimName;
}

QString RadiusLE::getMarkupType() const
{
    return QString("Radius");
}

RadiusTE::RadiusTE(double nominal, double UT, double LT, QString extraParam1, QString extraParam2)
    : TurbineParameter(nominal, UT, LT, extraParam1, extraParam2)
{
    type = TurbineParameter::Type::TERadius;
}

void RadiusTE::calculateNominal(const QString& nominalCurve, const Function18Params& params, Project* project)
{
    nominal = Algorithms::getRadiusOfTrailingEdge(nominalCurve, &params, extraParam1.toDouble(), project).radius();
}

void RadiusTE::createMeasured(const QString& nominalCurve, const QString& measuredCurve, const Function18Params& params, Project* project)
{
    auto circle = Algorithms::getRadiusOfTrailingEdge(measuredCurve, &params, extraParam1.toDouble(), project);
    measured = circle.radius();

    auto circleName = QString("%1_TE_CRadius").arg(measuredCurve);
    auto circleFigure = new CircleFigure(circleName, circle.center(), circle.normal(), circle.radius());
    project->safeInsert(circleName, circleFigure);

    auto value = DimFigure::Value(DimFigure::ValueType::Radius);
    value.nominal = nominal;
    value.measurement = measured;
    value.upperTolerance = UT;
    value.lowerTolerance = LT;

    auto dimName = QString("%1_TE_Radius").arg(nominalCurve);
    auto labelPoint = Point(circleFigure->center().x + circleFigure->radius() + 1, circleFigure->center().y);
    auto dimFigure = new DimFigure(dimName, labelPoint, circleName);
    dimFigure->setDimType(DimFigure::DimType::Radius);
    dimFigure->addValue(value);
    project->safeInsert(dimName, dimFigure);

    _dimName = dimName;
}

QString RadiusTE::getMarkupType() const
{
    return QString("Radius");
}

ShiftX::ShiftX(double nominal, double UT, double LT, QString extraParam1, QString extraParam2)
    : TurbineParameter(nominal, UT, LT, extraParam1, extraParam2)
{
    type = TurbineParameter::Type::ShiftX;
}

void ShiftX::calculateNominal(const QString& nominalCurve, const Function18Params& params, Project* project)
{
}

void ShiftX::createMeasured(const QString& nominalCurve, const QString& measuredCurve, const Function18Params& params, Project* project)
{
}

QString ShiftX::getMarkupType() const
{
    return QString();
}

ShiftY::ShiftY(double nominal, double UT, double LT, QString extraParam1, QString extraParam2)
    : TurbineParameter(nominal, UT, LT, extraParam1, extraParam2)
{
    type = TurbineParameter::Type::ShiftY;
}

void ShiftY::calculateNominal(const QString& nominalCurve, const Function18Params& params, Project* project)
{
}

void ShiftY::createMeasured(const QString& nominalCurve, const QString& measuredCurve, const Function18Params& params, Project* project)
{
}

QString ShiftY::getMarkupType() const
{
    return QString();
}

Turn::Turn(double nominal, double UT, double LT, QString extraParam1, QString extraParam2)
    : TurbineParameter(nominal, UT, LT, extraParam1, extraParam2)
{
    type = TurbineParameter::Type::Turn;
}

void Turn::calculateNominal(const QString& nominalCurve, const Function18Params& params, Project* project)
{
}

void Turn::createMeasured(const QString& nominalCurve, const QString& measuredCurve, const Function18Params& params, Project* project)
{
}

QString Turn::getMarkupType() const
{
    return QString();
}

MinX::MinX(double nominal, double UT, double LT, QString extraParam1, QString extraParam2)
    : TurbineParameter(nominal, UT, LT, extraParam1, extraParam2)
{
    type = TurbineParameter::Type::MinX;
}

void MinX::calculateNominal(const QString& nominalCurve, const Function18Params& params, Project* project)
{
    nominal = Algorithms::getMinX(nominalCurve, &params, project);
}

void MinX::createMeasured(const QString& nominalCurve, const QString& measuredCurve, const Function18Params& params, Project* project)
{
    measured = Algorithms::getMinX(measuredCurve, &params, project);
    auto minXPoint = CurvePoint(measured, 0, 0);

    auto pointName = QString("%1_MinX_P").arg(measuredCurve);
    auto pointFigure = new PointFigure(pointName, minXPoint);
    project->safeInsert(pointName, pointFigure);

    auto value = DimFigure::Value(DimFigure::ValueType::X);
    value.nominal = nominal;
    value.measurement = measured;
    value.upperTolerance = UT;
    value.lowerTolerance = LT;

    auto dimName = QString("%1 MinX").arg(nominalCurve);
    auto labelPoint = Point(minXPoint.x + 1, minXPoint.y);
    auto dimFigure = new DimFigure(dimName, labelPoint, pointName);
    dimFigure->setDimType(DimFigure::DimType::Position);
    dimFigure->addValue(value);
    project->safeInsert(dimFigure->name(), dimFigure);

    _dimName = dimName;
}

QString MinX::getMarkupType() const
{
    return QString("X");
}
