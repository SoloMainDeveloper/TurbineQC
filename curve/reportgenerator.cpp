#include "curve/pch.h"
#include "reportgenerator.h"

ReportGenerator::ReportGenerator(Project *project, Plot *plot, ReportData *reportData, const QString &filePath) {
    _project = project;
    _plot = plot;
    _reportData = reportData;

    _nomMaxDia = new CircleFigure();
    _measMaxDia = new CircleFigure();
    _nomMCL = new CurveFigure();
    _measMCL = new CurveFigure();
    _nomChord = new LineFigure();
    _measChord = new LineFigure();

    _index = "<!DOCTYPE html>\n \
              <html lang=\"en\">\n \
                  <head>\n \
                      <meta charset=\"UTF-8\">\n \
                      <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n \
                      <title>Document</title>\n \
                  </head>\n \
                  <body style=\"margin: 0; padding : 0;\">\n \
                      %1\n \
                      <div style=\"width: 90vw; height: 96vh; border: 1px solid; margin: auto; display: grid; grid-template-columns: 1fr 1fr 1.5fr; grid-template-rows: 2.5fr 1.5fr;\">\n \
                          %2\n \
                          %3\n \
                          %4\n \
                          %5\n \
                          %6\n \
                      </div>\n \
                  </body>\n \
              </html>";

    _tableRowTemplate = "<tr>\n \
                             <td style=\"font-size: 12px;\"><b>%1</b></td>\n \
                         </tr>\n \
                         <tr>\n \
                             <td style=\"font-size: 12px; text-align: center;\"><b>%2</b></td>\n \
                             <td style=\"font-size: 12px; text-align: center;\">%3</td>\n \
                             <td style=\"font-size: 12px; text-align: center;\">%4</td>\n \
                             <td style=\"font-size: 12px; text-align: center;\">%5</td>\n \
                             <td style=\"font-size: 12px; text-align: center;\">%6</td>\n \
                             <td style=\"font-size: 12px; text-align: center;\">%7</td>\n \
                             %8\n \
                         </tr>";

    _reportPath = filePath;
    _screenshotsDir = QFileInfo(filePath).absolutePath() + "/screenshots";
    _updatedNomCurveName = "%1_forBestFit";
    _updatedMeasCurveName = "%1_forBestFit";
    _globalCurveName = "global_%1";
    _globalCVName = "globalCV_%1"; //CV - convex
    _globalCCName = "globalCC_%1"; //CC - concave
    _nomCurveName = "";
    _measCurveName = "";
    _nomMaxDiaName = "%1_MaxDia";
    _measMaxDiaName = "%1_MaxDia";
    _nomMCLName = "%1_MCL";
    _measMCLName = "%1_MCL";
    _nomChordName = "%1_CntctLine";
    _measChordName = "%1_CntctLine";

    _isCreateMaxDia = false;
    _isCreateMCL = false;
    _isCreateChord = false;

    makeDirectories();
}

void ReportGenerator::makeDirectories() {
    if(!QDir(_screenshotsDir).exists()) {
        QDir().mkdir(_screenshotsDir);
    }
}

void ReportGenerator::createReport(const CurveFigure *nominalCurve, const CurveFigure* measuredCurve) {
    if(_reportData->reportTemplate() != ReportData::Template::AirfoilReport1) {
        return;
    }

    initialization(nominalCurve->name(), measuredCurve->name());

    if(_reportData->profileType() == ReportData::Profile::WithoutEdges) {
        showProfileWithoutEdges(nominalCurve->points(), measuredCurve->points());
    } else if(_reportData->profileType() == ReportData::Profile::WithoutTE) {
        showProfileWithoutTE(nominalCurve->points(), measuredCurve->points());
    } else if(_reportData->profileType() == ReportData::Profile::WithoutEdgesLSQ) {
        showProfileWithoutEdgesLSQ(nominalCurve->points(), measuredCurve->points());
    } else if(_reportData->profileType() == ReportData::Profile::WithoutEdgesForm) {
        showProfileWithoutEdgesForm(nominalCurve->points(), measuredCurve->points());
    } else {
        showWholeProfile(nominalCurve->points(), measuredCurve->points());
    }

    if(_reportData->needPrintWithTemplate()) {
        createMarkup();
    }
}

void ReportGenerator::initialization(const QString &nomCurveName, const QString &measCurveName) {
    _nomCurveName = nomCurveName;
    if(nomCurveName == measCurveName) {
        _measCurveName = "_" + measCurveName;
    } else {
        _measCurveName = measCurveName;
    }
    _updatedNomCurveName = _updatedNomCurveName.arg(_nomCurveName);
    _updatedMeasCurveName = _updatedMeasCurveName.arg(_measCurveName);
    _globalCurveName = _globalCurveName.arg(_nomCurveName);
    _globalCVName = _globalCVName.arg(_nomCurveName);
    _globalCCName = _globalCCName.arg(_nomCurveName);
    _nomMaxDiaName = _nomMaxDiaName.arg(_nomCurveName);
    _measMaxDiaName = _measMaxDiaName.arg(_measCurveName);
    _nomMCLName = _nomMCLName.arg(_nomCurveName);
    _measMCLName = _measMCLName.arg(_measCurveName);
    _nomChordName = _nomChordName.arg(_nomCurveName);
    _measChordName = _measChordName.arg(_measCurveName);

    if(_project->containsFigure(_globalCurveName)) {
        _project->removeFigure(_globalCurveName);
    }
    if(_project->containsFigure(_globalCVName)) {
        _project->removeFigure(_globalCVName);
    }
    if(_project->containsFigure(_globalCCName)) {
        _project->removeFigure(_globalCCName);
    }
    if(_project->containsFigure(_nomMaxDiaName)) {
        _project->removeFigure(_nomMaxDiaName);
    }
    if(_project->containsFigure(_measMaxDiaName)) {
        _project->removeFigure(_measMaxDiaName);
    }
    if(_project->containsFigure(_nomMCLName)) {
        _project->removeFigure(_nomMCLName);
    }
    if(_project->containsFigure(_measMCLName)) {
        _project->removeFigure(_measMCLName);
    }
    if(_project->containsFigure(_nomChordName)) {
        _project->removeFigure(_nomChordName);
    }
    if(_project->containsFigure(_measChordName)) {
        _project->removeFigure(_measChordName);
    }
}

void ReportGenerator::showWholeProfile(const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &measuredPoints) {
    auto updatedMeasPoints = createUpdatedMeasuredCurve(nominalPoints, measuredPoints);

    auto params6 = getParams6();
    auto res6 = CurveLibrary::function6(updatedMeasPoints, nominalPoints, params6);
    auto globalCurve = new CurveFigure(_globalCurveName, res6.curve.points());
    _project->safeInsert(_globalCurveName, globalCurve);
    _reportData->setBestFitValues(res6.offsetX, res6.offsetY, res6.rotation);

    createAditionalFigures(nominalPoints);

    makeScreenshotOfEdges();

    auto params4 = getParams4();
    Algorithms::calculateDeviations(_updatedNomCurveName, _globalCurveName, _globalCurveName, &params4, _project);

    auto globalCurveWithDevs = findCurve(_globalCurveName);
    makeScreenshotOfGlobal(globalCurveWithDevs->points(), true);
    deleteCurves();
}

