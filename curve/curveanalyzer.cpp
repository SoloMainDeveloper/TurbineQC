#include "curve/pch.h"

#include "curveanalyzer.h"
#include "algorithms.h"
#include "curveanalyzer.h"
#include "functionparamsfactory.h"
#include "project.h"
#include "reportgenerator.h"
#include "reportsettings.h"

CurveAnalyzer::CurveAnalyzer(std::shared_ptr<ReportSettings> reportSettings)
{
    _project = &Project::instance();

    _reportSettings = reportSettings;

    _paramsFactory = std::make_unique<FunctionParamsFactory>(_reportSettings);

    _nominalName = _reportSettings->nominalName();
    _measuredName = _reportSettings->measuredName();

    if(_nominalName == _measuredName) {
        _measuredName = "_" + _measuredName;
    }

    auto globalNames = ReportGenerator::getTemplateGlobalNames(_nominalName, _measuredName);
    using GlobalName = ReportGenerator::GlobalName;
    _globalName = globalNames[GlobalName::GlobalCurve];
    _globalCVName = globalNames[GlobalName::GlobalCV];
    _globalCCName = globalNames[GlobalName::GlobalCC];
    _globalLEName = globalNames[GlobalName::GlobalLE];
    _globalTEName = globalNames[GlobalName::GlobalTE];

    auto dummyNames = ReportGenerator::getTemplateInterimNames(_nominalName, _measuredName);
    using InterimName = ReportGenerator::InterimName;

    _dummyNominalName = dummyNames[InterimName::NominalCurve];
    _dummyMeasuredName = dummyNames[InterimName::MeasuredCurve];

    _dummyNominalCVName = dummyNames[InterimName::NominalCV];
    _dummyMeasuredCVName = dummyNames[InterimName::MeasuredCV];
    _dummyNominalCCName = dummyNames[InterimName::NominalCC];
    _dummyMeasuredCCName = dummyNames[InterimName::MeasuredCC];

    _dummyNominalLEName = dummyNames[InterimName::NominalLE];
    _dummyMeasuredLEName = dummyNames[InterimName::MeasuredLE];
    _dummyNominalTEName = dummyNames[InterimName::NominalTE];
    _dummyMeasuredTEName = dummyNames[InterimName::MeasuredTE];
}

GlobalCurveMap CurveAnalyzer::run()
{
    auto params18 = _paramsFactory->params18();
    auto [nominalParts, measuredParts] = analyzeProfile(params18);

    using Profile = ReportSettings::Profile;
    auto profileType = _reportSettings->profileType();
    GlobalCurveMap result;

    if(profileType == Profile::Whole) {
        GlobalCurveMap globalCurve = analyzeWholeProfile(params18);
        result.insert(globalCurve);
    }
    else if(profileType == Profile::WithoutTE) {
        GlobalCurveMap globalCurve = analyzeProfileWithoutTE(params18);
        result.insert(globalCurve);
    }
    else if(profileType == Profile::WithoutEdges) {
        GlobalCurveMap globalCurves = analyzeProfileWithoutEdges(nominalParts, measuredParts, params18);
        result.insert(globalCurves);
    }
    else if(profileType == Profile::WithoutEdgesLSQ) {
        GlobalCurveMap globalCurves = analyzeProfileWithoutEdgesLSQ(nominalParts, measuredParts, params18);
        result.insert(globalCurves);
    }
    else if(profileType == Profile::WithoutEdgesForm) {
        GlobalCurveMap globalCurves = analyzeProfileWithoutEdgesForm(nominalParts, measuredParts, params18);
        result.insert(globalCurves);
    }
    else {
        return result;
    }

    CurveParts globalParts = Algorithms::divideCurveIntoParts(_dummyMeasuredName, &params18, _project);

    using EdgeBestFit = ReportSettings::EdgeBestFit;

    EdgeBestFit leadingEdgeBestFit = _reportSettings->leadingEdgeBestFit();
    auto lineLEBestFit = QString("%1_LE_BF").arg(_measuredName);
    calculateEdgeBestFit(_dummyNominalLEName, nominalParts.pointsOfLE, _dummyMeasuredLEName,
        measuredParts.pointsOfLE, lineLEBestFit, leadingEdgeBestFit);
    const CurveFigure* leadingEdge = calculateEdgeDeviations(params18, _dummyNominalLEName,
        _dummyMeasuredLEName, _globalLEName, globalParts.pointsOfLE, leadingEdgeBestFit);
    result.insert({ { FigureCreator::CurveType::GlobalLE, { leadingEdge->name(), leadingEdge->points() } } });

    EdgeBestFit trailingEdgeBestFit = _reportSettings->trailingEdgeBestFit();
    auto lineTEBestFit = QString("%1_TE_BF").arg(_measuredName);
    calculateEdgeBestFit(_dummyNominalTEName, nominalParts.pointsOfTE, _dummyMeasuredTEName,
        measuredParts.pointsOfTE, lineTEBestFit, _reportSettings->trailingEdgeBestFit());
    const CurveFigure* trailingEdge = calculateEdgeDeviations(params18, _dummyNominalTEName,
        _dummyMeasuredTEName, _globalTEName, globalParts.pointsOfTE, trailingEdgeBestFit);
    result.insert({ { FigureCreator::CurveType::GlobalTE, { trailingEdge->name(), trailingEdge->points() } } });

    deleteDummyCurves();

    return result;
}

