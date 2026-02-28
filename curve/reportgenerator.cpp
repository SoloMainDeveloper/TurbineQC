#include "curve/pch.h"
#include "reportgenerator.h"

Project* ReportGenerator::_project = new Project();
std::shared_ptr<ReportSettings> ReportGenerator::_reportSettings = std::make_shared<ReportSettings>();
CurveParts ReportGenerator::_curveParts = CurveParts();
QVector<QString> ReportGenerator::_curvesToDelete = QVector<QString>();

QString ReportGenerator::_nominalCurveName = "";
QString ReportGenerator::_measuredCurveName = "";
QString ReportGenerator::_globalCurveName = "global_%1";
QString ReportGenerator::_globalCVName = "globalCV_%1";
QString ReportGenerator::_globalCCName = "globalCC_%1";
QString ReportGenerator::_nominalMaxDiaName = "%1_MaxDia";
QString ReportGenerator::_measuredMaxDiaName = "%1_MaxDia";
QString ReportGenerator::_nominalMCLName = "%1_MCL";
QString ReportGenerator::_measuredMCLName = "%1_MCL";
QString ReportGenerator::_nominalContactLineName = "%1_CntctLine";
QString ReportGenerator::_measuredContactLineName = "%1_CntctLine";

void ReportGenerator::createReport(Project *project, Plot *plot, std::shared_ptr<ReportSettings> reportSettings) {
    initialization(project, plot, reportSettings);

    auto nominalCurve = dynamic_cast<const CurveFigure*>(project->findFigure(reportSettings->nominalName()));
    auto measuredCurve = dynamic_cast<const CurveFigure*>(project->findFigure(reportSettings->measuredName()));
    auto nominalPoints = nominalCurve->points();
    auto measuredPoints = measuredCurve->points();
    analyzeProfile(plot, nominalPoints, measuredPoints);

    if(reportSettings->needPrintWithTemplate()) {
        auto creatingMarkup = CreatingMarkup(reportSettings, _curveParts);
        creatingMarkup.run(project);
    }
}

void ReportGenerator::initialization(Project *project, Plot *plot, std::shared_ptr<ReportSettings> reportSettings) {
    _project = project;
    _reportSettings = reportSettings;
    _nominalCurveName = reportSettings->nominalName();
    _measuredCurveName = reportSettings->measuredName();
    if(_nominalCurveName == _measuredCurveName) {
        _measuredCurveName = "_" + _measuredCurveName;
    }
    _globalCurveName = _globalCurveName.arg(_nominalCurveName);
    _globalCVName = _globalCVName.arg(_nominalCurveName);
    _globalCCName = _globalCCName.arg(_nominalCurveName);
    _nominalMaxDiaName = _nominalMaxDiaName.arg(_nominalCurveName);
    _measuredMaxDiaName = _measuredMaxDiaName.arg(_measuredCurveName);
    _nominalMCLName = _nominalMCLName.arg(_nominalCurveName);
    _measuredMCLName = _measuredMCLName.arg(_measuredCurveName);
    _nominalContactLineName = _nominalContactLineName.arg(_nominalCurveName);
    _measuredContactLineName = _measuredContactLineName.arg(_measuredCurveName);
}