void ReportGenerator::showProfileWithoutEdges(const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &measuredPoints) {
    auto updatedMeasPoints = createUpdatedMeasuredCurve(nominalPoints, measuredPoints);

    auto params6 = getParams6();
    auto res6 = CurveLibrary::function6(updatedMeasPoints, nominalPoints, params6);
    auto globalCurve = new CurveFigure(_globalCurveName, res6.curve.points());
    _project->safeInsert(_globalCurveName, globalCurve);
    _reportData->setBestFitValues(res6.offsetX, res6.offsetY, res6.rotation);
    _curvesToDelete.append(_globalCurveName);

    createAditionalFigures(nominalPoints);

    makeScreenshotOfEdges();
    makeScreenshotOfGlobal(_measuredRes18.curveHigh.points(), _measuredRes18.curveLow.points());
    deleteCurves();
}

void ReportGenerator::showProfileWithoutTE(const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &measuredPoints) {
    auto updatedMeasPoints = createUpdatedMeasuredCurve(nominalPoints, measuredPoints);

    auto params6 = getParams6();
    auto res6 = CurveLibrary::function6(updatedMeasPoints, nominalPoints, params6);
    auto globalCurve = new CurveFigure(_globalCurveName, res6.curve.points());
    _project->safeInsert(_globalCurveName, globalCurve);
    _reportData->setBestFitValues(res6.offsetX, res6.offsetY, res6.rotation);

    createAditionalFigures(nominalPoints);

    makeScreenshotOfEdges();

    auto updatedNomNameWithoutTE = QString("%1_withoutTE").arg(_nomCurveName);
    auto updatedNomPointsWithoutTE = getUpdatedPoints(_nominalRes18, true);
    insertCurveInProject(updatedNomNameWithoutTE, updatedNomPointsWithoutTE, true);

    auto params4 = getParams4();
    Algorithms::calculateDeviations(updatedNomNameWithoutTE, _globalCurveName, _globalCurveName, &params4, _project);

    auto globalCurveWithDevs = findCurve(_globalCurveName);
    makeScreenshotOfGlobal(globalCurveWithDevs->points());
    deleteCurves();
}

void ReportGenerator::showProfileWithoutEdgesLSQ(const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &measuredPoints) {
    auto updatedMeasPoints = createUpdatedMeasuredCurve(nominalPoints, measuredPoints);

    auto params18 = getParams18();
    auto res18AfterUpdated = CurveLibrary::function18(updatedMeasPoints, params18);

    auto params6 = getParams6();
    Algorithms::calculateBestFit(_nomCurveName, _updatedMeasCurveName, _globalCurveName, &params6, _project, _reportData);
    _curvesToDelete.append(_globalCurveName);

    createAditionalFigures(nominalPoints);

    makeScreenshotOfEdges();

    auto nominalCVName = QString("%1_nominalCV").arg(_nomCurveName);
    auto nominalCVPoints = _nominalRes18.curveHigh.points();
    insertCurveInProject(nominalCVName, nominalCVPoints, true);
    auto measuredCVName = QString("%1_measuredCV").arg(_measCurveName);
    auto measuredCVPoints = res18AfterUpdated.curveHigh.points();
    insertCurveInProject(measuredCVName, measuredCVPoints, true);

    auto params6ForPartOfGlobal = Function6Params(true, Function6Params::Algorithm::Curve, false, true, true, true);
    auto res6CV = CurveLibrary::function6(measuredCVPoints, nominalCVPoints, params6ForPartOfGlobal);
    auto globalCV = new CurveFigure(_globalCVName, res6CV.curve.points());
    _project->safeInsert(_globalCVName, globalCV);
    auto params4ForPartOfGlobal = getParams4();
    Algorithms::calculateDeviations(nominalCVName, _globalCVName, _globalCVName, &params4ForPartOfGlobal, _project);

    auto nominalCCName = QString("%1_nominalCC").arg(_nomCurveName);
    auto nominalCCPoints = _nominalRes18.curveLow.points();
    insertCurveInProject(nominalCCName, nominalCCPoints, true);
    auto measuredCCName = QString("%1_measuredCC").arg(_measCurveName);
    auto measuredCCPoints = res18AfterUpdated.curveLow.points();
    insertCurveInProject(measuredCCName, measuredCCPoints, true);

    auto res6CC = CurveLibrary::function6(measuredCCPoints, nominalCCPoints, params6ForPartOfGlobal);
    auto globalCC = new CurveFigure(_globalCCName, res6CC.curve.points());
    _project->safeInsert(_globalCCName, globalCC);
    _reportData->setBestFitValues(res6CV.offsetX, res6CV.offsetY, res6CV.rotation, res6CC.offsetX, res6CC.offsetY, res6CC.rotation);
    Algorithms::calculateDeviations(nominalCCName, _globalCCName, _globalCCName, &params4ForPartOfGlobal, _project);

    auto globalCVWithDevs = findCurve(_globalCVName);
    auto globalCCWithDevs = findCurve(_globalCCName);
    makeScreenshotOfGlobal(globalCVWithDevs->points(), globalCCWithDevs->points());
    deleteCurves();
}

void ReportGenerator::showProfileWithoutEdgesForm(const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &measuredPoints) {
    createUpdatedMeasuredCurve(nominalPoints, measuredPoints);

    auto params6 = getParams6();
    Algorithms::calculateBestFit(_nomCurveName, _updatedMeasCurveName, _globalCurveName, &params6, _project, _reportData);
    _curvesToDelete.append(_globalCurveName);

    createAditionalFigures(nominalPoints);

    auto globalCurve = findCurve(_globalCurveName);
    auto params18 = getParams18();
    auto res18AfterBestFit = CurveLibrary::function18(globalCurve->points(), params18);

    makeScreenshotOfEdges();

    auto nominalCVName = QString("%1_nominalCV").arg(_nomCurveName);
    auto nominalCVPoints = _nominalRes18.curveHigh.points();
    insertCurveInProject(nominalCVName, nominalCVPoints, true);
    auto measuredCVName = QString("%1_measuredCV").arg(_measCurveName);
    auto measuredCVPoints = res18AfterBestFit.curveHigh.points();
    auto params21 = getParams21();
    auto res21CV = CurveLibrary::function21(measuredCVPoints, nominalCVPoints, nominalCVPoints, params21);
    insertCurveInProject(measuredCVName, res21CV.curve.points(), true);
    auto params4ForPartOfGlobal = getParams4();
    Algorithms::calculateDeviations(nominalCVName, measuredCVName, _globalCVName, &params4ForPartOfGlobal, _project);

    auto nominalCCName = QString("%1_nominalCC").arg(_nomCurveName);
    auto nominalCCPoints = _nominalRes18.curveLow.points();
    insertCurveInProject(nominalCCName, nominalCCPoints, true);
    auto measuredCCName = QString("%1_measuredCC").arg(_measCurveName);
    auto measuredCCPoints = res18AfterBestFit.curveLow.points();
    auto res21CC = CurveLibrary::function21(measuredCCPoints, nominalCCPoints, nominalCCPoints, params21);
    insertCurveInProject(measuredCCName, res21CC.curve.points(), true);
    _reportData->setBestFitValues(res21CV.offsetX, res21CV.offsetY, res21CV.rotation, res21CC.offsetX, res21CC.offsetY, res21CC.rotation);
    Algorithms::calculateDeviations(nominalCCName, measuredCCName, _globalCCName, &params4ForPartOfGlobal, _project);

    auto globalCVWithDevs = findCurve(_globalCVName);
    auto globalCCWithDevs = findCurve(_globalCCName);
    makeScreenshotOfGlobal(globalCVWithDevs->points(), globalCCWithDevs->points());
    deleteCurves();
}

