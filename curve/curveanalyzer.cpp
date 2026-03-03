#include "curve/pch.h"
#include "curveanalyzer.h"

CurveAnalyzer::CurveAnalyzer(Project *project, std::shared_ptr<ReportSettings> reportSettings) : _project(project), _reportSettings(reportSettings) {
    initialization();
}

void CurveAnalyzer::initialization() {
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
    _dummyNomName = dummyNames[InterimName::NominalCurve];
    _dummyMeasName = dummyNames[InterimName::MeasuredCurve];
    _dummyNomCVName = dummyNames[InterimName::NominalCV];
    _dummyMeasCVName = dummyNames[InterimName::MeasuredCV];
    _dummyNomCCName = dummyNames[InterimName::NominalCC];
    _dummyMeasCCName = dummyNames[InterimName::MeasuredCC];
}

GlobalCurveMap CurveAnalyzer::run() {
    auto params18 = FunctionParamsGenerator::params18(_project, _reportSettings);
    auto [nominalParts, measuredParts] = analyzeProfile(&params18);

    using Profile = ReportSettings::Profile;
    auto profileType = _reportSettings->profileType();
    GlobalCurveMap result;

    if(profileType == Profile::Whole) {
        result = analyzeWholeProfile(&params18);
    } else if(profileType == Profile::WithoutTE) {
        result = analyzeProfileWithoutTE(&params18);
    } else if(profileType == Profile::WithoutEdges) {
        result = analyzeProfileWithoutEdges(&params18);
    } else if(profileType == Profile::WithoutEdgesLSQ) {
        result = analyzeProfileWithoutEdgesLSQ(nominalParts, measuredParts, &params18);
    } else if(profileType == Profile::WithoutEdgesForm) {
        result = analyzeProfileWithoutEdgesForm(nominalParts, measuredParts, &params18);
    } else {
        return result;
    }
    deleteDummyCurves();

    return result;
}

QPair<CurveParts, CurveParts> CurveAnalyzer::analyzeProfile(const Function18Params *params18) {
    auto nominalCurve = dynamic_cast<const CurveFigure*>(_project->findFigure(_nominalName));
    auto measuredCurve = dynamic_cast<const CurveFigure*>(_project->findFigure(_measuredName));

    insertCurveInProject(_dummyNomName, nominalCurve->points(), true);
    insertCurveInProject(_dummyMeasName, measuredCurve->points(), true);

    auto nominalParts = Algorithms::divideCurveIntoParts(_dummyNomName, params18, _project);
    auto direction = _reportSettings->directionOfLE();
    Algorithms::reassembleWholeCurve(_dummyNomName, nominalParts, direction, _project);

    calculatePreprocessingFunctions(_nominalName, _dummyMeasName);

    Algorithms::calculateMeasuredParams(_project, _reportSettings, _dummyMeasName);
    FigureCreator::createAdditionalFigures(_project, _reportSettings);

    auto measuredParts = Algorithms::divideCurveIntoParts(_dummyMeasName, params18, _project);
    Algorithms::reassembleWholeCurve(_dummyMeasName, measuredParts, direction, _project);

    //Need to check later
    /*if(_reportSettings->isLEStretch() || _reportSettings->isTEStretch()) {
        measuredParts = getCurvePartsAfterStretching(_dummyNomName, _dummyMeasName, params18);
        Algorithms::reassembleWholeCurve(_dummyMeasName, measuredParts, direction, _project);
    }*/

    auto globalBestFit = _reportSettings->globalBestFit();
    using GlobalBestFit = ReportSettings::GlobalBestFit;
    if(globalBestFit == GlobalBestFit::Whole) {
        auto params19 = Function19Params();
        Algorithms::regenerateCurve(_dummyMeasName, _dummyMeasName, &params19, _project);
        auto params6 = FunctionParamsGenerator::params6(_reportSettings, true);
        calculateBestFit(_dummyNomName, _dummyMeasName, params6);
    } else if(globalBestFit == GlobalBestFit::WithoutEdges) {
        auto params6 = FunctionParamsGenerator::params6(_reportSettings, true);
        calculateBestFitWithAlignment(_dummyNomName, _dummyMeasName, params6, nominalParts, &measuredParts);
    } else if(globalBestFit == GlobalBestFit::MinForm) {
        auto params21 = FunctionParamsGenerator::params21(_reportSettings, true);
        calculateBestFit(_nominalName, _dummyMeasName, params21);
    }
    FigureCreator::alignAdditionalFigures(_project, _reportSettings);

    return QPair<CurveParts, CurveParts>(nominalParts, measuredParts);
}