void ReportGenerator::analyzeProfile(Plot *plot, const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &measuredPoints) {
    auto params18 = getParams18();
    auto nominalRes18 = getResult18(nominalPoints, params18);
    auto updatedNomPoints = getReassembledPoints(nominalRes18);
    auto updatedMeasPoints = getUpdatedMeasuredPoints(updatedNomPoints, measuredPoints, params18);
    auto updatedMeasRes18 = getResult18(updatedMeasPoints, params18);

    auto globalPointsAfterBf = getPointsAfterBestFit(nominalPoints, updatedMeasPoints);
    auto globalCurveNameAfterBf = QString("%1_AfterBf").arg(_measuredCurveName);
    auto globalCurveAfterBf = new CurveFigure(globalCurveNameAfterBf, globalPointsAfterBf);
    _project->safeInsert(globalCurveNameAfterBf, globalCurveAfterBf, false);
    createAditionalFigures(_nominalCurveName, globalCurveNameAfterBf);

    auto globalRes18 = getResult18WithDevs(nominalPoints, globalPointsAfterBf, params18);
    auto profileType = _reportSettings->profileType();
    if(profileType == ReportSettings::Profile::Whole) {
        analyzeWholeProfile(plot, updatedNomPoints, globalPointsAfterBf, globalRes18);
    } else if(profileType == ReportSettings::Profile::WithoutTE) {
        analyzeProfileWithoutTE(plot, nominalPoints, globalPointsAfterBf, globalRes18);
    } else if(profileType == ReportSettings::Profile::WithoutEdges) {
        analyzeProfileWithoutEdges(plot, globalRes18);
    } else if(profileType == ReportSettings::Profile::WithoutEdgesLSQ) {
        analyzeProfileWithoutEdgesLSQ(plot, nominalRes18, updatedMeasRes18, globalRes18);
    } else if(profileType == ReportSettings::Profile::WithoutEdgesForm) {
        analyzeProfileWithoutEdgesForm(plot, nominalRes18, updatedMeasRes18, globalRes18);
    }
}

QVector<CurvePoint> ReportGenerator::getReassembledPoints(const Function18Result &res18) {
    auto globalBestFit = _reportSettings->globalBestFit();

    if(globalBestFit == ReportSettings::GlobalBestFit::Whole) {
        return getReassembledPointsOfWholeCurve(res18);
    } else if(globalBestFit == ReportSettings::GlobalBestFit::WithoutEdges) {
        return getReassembledPointsWithoutEdges(res18);
    }
    return QVector<CurvePoint>();
}

QVector<CurvePoint> ReportGenerator::getReassembledPointsOfWholeCurve(const Function18Result &res18) {
    auto curvePointsOfLE = res18.curveLE.points();
    auto curvePointsOfTE = res18.curveTE.points();
    auto lowCurvePoints = res18.curveLow.points();
    auto highCurvePoints = res18.curveHigh.points();

    if(_reportSettings->directionOfLE() == ReportSettings::LEDirection::PlusX
        || _reportSettings->directionOfLE() == ReportSettings::LEDirection::PlusY) {
        curvePointsOfLE.removeFirst();
        highCurvePoints.removeFirst();
        curvePointsOfTE.removeFirst();
        curvePointsOfTE.removeLast();
        return lowCurvePoints + curvePointsOfLE + highCurvePoints + curvePointsOfTE;
    } else {
        curvePointsOfLE.removeFirst();
        lowCurvePoints.removeFirst();
        curvePointsOfTE.removeFirst();
        curvePointsOfTE.removeLast();
        return highCurvePoints + curvePointsOfLE + lowCurvePoints + curvePointsOfTE;
    }
}

QVector<CurvePoint> ReportGenerator::getReassembledPointsWithoutTE(const Function18Result &res18) {
    auto curvePointsOfLE = res18.curveLE.points();
    auto lowCurvePoints = res18.curveLow.points();
    auto highCurvePoints = res18.curveHigh.points();
    auto directionOfLE = _reportSettings->directionOfLE();

    if(directionOfLE == ReportSettings::LEDirection::PlusX || directionOfLE == ReportSettings::LEDirection::PlusY) {
        curvePointsOfLE.removeFirst();
        highCurvePoints.removeFirst();
        return lowCurvePoints + curvePointsOfLE + highCurvePoints;
    } else {
        curvePointsOfLE.removeFirst();
        lowCurvePoints.removeFirst();
        return highCurvePoints + curvePointsOfLE + lowCurvePoints;
    }
}