QPair<CurveAnalyzer::CurveParts, CurveAnalyzer::CurveParts> CurveAnalyzer::analyzeProfile(const Function18Params& params18)
{
    auto nominalCurve = dynamic_cast<const CurveFigure*>(_project->findFigure(_nominalName));
    auto measuredCurve = dynamic_cast<const CurveFigure*>(_project->findFigure(_measuredName));

    insertCurveInProject(_dummyNominalName, nominalCurve->points(), true);
    insertCurveInProject(_dummyMeasuredName, measuredCurve->points(), true);

    auto nominalParts = Algorithms::divideCurveIntoParts(_dummyNominalName, &params18, _project);
    auto direction = _reportSettings->leadingEdgeDirection();
    Algorithms::reassembleWholeCurve(_dummyNominalName, nominalParts, direction, _project);

    calculatePreprocessingFunctions(_nominalName, _dummyMeasuredName);

    Algorithms::calculateMeasuredParams(_reportSettings, _dummyMeasuredName);
    FigureCreator::createAdditionalFigures(_project, _reportSettings);

    auto measuredParts = Algorithms::divideCurveIntoParts(_dummyMeasuredName, &params18, _project);
    Algorithms::reassembleWholeCurve(_dummyMeasuredName, measuredParts, direction, _project);

    auto globalBestFit = _reportSettings->globalBestFit();
    using GlobalBestFit = ReportSettings::GlobalBestFit;
    if(globalBestFit == GlobalBestFit::WholeLSQ) {
        auto params19 = Function19Params();
        Algorithms::regenerateCurve(_dummyMeasuredName, _dummyMeasuredName, &params19);
        auto params6 = _paramsFactory->params6ForGlobalFit();
        calculateBestFit(_dummyNominalName, _dummyMeasuredName, params6);
    }
    else if(globalBestFit == GlobalBestFit::WithoutEdgesLSQ) {
        auto params6 = _paramsFactory->params6ForGlobalFit();
        calculateBestFitWithAlignment(_dummyNominalName, _dummyMeasuredName, params6, nominalParts, &measuredParts);
    }
    else if(globalBestFit == GlobalBestFit::MinForm) {
        auto params21 = _paramsFactory->params21ForGlobalFit();
        calculateBestFit(_nominalName, _dummyMeasuredName, params21);
    }
    FigureCreator::alignAdditionalFigures(_project, _reportSettings);

    return QPair<CurveParts, CurveParts>(nominalParts, measuredParts);
}

GlobalCurveMap CurveAnalyzer::analyzeWholeProfile(const Function18Params& params18)
{
    auto params4 = _paramsFactory->params4(params18, true);
    Algorithms::calculateDeviations(_dummyNominalName, _dummyMeasuredName, _dummyMeasuredName, &params4);

    auto globalCurve = getCurveFromProject(_dummyMeasuredName);

    return GlobalCurveMap { { FigureCreator::CurveType::WholeGlobal, { _globalName, globalCurve->points() } } };
}

GlobalCurveMap CurveAnalyzer::analyzeProfileWithoutTE(const Function18Params& params18)
{
    auto params4 = _paramsFactory->params4(params18, true);
    Algorithms::calculateDeviations(_dummyNominalName, _dummyMeasuredName, _dummyMeasuredName, &params4);

    auto globalParts = Algorithms::divideCurveIntoParts(_dummyMeasuredName, &params18, _project);
    Algorithms::reassembleCurveWithoutTE(_globalName, globalParts, _reportSettings->leadingEdgeDirection(), _project);

    auto globalCurve = getCurveFromProject(_globalName);

    return GlobalCurveMap { { FigureCreator::CurveType::WholeGlobal, { globalCurve->name(), globalCurve->points() } } };
}

