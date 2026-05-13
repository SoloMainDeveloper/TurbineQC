#pragma once

#include "plot.h"
#include <QWidget>

class MockPlot : public Plot {
public:
    MockPlot(QWidget* parent = nullptr);

    void setProject(Project* project) override {};
    QString getScreenshotInBase64(int width, int height, ReportSettings::Axis axisType) override;
    void setGridVisible(bool enabled) override {};
    void setAxesVisible(bool enabled) override {};
    void setBuffering(bool enabled) override {};
    void unloadBuffer() override {};
    void loadFigure(const Figure* figure) override {};
    void clearLayer(const QString& layerName) override {};
    Point centerPoint() const override;
    const Figure* currentFigure() const override;
    const Point pixelToCoord(const Point& pixel) const override;
    const Point coordToPixel(const Point& coord) const override;
    const void findNearestCurvePoint(const Point& point, const CurveFigure* curveFigure,
        CurvePoint& result, int& resultIndex) const override {};

public slots:
    void addFigure(Figure* figure) override {};
    void renameFigure(const QString figureName, const QString newName) override {};
    void removeFigure(const QString figureName) override {};
    void changeCurrentFigure(const QString& currentFigureName, const QString& previousFigureName) override {};
    void changeFigureVisibility(const QString figureName, bool visible) override {};
    void changeCurveParameters(const QString curveName, bool showPoints, bool connectPoints,
        bool showVectors, bool closed, bool showNumbering, int numberingInterval,
        double amplification, bool showTolerances, bool showDeviations, bool connectDeviations, bool highLightOut) override {};
    void changeDimensionParameters(const QString dimensionName, bool showNumbers, bool showTolerances, bool freePosition) override {};
    void changeCurveTolerance(const QString curveName) override {};
    void changeFigureColor(const QString figureName) override {};
    void changeScale(double magnitude, const Point& center, bool needToReplot) override {};
    void createRadiusDimension() override {};
    void createDiameterDimension() override {};
    void createPerimeterDimension(double perimeter) override {};
    void changeDimensionValue(const QString& dimName, const DimFigure::Value& value) override {};
    void changeFigureCoords(const QString figureName) override {};
    void changeEditedFigure(const QString figureName) override {};
    void cancelInteractiveOperations() override {};

    void zoomExtents() override {};
    void zoomToPoint(double scaleFactor, const Point center) override {};
    void zoomPlus() override {};
    void zoomPlusToPoint(const Point& point, bool needToReplot) override {};
    void zoomMinus() override {};
    void zoomMinusToPoint(const Point& point, bool needToReplot) override {};
};