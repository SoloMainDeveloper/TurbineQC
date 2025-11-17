#pragma once

#include "curve/figure.h"
#include "curve/project.h"

class Plot : public QCustomPlot {
    Q_OBJECT

public:
    explicit Plot(QWidget *parent = nullptr);

    void setProject(Project *_project);

public slots:
    void addFigure(Figure *figure);
    void renameFigure(const QString figureName, const QString newName);
    void removeFigure(const QString figureName);
    void changeFigureVisibility(const QString figureName, bool visible);
    void changeCurveParameters(const QString curveName, bool showPoints, bool connectPoints,
        bool showVectors, bool closed, bool showNumbering, int numberingInterval);

private:
    class Curve : public QCPCurve {
    public:
        Curve(QCPAxis *keyAxis, QCPAxis *valueAxis);
        void connectPoints();
        void showPoints();

    private:
        const int _scatterSize = 3;
        const int _scatterSkip = 0;
    };

    Project *_project;

    void createFigure(const Figure *figure);
    void updateFigure(const QString &figureName);
    void deleteFigure(const QString &figureName);

    void addCurveLayer(const CurveFigure &curveFigure);
    void addPointLayer(const PointFigure &pointFigure);
    void addLineLayer(const LineFigure &lineFigure);
    void addCircleLayer(const CircleFigure &circleFigure);
    void calculateCircleBox(const CircleFigure &circleFigure, QCPItemEllipse *circle);

    const double _penWidth = 1;
    const double _selectedObjectPenWidth = 1.5;
    const double _pointRadius = 5;
};
