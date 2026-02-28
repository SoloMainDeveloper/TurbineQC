#pragma once

#include "plot.h"
#include "project.h"
#include "curvelibrary.h"
#include "functionparams.h"
#include "reportsettings.h"
#include "algorithms.h"
#include "creatingmarkup.h"

class ReportSettings;

class ReportGenerator {
public:
    enum class EdgeType {
        LE,
        TE,
    };

    static void createReport(Project *project, Plot *plot, std::shared_ptr<ReportSettings> reportSettings);

private:
    static Project *_project;
    static std::shared_ptr<ReportSettings> _reportSettings;
    static CurveParts _curveParts;
    static QVector<QString> _curvesToDelete;

    static QString _nominalCurveName;
    static QString _measuredCurveName;
    static QString _globalCurveName;
    static QString _globalCVName;
    static QString _globalCCName;
    static QString _nominalMaxDiaName;
    static QString _measuredMaxDiaName;
    static QString _nominalMCLName;
    static QString _measuredMCLName;
    static QString _nominalContactLineName;
    static QString _measuredContactLineName;

    static void initialization(Project *project, Plot *plot, std::shared_ptr<ReportSettings> reportSettings);
    static void analyzeProfile(Plot *plot, const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &measuredPoints);
    static QVector<CurvePoint> getReassembledPoints(const Function18Result &res18);
    static QVector<CurvePoint> getReassembledPointsOfWholeCurve(const Function18Result &res18);
    static QVector<CurvePoint> getReassembledPointsWithoutTE(const Function18Result &res18);
    static QVector<CurvePoint> getReassembledPointsWithoutEdges(const Function18Result &res18);
    static QVector<CurvePoint> getUpdatedMeasuredPoints(const QVector<CurvePoint> &updatedNomPoints, const QVector<CurvePoint> &measuredPoints, const Function18Params &params18);
    static QVector<CurvePoint> getPointsAfterStretching(const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &measuredPoints, const Function18Params &params18);
    static QVector<CurvePoint> getPointsAfterPreProcess(const QVector<CurvePoint> &points);
    static QVector<CurvePoint> getPointsAfterBestFit(const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &updatedMeasPoints);
    static void analyzeWholeProfile(Plot *plot, const QVector<CurvePoint> &updatedNomPoints, const QVector<CurvePoint> &globalPointsAfterBestFit, const Function18Result &globalRes18);
    static void analyzeProfileWithoutTE(Plot *plot, const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &globalPointsAfterBestFit, const Function18Result &globalRes18);
    static void analyzeProfileWithoutEdges(Plot *plot, const Function18Result &globalRes18);
    static void analyzeProfileWithoutEdgesLSQ(Plot *plot, const Function18Result &nominalRes18, const Function18Result &updatedMeasRes18, const Function18Result &globalRes18);
    static void analyzeProfileWithoutEdgesForm(Plot *plot, const Function18Result &nominalRes18, const Function18Result &updatedMeasRes18, const Function18Result &globalRes18);
    static Function18Result getResult18(const QVector<CurvePoint> &points, const Function18Params &params18);
    static Function18Result getResult18WithDevs(const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &globalPoints, const Function18Params &params18);
    static const Function4Params getParams4(bool isClosed = false);
    static const Function6Params getParams6();
    static const Function18Params getParams18();
    static const Function21Params getParams21();
    static void insertCurveInProject(const QString &curveName, const QVector<CurvePoint> &points, bool needDelete = false);
    static const CurveFigure* findCurve(const QString &curveName);
    static void createAditionalFigures(const QString &nominalCurveName, const QString &globalCurveNameAfterBf);
    static void setVisibilityAdditionalFigures();
    static void createEdgesAndMakeScreenshots(Plot *plot, const QVector<CurvePoint> &pointsOfLE, const QVector<CurvePoint> &pointsOfTE);
    static void createEdge(const QVector<CurvePoint> &points, double amplification, EdgeType edgeType, ReportSettings::TypeOfShowDevs devsType);
    static void createNumericalDeviations(const QVector<CurvePoint> &points, ReportSettings::TypeOfShowDevs devsType, EdgeType edgeType);
    static void makeScreenshotOfEdge(Plot *plot, EdgeType edgeType, ReportSettings::Axis axisType, Plot::Position position);
    static void createGlobalCurve(const QVector<CurvePoint> &globalPoints, bool isClosed = false);
    static void createPartsOfCurve(const CurveParts &curveParts, bool isResetVisibilityForAllFigures = false);
    static void addTable(const QString &tableName, const CurveFigure *globalCurve, const Point &labelPoint = Point(0, 0));
    static void makeScreenshotOfGlobal(Plot *plot);
    static void deleteCurves();
};