Function18Result ReportGenerator::getResult18WithDeviations() {
    auto name = "globalCurveForEdges";
    auto params4 = getParams4(true);
    Algorithms::calculateDeviations(_nomCurveName, _globalCurveName, name, &params4, _project);
    _curvesToDelete.append(name);

    auto curve = findCurve(name);
    auto params18 = getParams18();
    auto res18 = CurveLibrary::function18(curve->points(), params18);

    return res18;
}

QVector<CurvePoint> ReportGenerator::createUpdatedMeasuredCurve(const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &measuredPoints) {
    auto pointsAfterPreProcess = getPointsAfterPreProcess(measuredPoints);
    auto params18 = getParams18();
    _nominalRes18 = CurveLibrary::function18(nominalPoints, params18);
    auto measRes18 = CurveLibrary::function18(pointsAfterPreProcess, params18);

    auto updatedNomPoints = getUpdatedPoints(_nominalRes18);
    insertCurveInProject(_updatedNomCurveName, updatedNomPoints, true);

    auto updatedMeasPoints = getUpdatedPoints(measRes18);
    if(_reportData->isLEStretch() || _reportData->isTEStretch()) {
        updatedMeasPoints = getPointsAfterStretching(updatedNomPoints, updatedMeasPoints);
    }
    auto params19 = Function19Params();
    auto res19 = CurveLibrary::function19(updatedMeasPoints, params19);
    insertCurveInProject(_updatedMeasCurveName, res19.curve.points(), true);

    return res19.curve.points();
}

