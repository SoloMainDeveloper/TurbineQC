#include "curve/pch.h"

#include "algorithms.h"
#include "deviationstatistics.h"
#include "figurecreator.h"
#include "functionparamsfactory.h"
#include "reportgenerator.h"

void FigureCreator::createAdditionalFigures(Project* project, std::shared_ptr<ReportSettings> reportSettings)
{
    auto nominalName = reportSettings->nominalName();
    auto measuredName = reportSettings->measuredName();

    auto dummyNames = GeometryAnalysisService::getTemplateInterimNames(nominalName, measuredName);
    auto preparedMeasName = dummyNames[GeometryAnalysisService::InterimName::MeasuredCurve];

    auto additionalNames = GeometryAnalysisService::getTemplateAdditionalNames(nominalName, preparedMeasName);
    auto params18 = FunctionParamsFactory(reportSettings).params18();

    using Name = GeometryAnalysisService::AdditionalName;

    if(reportSettings->needMCL()) {
        BladeGeometryService::createMiddleCurve(nominalName, additionalNames[Name::NominalMCL], &params18, project);
        BladeGeometryService::createMiddleCurve(preparedMeasName, additionalNames[Name::MeasuredMCL], &params18, project, Qt::blue);
    }
    if(reportSettings->needMaxDiameter()) {
        BladeGeometryService::createMaxCircle(nominalName, params18, additionalNames[Name::NominalMaxDia], Qt::black);
        BladeGeometryService::createMaxCircle(preparedMeasName, params18, additionalNames[Name::MeasuredMaxDia], Qt::blue);
    }
    if(reportSettings->needContactLine()) {
        BladeGeometryService::createContactLine(nominalName, additionalNames[Name::NominalCntctLine], &params18, project);
        BladeGeometryService::createContactLine(preparedMeasName, additionalNames[Name::MeasuredCntctLine], &params18, project, Qt::blue);
    }
}

