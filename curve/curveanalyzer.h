#pragma once

#include "functionparamsgenerator.h"
#include "figurecreator.h"
#include "screenshotcreator.h"
#include "project.h"
#include "reportgenerator.h"

using CurveType = FigureCreator::CurveType;
using GlobalCurveMap = QMap<CurveType, QPair<QString, QVector<CurvePoint>>>;

class CurveAnalyzer {
public:
    struct CurveParts {
        QVector<CurvePoint> pointsOfLE;
        QVector<CurvePoint> pointsOfTE;
        QVector<CurvePoint> pointsOfHigh;
        QVector<CurvePoint> pointsOfLow;
    };

    explicit CurveAnalyzer(Project *project, std::shared_ptr<ReportSettings> reportSettings);
    virtual ~CurveAnalyzer() = default;

    GlobalCurveMap run();

private:
    Project *_project;
    std::shared_ptr<ReportSettings> _reportSettings;
    CurveParts _curveParts;
    QSet<QString> _curvesToDelete;

    QString _nominalName;
    QString _measuredName;

    QString _dummyNomName;
    QString _dummyMeasName;
    QString _dummyNomCVName;
    QString _dummyNomCCName;
    QString _dummyMeasCVName;
    QString _dummyMeasCCName;

    QString _globalName;
    QString _globalCVName;
    QString _globalCCName;
    QString _globalLEName;
    QString _globalTEName;

    void initialization();

    QPair<CurveParts, CurveParts> analyzeProfile(const Function18Params *params18);
    GlobalCurveMap analyzeWholeProfile(const Function18Params *params18);
    GlobalCurveMap analyzeProfileWithoutTE(const Function18Params *params18);
    GlobalCurveMap analyzeProfileWithoutEdges(const Function18Params *params18);
    GlobalCurveMap analyzeProfileWithoutEdgesLSQ(const CurveParts &nominalParts, const CurveParts &measuredParts, const Function18Params *params18);
    GlobalCurveMap analyzeProfileWithoutEdgesForm(const CurveParts &nominalParts, const CurveParts &measuredParts, const Function18Params *params18);

    void calculatePreprocessingFunctions(const QString &updatedNomName, const QString &updatedMeasName);
    CurveParts getCurvePartsAfterStretching(const QString &nominalName, const QString &measuredName, const Function18Params &params18);
    template <class T> void calculateBestFit(const QString &updateNomName, const QString &updateMeasName, const T &params);
    void calculateBestFitWithAlignment(const QString &updateNomName, const QString &updateMeasName, const Function6Params &params6, const CurveParts &nominalParts, CurveParts *measuredParts);

    CurveParts alignCurveParts(const CurveParts &curveParts, const Function18Params &params18);

    void insertCurveInProject(const QString &curveName, const QVector<CurvePoint> &points, bool needToDelete = false);
    const CurveFigure* getCurveFromProject(const QString &curveName);
    void deleteDummyCurves();
};