QVector<CurvePoint> ReportGenerator::getReassembledPointsWithoutEdges(const Function18Result &res18) {
    auto lowCurvePoints = res18.curveLow.points();
    auto highCurvePoints = res18.curveHigh.points();

    return highCurvePoints + lowCurvePoints;
}

QVector<CurvePoint> ReportGenerator::getUpdatedMeasuredPoints(const QVector<CurvePoint> &updatedNomPoints, const QVector<CurvePoint> &measuredPoints, const Function18Params &params18) {
    auto measuredPointsAfterPreProcess = getPointsAfterPreProcess(measuredPoints);
    auto measuredRes18 = CurveLibrary::function18(measuredPointsAfterPreProcess, params18);
    auto updatedMeasPoints = getReassembledPoints(measuredRes18);

    if(_reportSettings->isLEStretch() || _reportSettings->isTEStretch()) {
        updatedMeasPoints = getPointsAfterStretching(updatedNomPoints, updatedMeasPoints, params18);
    }
    if(_reportSettings->globalBestFit() == ReportSettings::GlobalBestFit::Whole) {
        auto params19 = Function19Params();
        auto res19 = CurveLibrary::function19(updatedMeasPoints, params19);
        updatedMeasPoints = res19.curve.points();
    }
    return updatedMeasPoints;
}

QVector<CurvePoint> ReportGenerator::getPointsAfterStretching(const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &measuredPoints, const Function18Params &params18) {
    auto params31 = Function31Params(_reportSettings->isLEStretch(), _reportSettings->isTEStretch());
    auto res31 = CurveLibrary::function31(nominalPoints, measuredPoints, params31);

    auto params6 = Function6Params();
    auto res6 = CurveLibrary::function6(res31.curve.points(), measuredPoints, params6);

    auto res18 = CurveLibrary::function18(res6.curve.points(), params18);
    auto updatedPoints = getReassembledPoints(res18);

    return updatedPoints;
}

QVector<CurvePoint> ReportGenerator::getPointsAfterPreProcess(const QVector<CurvePoint> &points) {
    if(_reportSettings->needSortPoints()) {
        auto params1 = Function1Params(0, 0.02, 0, true, true, FunctionParams::Direction::Left, true);
        auto res1 = CurveLibrary::function1(points, params1);
        return res1.curve.points();
    }
    if(_reportSettings->needRemoveEqualPoints()) {
        auto params1 = Function1Params(0, _reportSettings->limitForEqualPoints(), 0, true, true, FunctionParams::Direction::Left, true);
        auto res1 = CurveLibrary::function1(points, params1);
        return res1.curve.points();
    }
    if(_reportSettings->needRadiusCompensation()) {
        auto params1 = Function1Params(0, 0.02, 0, true, true, FunctionParams::Direction::Left, true);
        auto res1 = CurveLibrary::function1(points, params1);
        auto params3 = Function3Params(_reportSettings->radiusCompensation(), true, false);
        auto res3 = CurveLibrary::function3(res1.curve.points(), params3);
        return res3.curve.points();
    }
    if(_reportSettings->needUse3DVectors()) {
        //to do
    }
    return points;
}

QVector<CurvePoint> ReportGenerator::getPointsAfterBestFit(const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &updatedMeasPoints) {
    auto params6 = getParams6();
    auto res6 = CurveLibrary::function6(updatedMeasPoints, nominalPoints, params6);

    _reportSettings->setBestFitValues(res6.offsetX, res6.offsetY, res6.rotation);

    return res6.curve.points();
}