void ReportGenerator::insertCurveInProject(const QString &curveName, const QVector<CurvePoint> &points, bool needDelete) {
    auto curve = new CurveFigure(curveName, points);
    _project->safeInsert(curveName, curve);

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

QVector<CurvePoint> ReportGenerator::getPointsAfterPreProcess(const QVector<CurvePoint> &points) {
    if(_reportData->needSortPoints()) {
        auto param1 = Function1Params(0, 0.02, 0, true, true, FunctionParams::Direction::Left, true);
        auto res1 = CurveLibrary::function1(points, param1);
        return res1.curve.points();
    } else if(_reportData->needRemoveEqualPoints()) {
        auto param1 = Function1Params(0, _reportData->limitForEqualPoints(), 0, true, true, FunctionParams::Direction::Left, true);
        auto res1 = CurveLibrary::function1(points, param1);
        return res1.curve.points();
    } else if(_reportData->needRadiusCompensation()) {

    } else if(_reportData->needUse3DVectors()) {

    } else {
        QVector<CurvePoint> oldPoints = points;
        return oldPoints;
    }
}

QVector<CurvePoint> ReportGenerator::getUpdatedPoints(Function18Result &res18, bool needWithoutTE) {
    auto curvePointsOfLE = res18.curveLE.points();
    auto curvePointsOfTE = res18.curveTE.points();
    auto lowCurvePoints = res18.curveLow.points();
    auto highCurvePoints = res18.curveHigh.points();

    if(_reportData->directionOfLE() == ReportData::LEDirection::PlusX
        || _reportData->directionOfLE() == ReportData::LEDirection::PlusY) {
        curvePointsOfLE.removeFirst();
        highCurvePoints.removeFirst();
        curvePointsOfTE.removeFirst();
        curvePointsOfTE.removeLast();
        if(needWithoutTE) {
            return lowCurvePoints + curvePointsOfLE + highCurvePoints;
        } else {
            return lowCurvePoints + curvePointsOfLE + highCurvePoints + curvePointsOfTE;
        }
    } else {
        curvePointsOfLE.removeFirst();
        lowCurvePoints.removeFirst();
        curvePointsOfTE.removeFirst();
        curvePointsOfTE.removeLast();
        if(needWithoutTE) {
            return highCurvePoints + curvePointsOfLE + lowCurvePoints;
        } else {
            return highCurvePoints + curvePointsOfLE + lowCurvePoints + curvePointsOfTE;
        }
    }
}

QVector<CurvePoint> ReportGenerator::getPointsAfterStretching(const QVector<CurvePoint> &nomPoints, const QVector<CurvePoint> &measPoints) {
    auto params31 = Function31Params(_reportData->isLEStretch(), _reportData->isTEStretch());
    auto res31 = CurveLibrary::function31(nomPoints, measPoints, params31);

    auto params6 = Function6Params();
    auto res6 = CurveLibrary::function6(res31.curve.points(), measPoints, params6);

    auto params18 = getParams18();
    auto res18 = CurveLibrary::function18(res6.curve.points(), params18);
    auto updatedPoints = getUpdatedPoints(res18);
    return updatedPoints;
}

Function4Params ReportGenerator::getParams4(bool isClosed) {
    auto evaluationPlace = 1;
    auto evaluationDirection = 1;
    if(_reportData->evaluationPlace() == ReportData::Evaluation::Measured) {
        evaluationPlace = 2;
    }
    if(_reportData->evaluationDirection() == ReportData::Evaluation::Measured) {
        evaluationDirection = 2;
    }
    isClosed = false;
    if(_reportData->profileType() == ReportData::Profile::Whole) {
        isClosed = true;
    }
    return Function4Params(0, evaluationPlace, evaluationDirection, isClosed);
}

Function6Params ReportGenerator::getParams6() {
    auto bestFitType = _reportData->bestFitType();

    if(bestFitType == ReportData::BestFitType::OnlyTranslation) {
        return Function6Params(true, Function6Params::Algorithm::Curve, true, true, true, false);
    } else if(bestFitType == ReportData::BestFitType::OnlyRotation) {
        return Function6Params(false, Function6Params::Algorithm::Curve, true, false, false, true);
    } else if(bestFitType == ReportData::BestFitType::OnlyXTranslation) {
        return Function6Params(false, Function6Params::Algorithm::Curve, true, true, false, false);
    } else if(bestFitType == ReportData::BestFitType::OnlyYTranslation) {
        return Function6Params(false, Function6Params::Algorithm::Curve, true, false, true, false);
    } else if(bestFitType == ReportData::BestFitType::XTranslationAndRotation) {
        return Function6Params(true, Function6Params::Algorithm::Curve, true, true, false, true);
    } else if(bestFitType == ReportData::BestFitType::YTranslationAndRotation) {
        return Function6Params(true, Function6Params::Algorithm::Curve, true, false, true, true);
    } else {
        return Function6Params();
    }
}

Function18Params ReportGenerator::getParams18() {
    if(_reportData->dimension() == ReportData::Dimension::Length) {
        auto params18 = Function18Params(0, 1, 1);
        auto chordLength = Algorithms::getChordLength(_nomCurveName, &params18, _project);
        auto zoneLEInPercent = _reportData->zoneLE() / chordLength * 100;
        auto zoneTEInPercent = _reportData->zoneTE() / chordLength * 100;
        return Function18Params(static_cast<int>(_reportData->directionOfLE()), zoneLEInPercent, zoneTEInPercent);
    } else {
        return Function18Params(static_cast<int>(_reportData->directionOfLE()), _reportData->zoneLE(), _reportData->zoneTE());
    }
}

Function21Params ReportGenerator::getParams21() {
    auto bestFitType = _reportData->bestFitType();

    if(bestFitType == ReportData::BestFitType::OnlyTranslation) {
        return Function21Params(0, true, true, false);
    } else if(bestFitType == ReportData::BestFitType::OnlyRotation) {
        return Function21Params(0, false, false, true);
    } else if(bestFitType == ReportData::BestFitType::OnlyXTranslation) {
        return Function21Params(0, true, false, false);
    } else if(bestFitType == ReportData::BestFitType::OnlyYTranslation) {
        return Function21Params(0, false, true, false);
    } else if(bestFitType == ReportData::BestFitType::XTranslationAndRotation) {
        return Function21Params(0, true, false, true);
    } else if(bestFitType == ReportData::BestFitType::YTranslationAndRotation) {
        return Function21Params(0, false, true, true);
    } else {
        return Function21Params();
    }
}

void ReportGenerator::createAditionalFigures(const QVector<CurvePoint> &nominalPoints) {
    if(_reportData->needMaxDiameter() || _reportData->needMCL() || _reportData->needChord()) {
        auto globalCurve = findCurve(_globalCurveName);
        auto params18 = Function18Params(0, 1, 1);
        auto nomRes18 = CurveLibrary::function18(nominalPoints, params18);
        auto measRes18 = CurveLibrary::function18(globalCurve->points(), params18);

        if(_reportData->needMaxDiameter()) {
            auto nomCircle = nomRes18.maxCircle;
            auto measCircle = measRes18.maxCircle;
            _nomMaxDia = new CircleFigure(_nomMaxDiaName, nomCircle.center(), nomCircle.normal(), nomCircle.radius());
            _measMaxDia = new CircleFigure(_measMaxDiaName, measCircle.center(), measCircle.normal(), measCircle.radius());
            _nomMaxDia->setColor(Qt::black);
            _measMaxDia->setColor(Qt::blue);
            _project->safeInsert(_nomMaxDia->name(), _nomMaxDia);
            _project->safeInsert(_measMaxDia->name(), _measMaxDia);
            _isCreateMaxDia = true;
        }
        if(_reportData->needMCL()) {
            auto nomMCL = nomRes18.middleCurve;
            auto measMCL = measRes18.middleCurve;
            _nomMCL = new CurveFigure(_nomMCLName, nomMCL.points());
            _measMCL = new CurveFigure(_measMCLName, measMCL.points());
            _nomMCL->setColor(Qt::black);
            _measMCL->setColor(Qt::blue);
            _project->safeInsert(_nomMCL->name(), _nomMCL);
            _project->safeInsert(_measMCL->name(), _measMCL);
            _isCreateMCL = true;
        }
        if(_reportData->needChord()) {
            auto nomLine = nomRes18.chordLine;
            auto measLine = measRes18.chordLine;
            _nomChord = new LineFigure(_nomChordName, nomLine.origin(), nomLine.direction());
            _measChord = new LineFigure(_measChordName, measLine.origin(), measLine.direction());
            _nomChord->setColor(Qt::black);
            _measChord->setColor(Qt::blue);
            _project->safeInsert(_nomChord->name(), _nomChord);
            _project->safeInsert(_measChord->name(), _measChord);
            _isCreateChord = true;
        }
    }
}

void ReportGenerator::setVisibilityAditionalFigures() {
    QStringList figureNames;
    if(_isCreateMCL) {
        figureNames.append(_nomMCL->name());
        figureNames.append(_measMCL->name());
    }
    if(_isCreateMaxDia) {
        figureNames.append(_nomMaxDia->name());
        figureNames.append(_measMaxDia->name());
    }
    if(_isCreateChord) {
        figureNames.append(_nomChord->name());
        figureNames.append(_measChord->name());
    }
    _project->setVisibility(figureNames);
}

void ReportGenerator::makeScreenshotOfEdges() {
    _measuredRes18 = getResult18WithDeviations();
    if(_reportData->isLEVisible()) {
        makeScreenshotOfEdge(_measuredRes18.curveLE.points(), "LE", _reportData->amplificationOfLE(),
            _reportData->axisTypeOfLE(), _reportData->typeOfShowDevsLE(), Plot::Position::Right);
    }
    if(_reportData->isTEVisible()) {
        makeScreenshotOfEdge(_measuredRes18.curveTE.points(), "TE", _reportData->amplificationOfTE(),
            _reportData->axisTypeOfTE(), _reportData->typeOfShowDevsTE(), Plot::Position::Left);
    }
}

void ReportGenerator::makeScreenshotOfEdge(const QVector<CurvePoint> &points, const QString &edgeName, double amplification,
    ReportData::Axis axisType, ReportData::TypeOfShowDevs devsType, Plot::Position position) {
    _project->resetVisibilityForAllFigures();

    auto curve = new CurveFigure(QString("%1_%2").arg(edgeName).arg(_nomCurveName), points);
    curve->setShowDeviations(true);
    curve->setConnectDeviations(true);
    curve->setAmplification(amplification);

    QVector<CurvePoint> pointsWithInterval;
    if(devsType == ReportData::TypeOfShowDevs::Auto) {
        auto pointsInterval = 4;
        if(points.length() > 50) {
            pointsInterval = points.length() / 12;
        }
        for(auto i = 0; i < points.length(); i += pointsInterval) {
            pointsWithInterval.append(points[i]);
        }
    } else if(devsType == ReportData::TypeOfShowDevs::Set) {
        auto pointsInterval = 0;
        if(edgeName == "LE") {
            pointsInterval = _reportData->valueOfSetShowDevsLE() + 1;
        } else {
            pointsInterval = _reportData->valueOfSetShowDevsTE() + 1;
        }
        for(auto i = 0; i < points.length(); i += pointsInterval) {
            pointsWithInterval.append(points[i]);
        }
    } else if(devsType == ReportData::TypeOfShowDevs::OnEdge) {
        pointsWithInterval.append(points[points.length() / 2]);
    }
    auto curveWithInterval = new CurveFigure(QString("%1_with_interval_%2").arg(edgeName).arg(_nomCurveName), pointsWithInterval);
    curveWithInterval->setConnectPoints(false);
    curveWithInterval->setShowPoints(true);
    curveWithInterval->setShowNumericalDeviations(true);
    _project->insertFigure(curveWithInterval);

    _project->insertFigure(curve);

    _plot->rescaleToCurve(position);
    if(_isCreateMCL) {
        _project->setVisibility({ _nomMCL->name(), _measMCL->name() });
    }
    _plot->makeScreenshot(_screenshotsDir + QString("/%1_%2.png").arg(edgeName).arg(_reportData->reportName()), 320, 200, _reportData->axisTypeOfLE());
    _project->removeFigure(curve->name());
    if(devsType != ReportData::TypeOfShowDevs::FromNominal) {
        _project->removeFigure(curveWithInterval->name());
    }
}

void ReportGenerator::makeScreenshotOfGlobal(const QVector<CurvePoint> &resultPoints, bool needClosed) {
    _project->resetVisibilityForAllFigures();

    auto globalCurve = new CurveFigure(_globalCurveName, resultPoints);
    globalCurve->setClosed(needClosed);
    globalCurve->setShowDeviations(true);
    globalCurve->setConnectDeviations(true);
    globalCurve->setAmplification(_reportData->globalAmplification());

    _project->safeInsert(_globalCurveName, globalCurve);

    _plot->rescaleToCurve(Plot::Position::Center);
    setVisibilityAditionalFigures();
    _plot->makeScreenshot(_screenshotsDir + QString("/global_%1.png").arg(_reportData->reportName()), 600, 350, _reportData->globalAxisType());
    _project->setVisibility({ _nomCurveName });
    _project->setCurrentFigure(_nomCurveName);
}

void ReportGenerator::makeScreenshotOfGlobal(const QVector<CurvePoint> &resultCVPoints, const QVector<CurvePoint> &resultCCPoints) {
    _project->resetVisibilityForAllFigures();

    auto globalCV = new CurveFigure(_globalCVName, resultCVPoints);
    globalCV->setShowDeviations(true);
    globalCV->setConnectDeviations(true);
    globalCV->setAmplification(_reportData->globalAmplification());

    auto globalCC = new CurveFigure(_globalCCName, resultCCPoints);
    globalCC->setShowDeviations(true);
    globalCC->setConnectDeviations(true);
    globalCC->setAmplification(_reportData->globalAmplification());

    _project->safeInsert(_globalCVName, globalCV);
    _project->safeInsert(_globalCCName, globalCC);

    _plot->rescaleToCurve(Plot::Position::Center);
    setVisibilityAditionalFigures();
    _plot->makeScreenshot(_screenshotsDir + QString("/global_%1.png").arg(_reportData->reportName()), 600, 350, _reportData->globalAxisType());
    _project->setVisibility({ _nomCurveName });
    _project->setCurrentFigure(_nomCurveName);
}

void ReportGenerator::deleteCurves() {
    for(auto nameCurve : _curvesToDelete) {
        _project->removeFigure(nameCurve);
    }
}

void ReportGenerator::createMarkup() {
    auto comment = getComment();
    auto globalView = getGlobalView();
    auto parameters = getParameters();
    auto LEView = getLEView();
    auto TEView = getTEView();
    auto partData = getPartData();

    QFile file(_reportPath);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << _index.arg(comment).arg(globalView).arg(parameters).arg(LEView).arg(TEView).arg(partData);
        file.close();
    }
}

