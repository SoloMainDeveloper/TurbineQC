#pragma once

#include "reportsettings.h"

class Project;

class Plot : public QCustomPlot {
    Q_OBJECT

public:
    explicit Plot(QWidget* parent = nullptr);

    static Plot& instance();
    static void initialize(Plot* plot);

    static void setTestingInstance(Plot* testInstance);
    static void resetToRealInstance();

    virtual void setProject(Project* project);
    virtual QString getScreenshotInBase64(int width, int height, ReportSettings::Axis axisType);
    virtual void setGridVisible(bool enabled);
    virtual void setAxesVisible(bool enabled);
    virtual void setBuffering(bool enabled);
    virtual void unloadBuffer();
    virtual void loadFigure(const Figure* figure);
    virtual void clearLayer(const QString& layerName);
    virtual Point centerPoint() const;
    virtual const Figure* currentFigure() const;
    virtual const Point pixelToCoord(const Point& pixel) const;
    virtual const Point coordToPixel(const Point& coord) const;
    virtual const void findNearestCurvePoint(const Point& point, const CurveFigure* curveFigure, CurvePoint& result, int& resultIndex) const;

public slots:
    virtual void addFigure(Figure* figure);
    virtual void renameFigure(const QString figureName, const QString newName);
    virtual void removeFigure(const QString figureName);
    virtual void changeCurrentFigure(const QString& currentFigureName, const QString& previousFigureName);
    virtual void changeFigureVisibility(const QString figureName, bool visible);
    virtual void changeCurveParameters(const QString curveName, bool showPoints, bool connectPoints,
        bool showVectors, bool closed, bool showNumbering, int numberingInterval,
        double amplification, bool showTolerances, bool showDeviations, bool connectDeviations, bool highLightOut);
    virtual void changeDimensionParameters(const QString dimensionName, bool showNumbers, bool showTolerances, bool freePosition);
    virtual void changeCurveTolerance(const QString curveName);
    virtual void changeFigureColor(const QString figureName);
    virtual void changeScale(double magnitude, const Point& center, bool needToReplot);
    virtual void createRadiusDimension();
    virtual void createDiameterDimension();
    virtual void createPerimeterDimension(double perimeter);
    virtual void changeDimensionValue(const QString& dimName, const DimFigure::Value& value);
    virtual void changeFigureCoords(const QString figureName);
    virtual void changeEditedFigure(const QString figureName);
    virtual void cancelInteractiveOperations();

    virtual void zoomExtents();
    virtual void zoomToPoint(double scaleFactor, const Point center);
    virtual void zoomPlus();
    virtual void zoomPlusToPoint(const Point& point, bool needToReplot);
    virtual void zoomMinus();
    virtual void zoomMinusToPoint(const Point& point, bool needToReplot);

signals:
    void currentFigureChanged(const QString& name);
    void figureEditDialogRequested(const QString figureName);
    void rescaled(double scaleFactor, const Point& center, bool needToReplot);
    void projectMousePressed(const Point pos);

private:
    void updateFigure(const QString& figureName);
    void deleteFigure(const QString& figureName);

    void addCurveLayer(const CurveFigure* curveFigure);
    void addPointLayer(const PointFigure* pointFigure);
    void addLineLayer(const LineFigure* lineFigure);
    void addCircleLayer(const CircleFigure* circleFigure);
    void addDimLayer(const DimFigure* dimFigure);
    void addTextLayer(const TextFigure* textFigure);

    void drawTable(const DimFigure* dimFigure);
    void drawDistanceBetweenCurvePoints(const DimFigure* dimFigure);
    void drawCallout(const DimFigure* dimFigure);
    void drawCalloutLine(const QString& name, const QColor& color, const Point& startPoint, const Point& endPoint);
    void drawText(const TextFigure* textFigure);
    void drawImage(const TextFigure* textFigure);

    LineFigure* createLineFigure(const QString& name, const Point& startPoint, const Point& endPoint, const QCPLineEnding& head = QCPLineEnding::esNone, const QCPLineEnding& tail = QCPLineEnding::esNone, const QColor& color = Qt::black);
    const Point findNearestFigurePoint(const Point& point, const Figure* figure);
    const QHash<double, double> intersectionLineAndRect(const double k, const double b, const QRectF& rect) const;
    QPointF toQPointF(const Point& point) const;
    const QString getTextByValueType(const DimFigure::ValueType& valueType) const;
    void setCurveDecoration(const CurveFigure* curveFigure, QCPCurve* curve);
    void drawCurve(const CurveFigure* curveFigure);
    void drawCurveVectors(const CurveFigure* curveFigure);
    void drawCurveNumbering(const CurveFigure* curveFigure);
    void drawCurveTolerances(const CurveFigure* curveFigure);
    void drawCurveDeviations(const CurveFigure* curveFigure);

    QString toBase64(const QImage& image);

    const double _labelOffsetPx = 5;
    const double _penWidth = 1;
    const double _currentFigurePenWidth = 2;
    const double _pointRadius = 0.1;
    const int _offsetCalloutPx = 5;
    const double _defaultMagnitude = 1.5511657710909;
    const double _defaultScaleFactor = 1.35;
    const double _magnificationFactor = 1.25;
    const double _reductionFactor = 0.8;
    double _defaultXRangeLength = 16;
    double _defaultYRangeLength = 9;
    double _magnitude = 1;
    double _defaultFontSize = 0.25;
    bool _calloutRendering = false;

    bool _isBuffering = false;
    QSet<QString> _buffer;

    static Plot* _instance;
    Project* _project;
    DimFigure* _calloutDimension = nullptr;

    QTimer _replotTimer;

    void onItemClicked(QCPAbstractItem* item, QMouseEvent* event);
    void onItemDoubleClicked(QCPAbstractItem* item, QMouseEvent* event);
    void onPlottableClicked(QCPAbstractPlottable* plottable, int dataIndex, QMouseEvent* event);
    void onPlottableDoubleClicked(QCPAbstractPlottable* plottable, int dataIndex, QMouseEvent* event);
    void onMouseWheel(QWheelEvent* event);
    void onMouseMove(QMouseEvent* event);
    void onMousePress(QMouseEvent* event);
};