void ReportGenerator::analyzeWholeProfile(Plot *plot, const QVector<CurvePoint> &updatedNomPoints, const QVector<CurvePoint> &globalPointsAfterBestFit, const Function18Result &globalRes18) {
    auto params4 = getParams4(true);
    auto res4 = CurveLibrary::function4(updatedNomPoints, globalPointsAfterBestFit, params4);
    auto globalPointsAfterCalcDevs = res4.curve.points();

    _curveParts = CurveParts(
        globalRes18.curveLE.points(),
        globalRes18.curveTE.points(),
        globalRes18.curveHigh.points(),
        globalRes18.curveLow.points()
    );
    createEdgesAndMakeScreenshots(plot, _curveParts.pointsOfLE(), _curveParts.pointsOfTE());

    createGlobalCurve(globalPointsAfterCalcDevs, true);
    makeScreenshotOfGlobal(plot);

    createPartsOfCurve(_curveParts);

    deleteCurves();
}

void ReportGenerator::analyzeProfileWithoutTE(Plot *plot, const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &globalPointsAfterBestFit, const Function18Result &globalRes18) {
    auto params18 = getParams18();
    auto nominalRes18 = getResult18(nominalPoints, params18);

    auto updatedNomPoints = getReassembledPointsWithoutTE(nominalRes18);
    auto params4 = getParams4();
    auto res4 = CurveLibrary::function4(updatedNomPoints, globalPointsAfterBestFit, params4);
    auto globalPointsAfterCalcDevs = res4.curve.points();

    _curveParts = CurveParts(
        globalRes18.curveLE.points(),
        globalRes18.curveTE.points(),
        globalRes18.curveHigh.points(),
        globalRes18.curveLow.points()
    );
    createEdgesAndMakeScreenshots(plot, _curveParts.pointsOfLE(), _curveParts.pointsOfTE());

    createGlobalCurve(globalPointsAfterCalcDevs);
    makeScreenshotOfGlobal(plot);

    createPartsOfCurve(_curveParts);
    deleteCurves();
}

void ReportGenerator::analyzeProfileWithoutEdges(Plot *plot, const Function18Result &globalRes18) {
    _curveParts = CurveParts(
        globalRes18.curveLE.points(),
        globalRes18.curveTE.points(),
        globalRes18.curveHigh.points(),
        globalRes18.curveLow.points()
    );
    createEdgesAndMakeScreenshots(plot, _curveParts.pointsOfLE(), _curveParts.pointsOfTE());

    createPartsOfCurve(_curveParts, true);
    makeScreenshotOfGlobal(plot);

    deleteCurves();
}

void ReportGenerator::analyzeProfileWithoutEdgesLSQ(Plot *plot, const Function18Result &nominalRes18, const Function18Result &updatedMeasRes18, const Function18Result &globalRes18) {
    auto nominalCVPoints = nominalRes18.curveHigh.points();
    auto nominalCCPoints = nominalRes18.curveLow.points();
    auto measuredCVPoints = updatedMeasRes18.curveHigh.points();
    auto measuredCCPoints = updatedMeasRes18.curveLow.points();

    auto params6 = Function6Params(true, Function6Params::Algorithm::Curve, false, true, true, true);
    auto res6CV = CurveLibrary::function6(measuredCVPoints, nominalCVPoints, params6);
    auto globalCVPoints = res6CV.curve.points();
    auto res6CC = CurveLibrary::function6(measuredCCPoints, nominalCCPoints, params6);
    auto globalCCPoints = res6CC.curve.points();
    _reportSettings->setBestFitValues(res6CV.offsetX, res6CV.offsetY, res6CV.rotation, res6CC.offsetX, res6CC.offsetY, res6CC.rotation);

    auto params4 = getParams4();
    auto res4CV = CurveLibrary::function4(nominalCVPoints, globalCVPoints, params4);
    globalCVPoints = res4CV.curve.points();
    auto res4CC = CurveLibrary::function4(nominalCCPoints, globalCCPoints, params4);
    globalCCPoints = res4CC.curve.points();

    _curveParts = CurveParts(
        globalRes18.curveLE.points(),
        globalRes18.curveTE.points(),
        globalCVPoints,
        globalCCPoints
    );
    createEdgesAndMakeScreenshots(plot, _curveParts.pointsOfLE(), _curveParts.pointsOfTE());

    createPartsOfCurve(_curveParts, true);
    makeScreenshotOfGlobal(plot);

    deleteCurves();
}