QString ReportGenerator::getComment() {
    auto comment = _reportData->comment();
    return QString("<p style=\"width: 90vw; margin: 0; margin:auto;\">%1</p>").arg(comment);
}

QString ReportGenerator::getGlobalView() {
    auto xShift = _reportData->xShift();
    auto yShift = _reportData->yShift();
    auto rotation = _reportData->rotation();
    auto xShiftCV = _reportData->xShiftCV();
    auto yShiftCV = _reportData->yShiftCV();
    auto rotationCV = _reportData->rotationCV();
    auto xShiftCC = _reportData->xShiftCC();
    auto yShiftCC = _reportData->yShiftCC();
    auto rotationCC = _reportData->rotationCC();
    auto profileType = _reportData->profileType();
    auto title = QString("<b>Section %1 - global view</b> Fit: Whole profile (LSQ) / ").arg(_nomCurveName);
    QString bestFitInfo = "";

    if(_reportData->bestFitType() == ReportData::BestFitType::OnlyTranslation) {
        title += "Translation";
        if(profileType == ReportData::Profile::Whole || profileType == ReportData::Profile::WithoutTE || profileType == ReportData::Profile::WithoutEdges) {
            bestFitInfo = QString("Best-fit: X: %1, Y: %2")
                .arg(QString::number(xShift, 'f', 3)).arg(QString::number(yShift, 'f', 3));
        } else {
            bestFitInfo = QString("Best-fit: CV - X: %1, Y: %2 / CC - X: %3, Y: %4")
                .arg(QString::number(xShiftCV, 'f', 3)).arg(QString::number(yShiftCV, 'f', 3))
                .arg(QString::number(xShiftCC, 'f', 3)).arg(QString::number(yShiftCC, 'f', 3));
        }
    } else if(_reportData->bestFitType() == ReportData::BestFitType::OnlyRotation) {
        title += "Rotation";
        if(profileType == ReportData::Profile::Whole || profileType == ReportData::Profile::WithoutTE || profileType == ReportData::Profile::WithoutEdges) {
            bestFitInfo = QString("Best-fit: Rotation: %1").arg(QString::number(rotation, 'f', 3));
        } else {
            bestFitInfo = QString("Best-fit: CV - Rotation: %1 / CC - Rotation: %2")
                .arg(QString::number(rotationCV, 'f', 3)).arg(QString::number(rotationCC, 'f', 3));
        }
    } else if(_reportData->bestFitType() == ReportData::BestFitType::OnlyXTranslation) {
        title += "X Translation";
        if(profileType == ReportData::Profile::Whole || profileType == ReportData::Profile::WithoutTE || profileType == ReportData::Profile::WithoutEdges) {
            bestFitInfo = QString("Best-fit: X: %1").arg(QString::number(xShift, 'f', 3));
        } else {
            bestFitInfo = QString("Best-fit: CV - X: %1 / CC - X: %2").arg(QString::number(xShiftCV, 'f', 3)).arg(QString::number(xShiftCC, 'f', 3));
        }
    } else if(_reportData->bestFitType() == ReportData::BestFitType::OnlyYTranslation) {
        title += "Y Translation";
        if(profileType == ReportData::Profile::Whole || profileType == ReportData::Profile::WithoutTE || profileType == ReportData::Profile::WithoutEdges) {
            bestFitInfo = QString("Best-fit: Y: %1").arg(QString::number(yShift, 'f', 3));
        } else {
            bestFitInfo = QString("Best-fit: CV - Y: %1 / CC - Y: %2").arg(QString::number(yShiftCV, 'f', 3)).arg(QString::number(yShiftCC, 'f', 3));
        }
    } else if(_reportData->bestFitType() == ReportData::BestFitType::XTranslationAndRotation) {
        title += "X Translation and Rotation";
        if(profileType == ReportData::Profile::Whole || profileType == ReportData::Profile::WithoutTE || profileType == ReportData::Profile::WithoutEdges) {
            bestFitInfo = QString("Best-fit: X: %1, Rotation: %2").arg(QString::number(xShift, 'f', 3)).arg(QString::number(rotation, 'f', 3));
        } else {
            bestFitInfo = QString("Best-fit: CV - X: %1, Rotation: %2 / CC - X: %3, Rotation: %4")
                .arg(QString::number(xShiftCV, 'f', 3)).arg(QString::number(rotationCV, 'f', 3))
                .arg(QString::number(xShiftCC, 'f', 3)).arg(QString::number(rotationCC, 'f', 3));
        }
    } else if(_reportData->bestFitType() == ReportData::BestFitType::YTranslationAndRotation) {
        title += "Y Translation and Rotation";
        if(profileType == ReportData::Profile::Whole || profileType == ReportData::Profile::WithoutTE || profileType == ReportData::Profile::WithoutEdges) {
            bestFitInfo = QString("Best-fit: Y: %1, Rotation: %2").arg(QString::number(yShift, 'f', 3)).arg(QString::number(rotation, 'f', 3));
        } else {
            bestFitInfo = QString("Best-fit: CV - Y: %1, Rotation: %2 / CC - Y: %3, Rotation: %4")
                .arg(QString::number(yShiftCV, 'f', 3)).arg(QString::number(rotationCV, 'f', 3))
                .arg(QString::number(yShiftCC, 'f', 3)).arg(QString::number(rotationCC, 'f', 3));
        }
    } else {
        title += "Translation and Rotation";
        if(profileType == ReportData::Profile::Whole || profileType == ReportData::Profile::WithoutTE || profileType == ReportData::Profile::WithoutEdges) {
            bestFitInfo = QString("Best-fit: X: %1, Y: %2, Rotation: %3")
                .arg(QString::number(xShift, 'f', 3)).arg(QString::number(yShift, 'f', 3)).arg(QString::number(rotation, 'f', 3));
        } else {
            bestFitInfo = QString("Best-fit: CV - X: %1, Y: %2, Rotation: %3 / CC - X: %4, Y: %5, Rotation: %6")
                .arg(QString::number(xShiftCV, 'f', 3)).arg(QString::number(yShiftCV, 'f', 3)).arg(QString::number(rotationCV, 'f', 3))
                .arg(QString::number(xShiftCC, 'f', 3)).arg(QString::number(yShiftCC, 'f', 3)).arg(QString::number(rotationCC, 'f', 3));
        }
    }

    title += QString(" / Error amp: %1x").arg(QString::number(_reportData->globalAmplification(), 'f', 0));
    auto convexFormTable = getTable(_measuredRes18.curveHigh.points(), QString("Convex_%1").arg(_nomCurveName));
    auto concaveFormTable = getTable(_measuredRes18.curveLow.points(), QString("Concave_%1").arg(_nomCurveName));

    return QString("<div class = \"global-view\" id=\"rectangle\" style=\"border: 1px solid; grid-column: span 2; margin: 5px 2.5px 2.5px 5px;\">\n \
                       <div id=\"rectangle\" style=\"background-color: orange; width: 100%; height: 10%; text-align: center;\">\n \
                           <p style=\"margin: 0;\">%1</p>\n \
                           <p style=\"margin: 0;\">%2</p>\n \
                       </div>\n \
                       <div style=\"position: absolute; display: flex; flex-direction: column; gap: 20px;\">\n \
                           %3\n \
                           %4\n \
                       </div>\n \
                       <img style=\"width: 100%; max-height: 340px; display: block;\" src=\"Screenshots/global_%5.png\">\n \
                    </div>").arg(title).arg(bestFitInfo).arg(convexFormTable).arg(concaveFormTable).arg(_reportData->reportName());
}

