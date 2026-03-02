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
    auto globalNames = ReportGenerator::getTemplateGlobalNames(_nominalName);
    using GlobalName = ReportGenerator::GlobalName;
    _globalName = globalNames[GlobalName::GlobalCurve];
    _globalCVName = globalNames[GlobalName::GlobalCV];
    _globalCCName = globalNames[GlobalName::GlobalCC];
    _globalLEName = globalNames[GlobalName::GlobalLE];
    _globalTEName = globalNames[GlobalName::GlobalTE];

    _dummyNomName = QString("dummy_%1").arg(_nominalName);
    _dummyMeasName = QString("dummy_%1").arg(_measuredName);
    _dummyNomCVName = QString("dummy_CV_%1").arg(_nominalName);
    _dummyMeasCVName = QString("dummy_CV_%1").arg(_measuredName);
    _dummyNomCCName = QString("dummy_CC_%1").arg(_nominalName);
    _dummyMeasCCName = QString("dummy_CC_%1").arg(_measuredName);
}

GlobalCurveMap CurveAnalyzer::run() {
    auto [nominalParts, measuredParts] = analyzeProfile();

    using Profile = ReportSettings::Profile;
    auto profileType = _reportSettings->profileType();
    GlobalCurveMap result;

    if(profileType == Profile::Whole) {
        result = analyzeWholeProfile();
    } else if(profileType == Profile::WithoutTE) {
        result = analyzeProfileWithoutTE();
    } else if(profileType == Profile::WithoutEdges) {
        result = analyzeProfileWithoutEdges();
    } else if(profileType == Profile::WithoutEdgesLSQ) {
        result = analyzeProfileWithoutEdgesLSQ(nominalParts, measuredParts);
    } else if(profileType == Profile::WithoutEdgesForm) {
        result = analyzeProfileWithoutEdgesForm(nominalParts, measuredParts);
    } else {
        return result;
    }
    deleteDummyCurves();

    return result;
}

QPair<CurveAnalyzer::CurveParts, CurveAnalyzer::CurveParts> CurveAnalyzer::analyzeProfile() {
    auto nominalCurve = dynamic_cast<const CurveFigure*>(_project->findFigure(_nominalName));
    auto measuredCurve = dynamic_cast<const CurveFigure*>(_project->findFigure(_measuredName));

    insertCurveInProject(_dummyNomName, nominalCurve->points(), true);
    insertCurveInProject(_dummyMeasName, measuredCurve->points(), true);

    auto params18 = FunctionParamsGenerator::params18(_project, _reportSettings);
    auto nominalParts = Algorithms::divideCurveIntoParts(_dummyNomName, &params18, _project);
    reassemblePointsIntoWholeCurve(_dummyNomName, nominalParts);

    calculatePreprocessingFunctions(_nominalName, _dummyMeasName);
    auto measuredParts = Algorithms::divideCurveIntoParts(_dummyMeasName, &params18, _project);
    reassemblePointsIntoWholeCurve(_dummyMeasName, measuredParts);

    if(_reportSettings->isLEStretch() || _reportSettings->isTEStretch()) {
        measuredParts = getCurvePartsAfterStretching(_dummyNomName, _dummyMeasName, params18);
        reassemblePointsIntoWholeCurve(_dummyMeasName, measuredParts);
    }
    Algorithms::calculateMeasuredParams(_project, _reportSettings, _dummyMeasName);
    auto figureCreator = FigureCreator(_project, _reportSettings);
    figureCreator.createAdditionalFigures(_dummyMeasName);

    auto globalBestFit = _reportSettings->globalBestFit();
    using GlobalBestFit = ReportSettings::GlobalBestFit;
    if(globalBestFit == GlobalBestFit::Whole) {
        auto params19 = Function19Params();
        Algorithms::regenerateCurve(_dummyMeasName, _dummyMeasName, &params19, _project);
        auto params6 = FunctionParamsGenerator::params6(_reportSettings);
        calculateBestFit(_dummyNomName, _dummyMeasName, params6);
    } else if(globalBestFit == GlobalBestFit::WithoutEdges) {
        calculateBestFitWithAlignment(_dummyNomName, _dummyMeasName, nominalParts, measuredParts);
    } else if(globalBestFit == GlobalBestFit::MinForm) {
        auto params21 = FunctionParamsGenerator::params21(_reportSettings);
        calculateBestFit(_nominalName, _dummyMeasName, params21);
    }
    figureCreator.alignAdditionalFigures();

    return QPair<CurveParts, CurveParts>(nominalParts, measuredParts);
}

