#pragma once
#include "curve/pch.h"
#include "curve/project.h"
#include "curve/reportSettings.h"

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
    void rescale(Position position);
    QImage getScreenshot(int width, int height, ReportSettings::Axis axisType);
    void createFigureWithoutReplot(const Figure *figure);
    void clearFiguresFromLayer(const QString &layerName);
    Point centerPoint() const;
    double pointRadius();
    double defaultFontSize() const;
    const Figure* currentFigure() const;
    const Point pixelToCoord(const Point &pixel) const;
    const Point coordToPixel(const Point &coord) const;

public slots:
    void addFigure(Figure *figure);
    void renameFigure(const QString figureName, const QString newName);
    void removeFigure(const QString figureName);
    void changeCurrentFigure(const QString &currentFigureName, const QString &previousFigureName);
    void changeFigureVisibility(const QString figureName, bool visible);
    void changeCurveParameters(const QString curveName, bool showPoints, bool connectPoints,
        bool showVectors, bool closed, bool showNumbering, int numberingInterval,
        double amplification, bool showTolerances, bool showDeviations, bool connectDeviations, bool highLightOut);
    void changeDimensionParameters(const QString dimensionName, bool showNumbers, bool showTolerances, bool freePosition);
    void changeCurveTolerance(const QString curveName);
    void changeFigureColor(const QString figureName);
    void changeScale(double magnitude, const Point &center);
    void createRadiusDimension();
    void createDiameterDimension();
    void createPerimeterDimension(double perimeter);
    void changeDimensionValue(const QString &dimName, const DimFigure::Value &value);
    void changeFigureCoords(const QString figureName);
    void changeEditedFigure(const QString figureName);
    void cancelInteractiveOperations();

    void zoomExtents();
    void zoomPlus();
    void zoomPlusToPoint(const Point &point);
    void zoomMinus();
    void zoomMinusToPoint(const Point &point);

signals:
    void currentFigureChanged(const QString &name);
    void figureEditDialogRequested(const QString figureName);
    void rescaled(double scaleFactor, const Point &center);
    void projectMouseMoved(const Point pos);
    void projectMousePressed(const Point pos);

private:
    class Curve : public QCPCurve {
    public:
        Curve(Plot *plot, const CurveFigure *curveFigure, QCPAxis *keyAxis, QCPAxis *valueAxis);
        void requestConnectPoints();
        void requestShowPoints();
        void requestShowVectors();
        void requestShowNumbering();
        void requestShowTolerances();
        void requestShowDeviations();
        void requestShowNumericalDeviations();
        void requestConnectDeviations();

    private:
        Plot *_plot;
        const CurveFigure *_curveFigure;
        const int _scatterSize = 3;
        const int _scatterSkip = 0;
    };

    void updateFigure(const QString &figureName);
    void deleteFigure(const QString &figureName);

    void addCurveLayer(const CurveFigure &curveFigure);
    void addPointLayer(const PointFigure &pointFigure);
    void addLineLayer(const LineFigure &lineFigure);
    void addCircleLayer(const CircleFigure &circleFigure);
    void addDimLayer(const DimFigure *dimFigure);
    void addTextLayer(const TextFigure *textFigure);

    void drawTable(const DimFigure *dimFigure);
    void drawDistanceBetweenCurvePoints(const DimFigure *dimFigure);
    void drawCallout(const DimFigure *dimFigure);
    void drawCalloutLine(const QString &name, const QColor &color, const Point &startPoint, const Point &endPoint);
    void drawText(const TextFigure *textFigure);
    void drawImage(const TextFigure *textFigure);

    void calculateCircleBox(const CircleFigure &circleFigure, QCPItemEllipse *circle);
    void replotTimerTimeout();
    LineFigure* createLineFigure(const QString &name, const Point &startPoint, const Point &endPoint, const QCPLineEnding &head = QCPLineEnding::esNone, const QCPLineEnding &tail = QCPLineEnding::esNone, const QColor &color = Qt::black);
    const Point findNearestFigurePoint(const Point &point, const Figure* figure);
    const QHash<double, double> intersectionLineAndRect(const double k, const double b, const QRectF &rect) const;
    QPointF toQPointF(const Point &point) const;
    const QString getTextByValueType(const DimFigure::ValueType &valueType) const;

    void rescaleToCenter(double xMin, double xMax, double yMin, double yMax);
    void rescaleToRight(double xMin, double xMax, double yMin, double yMax);
    void rescaleToLeft(double xMin, double xMax, double yMin, double yMax);

    const double _labelOffsetPx = 5;
    const int _columnWidthPx = 130;
    const int _rowHeightPx = 20;
    const int _separatorHeightPx = 3;
    const double _penWidth = 1;
    const double _currentFigurePenWidth = 2;
    const double _minPointRadius = 0.1;
    const double _maxPointRadius = 0.25;
    const int _offsetCalloutPx = 5;
    const double _defaultScaleFactor = 1.5511657710909;
    const double _magnificationFactor = 1.25;
    const double _reductionFactor = 0.8;
    const double _ratioXToY = 7.0 / 5;
    const double _ratioYToX = 5.0 / 7;
    double _magnitude = 1;
    double _defaultFontSize = 0.25;
    bool _calloutRendering = false;
    bool _calloutFromProject = false;

    Project *_project;
    QTimer *_replotTimer;
    DimFigure *_calloutDimension = nullptr;

    void onItemClicked(QCPAbstractItem *item, QMouseEvent *event);
    void onItemDoubleClicked(QCPAbstractItem *item, QMouseEvent *event);
    void onPlottableClicked(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event);
    void onPlottableDoubleClicked(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event);
    void onMouseWheel(QWheelEvent *event);
    void onMouseMove(QMouseEvent *event);
    void onMousePress(QMouseEvent *event);
    void onMouseRelease(QMouseEvent *event);
};