GlobalCurveMap CurveAnalyzer::analyzeProfileWithoutEdges(const CurveParts& nominalParts,
    const CurveParts& measuredParts, const Function18Params& params18)
{
    auto params4 = _paramsFactory->params4(params18, true);
    Algorithms::calculateDeviations(_dummyNominalName, _dummyMeasuredName, _dummyMeasuredName, &params4);

    params4 = _paramsFactory->params4(params18, false);

    insertCurveInProject(_dummyNominalCVName, nominalParts.pointsOfHigh, true);
    insertCurveInProject(_dummyMeasuredCVName, measuredParts.pointsOfHigh, true);
    Algorithms::calculateDeviations(_dummyNominalCVName, _dummyMeasuredCVName, _dummyMeasuredCVName, &params4);

    insertCurveInProject(_dummyNominalCCName, nominalParts.pointsOfLow, true);
    insertCurveInProject(_dummyMeasuredCCName, measuredParts.pointsOfLow, true);
    Algorithms::calculateDeviations(_dummyNominalCCName, _dummyMeasuredCCName, _dummyMeasuredCCName, &params4);

    auto globalConvexCurve = getCurveFromProject(_dummyMeasuredCVName);
    auto globalConcaveCurve = getCurveFromProject(_dummyMeasuredCCName);

    return GlobalCurveMap {
        { FigureCreator::CurveType::GlobalCV, { _globalCVName, globalConvexCurve->points() } },
        { FigureCreator::CurveType::GlobalCC, { _globalCCName, globalConcaveCurve->points() } },
    };
}

GlobalCurveMap CurveAnalyzer::analyzeProfileWithoutEdgesLSQ(const CurveParts& nominalParts,
    const CurveParts& measuredParts, const Function18Params& params18)
{
    insertCurveInProject(_dummyNominalCVName, nominalParts.pointsOfHigh, true);
    insertCurveInProject(_dummyMeasuredCVName, measuredParts.pointsOfHigh, true);
    insertCurveInProject(_dummyNominalCCName, nominalParts.pointsOfLow, true);
    insertCurveInProject(_dummyMeasuredCCName, measuredParts.pointsOfLow, true);

    auto lineCVName = QString("%1_CV_Fit").arg(_measuredName);
    auto lineCCName = QString("%1_CC_Fit").arg(_measuredName);
    auto params6 = _paramsFactory->params6ForLocalFit();
    Algorithms::calculateBestFit(_dummyMeasuredCVName, _dummyNominalCVName, _dummyMeasuredCVName, lineCVName, &params6);
    Algorithms::calculateBestFit(_dummyMeasuredCCName, _dummyNominalCCName, _dummyMeasuredCCName, lineCCName, &params6);

    auto lineCVBF = dynamic_cast<const LineFigure*>(_project->findFigure(lineCVName));
    auto lineCCBF = dynamic_cast<const LineFigure*>(_project->findFigure(lineCCName));

    ARGUMENT_ASSERT(lineCVBF && lineCCBF, "Calculate LSQ best-fit: line`s not found");

    auto offsetXCV = lineCVBF->origin().x;
    auto offsetYCV = lineCVBF->origin().y;
    auto rotationCV = lineCVBF->direction().y / lineCVBF->direction().x;
    auto offsetXCC = lineCCBF->origin().x;
    auto offsetYCC = lineCCBF->origin().y;
    auto rotationCC = lineCCBF->direction().y / lineCCBF->direction().x;
    _reportSettings->setBestFitValues(offsetXCV, offsetYCV, rotationCV, offsetXCC, offsetYCC, rotationCC);

    auto curveCV = CurveFigure(QString(), measuredParts.pointsOfHigh);
    curveCV.alignment(-rotationCV, -offsetXCV, -offsetYCV);
    insertCurveInProject(_dummyMeasuredCVName, curveCV.points(), true);

    auto curveCC = CurveFigure(QString(), measuredParts.pointsOfLow);
    curveCC.alignment(-rotationCC, -offsetXCC, -offsetYCC);
    insertCurveInProject(_dummyMeasuredCCName, curveCC.points(), true);

    auto params4 = _paramsFactory->params4(params18, false);
    Algorithms::calculateDeviations(_dummyNominalCVName, _dummyMeasuredCVName, _dummyMeasuredCVName, &params4);
    Algorithms::calculateDeviations(_dummyNominalCCName, _dummyMeasuredCCName, _dummyMeasuredCCName, &params4);

    auto curveParts = Algorithms::divideCurveIntoParts(_dummyMeasuredName, &params18, _project);
    Algorithms::reassembleWholeCurve(_dummyMeasuredName, curveParts, _reportSettings->leadingEdgeDirection(), _project);

    params4 = _paramsFactory->params4(params18, true);
    Algorithms::calculateDeviations(_nominalName, _dummyMeasuredName, _dummyMeasuredName, &params4);

    auto globalConvexCurve = getCurveFromProject(_dummyMeasuredCVName);
    auto globalConcaveCurve = getCurveFromProject(_dummyMeasuredCCName);

    return GlobalCurveMap {
        { FigureCreator::CurveType::GlobalCV, { _globalCVName, globalConvexCurve->points() } },
        { FigureCreator::CurveType::GlobalCC, { _globalCCName, globalConcaveCurve->points() } },
    };
}