QString ReportGenerator::getParameters() {
    auto maxWidth = _reportData->onMaxWidth() ? getMaxWidth() : "";
    auto xMaxWidth = _reportData->onXMaxWidth() ? getXMaxWidth() : "";
    auto yMaxWidth = _reportData->onYMaxWidth() ? getYMaxWidth() : "";
    auto chordLength = _reportData->onChordLength() ? getChordLength() : "";
    auto LEWidth = _reportData->onLEWidth() ? getLEWidth() : "";
    auto TEWidth = _reportData->onTEWidth() ? getTEWidth() : "";
    auto LERadius = _reportData->onLERadius() ? getLERadius() : "";
    auto TERadius = _reportData->onTERadius() ? getTERadius() : "";

    return QString("<div class=\"parameters\" id=\"rectangle\" style=\"border: 1px solid; margin: 5px 5px 2.5px 2.5px;\">\n \
                       <div id=\"rectangle\" style=\"background-color: orange; width: 100%; height: 10%; display: flex; align-items: center; justify-content: center;\">\n \
                           <p style=\"margin: 0;\"><b>Airfoil parameters</b></p>\n \
                       </div>\n \
                       <div>\n \
                           <table style=\"width: 100%;\">\n \
                               <tr>\n \
                                   <th style=\"font-size: 14px; text-align: center;\">Axis</th>\n \
                                   <th style=\"font-size: 14px; text-align: center;\">Nominal</th>\n \
                                   <th style=\"font-size: 14px; text-align: center;\">Upper tol.</th>\n \
                                   <th style=\"font-size: 14px; text-align: center;\">Lower tol.</th>\n \
                                   <th style=\"font-size: 14px; text-align: center;\">Measured</th>\n \
                                   <th style=\"font-size: 14px; text-align: center;\">Dev.</th>\n \
                                   <th style=\"font-size: 14px; text-align: center;\">OOT</th>\n \
                               </tr>\n \
                                   %1\n \
                                   %2\n \
                                   %3\n \
                                   %4\n \
                                   %5\n \
                                   %6\n \
                                   %7\n \
                                   %8\n \
                           </table>\n \
                         </div>\n \
                     </div>").arg(maxWidth).arg(xMaxWidth).arg(yMaxWidth).arg(chordLength).arg(LEWidth).arg(TEWidth).arg(LERadius).arg(TERadius);
}

QString ReportGenerator::getMaxWidth() {
    auto title = "Maximum diameter";
    auto type = "Diameter";
    QString oot = "<td></td>";

    if(_reportData->nominalMaxWidth() == 0 && _reportData->upTolMaxWidth() == 0 && _reportData->downTolMaxWidth() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg("").arg("").arg("")
            .arg(QString::number(_reportData->measuredMaxWidth(), 'f', 3)).arg("").arg(oot);
    } else if(_reportData->upTolMaxWidth() == 0 && _reportData->downTolMaxWidth() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportData->nominalMaxWidth(), 'f', 3)).arg("").arg("")
            .arg(QString::number(_reportData->measuredMaxWidth(), 'f', 3)).arg(QString::number(_reportData->deviationMaxWidth(), 'f', 3)).arg(oot);
    } else {
        oot = getOOT(_reportData->upTolMaxWidth(), _reportData->downTolMaxWidth(), _reportData->deviationMaxWidth());
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportData->nominalMaxWidth(), 'f', 3))
            .arg(QString::number(_reportData->upTolMaxWidth(), 'f', 3)).arg(QString::number(_reportData->downTolMaxWidth(), 'f', 3))
            .arg(QString::number(_reportData->measuredMaxWidth(), 'f', 3)).arg(QString::number(_reportData->deviationMaxWidth(), 'f', 3)).arg(oot);
    }

}

QString ReportGenerator::getXMaxWidth() {
    auto title = "XDiameter";
    auto type = "X";
    QString oot = "<td></td>";

    if(_reportData->nominalXMaxWidth() == 0 && _reportData->upTolXMaxWidth() == 0 && _reportData->downTolXMaxWidth() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg("").arg("").arg("")
            .arg(QString::number(_reportData->measuredXMaxWidth(), 'f', 3)).arg("").arg(oot);
    } else if(_reportData->upTolXMaxWidth() == 0 && _reportData->downTolXMaxWidth() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportData->nominalXMaxWidth(), 'f', 3)).arg("").arg("")
            .arg(QString::number(_reportData->measuredXMaxWidth(), 'f', 3)).arg(QString::number(_reportData->deviationXMaxWidth(), 'f', 3)).arg(oot);
    } else {
        auto oot = getOOT(_reportData->upTolXMaxWidth(), _reportData->downTolXMaxWidth(), _reportData->deviationXMaxWidth());
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportData->nominalXMaxWidth(), 'f', 3))
            .arg(QString::number(_reportData->upTolXMaxWidth(), 'f', 3)).arg(QString::number(_reportData->downTolXMaxWidth(), 'f', 3))
            .arg(QString::number(_reportData->measuredXMaxWidth(), 'f', 3)).arg(QString::number(_reportData->deviationXMaxWidth(), 'f', 3)).arg(oot);
    }
}

QString ReportGenerator::getYMaxWidth() {
    auto title = "YDiameter";
    auto type = "Y";
    QString oot = "<td></td>";

    if(_reportData->nominalYMaxWidth() == 0 && _reportData->upTolYMaxWidth() == 0 && _reportData->downTolYMaxWidth() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg("").arg("").arg("")
            .arg(QString::number(_reportData->measuredYMaxWidth(), 'f', 3)).arg("").arg(oot);
    } else if(_reportData->upTolYMaxWidth() == 0 && _reportData->downTolYMaxWidth() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportData->nominalYMaxWidth(), 'f', 3)).arg("").arg("")
            .arg(QString::number(_reportData->measuredYMaxWidth(), 'f', 3)).arg(QString::number(_reportData->deviationYMaxWidth(), 'f', 3)).arg(oot);
    } else {
        auto oot = getOOT(_reportData->upTolYMaxWidth(), _reportData->downTolYMaxWidth(), _reportData->deviationYMaxWidth());
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportData->nominalYMaxWidth(), 'f', 3))
            .arg(QString::number(_reportData->upTolYMaxWidth(), 'f', 3)).arg(QString::number(_reportData->downTolYMaxWidth(), 'f', 3))
            .arg(QString::number(_reportData->measuredYMaxWidth(), 'f', 3)).arg(QString::number(_reportData->deviationYMaxWidth(), 'f', 3)).arg(oot);
    }
}