void ReportGenerator::analyzeProfileWithoutEdgesForm(Plot *plot, const Function18Result &nominalRes18, const Function18Result &updatedMeasRes18, const Function18Result &globalRes18) {
    auto nominalCVPoints = nominalRes18.curveHigh.points();
    auto nominalCCPoints = nominalRes18.curveLow.points();
    auto measuredCVPoints = updatedMeasRes18.curveHigh.points();
    auto measuredCCPoints = updatedMeasRes18.curveLow.points();

    auto params21 = getParams21();
    auto res21CV = CurveLibrary::function21(measuredCVPoints, nominalCVPoints, nominalCVPoints, params21);
    auto globalCVPoints = res21CV.curve.points();
    auto res21CC = CurveLibrary::function21(measuredCCPoints, nominalCCPoints, nominalCCPoints, params21);
    auto globalCCPoints = res21CC.curve.points();
    _reportSettings->setBestFitValues(res21CV.offsetX, res21CV.offsetY, res21CV.rotation, res21CC.offsetX, res21CC.offsetY, res21CC.rotation);

    auto params4 = getParams4();
    auto res4CV = CurveLibrary::function4(nominalCVPoints, globalCVPoints, params4);
    globalCVPoints = res4CV.curve.points();
    auto res4CC = CurveLibrary::function4(nominalCCPoints, globalCCPoints, params4);
    globalCCPoints = res4CC.curve.points();

    _curveParts = CurveParts(
        globalRes18.curveLE.points(),
        globalRes18.curveTE.points(),
        globalCVPoints,
        globalCCPoints
    );
    createEdgesAndMakeScreenshots(plot, _curveParts.pointsOfLE(), _curveParts.pointsOfTE());

    createPartsOfCurve(_curveParts, true);
    makeScreenshotOfGlobal(plot);

    deleteCurves();
}

Function18Result ReportGenerator::getResult18(const QVector<CurvePoint> &points, const Function18Params &params18) {
    auto res18 = CurveLibrary::function18(points, params18);

    return res18;
}

Function18Result ReportGenerator::getResult18WithDevs(const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &globalPoints, const Function18Params &params18) {
    auto params4 = getParams4(true);
    auto res4 = CurveLibrary::function4(nominalPoints, globalPoints, params4);

    auto points = res4.curve.points();
    auto res18 = CurveLibrary::function18(points, params18);

    return res18;
}

void ReportGenerator::insertCurveInProject(const QString &curveName, const QVector<CurvePoint> &points, bool needDelete) {
    auto curve = new CurveFigure(curveName, points);
    _project->safeInsert(curveName, curve, false);

    if(needDelete) {
        _curvesToDelete.append(curveName);
    }
}