GlobalCurveMap CurveAnalyzer::analyzeProfileWithoutEdgesForm(const CurveParts& nominalParts,
    const CurveParts& measuredParts, const Function18Params& params18)
{
    auto updatedMeasParts = alignCurveParts(measuredParts, params18);
    insertCurveInProject(_dummyNominalCVName, nominalParts.pointsOfHigh, true);
    insertCurveInProject(_dummyMeasuredCVName, updatedMeasParts.pointsOfHigh, true);
    insertCurveInProject(_dummyNominalCCName, nominalParts.pointsOfLow, true);
    insertCurveInProject(_dummyMeasuredCCName, updatedMeasParts.pointsOfLow, true);

    auto lineCVName = QString("%1_CV_Fit").arg(_measuredName);
    auto lineCCName = QString("%1_CC_Fit").arg(_measuredName);
    auto params21 = _paramsFactory->params21ForLocalFit();
    Algorithms::calculateBestFit(_dummyNominalCVName, _dummyMeasuredCVName, _dummyMeasuredCVName, lineCVName, &params21);
    Algorithms::calculateBestFit(_dummyNominalCCName, _dummyMeasuredCCName, _dummyMeasuredCCName, lineCCName, &params21);

    auto lineCVBF = dynamic_cast<const LineFigure*>(_project->findFigure(lineCVName));
    auto lineCCBF = dynamic_cast<const LineFigure*>(_project->findFigure(lineCCName));

    ARGUMENT_ASSERT(lineCVBF && lineCCBF, "Calculate Form best-fit: line`s not found");

    auto offsetXCV = lineCVBF->origin().x;
    auto offsetYCV = lineCVBF->origin().y;
    auto rotationCV = lineCVBF->direction().y / lineCVBF->direction().x;
    auto offsetXCC = lineCCBF->origin().x;
    auto offsetYCC = lineCCBF->origin().y;
    auto rotationCC = lineCCBF->direction().y / lineCCBF->direction().x;
    _reportSettings->setBestFitValues(offsetXCV, offsetYCV, rotationCV, offsetXCC, offsetYCC, rotationCC);

    auto params4 = _paramsFactory->params4(params18, false);
    Algorithms::calculateDeviations(_dummyNominalCVName, _dummyMeasuredCVName, _dummyMeasuredCVName, &params4);
    Algorithms::calculateDeviations(_dummyNominalCCName, _dummyMeasuredCCName, _dummyMeasuredCCName, &params4);

    params4 = _paramsFactory->params4(params18, true);
    Algorithms::calculateDeviations(_nominalName, _dummyMeasuredName, _dummyMeasuredName, &params4);

    auto globalConvexCurve = getCurveFromProject(_dummyMeasuredCVName);
    auto globalConcaveCurve = getCurveFromProject(_dummyMeasuredCCName);

    return GlobalCurveMap {
        { FigureCreator::CurveType::GlobalCV, { _globalCVName, globalConvexCurve->points() } },
        { FigureCreator::CurveType::GlobalCC, { _globalCCName, globalConcaveCurve->points() } },
    };
}