QString ReportGenerator::getChordLength() {
    auto title = "Chord length";
    auto type = "Distance 2D";
    QString oot = "<td></td>";

    if(_reportData->nominalChordLength() == 0 && _reportData->upTolChordLength() == 0 && _reportData->downTolChordLength() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg("").arg("").arg("")
            .arg(QString::number(_reportData->measuredChordLength(), 'f', 3)).arg("").arg(oot);
    } else if(_reportData->upTolChordLength() == 0 && _reportData->downTolChordLength() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportData->nominalChordLength(), 'f', 3)).arg("").arg("")
            .arg(QString::number(_reportData->measuredChordLength(), 'f', 3)).arg(QString::number(_reportData->deviationMaxWidth(), 'f', 3)).arg(oot);
    } else {
        auto oot = getOOT(_reportData->upTolChordLength(), _reportData->downTolChordLength(), _reportData->deviationMaxWidth());
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportData->nominalChordLength(), 'f', 3))
            .arg(QString::number(_reportData->upTolChordLength(), 'f', 3)).arg(QString::number(_reportData->downTolChordLength(), 'f', 3))
            .arg(QString::number(_reportData->measuredChordLength(), 'f', 3)).arg(QString::number(_reportData->deviationMaxWidth(), 'f', 3)).arg(oot);
    }
}

QString ReportGenerator::getLEWidth() {
    auto title = "LE width";
    auto type = "Distance 2D";
    QString oot = "<td></td>";

    if(_reportData->nominalLEWidth() == 0 && _reportData->upTolLEWidth() == 0 && _reportData->downTolLEWidth() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg("").arg("").arg("")
            .arg(QString::number(_reportData->measuredLEWidth(), 'f', 3)).arg("").arg(oot);
    } else if(_reportData->upTolLEWidth() == 0 && _reportData->downTolLEWidth() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportData->nominalLEWidth(), 'f', 3)).arg("").arg("")
            .arg(QString::number(_reportData->measuredLEWidth(), 'f', 3)).arg(QString::number(_reportData->deviationLEWidth(), 'f', 3)).arg(oot);
    } else {
        auto oot = getOOT(_reportData->upTolLEWidth(), _reportData->downTolLEWidth(), _reportData->deviationLEWidth());
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportData->nominalLEWidth(), 'f', 3))
            .arg(QString::number(_reportData->upTolLEWidth(), 'f', 3)).arg(QString::number(_reportData->downTolLEWidth(), 'f', 3))
            .arg(QString::number(_reportData->measuredLEWidth(), 'f', 3)).arg(QString::number(_reportData->deviationLEWidth(), 'f', 3)).arg(oot);
    }
}

QString ReportGenerator::getTEWidth() {
    auto title = "TE width";
    auto type = "Distance 2D";
    QString oot = "<td></td>";

    if(_reportData->nominalTEWidth() == 0 && _reportData->upTolTEWidth() == 0 && _reportData->downTolTEWidth() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg("").arg("").arg("")
            .arg(QString::number(_reportData->measuredTEWidth(), 'f', 3)).arg("").arg(oot);
    } else if(_reportData->upTolTEWidth() == 0 && _reportData->downTolTEWidth() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportData->nominalTEWidth(), 'f', 3)).arg("").arg("")
            .arg(QString::number(_reportData->measuredTEWidth(), 'f', 3)).arg(QString::number(_reportData->deviationTEWidth(), 'f', 3)).arg(oot);
    } else {
        auto oot = getOOT(_reportData->upTolTEWidth(), _reportData->downTolTEWidth(), _reportData->deviationTEWidth());
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportData->nominalTEWidth(), 'f', 3))
            .arg(QString::number(_reportData->upTolTEWidth(), 'f', 3)).arg(QString::number(_reportData->downTolTEWidth(), 'f', 3))
            .arg(QString::number(_reportData->measuredTEWidth(), 'f', 3)).arg(QString::number(_reportData->deviationTEWidth(), 'f', 3)).arg(oot);
    }
}

QString ReportGenerator::getLERadius() {
    auto title = "LE radius";
    auto type = "Radius";
    QString oot = "<td></td>";

    if(_reportData->nominalLERadius() == 0 && _reportData->upTolLERadius() == 0 && _reportData->downTolLERadius() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg("").arg("").arg("")
            .arg(QString::number(_reportData->measuredLERadius(), 'f', 3)).arg("").arg(oot);
    } else if(_reportData->upTolLERadius() == 0 && _reportData->downTolLERadius() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportData->nominalLERadius(), 'f', 3)).arg("").arg("")
            .arg(QString::number(_reportData->measuredLERadius(), 'f', 3)).arg(QString::number(_reportData->deviationLERadius(), 'f', 3)).arg(oot);
    } else {
        auto oot = getOOT(_reportData->upTolLERadius(), _reportData->downTolLERadius(), _reportData->deviationLERadius());
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportData->nominalLERadius(), 'f', 3))
            .arg(QString::number(_reportData->upTolLERadius(), 'f', 3)).arg(QString::number(_reportData->downTolLERadius(), 'f', 3))
            .arg(QString::number(_reportData->measuredLERadius(), 'f', 3)).arg(QString::number(_reportData->deviationLERadius(), 'f', 3)).arg(oot);
    }
}

QString ReportGenerator::getTERadius() {
    auto title = "TE radius";
    auto type = "Radius";
    QString oot = "<td></td>";

    if(_reportData->nominalTERadius() == 0 && _reportData->upTolTERadius() == 0 && _reportData->downTolTERadius() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg("").arg("").arg("")
            .arg(QString::number(_reportData->measuredTERadius(), 'f', 3)).arg("").arg(oot);
    } else if(_reportData->upTolTERadius() == 0 && _reportData->downTolTERadius() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportData->nominalTERadius(), 'f', 3)).arg("").arg("")
            .arg(QString::number(_reportData->measuredTERadius(), 'f', 3)).arg(QString::number(_reportData->deviationTERadius(), 'f', 3)).arg(oot);
    } else {
        auto oot = getOOT(_reportData->upTolTERadius(), _reportData->downTolTERadius(), _reportData->deviationTERadius());
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportData->nominalTERadius(), 'f', 3))
            .arg(QString::number(_reportData->upTolTERadius(), 'f', 3)).arg(QString::number(_reportData->downTolTERadius(), 'f', 3))
            .arg(QString::number(_reportData->measuredTERadius(), 'f', 3)).arg(QString::number(_reportData->deviationTERadius(), 'f', 3)).arg(oot);
    }
}

QString ReportGenerator::getOOT(double upperTolerance, double downTolerance, double deviation) {
    auto difference = 0.0;
    if(deviation >= downTolerance && deviation <= upperTolerance) {
        auto step = (upperTolerance - downTolerance) / 10;
        return QString("<td><input type=\"range\" min=\"%1\" max=\"%2\" step=\"%3\" value=\"%4\" \
            style=\"width: 40px; height: 4px; accent-color: green; pointer-events: none; vertical-align:middle;\"></td>")
            .arg(downTolerance).arg(upperTolerance).arg(step).arg(deviation);
    } else {
        if(deviation < downTolerance) {
            difference = deviation - downTolerance;
        } else {
            difference = deviation - upperTolerance;
        }
        return QString("<td style=\"font-size: 14px; text-align: center; color: red;\">%1</td>\n")
            .arg(QString::number(difference, 'f', 3));
    }
}