const CurveFigure* ReportGenerator::findCurve(const QString &curveName) {
    auto figure = _project->findFigure(curveName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    assert(curve);

    return curve;
}

const Function4Params ReportGenerator::getParams4(bool isClosed) {
    auto evaluationPlace = _reportSettings->evaluationPlace() == ReportSettings::Evaluation::Nominal ? 1 : 2;
    auto evaluationDirection = _reportSettings->evaluationDirection() == ReportSettings::Evaluation::Nominal ? 1 : 2;
    return Function4Params(0, evaluationPlace, evaluationDirection, isClosed);
}

const Function6Params ReportGenerator::getParams6() {
    auto bestFitType = _reportSettings->bestFitType();
    if(bestFitType == ReportSettings::BestFitType::OnlyTranslation) {
        return Function6Params(true, Function6Params::Algorithm::Curve, true, true, true, false);
    } else if(bestFitType == ReportSettings::BestFitType::OnlyRotation) {
        return Function6Params(false, Function6Params::Algorithm::Curve, true, false, false, true);
    } else if(bestFitType == ReportSettings::BestFitType::OnlyXTranslation) {
        return Function6Params(false, Function6Params::Algorithm::Curve, true, true, false, false);
    } else if(bestFitType == ReportSettings::BestFitType::OnlyYTranslation) {
        return Function6Params(false, Function6Params::Algorithm::Curve, true, false, true, false);
    } else if(bestFitType == ReportSettings::BestFitType::XTranslationAndRotation) {
        return Function6Params(true, Function6Params::Algorithm::Curve, true, true, false, true);
    } else if(bestFitType == ReportSettings::BestFitType::YTranslationAndRotation) {
        return Function6Params(true, Function6Params::Algorithm::Curve, true, false, true, true);
    } else {
        return Function6Params();
    }
}

const Function18Params ReportGenerator::getParams18() {
    if(_reportSettings->measureType() == ReportSettings::MeasureType::Length) {
        auto params18 = Function18Params(0, 1, 1);
        auto contactLineLength = Algorithms::getContactLineLength(_nominalCurveName, &params18, _project);
        auto zoneLEInPercent = _reportSettings->zoneLE() / contactLineLength * 100;
        auto zoneTEInPercent = _reportSettings->zoneTE() / contactLineLength * 100;
        return Function18Params(static_cast<int>(_reportSettings->directionOfLE()), zoneLEInPercent, zoneTEInPercent);
    } else {
        return Function18Params(static_cast<int>(_reportSettings->directionOfLE()), _reportSettings->zoneLE(), _reportSettings->zoneTE());
    }
}

const Function21Params ReportGenerator::getParams21() {
    auto bestFitType = _reportSettings->bestFitType();
    if(bestFitType == ReportSettings::BestFitType::OnlyTranslation) {
        return Function21Params(0, true, true, false);
    } else if(bestFitType == ReportSettings::BestFitType::OnlyRotation) {
        return Function21Params(0, false, false, true);
    } else if(bestFitType == ReportSettings::BestFitType::OnlyXTranslation) {
        return Function21Params(0, true, false, false);
    } else if(bestFitType == ReportSettings::BestFitType::OnlyYTranslation) {
        return Function21Params(0, false, true, false);
    } else if(bestFitType == ReportSettings::BestFitType::XTranslationAndRotation) {
        return Function21Params(0, true, false, true);
    } else if(bestFitType == ReportSettings::BestFitType::YTranslationAndRotation) {
        return Function21Params(0, false, true, true);
    } else {
        return Function21Params();
    }
}

void ReportGenerator::createAditionalFigures(const QString &nominalCurveName, const QString &globalCurveNameAfterBf) {
    auto params18 = getParams18();

    if(_reportSettings->needMaxDiameter()) {
        Algorithms::createMaxCircle(nominalCurveName, _measuredMaxDiaName, &params18, _project);
        Algorithms::createMaxCircle(globalCurveNameAfterBf, _nominalMaxDiaName, &params18, _project, Qt::blue);
    }
    if(_reportSettings->needMCL()) {
        Algorithms::createMiddleCurve(nominalCurveName, _measuredMCLName, &params18, _project);
        Algorithms::createMiddleCurve(globalCurveNameAfterBf, _nominalMCLName, &params18, _project, Qt::blue);
    }
    if(_reportSettings->needContactLine()) {
        Algorithms::createContactLine(nominalCurveName, _measuredContactLineName, &params18, _project);
        Algorithms::createContactLine(globalCurveNameAfterBf, _nominalContactLineName, &params18, _project, Qt::blue);
    }
}

void ReportGenerator::createEdgesAndMakeScreenshots(Plot *plot, const QVector<CurvePoint> &pointsOfLE, const QVector<CurvePoint> &pointsOfTE) {
    if(_reportSettings->isLEVisible()) {
        createEdge(pointsOfLE, _reportSettings->amplificationOfLE(), EdgeType::LE, _reportSettings->typeOfShowDevsLE());
        if(_reportSettings->needPrintWithTemplate()) {
            makeScreenshotOfEdge(plot, EdgeType::LE, _reportSettings->axisTypeOfLE(), Plot::Position::Right);
        }
    }
    if(_reportSettings->isTEVisible()) {
        createEdge(pointsOfTE, _reportSettings->amplificationOfTE(), EdgeType::TE, _reportSettings->typeOfShowDevsTE());
        if(_reportSettings->needPrintWithTemplate()) {
            makeScreenshotOfEdge(plot, EdgeType::TE, _reportSettings->axisTypeOfTE(), Plot::Position::Left);
        }
    }
}

void ReportGenerator::createEdge(const QVector<CurvePoint> &points, double amplification, EdgeType edgeType, ReportSettings::TypeOfShowDevs devsType) {
    _project->resetVisibilityForAllFigures();

    auto edgeName = edgeType == EdgeType::LE ? "LE" : "TE";
    auto curveName = QString("%1_%2").arg(edgeName).arg(_nominalCurveName);
    auto curve = new CurveFigure(curveName, points);
    curve->setShowDeviations(true);
    curve->setConnectDeviations(true);
    curve->setAmplification(amplification);
    createNumericalDeviations(points, devsType, edgeType);
    _project->safeInsert(curveName, curve, false);
    addTable(QString("%1_%2_Form").arg(edgeName).arg(_nominalCurveName), curve);
}

void ReportGenerator::createNumericalDeviations(const QVector<CurvePoint> &points, ReportSettings::TypeOfShowDevs devsType, EdgeType edgeType) {
    QVector<CurvePoint> pointsWithInterval;
    if(devsType == ReportSettings::TypeOfShowDevs::Auto) {
        auto pointsInterval = 4;
        if(points.length() > 50) {
            pointsInterval = points.length() / 12;
        }
        for(auto i = 0; i < points.length(); i += pointsInterval) {
            pointsWithInterval.append(points[i]);
        }
    } else if(devsType == ReportSettings::TypeOfShowDevs::Set) {
        auto pointsInterval = edgeType == EdgeType::LE ? _reportSettings->valueOfSetShowDevsLE() + 1 : _reportSettings->valueOfSetShowDevsTE() + 1;
        for(auto i = 0; i < points.length(); i += pointsInterval) {
            pointsWithInterval.append(points[i]);
        }
    } else if(devsType == ReportSettings::TypeOfShowDevs::OnEdge) {
        pointsWithInterval.append(points[points.length() / 2]);
    }
    auto edgeName = edgeType == EdgeType::LE ? "LE" : "TE";
    auto curveWithIntervalName = QString("%1_with_interval_%2").arg(edgeName).arg(_nominalCurveName);
    auto curveWithInterval = new CurveFigure(curveWithIntervalName, pointsWithInterval);
    curveWithInterval->setConnectPoints(false);
    curveWithInterval->setShowPoints(true);
    curveWithInterval->setShowNumericalDeviations(true);
    _project->safeInsert(curveWithIntervalName, curveWithInterval, false);
    _curvesToDelete.append(curveWithIntervalName);
}

void ReportGenerator::makeScreenshotOfEdge(Plot *plot, EdgeType edgeType, ReportSettings::Axis axisType, Plot::Position position) {
    plot->rescale(position);
    if(_reportSettings->needMCL()) {
        _project->setVisibility({ _nominalMCLName, _measuredMCLName });
    }
    auto screenshot = plot->getScreenshot(320, 200, axisType);
    if(edgeType == EdgeType::LE) {
        _reportSettings->setScreenshotOfLE(screenshot);
    } else {
        _reportSettings->setScreenshotOfTE(screenshot);
    }
}

void ReportGenerator::createGlobalCurve(const QVector<CurvePoint> &globalPoints, bool isClosed) {
    _project->resetVisibilityForAllFigures();

    auto globalCurve = new CurveFigure(_globalCurveName, globalPoints);
    globalCurve->setClosed(isClosed);
    globalCurve->setShowDeviations(true);
    globalCurve->setConnectDeviations(true);
    globalCurve->setAmplification(_reportSettings->globalAmplification());
    _project->safeInsert(_globalCurveName, globalCurve, false);
    addTable(_nominalCurveName + "_Form", globalCurve);
}

void ReportGenerator::createPartsOfCurve(const CurveParts &curveParts, bool isResetVisibilityForAllFigures) {
    if(isResetVisibilityForAllFigures) {
        _project->resetVisibilityForAllFigures();
    }

    auto globalCV = new CurveFigure(_globalCVName, curveParts.pointsOfHigh());
    globalCV->setShowDeviations(true);
    globalCV->setConnectDeviations(true);
    globalCV->setAmplification(_reportSettings->globalAmplification());
    _project->safeInsert(_globalCVName, globalCV, false);
    addTable(_nominalCurveName + "_Convex_Form", globalCV);

    auto globalCC = new CurveFigure(_globalCCName, curveParts.pointsOfLow());
    globalCC->setShowDeviations(true);
    globalCC->setConnectDeviations(true);
    globalCC->setAmplification(_reportSettings->globalAmplification());
    _project->safeInsert(_globalCCName, globalCC, false);
    addTable(_nominalCurveName + "_Concave_Form", globalCC);
}

void ReportGenerator::addTable(const QString &tableName, const CurveFigure *globalCurve, const Point &labelPoint) {
    auto table = new DimFigure(tableName, labelPoint, globalCurve);
    table->setDimType(DimFigure::DimType::Form);
    table->setRenderType(DimFigure::RenderType::Table);
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
        DimFigure::Value(DimFigure::ValueType::MinMax, true, abs(maxDeviation) + abs(minDeviation)),
            DimFigure::Value(DimFigure::ValueType::Form, true, std::max(abs(maxDeviation), abs(minDeviation) * 2)),
            DimFigure::Value(DimFigure::ValueType::Min, true, minDeviation),
            DimFigure::Value(DimFigure::ValueType::Max, true, maxDeviation),
            DimFigure::Value(DimFigure::ValueType::MaxAbs, false, abs(std::max(minDeviation, maxDeviation))),

            // TODO: should be corrected with constant tolerance
            DimFigure::Value(DimFigure::ValueType::SupUT, true, maxDeviation),
            DimFigure::Value(DimFigure::ValueType::InfLT, true, minDeviation),
    });
    _project->safeInsert(tableName, table, false);
}