GlobalCurveMap CurveAnalyzer::analyzeWholeProfile(const Function18Params *params18) {
    auto params4 = FunctionParamsGenerator::params4(_project, _reportSettings, params18, true);
    Algorithms::calculateDeviations(_dummyNomName, _dummyMeasName, _dummyMeasName, &params4, _project);

    auto globalPoints = getCurveFromProject(_dummyMeasName)->points();
    auto globalParts = Algorithms::divideCurveIntoParts(_dummyMeasName, params18, _project);

    return {
        { CurveType::WholeGlobal, { _globalName, globalPoints } },
        { CurveType::GlobalCC, { _globalCCName, globalParts.pointsOfLow } },
        { CurveType::GlobalCV, { _globalCVName, globalParts.pointsOfHigh } },
        { CurveType::GlobalLE, { _globalLEName, globalParts.pointsOfLE } },
        { CurveType::GlobalTE, { _globalTEName, globalParts.pointsOfTE } }
    };
}

GlobalCurveMap CurveAnalyzer::analyzeProfileWithoutTE(const Function18Params *params18) {
    auto params4 = FunctionParamsGenerator::params4(_project, _reportSettings, params18, true);
    Algorithms::calculateDeviations(_dummyNomName, _dummyMeasName, _dummyMeasName, &params4, _project);

    auto globalParts = Algorithms::divideCurveIntoParts(_dummyMeasName, params18, _project);
    Algorithms::reassembleCurveWithoutTE(_dummyMeasName, globalParts, _reportSettings->directionOfLE(), _project);
    auto globalPoints = getCurveFromProject(_dummyMeasName)->points();

    return {
        { CurveType::GlobalWithoutTE, { _globalName, globalPoints } },
        { CurveType::GlobalCC, { _globalCCName, globalParts.pointsOfLow } },
        { CurveType::GlobalCV, { _globalCVName, globalParts.pointsOfHigh } },
        { CurveType::GlobalLE, { _globalLEName, globalParts.pointsOfLE } },
        { CurveType::GlobalTE, { _globalTEName, globalParts.pointsOfTE } }
    };
}

GlobalCurveMap CurveAnalyzer::analyzeProfileWithoutEdges(const Function18Params *params18) {
    auto params4 = FunctionParamsGenerator::params4(_project, _reportSettings, params18, true);
    Algorithms::calculateDeviations(_dummyNomName, _dummyMeasName, _dummyMeasName, &params4, _project);

    auto globalParts = Algorithms::divideCurveIntoParts(_dummyMeasName, params18, _project);

    return {
        { CurveType::GlobalCC, { _globalCCName, globalParts.pointsOfLow } },
        { CurveType::GlobalCV, { _globalCVName, globalParts.pointsOfHigh } },
        { CurveType::GlobalLE, { _globalLEName, globalParts.pointsOfLE } },
        { CurveType::GlobalTE, { _globalTEName, globalParts.pointsOfTE } }
    };
}

GlobalCurveMap CurveAnalyzer::analyzeProfileWithoutEdgesLSQ(const CurveParts &nominalParts, const CurveParts &measuredParts, const Function18Params *params18) {
    insertCurveInProject(_dummyNomCVName, nominalParts.pointsOfHigh, true);
    insertCurveInProject(_dummyMeasCVName, measuredParts.pointsOfHigh, true);
    insertCurveInProject(_dummyNomCCName, nominalParts.pointsOfLow, true);
    insertCurveInProject(_dummyMeasCCName, measuredParts.pointsOfLow, true);

    auto lineCVName = QString("%1_CV_Fit").arg(_measuredName);
    auto lineCCName = QString("%1_CC_Fit").arg(_measuredName);
    auto params6 = FunctionParamsGenerator::params6(_reportSettings);
    Algorithms::calculateBestFit(_dummyMeasCVName, _dummyNomCVName, _dummyMeasCVName, lineCVName, &params6, _project);
    Algorithms::calculateBestFit(_dummyMeasCCName, _dummyNomCCName, _dummyMeasCCName, lineCCName, &params6, _project);

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
    insertCurveInProject(_dummyMeasCVName, curveCV.points(), true);

    auto curveCC = CurveFigure(QString(), measuredParts.pointsOfLow);
    curveCC.alignment(-rotationCC, -offsetXCC, -offsetYCC);
    insertCurveInProject(_dummyMeasCCName, curveCC.points(), true);

    auto params4 = FunctionParamsGenerator::params4(_project, _reportSettings, params18);
    Algorithms::calculateDeviations(_dummyNomCVName, _dummyMeasCVName, _dummyMeasCVName, &params4, _project);
    Algorithms::calculateDeviations(_dummyNomCCName, _dummyMeasCCName, _dummyMeasCCName, &params4, _project);
    auto globalCVPoints = getCurveFromProject(_dummyMeasCVName)->points();
    auto globalCCPoints = getCurveFromProject(_dummyMeasCCName)->points();

    auto curveParts = Algorithms::divideCurveIntoParts(_dummyMeasName, params18, _project);
    Algorithms::reassembleWholeCurve(_dummyMeasName, curveParts, _reportSettings->directionOfLE(), _project);
    params4 = FunctionParamsGenerator::params4(_project, _reportSettings, params18, true);
    Algorithms::calculateDeviations(_nominalName, _dummyMeasName, _dummyMeasName, &params4, _project);

    auto globalParts = Algorithms::divideCurveIntoParts(_dummyMeasName, params18, _project);

    return {
        { CurveType::GlobalCC, { _globalCCName, globalCCPoints } },
        { CurveType::GlobalCV, { _globalCVName, globalCVPoints } },
        { CurveType::GlobalLE, { _globalLEName, globalParts.pointsOfLE } },
        { CurveType::GlobalTE, { _globalTEName, globalParts.pointsOfTE } }
    };
}