QString ReportGenerator::getLEView() {
    QString title = "<p style=\"margin: 0;\"><b>LE - local view</b> Fit: No fit / Error amp: %1x</p>";
    title = title.arg(QString::number(_reportData->amplificationOfLE(), 'f', 0));
    auto table = getTable(_measuredRes18.curveLE.points(), QString("LE_%1").arg(_nomCurveName));
    auto img = QString("<img style=\"width: 100%; height: 90%; display: block;\" src=\"Screenshots/LE_%1.png\">").arg(_reportData->reportName());
    auto result = QString("<div class=\"LE-view\" id=\"rectangle\" style=\"border: 1px solid; margin: 2.5px 2.5px 5px 5px;\">\n \
                       <div id=\"rectangle\" style=\"background-color: orange; width: 100%; height: 10%; display: flex; align-items: center; justify-content: center;\">\n \
                           %1\n \
                       </div>\n \
                       <div style=\"position: absolute;\">\n \
                            %2\n \
                        </div>\n \
                       %3\n \
                    </div>");
    if(_reportData->isLEVisible()) {
        return result.arg(title).arg(table).arg(img);
    } else {
        return result.arg("").arg("").arg("");
    }
}

QString ReportGenerator::getTEView() {
    QString title = "<p style=\"margin: 0;\"><b>TE - local view</b> Fit: No fit / Error amp: %1x</p>";
    title = title.arg(QString::number(_reportData->amplificationOfTE(), 'f', 0));
    auto table = getTable(_measuredRes18.curveTE.points(), QString("TE_%1").arg(_nomCurveName), "style=\"position: absolute\"");
    auto img = QString("<img style=\"width: 100%; height: 90%; display: block;\" src=\"Screenshots/TE_%1.png\">").arg(_reportData->reportName());
    auto result = QString("<div class=\"TE-view\" id=\"rectangle\" style=\"border: 1px solid; margin: 2.5px 2.5px 5px 2.5px;\">\n \
                        <div id=\"rectangle\" style=\"background-color: orange; width: 100%; height: 10%; display: flex; align-items: center; justify-content: center;\">\n \
                            %1\n \
                        </div>\n \
                        <div style=\"position: relative; display: flex; justify-content: right;\">\n \
                            %2\n \
                        </div>\n \
                        %3\n \
                    </div>");
    if(_reportData->isTEVisible()) {
        return result.arg(title).arg(table).arg(img);
    } else {
        return result.arg("").arg("").arg("");
    }
}

QString ReportGenerator::getPartData() {
    auto time = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss");
    return QString("<div class=\"part-data\" id=\"rectangle\" style=\"margin: 2.5px 5px 5px 2.5px; display: grid; grid-template-rows: 0.6fr 0.4fr; gap: 5px;\">\n \
                        <div id=\"rectangle\" style=\"border: 1px solid;\">\n \
                            <div class=\"header\" id=\"rectangle\" style=\"background-color: orange; width: 100%; height: 18%; display: flex; align-items: center; justify-content: center;\">\n \
                                <p style=\"margin: 0;\"><b>Part data</b></p>\n \
                            </div>\n \
                            <div class=\"body\" style=\"width: 100%; height: 82%; display: grid; grid-template-columns: 1fr 1fr; grid-template-rows: 1fr 1fr 1fr;\">\n \
                                <div style=\"align-content: center; margin-left: 5%;\"><b>Description:</b></div>\n \
                                <div style=\"align-content: center; margin-left: 5%;\"><b>Part number:</b></div>\n \
                                <div style=\"align-content: center; margin-left: 5%;\"><b>Drawing:</b></div>\n \
                                <div style=\"align-content: center; margin-left: 5%;\"><b>Operator:</b></div>\n \
                                <div style=\"align-content: center; margin-left: 5%;\"><b>Order number:</b></div>\n \
                                <div style=\"align-content: center; margin-left: 5%;\"><b>Note:</b></div>\n \
                            </div>\n \
                        </div>\n \
                        <div id=\"rectangle\" style=\"border: 1px solid; display: flex; justify-content: center; align-items: center;\">\n \
                            <p style=\"margin: 0;\"><b>Time: %1</b></p>\n \
                        </div>\n \
                    </div>").arg(time);
}

QString ReportGenerator::getTable(const QVector<CurvePoint> &points, const QString &caption, const QString &style) {
    auto typesOfTableValue = _reportData->typesOfTableValue();
    QVector<double> deviations;
    for(auto &point : points) {
        deviations.append(point.dev);
    }
    auto result = QString(
        "<table %1>\n \
            <caption style=\"font-size: 14px;\"><b>%2</b></caption>\n \
            %3\n \
            %4\n \
            %5\n \
            %6\n \
            %7\n \
            %8\n \
            %9\n \
        </table>");
    auto tableRowTemplate = QString(
        "<tr>\n \
            <td style=\"text-align: center; font-size: 10px;\">%1:</td>\n \
            <td style=\"text-align: center; font-size: 10px;\">%2</td>\n \
        </tr>");
    QString minMaxPart = "";
    QString formPart = "";
    QString minPart = "";
    QString maxPart = "";
    QString maxAbsPart = "";
    QString supUTPart = "";
    QString infLTPart = "";

    for(auto [type, isNeed] : typesOfTableValue.asKeyValueRange()) {
        if(type == ReportData::TypeOfTableValue::MinMax && isNeed) {
            auto min = *std::min_element(deviations.begin(), deviations.end());
            auto max = *std::max_element(deviations.begin(), deviations.end());
            auto minMax = abs(min) + abs(max);
            minMaxPart = tableRowTemplate.arg("MinMax").arg(QString::number(minMax, 'f', 3));
        } else if(type == ReportData::TypeOfTableValue::Form && isNeed) {
            auto min = *std::min_element(deviations.begin(), deviations.end());
            auto max = *std::max_element(deviations.begin(), deviations.end());
            auto form = std::max(abs(min), abs(max)) * 2;
            formPart = tableRowTemplate.arg("Form").arg(QString::number(form, 'f', 3));
        } else if(type == ReportData::TypeOfTableValue::Min && isNeed) {
            auto min = *std::min_element(deviations.begin(), deviations.end());
            minPart = tableRowTemplate.arg("Min").arg(QString::number(min, 'f', 3));
        } else if(type == ReportData::TypeOfTableValue::Max && isNeed) {
            auto max = *std::max_element(deviations.begin(), deviations.end());
            maxPart = tableRowTemplate.arg("Max").arg(QString::number(max, 'f', 3));
        } else if(type == ReportData::TypeOfTableValue::MaxAbs && isNeed) {
            auto min = *std::min_element(deviations.begin(), deviations.end());
            auto max = *std::max_element(deviations.begin(), deviations.end());
            auto maxAbs = std::max(abs(min), abs(max));
            maxAbsPart = tableRowTemplate.arg("MaxAbs").arg(QString::number(maxAbs, 'f', 3));
        } else if(type == ReportData::TypeOfTableValue::SupUT && isNeed) {
            auto supUT = *std::max_element(deviations.begin(), deviations.end());
            supUTPart = tableRowTemplate.arg("SupUT").arg(QString::number(supUT, 'f', 3));
        } else if(type == ReportData::TypeOfTableValue::InfLT && isNeed) {
            auto infLT = *std::min_element(deviations.begin(), deviations.end());
            infLTPart = tableRowTemplate.arg("InfLT").arg(QString::number(infLT, 'f', 3));
        }
    }
    return result.arg(style).arg(caption).arg(minMaxPart).arg(formPart).arg(minPart).arg(maxPart).arg(maxAbsPart).arg(supUTPart).arg(infLTPart);
}

ReportGenerator::~ReportGenerator() {
}