void ReportGenerator::makeScreenshotOfGlobal(Plot *plot) {
    setVisibilityAdditionalFigures();
    _project->setCurrentFigure(_nominalCurveName);
    if(_reportSettings->needPrintWithTemplate()) {
        plot->rescale(Plot::Position::Center);
        auto screenshot = plot->getScreenshot(600, 350, _reportSettings->globalAxisType());
        _reportSettings->setScreenshotOfGlobal(screenshot);
    }
    _project->setVisibility({ _nominalCurveName });
}

void ReportGenerator::setVisibilityAdditionalFigures() {
    QStringList figureNames;
    if(_reportSettings->needMCL()) {
        figureNames.append(_nominalMCLName);
        figureNames.append(_measuredMCLName);
    }
    if(_reportSettings->needMaxDiameter()) {
        figureNames.append(_nominalMaxDiaName);
        figureNames.append(_measuredMaxDiaName);
    }
    if(_reportSettings->needContactLine()) {
        figureNames.append(_nominalContactLineName);
        figureNames.append(_measuredContactLineName);
    }
    _project->setVisibility(figureNames);
}

void ReportGenerator::deleteCurves() {
    for(auto nameCurve : _curvesToDelete) {
        _project->removeFigure(nameCurve);
    }
    _curvesToDelete.clear();
}
