#pragma once

#include <QObject>
#include "functionparamsgenerator.h"
#include "reportsettings.h"
#include "project.h"

class FigureCreator {
public:
    enum class CurveType {
        WholeGlobal,
        GlobalWithoutTE,
        GlobalCV,
        GlobalCC,
        GlobalLE,
        GlobalTE,
    };

    explicit FigureCreator(Project *project, std::shared_ptr<ReportSettings> reportSettings);
    virtual ~FigureCreator() = default;

    void run(const QMap<CurveType, QPair<QString, QVector<CurvePoint>>> &globalCurvesToCreate);
    void createAdditionalFigures(const QString &preparedMeasName);
    void alignAdditionalFigures();

private:
    Project *_project;
    std::shared_ptr<ReportSettings> _reportSettings;

    CurveFigure* createGlobalCurve(const QString &globalName, const QVector<CurvePoint> &globalPoints, bool isClosed);
    CurveFigure* createGlobalPart(const QString &curveName, const QVector<CurvePoint> &points);
    CurveFigure* createGlobalEdge(const QString &edgeName, const QVector<CurvePoint> &points, double amplification);
    void createDimension(const QString &dimensionName, const CurveFigure *globalCurve, const Point &labelPoint = Point(0, 0));
};