GlobalCurveMap CurveAnalyzer::analyzeWholeProfile() {
    auto params4 = FunctionParamsGenerator::params4(_reportSettings, true);
    Algorithms::calculateDeviations(_dummyNomName, _dummyMeasName, _dummyMeasName, &params4, _project);

    auto globalPoints = getCurveFromProject(_dummyMeasName)->points();
    auto params18 = FunctionParamsGenerator::params18(_project, _reportSettings);
    auto globalParts = Algorithms::divideCurveIntoParts(_dummyMeasName, &params18, _project);

    return {
        { CurveType::WholeGlobal, { _globalName, globalPoints } },
        { CurveType::GlobalCV, { _globalCVName, globalParts.pointsOfHigh } },
        { CurveType::GlobalCC, { _globalCCName, globalParts.pointsOfLow } },
        { CurveType::GlobalLE, { _globalLEName, globalParts.pointsOfLE } },
        { CurveType::GlobalTE, { _globalTEName, globalParts.pointsOfTE } }
    };
}

GlobalCurveMap CurveAnalyzer::analyzeProfileWithoutTE() {
    auto params4 = FunctionParamsGenerator::params4(_reportSettings, true);
    Algorithms::calculateDeviations(_dummyNomName, _dummyMeasName, _dummyMeasName, &params4, _project);

    auto params18 = FunctionParamsGenerator::params18(_project, _reportSettings);
    auto globalParts = Algorithms::divideCurveIntoParts(_dummyMeasName, &params18, _project);

    reassemblePointsIntoCurveWithoutTE(_dummyMeasName, globalParts);
    auto globalPoints = getCurveFromProject(_dummyMeasName)->points();

    return {
        { CurveType::GlobalWithoutTE, { _globalName, globalPoints } },
        { CurveType::GlobalCV, { _globalCVName, globalParts.pointsOfHigh } },
        { CurveType::GlobalCC, { _globalCCName, globalParts.pointsOfLow } },
        { CurveType::GlobalLE, { _globalLEName, globalParts.pointsOfLE } },
        { CurveType::GlobalTE, { _globalTEName, globalParts.pointsOfTE } }
    };
}

GlobalCurveMap CurveAnalyzer::analyzeProfileWithoutEdges() {
    auto params4 = FunctionParamsGenerator::params4(_reportSettings, true);
    Algorithms::calculateDeviations(_dummyNomName, _dummyMeasName, _dummyMeasName, &params4, _project);

    auto params18 = FunctionParamsGenerator::params18(_project, _reportSettings);
    auto globalParts = Algorithms::divideCurveIntoParts(_dummyMeasName, &params18, _project);

    return {
        { CurveType::GlobalCV, { _globalCVName, globalParts.pointsOfHigh } },
        { CurveType::GlobalCC, { _globalCCName, globalParts.pointsOfLow } },
        { CurveType::GlobalLE, { _globalLEName, globalParts.pointsOfLE } },
        { CurveType::GlobalTE, { _globalTEName, globalParts.pointsOfTE } }
    };
}

GlobalCurveMap CurveAnalyzer::analyzeProfileWithoutEdgesLSQ(const CurveParts &nominalParts, const CurveParts &measuredParts) {
    auto params18 = FunctionParamsGenerator::params18(_project, _reportSettings);
    auto updatedMeasParts = alignCurveParts(measuredParts, params18);

    insertCurveInProject(_dummyNomCVName, nominalParts.pointsOfHigh, true);
    insertCurveInProject(_dummyMeasCVName, updatedMeasParts.pointsOfHigh, true);
    insertCurveInProject(_dummyNomCCName, nominalParts.pointsOfLow, true);
    insertCurveInProject(_dummyMeasCCName, updatedMeasParts.pointsOfLow, true);

    auto lineCVName = QString("%1_CV_Fit").arg(_measuredName);
    auto lineCCName = QString("%1_CC_Fit").arg(_measuredName);
    auto params6 = Function6Params(true, Function6Params::Algorithm::Curve, false, true, true, true);
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

    auto curveCV = CurveFigure(QString(), updatedMeasParts.pointsOfHigh);
    curveCV.alignment(-rotationCV, -offsetXCV, -offsetYCV);
    insertCurveInProject(_dummyMeasCVName, curveCV.points(), true);

    auto curveCC = CurveFigure(QString(), updatedMeasParts.pointsOfLow);
    curveCC.alignment(-rotationCC, -offsetXCC, -offsetYCC);
    insertCurveInProject(_dummyMeasCCName, curveCC.points(), true);

    auto params4 = FunctionParamsGenerator::params4(_reportSettings);
    Algorithms::calculateDeviations(_dummyNomCVName, _dummyMeasCVName, _dummyMeasCVName, &params4, _project);
    Algorithms::calculateDeviations(_dummyNomCCName, _dummyMeasCCName, _dummyMeasCCName, &params4, _project);
    auto globalCVPoints = getCurveFromProject(_dummyMeasCVName)->points();
    auto globalCCPoints = getCurveFromProject(_dummyMeasCCName)->points();

    params4 = FunctionParamsGenerator::params4(_reportSettings, true);
    Algorithms::calculateDeviations(_dummyNomName, _dummyMeasName, _dummyMeasName, &params4, _project);
    auto globalParts = Algorithms::divideCurveIntoParts(_dummyMeasName, &params18, _project);

    return {
        { CurveType::GlobalCV, { _globalCVName, globalCVPoints } },
        { CurveType::GlobalCC, { _globalCCName, globalCCPoints } },
        { CurveType::GlobalLE, { _globalLEName, globalParts.pointsOfLE } },
        { CurveType::GlobalTE, { _globalTEName, globalParts.pointsOfTE } }
    };
}