GlobalCurveMap CurveAnalyzer::analyzeProfileWithoutEdgesForm(const CurveParts &nominalParts, const CurveParts &measuredParts, const Function18Params *params18) {
    auto updatedMeasParts = alignCurveParts(measuredParts, *params18);
    insertCurveInProject(_dummyNomCVName, nominalParts.pointsOfHigh, true);
    insertCurveInProject(_dummyMeasCVName, updatedMeasParts.pointsOfHigh, true);
    insertCurveInProject(_dummyNomCCName, nominalParts.pointsOfLow, true);
    insertCurveInProject(_dummyMeasCCName, updatedMeasParts.pointsOfLow, true);
    
    auto lineCVName = QString("%1_CV_Fit").arg(_measuredName);
    auto lineCCName = QString("%1_CC_Fit").arg(_measuredName);
    auto params21 = FunctionParamsGenerator::params21(_reportSettings);
    Algorithms::calculateBestFit(_dummyNomCVName, _dummyMeasCVName, _dummyMeasCVName, lineCVName, &params21, _project);
    Algorithms::calculateBestFit(_dummyNomCCName, _dummyMeasCCName, _dummyMeasCCName, lineCCName, &params21, _project);

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

    auto params4 = FunctionParamsGenerator::params4(_project, _reportSettings, params18);
    Algorithms::calculateDeviations(_dummyNomCVName, _dummyMeasCVName, _dummyMeasCVName, &params4, _project);
    Algorithms::calculateDeviations(_dummyNomCCName, _dummyMeasCCName, _dummyMeasCCName, &params4, _project);
    auto globalCVPoints = getCurveFromProject(_dummyMeasCVName)->points();
    auto globalCCPoints = getCurveFromProject(_dummyMeasCCName)->points();

    params4 = FunctionParamsGenerator::params4(_project, _reportSettings, params18, true);
    Algorithms::calculateDeviations(_nominalName, _dummyMeasName, _dummyMeasName, &params4, _project);
    auto globalParts = Algorithms::divideCurveIntoParts(_dummyMeasName, params18, _project);

    return {
        { CurveType::GlobalCC, { _globalCCName, globalCCPoints } },
        { CurveType::GlobalCV, { _globalCVName, globalCVPoints } },
        { CurveType::GlobalLE, { _globalLEName, globalParts.pointsOfLE } },
        { CurveType::GlobalTE, { _globalTEName, globalParts.pointsOfTE } }
    };
}

void CurveAnalyzer::calculatePreprocessingFunctions(const QString &updatedNomName, const QString &updatedMeasName) {
    if(_reportSettings->needSortPoints() || _reportSettings->needRemoveEqualPoints()) {
        auto params1 = Function1Params(0, _reportSettings->limitForEqualPoints(), 0, 
            true, true, FunctionParams::Direction::Left, _reportSettings->needSortPoints());
        Algorithms::calculateCurve(updatedMeasName, updatedMeasName, &params1, _project);
    }
    if(_reportSettings->needRadiusCompensation() && !_reportSettings->needUse3DVectors()) {
        auto params1 = Function1Params(0, 0.04, 0, true);
        Algorithms::calculateCurve(updatedMeasName, updatedMeasName, &params1, _project);
        auto params3 = Function3Params(_reportSettings->radiusCompensation(), true, false);
        Algorithms::makeRadiusCorrection(updatedMeasName, updatedMeasName, &params3, _project);
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
        Algorithms::makeRadiusCorrection(updatedMeasName, updatedMeasName, &params3, _project);
        Algorithms::calculateCurve(updatedMeasName, updatedMeasName, &params1, _project);
    }
}

