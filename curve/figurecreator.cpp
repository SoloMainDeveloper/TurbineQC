#include "curve/pch.h"
#include "figurecreator.h"

void FigureCreator::createAdditionalFigures(Project *project, std::shared_ptr<ReportSettings> reportSettings) {
    auto nominalName = reportSettings->nominalName();
    auto measuredName = reportSettings->measuredName();

    auto dummyNames = ReportGenerator::getTemplateInterimNames(nominalName, measuredName);
    auto preparedMeasName = dummyNames[ReportGenerator::InterimName::MeasuredCurve];

    auto additionalNames = ReportGenerator::getTemplateAdditionalNames(nominalName, preparedMeasName);
    auto params18 = FunctionParamsGenerator::params18(project, reportSettings);

    using Name = ReportGenerator::AdditionalName;

    if(reportSettings->needMCL()) {
        Algorithms::createMiddleCurve(nominalName, additionalNames[Name::NominalMCL], &params18, project);
        Algorithms::createMiddleCurve(preparedMeasName, additionalNames[Name::MeasuredMCL], &params18, project, Qt::blue);
    }
    if(reportSettings->needMaxDiameter()) {
        Algorithms::createMaxCircle(nominalName, additionalNames[Name::NominalMaxDia], &params18, project);
        Algorithms::createMaxCircle(preparedMeasName, additionalNames[Name::MeasuredMaxDia], &params18, project, Qt::blue);
    }
    if(reportSettings->needContactLine()) {
        Algorithms::createContactLine(nominalName, additionalNames[Name::NominalCntctLine], &params18, project);
        Algorithms::createContactLine(preparedMeasName, additionalNames[Name::MeasuredCntctLine], &params18, project, Qt::blue);
    }
}

void FigureCreator::alignAdditionalFigures(Project *project, std::shared_ptr<ReportSettings> reportSettings) {
    auto nominalName = reportSettings->nominalName();
    auto measuredName = reportSettings->measuredName();

    auto dummyNames = ReportGenerator::getTemplateInterimNames(nominalName, measuredName);
    auto preparedMeasName = dummyNames[ReportGenerator::InterimName::MeasuredCurve];

    auto additionalNames = ReportGenerator::getTemplateAdditionalNames(reportSettings->nominalName(), preparedMeasName);
    using Name = ReportGenerator::AdditionalName;

    auto xShift = reportSettings->xShift();
    auto yShift = reportSettings->yShift();
    auto rotation = reportSettings->rotation();

    if(reportSettings->needMCL()) {
        auto figure = project->findFigure(additionalNames[Name::MeasuredMCL]);
        auto measuredMCL = dynamic_cast<const CurveFigure*>(figure);

        ARGUMENT_ASSERT(measuredMCL, "Align additional figures: curve's not found");

        auto alignmentMCL = const_cast<CurveFigure*>(measuredMCL);
        alignmentMCL->alignment(rotation, xShift, yShift);
        project->safeInsert(alignmentMCL->name(), alignmentMCL);
    }
    if(reportSettings->needMaxDiameter()) {
        auto figure = project->findFigure(additionalNames[Name::MeasuredMaxDia]);
        auto measuredMaxDia = dynamic_cast<const CircleFigure*>(figure);

        ARGUMENT_ASSERT(measuredMaxDia, "Align additional figures: circle's not found");

        auto alignmentMaxDia = const_cast<CircleFigure*>(measuredMaxDia);
        alignmentMaxDia->alignment(rotation, xShift, yShift);
        project->safeInsert(alignmentMaxDia->name(), alignmentMaxDia);
    }
    if(reportSettings->needContactLine()) {
        auto figure = project->findFigure(additionalNames[Name::MeasuredCntctLine]);
        auto measuredCntctLine = dynamic_cast<const LineFigure*>(figure);

        ARGUMENT_ASSERT(measuredCntctLine, "Align additional figures: line's not found");

        auto alignmentCntctLine = const_cast<LineFigure*>(measuredCntctLine);
        alignmentCntctLine->alignment(rotation, xShift, yShift);
        project->safeInsert(alignmentCntctLine->name(), alignmentCntctLine);
    }
}

FigureCreator::FigureCreator(Project * project, std::shared_ptr<ReportSettings> reportSettings) 
    : _project(project), _reportSettings(reportSettings) {
}