GlobalCurveMap CurveAnalyzer::analyzeProfileWithoutEdgesForm(const CurveParts &nominalParts, const CurveParts &measuredParts) {
    auto params18 = FunctionParamsGenerator::params18(_project, _reportSettings);
    auto updatedMeasParts = alignCurveParts(measuredParts, params18);

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

    auto params4 = FunctionParamsGenerator::params4(_reportSettings);
    Algorithms::calculateDeviations(_dummyNomCVName, _dummyMeasCVName, _dummyMeasCVName, &params4, _project);
    Algorithms::calculateDeviations(_dummyNomCCName, _dummyMeasCCName, _dummyMeasCCName, &params4, _project);
    auto globalCVPoints = getCurveFromProject(_dummyMeasCVName)->points();
    auto globalCCPoints = getCurveFromProject(_dummyMeasCCName)->points();

    params4 = FunctionParamsGenerator::params4(_reportSettings, true);
    Algorithms::calculateDeviations(_dummyNomName, _dummyMeasName, _dummyMeasName, &params4, _project);
    auto globalParts = Algorithms::divideCurveIntoParts(_dummyMeasName, &params18, _project);

    return {
        { CurveType::GlobalCV, { _globalCVName, globalCVPoints } },
        { CurveType::GlobalCC, { _globalCCName, globalCCPoints } },
        { CurveType::GlobalLE, { _globalLEName, globalParts.pointsOfLE } },
        { CurveType::GlobalTE, { _globalTEName, globalParts.pointsOfTE } }
    };
}

void CurveAnalyzer::reassemblePointsIntoWholeCurve(const QString &curveName, const CurveParts &curveParts) {
    auto curvePointsOfLE = curveParts.pointsOfLE;
    auto curvePointsOfTE = curveParts.pointsOfTE;
    auto lowCurvePoints = curveParts.pointsOfLow;
    auto highCurvePoints = curveParts.pointsOfHigh;
    auto direction = _reportSettings->directionOfLE();

    QVector<CurvePoint> resultPoints;
    if(direction == ReportSettings::LEDirection::PlusX || direction == ReportSettings::LEDirection::PlusY) {
        curvePointsOfLE.removeFirst();
        highCurvePoints.removeFirst();
        curvePointsOfTE.removeFirst();
        curvePointsOfTE.removeLast();
        resultPoints = lowCurvePoints + curvePointsOfLE + highCurvePoints + curvePointsOfTE;
    } else if(direction == ReportSettings::LEDirection::MinusX) {
        curvePointsOfLE.removeFirst();
        highCurvePoints.removeFirst();
        curvePointsOfTE.removeFirst();
        curvePointsOfTE.removeLast();
        resultPoints = lowCurvePoints + curvePointsOfLE + highCurvePoints + curvePointsOfTE;
    } else {
        curvePointsOfLE.removeFirst();
        lowCurvePoints.removeFirst();
        curvePointsOfTE.removeFirst();
        curvePointsOfTE.removeLast();
        resultPoints = highCurvePoints + curvePointsOfLE + lowCurvePoints + curvePointsOfTE;
    }
    insertCurveInProject(curveName, resultPoints, true);
}

