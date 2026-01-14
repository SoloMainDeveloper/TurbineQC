#pragma once

#include "curve/figure.h"
#include "curve/project.h"

class Plot : public QCustomPlot {
    Q_OBJECT

public:
    explicit Plot(QWidget *parent = nullptr);

    void setProject(Project *project);

public slots:
    void addFigure(Figure *figure);
    void renameFigure(const QString figureName, const QString newName);
    void removeFigure(const QString figureName);
    void changeCurrentFigure(const QString &currentFigureName, const QString &previousFigureName);
    void changeFigureVisibility(const QString figureName, bool visible);
    void changeCurveParameters(const QString curveName,
        bool showPoints, bool connectPoints, bool showVectors, bool closed, bool showNumbering, int numberingInterval);
    void changeCurveTolerance(const QString curveName);

signals:
    void currentFigureChanged(const QString &name);
    void figureEditDialogRequested(const QString figureName);

private:
    class Curve : public QCPCurve {
    public:
        Curve(Plot *plot, const CurveFigure *curveFigure, QCPAxis *keyAxis, QCPAxis *valueAxis);
        void connectPoints();
        void showPoints();
        void showVectors();
        void showNumbering();
        void showTolerances();

    private:
        Plot *_plot;
        const CurveFigure *_curveFigure;
        const int _scatterSize = 3;
        const int _scatterSkip = 0;
    };

    // in percent
    const double _topMarginCurve = 22.5;
    const double _bottomMarginCurve = 20;
    const double _leftMarginCurve = 5;
    const double _rightMarginCurve = 5;
    bool _isFirstFigure = true;

    Project *_project;

    void createFigure(const Figure *figure);
    void updateFigure(const QString &figureName);
    void deleteFigure(const QString &figureName);

    void addCurveLayer(const CurveFigure &curveFigure);
    void addPointLayer(const PointFigure &pointFigure);
    void addLineLayer(const LineFigure &lineFigure);
    void addCircleLayer(const CircleFigure &circleFigure);

    void calculateCircleBox(const CircleFigure &circleFigure, QCPItemEllipse *circle);
    void clearFiguresFromLayer(const QString &layerName);
    void rescaleToCurve(const CurveFigure *curveFigure);

    const double _penWidth = 1;
    const double _currentFigurePenWidth = 2;
    const double _pointRadius = 0.25;

    void onItemClicked(QCPAbstractItem *item, QMouseEvent *event);
    void onItemDoubleClicked(QCPAbstractItem *item, QMouseEvent *event);
    void onPlottableClicked(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event);
    void onPlottableDoubleClicked(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event);
};
