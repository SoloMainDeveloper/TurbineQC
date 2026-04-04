#pragma once

#include "figure.h"
#include "figurecreator.h"
#include "functionparamsfactory.h"
#include "reportsettings.h"

using GlobalCurveMap = QMap<FigureCreator::CurveType, QPair<QString, QVector<CurvePoint>>>;

class Project;
// class FunctionParamsFactory;

class CurveAnalyzer {
public:
    struct CurveParts {
        QVector<CurvePoint> pointsOfLE;
        QVector<CurvePoint> pointsOfTE;
        QVector<CurvePoint> pointsOfHigh;
        QVector<CurvePoint> pointsOfLow;
    };

    explicit CurveAnalyzer(std::shared_ptr<ReportSettings> reportSettings);
    virtual ~CurveAnalyzer() = default;

    GlobalCurveMap run();

private:
    Project* _project;
    std::shared_ptr<ReportSettings> _reportSettings;

    std::unique_ptr<FunctionParamsFactory> _paramsFactory;

    CurveParts _curveParts;
    QSet<QString> _curvesToDelete;

    QString _nominalName;
    QString _measuredName;

    QString _dummyNominalName;
    QString _dummyMeasuredName;

    QString _dummyNominalCVName;
    QString _dummyMeasuredCVName;
    QString _dummyNominalCCName;
    QString _dummyMeasuredCCName;

    QString _dummyNominalLEName;
    QString _dummyMeasuredLEName;
    QString _dummyNominalTEName;
    QString _dummyMeasuredTEName;

    QString _globalName;
    QString _globalCVName;
    QString _globalCCName;
    QString _globalLEName;
    QString _globalTEName;

    QPair<CurveParts, CurveParts> analyzeProfile(const Function18Params& params18);
    GlobalCurveMap analyzeWholeProfile(const Function18Params& params18);
    GlobalCurveMap analyzeProfileWithoutTE(const Function18Params& params18);
    GlobalCurveMap analyzeProfileWithoutEdges(const CurveParts& nominalParts,
        const CurveParts& measuredParts, const Function18Params& params18);
    GlobalCurveMap analyzeProfileWithoutEdgesLSQ(const CurveParts& nominalParts,
        const CurveParts& measuredParts, const Function18Params& params18);
    GlobalCurveMap analyzeProfileWithoutEdgesForm(const CurveParts& nominalParts,
        const CurveParts& measuredParts, const Function18Params& params18);

    void calculateEdgeBestFit(
        const QString& nominalName, const QList<CurvePoint>& nominalPoints,
        const QString& measuredName, const QList<CurvePoint>& measuredPoints,
        const QString& lineBestFitName, ReportSettings::EdgeBestFit bestFit);

    const CurveFigure* calculateEdgeDeviations(const Function18Params& params18, const QString& nominalName, const QString& measuredName,
        const QString& globalName, const QList<CurvePoint>& resultGlobalPoints, ReportSettings::EdgeBestFit bestFit);

    void calculatePreprocessingFunctions(const QString& updatedNomName, const QString& updatedMeasName);
    CurveParts getCurvePartsAfterStretching(const QString& nominalName, const QString& measuredName, const Function18Params& params18);
    template<class T>
    void calculateBestFit(const QString& updateNomName, const QString& updateMeasName, const T& params);
    void calculateBestFitWithAlignment(const QString& updateNomName, const QString& updateMeasName, const Function6Params& params6, const CurveParts& nominalParts, CurveParts* measuredParts);

    CurveParts alignCurveParts(const CurveParts& curveParts, const Function18Params& params18);

    void insertCurveInProject(const QString& curveName, const QVector<CurvePoint>& points, bool needToDelete = false);
    const CurveFigure* getCurveFromProject(const QString& curveName);
    void deleteDummyCurves();
};