void CurveAnalyzer::calculateEdgeBestFit(
    const QString& nominalName, const QList<CurvePoint>& nominalPoints,
    const QString& measuredName, const QList<CurvePoint>& measuredPoints,
    const QString& lineBestFitName, ReportSettings::EdgeBestFit bestFit)
{
    insertCurveInProject(nominalName, nominalPoints, true);
    insertCurveInProject(measuredName, measuredPoints, true);

    using EdgeBestFit = ReportSettings::EdgeBestFit;

    switch(bestFit) {
        case EdgeBestFit::FreeFitForm: {
            Function6Params params6 = _paramsFactory->params6ForLocalFit();
            Algorithms::calculateBestFit(nominalName, measuredName, measuredName, lineBestFitName, &params6);

            Function21Params params21 = _paramsFactory->params21ForLocalFit();
            Algorithms::calculateBestFit(nominalName, measuredName, measuredName, lineBestFitName, &params21);

            break;
        }
        case EdgeBestFit::GlobalFit:
        case EdgeBestFit::NoFit:
        default: {
            break;
        }
    }
}

const CurveFigure* CurveAnalyzer::calculateEdgeDeviations(const Function18Params& params18,
    const QString& nominalName, const QString& measuredName, const QString& globalName,
    const QList<CurvePoint>& resultGlobalPoints, ReportSettings::EdgeBestFit bestFit)
{
    using EdgeBestFit = ReportSettings::EdgeBestFit;

    switch(bestFit) {
        case EdgeBestFit::FreeFitForm:
        case EdgeBestFit::NoFit: {
            Function4Params params4 = _paramsFactory->params4(params18, false);
            Algorithms::calculateDeviations(nominalName, measuredName, globalName, &params4);
            break;
        }
        case EdgeBestFit::GlobalFit: {
            insertCurveInProject(globalName, resultGlobalPoints);
            break;
        }
    }

    auto globalEdge = dynamic_cast<const CurveFigure*>(_project->findFigure(globalName));

    return globalEdge;
}

void CurveAnalyzer::calculatePreprocessingFunctions(const QString& updatedNomName, const QString& updatedMeasName)
{
    if(_reportSettings->needSortPoints() || _reportSettings->needRemoveEqualPoints()) {
        auto params1 = Function1Params(0, _reportSettings->limitForEqualPoints(), 0,
            true, true, FunctionParams::Direction::Left, _reportSettings->needSortPoints());
        Algorithms::calculateCurve(updatedMeasName, updatedMeasName, &params1, _project);
    }
    if(_reportSettings->needRadiusCompensation() && !_reportSettings->needUse3DVectors()) {
        auto params1 = Function1Params(0, 0.04, 0, true);
        Algorithms::calculateCurve(updatedMeasName, updatedMeasName, &params1, _project);
        auto params3 = Function3Params(_reportSettings->radiusCompensation(), true, false);
        Algorithms::makeRadiusCorrection(updatedMeasName, updatedMeasName, &params3);
        if(_reportSettings->needSortPoints() || _reportSettings->needRemoveEqualPoints()) {
            params1 = Function1Params(0, _reportSettings->limitForEqualPoints(), 0,
                true, true, FunctionParams::Direction::Left, _reportSettings->needSortPoints());
            Algorithms::calculateCurve(updatedMeasName, updatedMeasName, &params1, _project);
        }
    }
    if(_reportSettings->needUse3DVectors()) {
        auto params42 = Function42Params();
        auto radiusCorrection = _reportSettings->radiusCompensation();
        Algorithms::calculateCurveUsing3DVectors(updatedNomName, updatedMeasName, updatedMeasName, &params42, radiusCorrection, _project);
        auto params1 = Function1Params(0, 0.04, 0, true);
        Algorithms::calculateCurve(updatedMeasName, updatedMeasName, &params1, _project);
        auto params3 = Function3Params(radiusCorrection, true, false);
        Algorithms::makeRadiusCorrection(updatedMeasName, updatedMeasName, &params3);
        Algorithms::calculateCurve(updatedMeasName, updatedMeasName, &params1, _project);
    }
}

CurveAnalyzer::CurveParts CurveAnalyzer::getCurvePartsAfterStretching(const QString& nominalName,
    const QString& measuredName, const Function18Params& params18)
{
    auto params31 = Function31Params(_reportSettings->isLEStretch(), _reportSettings->isTEStretch());
    auto params6 = Function6Params();
    Algorithms::calculateStretch(nominalName, measuredName, measuredName, &params31, &params6, _project);

    return Algorithms::divideCurveIntoParts(measuredName, &params18, _project);
}