void FigureCreator::alignAdditionalFigures(Project* project, std::shared_ptr<ReportSettings> reportSettings)
{
    auto nominalName = reportSettings->nominalName();
    auto measuredName = reportSettings->measuredName();

    auto dummyNames = GeometryAnalysisService::getTemplateInterimNames(nominalName, measuredName);
    auto preparedMeasName = dummyNames[GeometryAnalysisService::InterimName::MeasuredCurve];

    auto additionalNames = GeometryAnalysisService::getTemplateAdditionalNames(reportSettings->nominalName(), preparedMeasName);
    using Name = GeometryAnalysisService::AdditionalName;

    auto xShift = reportSettings->xShift();
    auto yShift = reportSettings->yShift();
    auto rotation = reportSettings->rotation();

    if(xShift == 0 && yShift == 0 && rotation == 0) {
        return;
    }

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

FigureCreator::FigureCreator(std::shared_ptr<ReportSettings> reportSettings)
    : _reportSettings(reportSettings)
{
    _project = &Project::instance();
}

void FigureCreator::run(const QMap<CurveType, QPair<QString, QVector<CurvePoint>>>& globalCurvesToCreate)
{
    using FormName = GeometryAnalysisService::FormName;
    auto nominalName = _reportSettings->nominalName();
    auto templateFormName = GeometryAnalysisService::getTemplateFormNames(nominalName);

    for(auto [curveType, curve] : globalCurvesToCreate.asKeyValueRange()) {
        auto [name, points] = curve;
        switch(curveType) {
            case CurveType::WholeGlobal: {
                auto createdCurve = createGlobalCurve(name, points, true);
                createDimension(templateFormName[FormName::GlobalForm], createdCurve);
                break;
            }
            case CurveType::GlobalWithoutTE: {
                auto createdCurve = createGlobalCurve(name, points, false);
                createDimension(templateFormName[FormName::GlobalForm], createdCurve);
                break;
            }
            case CurveType::GlobalCV: {
                auto createdCurve = createGlobalPart(name, points);
                createDimension(templateFormName[FormName::CVForm], createdCurve);
                break;
            }
            case CurveType::GlobalCC: {
                auto createdCurve = createGlobalPart(name, points);
                createDimension(templateFormName[FormName::CCForm], createdCurve);
                break;
            }
            case CurveType::GlobalLE: {
                if(_reportSettings->isLeadingEdgeVisible()) {
                    auto createdCurve = createGlobalEdge(name, points, _reportSettings->leadingEdgeAmplification());
                    createDimension(templateFormName[FormName::LEForm], createdCurve);
                }
                break;
            }
            case CurveType::GlobalTE: {
                if(_reportSettings->isTrailingEdgeVisible()) {
                    auto createdCurve = createGlobalEdge(name, points, _reportSettings->trailingEdgeAmplification());
                    createDimension(templateFormName[FormName::TEForm], createdCurve);
                }
                break;
            }
            default:
                break;
        }
    }
}

CurveFigure* FigureCreator::createGlobalCurve(const QString& globalName, const QVector<CurvePoint>& globalPoints, bool isClosed)
{
    auto globalCurve = new CurveFigure(globalName, globalPoints);
    globalCurve->setClosed(isClosed);
    globalCurve->setShowDeviations(true);
    globalCurve->setConnectDeviations(true);
    globalCurve->setHighLightOut(true);
    globalCurve->setShowTolerances(true);
    globalCurve->setAmplification(_reportSettings->globalAmplification());
    _project->safeInsert(globalName, globalCurve, false);

    return globalCurve;
}

CurveFigure* FigureCreator::createGlobalEdge(const QString& edgeName, const QVector<CurvePoint>& points, double amplification)
{
    auto edge = new CurveFigure(edgeName, points);
    edge->setShowDeviations(true);
    edge->setConnectDeviations(true);
    edge->setHighLightOut(true);
    edge->setShowTolerances(true);
    edge->setAmplification(amplification);
    _project->safeInsert(edgeName, edge, false);

    return edge;
}

CurveFigure* FigureCreator::createGlobalPart(const QString& curveName, const QVector<CurvePoint>& points)
{
    auto curve = new CurveFigure(curveName, points);
    curve->setShowDeviations(true);
    curve->setConnectDeviations(true);
    curve->setHighLightOut(true);
    curve->setShowTolerances(true);
    curve->setAmplification(_reportSettings->globalAmplification());
    _project->safeInsert(curveName, curve, false);

    return curve;
}

void FigureCreator::createDimension(const QString& dimensionName, const CurveFigure* globalCurve, const Point& labelPoint)
{
    auto deviationStatistics = DeviationStatistics(globalCurve->points());

    QMap<int, QPair<DimFigure::ValueType, std::function<double()>>> statisticsMap = {
        { 6, { DimFigure::ValueType::MinMax, [&]() {
                  return deviationStatistics.minMax();
              } } },
        { 5, { DimFigure::ValueType::Form, [&]() {
                  return deviationStatistics.form();
              } } },
        { 4, { DimFigure::ValueType::Min, [&]() {
                  return deviationStatistics.min();
              } } },
        { 3, { DimFigure::ValueType::Max, [&]() {
                  return deviationStatistics.max();
              } } },
        { 2, { DimFigure::ValueType::MaxAbs, [&]() {
                  return deviationStatistics.maxAbs();
              } } },
        { 1, { DimFigure::ValueType::SupUT, [&]() {
                  return deviationStatistics.superiorUpperTolerance();
              } } },
        { 0, { DimFigure::ValueType::InfLT, [&]() {
                  return deviationStatistics.inferiorLowerTolerance();
              } } },
    };

    int outputFormMode = _reportSettings->outputFormMode();
    int formValuesNumber = 7;
    QString binCode = QString("%1").arg(outputFormMode, formValuesNumber, 2, QChar('0'));

    auto table = new DimFigure(dimensionName, labelPoint, globalCurve->name());
    table->setDimType(DimFigure::DimType::Form);
    table->setVisible(false);

    for(auto [key, statistics] : statisticsMap.asKeyValueRange()) {
        if(QString(binCode[key]).toInt()) {
            auto [type, value] = statistics;
            table->addValue(DimFigure::Value(type, true, value()));
        }
    }

    _project->safeInsert(dimensionName, table, false);
}