CurveAnalyzer::CurveParts CurveAnalyzer::getCurvePartsAfterStretching(const QString &nominalName, const QString &measuredName, const Function18Params &params18) {
    auto params31 = Function31Params(_reportSettings->isLEStretch(), _reportSettings->isTEStretch());
    auto params6 = Function6Params();
    Algorithms::calculateStretch(nominalName, measuredName, measuredName, &params31, &params6, _project);

    return Algorithms::divideCurveIntoParts(measuredName, &params18, _project);
}

template<class T>
void CurveAnalyzer::calculateBestFit(const QString &updatedNomName, const QString &updatedMeasName, const T &params) {
    auto lineName = QString("%1_BF").arg(_measuredName);
    Algorithms::calculateBestFit(updatedNomName, updatedMeasName, updatedMeasName, lineName, &params, _project);

    auto lineBF = dynamic_cast<const LineFigure*>(_project->findFigure(lineName));
    ARGUMENT_ASSERT(lineBF, "Calculate best-fit: figure`s not found");
    auto offsetX = lineBF->origin().x;
    auto offsetY = lineBF->origin().y;
    auto rotation = lineBF->direction().y / lineBF->direction().x;
    _reportSettings->setBestFitValues(offsetX, offsetY, rotation);
}

void CurveAnalyzer::calculateBestFitWithAlignment(const QString &updatedNomName, const QString &updatedMeasName, const Function6Params &params6, const CurveParts &nominalParts, CurveParts *measuredParts) {
    auto direction = _reportSettings->directionOfLE();
    Algorithms::reassembleCurveWithoutEdges(updatedNomName, nominalParts, direction, _project);
    Algorithms::reassembleCurveWithoutEdges(updatedMeasName, *measuredParts, direction, _project);

    auto lineName = QString("%1_BF").arg(_measuredName);
    Algorithms::calculateBestFit(updatedNomName, updatedMeasName, updatedMeasName, lineName, &params6, _project);

    auto curveBF = dynamic_cast<const CurveFigure*>(_project->findFigure(updatedMeasName));
    auto lineBF = dynamic_cast<const LineFigure*>(_project->findFigure(lineName));
    ARGUMENT_ASSERT(curveBF && lineBF, "Calculate best-fit with alignment: figure`s not found");
    auto pointsBF = curveBF->points();
    auto offsetX = lineBF->origin().x;
    auto offsetY = lineBF->origin().y;
    auto rotation = lineBF->direction().y / lineBF->direction().x;
    _reportSettings->setBestFitValues(offsetX, offsetY, rotation);

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

CurveAnalyzer::CurveParts CurveAnalyzer::alignCurveParts(const CurveParts &curveParts, const Function18Params &params18) {
    auto curveName = "dummy";
    Algorithms::reassembleWholeCurve(curveName, curveParts, _reportSettings->directionOfLE(), _project);
    auto points = getCurveFromProject(curveName)->points();

    auto curve = CurveFigure(curveName, points);
    curve.alignment(_reportSettings->rotation(), _reportSettings->xShift(), _reportSettings->yShift());
    insertCurveInProject(curveName, curve.points(), true);

    return Algorithms::divideCurveIntoParts(curveName, &params18, _project);
}

void CurveAnalyzer::insertCurveInProject(const QString &curveName, const QVector<CurvePoint> &points, bool needToDelete) {
    auto curve = new CurveFigure(curveName, points);
    _project->safeInsert(curveName, curve);

    if(needToDelete) {
        _curvesToDelete.insert(curveName);
    }
}

const CurveFigure* CurveAnalyzer::getCurveFromProject(const QString &curveName) {
    auto figure = _project->findFigure(curveName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Get curve from project: curve's not found");

    return curve;
}

void CurveAnalyzer::deleteDummyCurves() {
    for(auto nameCurve : _curvesToDelete) {
        _project->removeFigure(nameCurve);
    }
    _curvesToDelete.clear();
}
