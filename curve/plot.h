#pragma once
#include "curve/pch.h"
#include "curve/figure.h"
#include "curve/project.h"
#include "curve/reportdata.h"

class Plot : public QCustomPlot {
    Q_OBJECT

public:
    explicit Plot(QWidget *parent = nullptr);

    enum class Position {
        Center,
        Left,
        Right,
    };

    void setProject(Project *project);
    void rescaleToCurve(Position position);
    void makeScreenshot(const QString &filePath, int width, int height, ReportData::Axis axisType);
    void createFigureWithoutReplot(const Figure *figure);
    void clearFiguresFromLayer(const QString &layerName);
    Point currentCenterPoint() const;
    double pointRadius();
    double defaultFontSize() const;
    const Figure* currentFigure() const;
    const Point pixelToCoord(const Point &pixel) const;
    const Point coordToPixel(const Point &coord) const;

    // Settings
    double scaleCoefficient() const;
    Point centerPoint() const;

public slots:
    void addFigure(Figure *figure);
    void renameFigure(const QString figureName, const QString newName);
    void removeFigure(const QString figureName);
    void changeCurrentFigure(const QString &currentFigureName, const QString &previousFigureName);
    void changeFigureVisibility(const QString figureName, bool visible);
    void changeCurveParameters(const QString curveName, bool showPoints, bool connectPoints,
        bool showVectors, bool closed, bool showNumbering, int numberingInterval,
        double amplification, bool showTolerances, bool showDeviations, bool connectDeviations, bool highLightOut);
    void changeCurveTolerance(const QString curveName);
    void changeFigureColor(const QString figureName);
    void addDimension(DimFigure *dimFigure);
    void changeScale(double scaleFactor, const Point &center);
    void clear();

    void zoomExtents();
    void zoomPlus();
    void zoomPlusToPoint(const Point &point);
    void zoomMinus();
    void zoomMinusToPoint(const Point &point);

signals:
    void currentFigureChanged(const QString &name);
    void figureEditDialogRequested(const QString figureName);
    void renderDimension(const DimFigure *dimFigure);
    void scaled(double scaleFactor, const Point &center);

private:
    class Curve : public QCPCurve {
    public:
        Curve(Plot *plot, const CurveFigure *curveFigure, QCPAxis *keyAxis, QCPAxis *valueAxis);
        void connectPoints();
        void showPoints();
        void showVectors();
        void showNumbering();
        void showTolerances();
        void showDeviations();
        void showNumericalDeviations(int devNumericalInterval);
        void connectDeviations();

    private:
        Plot *_plot;
        const CurveFigure *_curveFigure;
        const int _scatterSize = 3;
        const int _scatterSkip = 0;
    };

    static char sign(double value);

    double _scaleFactor = 1.0;
    bool _isFirstFigure = true;

    Project *_project;
    QTimer *_replotTimer;

    void updateFigure(const QString &figureName);
    void deleteFigure(const QString &figureName);

    void addCurveLayer(const CurveFigure &curveFigure);
    void addPointLayer(const PointFigure &pointFigure);
    void addLineLayer(const LineFigure &lineFigure);
    void addCircleLayer(const CircleFigure &circleFigure);
    void calculateCircleBox(const CircleFigure &circleFigure, QCPItemEllipse *circle);
    void replotTimerTimeout();

    const double _figureMargin = 10; // in percent
    const double _penWidth = 1;
    const double _currentFigurePenWidth = 2;
    const double _minPointRadius = 0.1;
    const double _maxPointRadius = 0.25;
    const double _scaleStepPercent = 20;
    const double _defaultScaleFactor = 1.5511657710909;
    const double _pointRadius = 0.25;
    double _magnitude = 0.0;
    double _defaultFontSize = 0.25; // in pt

    void onItemClicked(QCPAbstractItem *item, QMouseEvent *event);
    void onItemDoubleClicked(QCPAbstractItem *item, QMouseEvent *event);
    void onPlottableClicked(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event);
    void onPlottableDoubleClicked(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event);
    void onMouseWheel(QWheelEvent *event);
};
