#pragma once
#include "figure.h"
#include "plot.h"
#include "curvelibrary.h"

class DimensionRender : public QObject {
    Q_OBJECT

public:
    DimensionRender(Plot *plot);
    void addRadius();
    void addDiameter();
    void addPerimeter();

signals:
    void dimensionAdded(DimFigure *dimFigure);

public slots:
    void onMouseMove(QMouseEvent *event);
    void onMousePress(QMouseEvent *event);
    void render(const DimFigure *dimFigure);

private:
    DimFigure::DimType _dimType = DimFigure::DimType::Distance;
    Plot *_plot;
    const CircleFigure *_circleFigure = nullptr;
    const CurveFigure *_curveFigure = nullptr;
    const double _offsetCalloutX = 0.1;
    const double _labelOffsetPx = 10;
    double _perimeter;

    void renderCallout(const QString &name, const Point &startPoint, const Point &offsetPoint, const Point &endPoint);
    const Point normalizeVector(const Point &vector) const;
    const LineFigure* createLineFigure(const QString &name, const Point &startPoint, const Point &endPoint, const QCPLineEnding &head = QCPLineEnding::esNone, const QCPLineEnding &tail = QCPLineEnding::esNone) const;
    const Point findNearestCirclePoint(const Point &point, const CircleFigure *circleFigure) const;
    const Point findNearestCurvePoint(const Point &point, const CurveFigure *curveFigure) const;
    const QHash<double, double> intersectionLineAndRect(const double k, const double b, const QRectF &rect) const;
    const QPointF toQPointF(const Point &point) const;
};