template<class T>
void CurveAnalyzer::calculateBestFit(const QString& updatedNomName, const QString& updatedMeasName, const T& params)
{
    auto lineName = QString("%1_BF").arg(_measuredName);
    Algorithms::calculateBestFit(updatedNomName, updatedMeasName, updatedMeasName, lineName, &params);

    auto lineBF = dynamic_cast<const LineFigure*>(_project->findFigure(lineName));

    ARGUMENT_ASSERT(lineBF, "Calculate best-fit: figure`s not found");

    auto offsetX = lineBF->origin().x;
    auto offsetY = lineBF->origin().y;
    auto rotation = lineBF->direction().y / lineBF->direction().x;
    _reportSettings->setBestFitValues(offsetX, offsetY, rotation);

    Algorithms::calculateMeasuredTransformParams(_reportSettings, lineName);
}

void CurveAnalyzer::calculateBestFitWithAlignment(const QString& updatedNomName, const QString& updatedMeasName,
    const Function6Params& params6, const CurveParts& nominalParts, CurveParts* measuredParts)
{
    auto direction = _reportSettings->leadingEdgeDirection();
    Algorithms::reassembleCurveWithoutEdges(updatedNomName, nominalParts, direction, _project);
    Algorithms::reassembleCurveWithoutEdges(updatedMeasName, *measuredParts, direction, _project);

    auto lineName = QString("%1_BF").arg(_measuredName);
    Algorithms::calculateBestFit(updatedNomName, updatedMeasName, updatedMeasName, lineName, &params6);

    auto curveBF = dynamic_cast<const CurveFigure*>(_project->findFigure(updatedMeasName));
    auto lineBF = dynamic_cast<const LineFigure*>(_project->findFigure(lineName));

    ARGUMENT_ASSERT(curveBF && lineBF, "Calculate best-fit with alignment: figure`s not found");

    auto pointsBF = curveBF->points();
    auto offsetX = lineBF->origin().x;
    auto offsetY = lineBF->origin().y;
    auto rotation = lineBF->direction().y / lineBF->direction().x;
    _reportSettings->setBestFitValues(offsetX, offsetY, rotation);

    Algorithms::calculateMeasuredTransformParams(_reportSettings, lineName);

    auto offsetLE = CurveFigure(QString(), measuredParts->pointsOfLE);
    auto offsetTE = CurveFigure(QString(), measuredParts->pointsOfTE);
    offsetLE.alignment(rotation, offsetX, offsetY);
    offsetTE.alignment(rotation, offsetX, offsetY);

    auto highPoints = pointsBF.mid(0, measuredParts->pointsOfHigh.size());
    auto lowPoints = pointsBF.mid(measuredParts->pointsOfHigh.size(), measuredParts->pointsOfLow.size());

    measuredParts->pointsOfLE = offsetLE.points();
    measuredParts->pointsOfTE = offsetTE.points();
    measuredParts->pointsOfHigh = highPoints;
    measuredParts->pointsOfLow = lowPoints;

    Algorithms::reassembleCurveWithoutEdges(updatedMeasName, *measuredParts, direction, _project);
    Algorithms::reassembleCurveWithoutEdges(updatedNomName, nominalParts, direction, _project);
}

CurveAnalyzer::CurveParts CurveAnalyzer::alignCurveParts(const CurveParts& curveParts, const Function18Params& params18)
{
    auto curveName = "dummy";
    Algorithms::reassembleWholeCurve(curveName, curveParts, _reportSettings->leadingEdgeDirection(), _project);
    auto points = getCurveFromProject(curveName)->points();

    auto curve = CurveFigure(curveName, points);
    curve.alignment(_reportSettings->rotation(), _reportSettings->xShift(), _reportSettings->yShift());
    insertCurveInProject(curveName, curve.points(), true);

    return Algorithms::divideCurveIntoParts(curveName, &params18, _project);
}

void CurveAnalyzer::insertCurveInProject(const QString& curveName, const QVector<CurvePoint>& points, bool needToDelete)
{
    auto curve = new CurveFigure(curveName, points);
    _project->safeInsert(curveName, curve);

    if(needToDelete) {
        _curvesToDelete.insert(curveName);
    }
}

const CurveFigure* CurveAnalyzer::getCurveFromProject(const QString& curveName)
{
    auto figure = _project->findFigure(curveName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Get curve from project: curve's not found");

    return curve;
}

void CurveAnalyzer::deleteDummyCurves()
{
    for(auto nameCurve : _curvesToDelete) {
        _project->removeFigure(nameCurve);
    }
    _curvesToDelete.clear();
}
