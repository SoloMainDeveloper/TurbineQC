#pragma once

#include "plot.h"
#include "project.h"
#include "curvelibrary.h"
#include "functionparams.h"
#include "reportdata.h"
#include "algorithms.h"

class ReportData;

class ReportGenerator {
public:
    explicit ReportGenerator(Project *project, Plot *plot, ReportData *reportData, const QString &filePath);
    virtual ~ReportGenerator();

    void createReport(const CurveFigure *nominalCurve, const CurveFigure *measuredCurve);

private:
    Project *_project;
    Plot *_plot;
    ReportData *_reportData;

    CircleFigure *_nomMaxDia;
    CircleFigure *_measMaxDia;
    CurveFigure  *_nomMCL;
    CurveFigure  *_measMCL;
    LineFigure *_nomChord;
    LineFigure *_measChord;

    Function18Result _nominalRes18;
    Function18Result _measuredRes18;

    QVector<QString> _curvesToDelete;

    QString _index;
    QString _tableRowTemplate;
    QString _screenshotsDir;
    QString _reportPath;
    QString _globalCurveName;
    QString _globalCVName;
    QString _globalCCName;
    QString _nomCurveName;
    QString _measCurveName;
    QString _updatedNomCurveName;
    QString _updatedMeasCurveName;
    QString _nomMaxDiaName;
    QString _measMaxDiaName;
    QString _nomMCLName;
    QString _measMCLName;
    QString _nomChordName;
    QString _measChordName;

    bool _isCreateMaxDia;
    bool _isCreateMCL;
    bool _isCreateChord;

    void makeDirectories();
    void initialization(const QString &nomCurveName, const QString &measCurveName);
    void showWholeProfile(const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &measuredPoints);
    void showProfileWithoutEdges(const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &measuredPoints);
    void showProfileWithoutTE(const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &measuredPoints);
    void showProfileWithoutEdgesLSQ(const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &measuredPoints);
    void showProfileWithoutEdgesForm(const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &measuredPoints);
    Function18Result getResult18WithDeviations();
    QVector<CurvePoint> createUpdatedMeasuredCurve(const QVector<CurvePoint> &nominalPoints, const QVector<CurvePoint> &measuredPoints);
    QVector<CurvePoint> getPointsAfterPreProcess(const QVector<CurvePoint> &points);
    QVector<CurvePoint> getUpdatedPoints(Function18Result &res18, bool needWithoutTE = false);
    QVector<CurvePoint> getPointsAfterStretching(const QVector<CurvePoint> &nomPoints, const QVector<CurvePoint> &measPoints);
    Function4Params getParams4(bool isClosed = false);
    Function6Params getParams6();
    Function18Params getParams18();
    Function21Params getParams21();
    void insertCurveInProject(const QString &curveName, const QVector<CurvePoint> &points, bool needDelete = false);
    const CurveFigure* findCurve(const QString &curveName);
    void createAditionalFigures(const QVector<CurvePoint> &nominalPoints);
    void setVisibilityAditionalFigures();
    void makeScreenshotOfEdges();
    void makeScreenshotOfEdge(const QVector<CurvePoint> &points, const QString &edgeName, double amplification, 
        ReportData::Axis axisType, ReportData::TypeOfShowDevs devsType, Plot::Position position);
    void makeScreenshotOfGlobal(const QVector<CurvePoint> &resultPoints, bool needClosed = false);
    void makeScreenshotOfGlobal(const QVector<CurvePoint> &resultCVPoints, const QVector<CurvePoint> &resultCCPoints);
    void deleteCurves();

    void createMarkup();
    QString getComment();
    QString getGlobalView();
    QString getParameters();
    QString getMaxWidth();
    QString getXMaxWidth();
    QString getYMaxWidth();
    QString getChordLength();
    QString getLEWidth();
    QString getTEWidth();
    QString getLERadius();
    QString getTERadius();
    QString getOOT(double upperTolerance, double downTolerance, double deviation);
    QString getLEView();
    QString getTEView();
    QString getTable(const QVector<CurvePoint> &points, const QString &caption, const QString &style = "");
    QString getPartData();
};