void FigureCreator::run(const QMap<CurveType, QPair<QString, QVector<CurvePoint>>> &globalCurvesToCreate) {
    using FormName = ReportGenerator::FormName;
    auto nominalName = _reportSettings->nominalName();
    auto templateFormName = ReportGenerator::getTemplateFormNames(nominalName);

    for(auto [curveType, curve] : globalCurvesToCreate.asKeyValueRange()) {
        auto [name, points] = curve;
        switch(curveType) {
            case CurveType::WholeGlobal:
            {
                auto createdCurve = createGlobalCurve(name, points, true);
                createDimension(templateFormName[FormName::GlobalForm], createdCurve);
                break;
            }
            case CurveType::GlobalWithoutTE:
            {
                auto createdCurve = createGlobalCurve(name, points, false);
                createDimension(templateFormName[FormName::GlobalForm], createdCurve);
                break;
            }
            case CurveType::GlobalCV:
            {
                auto createdCurve = createGlobalPart(name, points);
                createDimension(templateFormName[FormName::CVForm], createdCurve);
                break;
            }
            case CurveType::GlobalCC:
            {
                auto createdCurve = createGlobalPart(name, points);
                createDimension(templateFormName[FormName::CCForm], createdCurve);
                break;
            }
            case CurveType::GlobalLE:
            {
                if(_reportSettings->isLEVisible()) {
                    auto createdCurve = createGlobalEdge(name, points, _reportSettings->amplificationOfLE());
                    createDimension(templateFormName[FormName::LEForm], createdCurve);
                }
                break;
            }
            case CurveType::GlobalTE:
            {
                if(_reportSettings->isTEVisible()) {
                    auto createdCurve = createGlobalEdge(name, points, _reportSettings->amplificationOfTE());
                    createDimension(templateFormName[FormName::TEForm], createdCurve);
                }
                break;
            }
            default:
                break;
        }
    }
}

CurveFigure* FigureCreator::createGlobalCurve(const QString &globalName, const QVector<CurvePoint> &globalPoints, bool isClosed) {
    auto globalCurve = new CurveFigure(globalName, globalPoints);
    globalCurve->setClosed(isClosed);
    globalCurve->setShowDeviations(true);
    globalCurve->setConnectDeviations(true);
    globalCurve->setAmplification(_reportSettings->globalAmplification());
    _project->safeInsert(globalName, globalCurve, false);

    return globalCurve;
}

CurveFigure* FigureCreator::createGlobalEdge(const QString &edgeName, const QVector<CurvePoint> &points, double amplification) {
    auto edge = new CurveFigure(edgeName, points);
    edge->setShowDeviations(true);
    edge->setConnectDeviations(true);
    edge->setAmplification(amplification);
    _project->safeInsert(edgeName, edge, false);

    return edge;
}

CurveFigure* FigureCreator::createGlobalPart(const QString &curveName, const QVector<CurvePoint> &points) {
    auto curve = new CurveFigure(curveName, points);
    curve->setShowDeviations(true);
    curve->setConnectDeviations(true);
    curve->setAmplification(_reportSettings->globalAmplification());
    _project->safeInsert(curveName, curve, false);

    return curve;
}

void FigureCreator::createDimension(const QString &dimensionName, const CurveFigure *globalCurve, const Point &labelPoint) {
    auto table = new DimFigure(dimensionName, labelPoint, globalCurve->name());
    table->setDimType(DimFigure::DimType::Form);
    table->setVisible(false);

    auto &points = globalCurve->points();
    auto minDeviation = points[0].dev;
    auto maxDeviation = points[0].dev;
    for(auto &point : points) {
        if(point.dev < minDeviation) {
            minDeviation = point.dev;
        }
        if(point.dev > maxDeviation) {
            maxDeviation = point.dev;
        }
    }

    table->addValues(QVector<DimFigure::Value> {
        DimFigure::Value(DimFigure::ValueType::MinMax, true, abs(maxDeviation - minDeviation)),
            DimFigure::Value(DimFigure::ValueType::Form, true, std::max(abs(maxDeviation), abs(minDeviation) * 2)),
            DimFigure::Value(DimFigure::ValueType::Min, true, minDeviation),
            DimFigure::Value(DimFigure::ValueType::Max, true, maxDeviation),
            DimFigure::Value(DimFigure::ValueType::MaxAbs, false, abs(std::max(minDeviation, maxDeviation))),
            DimFigure::Value(DimFigure::ValueType::SupUT, true, maxDeviation > 0 ? maxDeviation : 0.0),
            DimFigure::Value(DimFigure::ValueType::InfLT, true, minDeviation < 0 ? minDeviation : 0.0),
    });
    _project->safeInsert(dimensionName, table, false);
}