void CurveAnalyzer::reassemblePointsIntoCurveWithoutTE(const QString &curveName, const CurveParts &curveParts) {
    auto curvePointsOfLE = curveParts.pointsOfLE;
    auto lowCurvePoints = curveParts.pointsOfLow;
    auto highCurvePoints = curveParts.pointsOfHigh;
    auto directionOfLE = _reportSettings->directionOfLE();

    QVector<CurvePoint> resultPoints;
    if(directionOfLE == ReportSettings::LEDirection::PlusX || directionOfLE == ReportSettings::LEDirection::PlusY) {
        curvePointsOfLE.removeFirst();
        highCurvePoints.removeFirst();
        resultPoints = lowCurvePoints + curvePointsOfLE + highCurvePoints;
    } else {
        curvePointsOfLE.removeFirst();
        lowCurvePoints.removeFirst();
        resultPoints = highCurvePoints + curvePointsOfLE + lowCurvePoints;
    }
    insertCurveInProject(curveName, resultPoints, true);
}

void CurveAnalyzer::reassemblePointsIntoCurveWithoutEdges(const QString &curveName, const CurveParts &curveParts) {
    auto highCurvePoints = curveParts.pointsOfHigh;
    auto lowCurvePoints = curveParts.pointsOfLow;
    auto resultPoints =  highCurvePoints + lowCurvePoints;
    insertCurveInProject(curveName, resultPoints, true);
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
        auto ballCenter = QString("%1_Ball_Center").arg(updatedNomName);
        auto params3 = Function3Params(_reportSettings->radiusCompensation(), true, true, FunctionParams::Direction::Right);
        Algorithms::makeRadiusCorrection(updatedNomName, ballCenter, &params3, _project);
        auto params42 = Function42Params();
        Algorithms::calculateCurveUsing3DVectors(ballCenter, updatedMeasName, updatedMeasName, &params42, _project);
        auto params1 = Function1Params(0, 0.04, 0, true);
        Algorithms::calculateCurve(updatedMeasName, updatedMeasName, &params1, _project);
        params3 = Function3Params(_reportSettings->radiusCompensation(), true, false);
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

void CurveAnalyzer::calculateBestFitWithAlignment(const QString &updatedNomName, const QString &updatedMeasName, const CurveParts &nominalParts, const CurveParts &measuredParts) {
    reassemblePointsIntoCurveWithoutEdges(_dummyNomName, nominalParts);
    reassemblePointsIntoCurveWithoutEdges(_dummyMeasName, measuredParts);

    auto lineName = QString("%1_BF").arg(_measuredName);
    auto params6 = FunctionParamsGenerator::params6(_reportSettings);
    Algorithms::calculateBestFit(updatedNomName, updatedMeasName, updatedMeasName, lineName, &params6, _project);

    auto curveBF = dynamic_cast<const CurveFigure*>(_project->findFigure(updatedMeasName));
    auto lineBF = dynamic_cast<const LineFigure*>(_project->findFigure(lineName));
    ARGUMENT_ASSERT(curveBF && lineBF, "Calculate best-fit with alignment: figure`s not found");
    auto pointsBF = curveBF->points();
    auto offsetX = lineBF->origin().x;
    auto offsetY = lineBF->origin().y;
    auto rotation = lineBF->direction().y / lineBF->direction().x;
    _reportSettings->setBestFitValues(offsetX, offsetY, rotation);

    auto offsetLE = CurveFigure(QString(), measuredParts.pointsOfLE);
    auto offsetTE = CurveFigure(QString(), measuredParts.pointsOfTE);
    offsetLE.alignment(rotation, offsetX, offsetY);
    offsetTE.alignment(rotation, offsetX, offsetY);
    auto highPoints = pointsBF.mid(0, measuredParts.pointsOfHigh.size());
    auto lowPoints = pointsBF.mid(measuredParts.pointsOfHigh.size(), measuredParts.pointsOfLow.size());
    auto globalParts = CurveParts(
        offsetLE.points(),
        offsetTE.points(),
        highPoints,
        lowPoints
    );
    reassemblePointsIntoWholeCurve(updatedMeasName, globalParts);
    reassemblePointsIntoWholeCurve(updatedNomName, nominalParts);
}

CurveAnalyzer::CurveParts CurveAnalyzer::alignCurveParts(const CurveParts &curveParts, const Function18Params &params18) {
    auto curveName = "dummy";
    reassemblePointsIntoWholeCurve(curveName, curveParts);
    auto points = getCurveFromProject(curveName)->points();

    auto curve = CurveFigure(curveName, points);
    curve.alignment(_reportSettings->rotation(), _reportSettings->xShift(), _reportSettings->yShift());
    insertCurveInProject(curveName, curve.points(), true);

    return Algorithms::divideCurveIntoParts(curveName, &params18, _project);
}

void CurveAnalyzer::insertCurveInProject(const QString &curveName, const QVector<CurvePoint> &points, bool needDelete) {
    auto curve = new CurveFigure(curveName, points);
    _project->safeInsert(curveName, curve);

    if(needDelete) {
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
