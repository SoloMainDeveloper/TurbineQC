#include "curve/pch.h"

#include "mockplot.h"

MockPlot::MockPlot(QWidget* parent)
    : Plot(parent)
{
}

QString MockPlot::getScreenshotInBase64(int width, int height, ReportSettings::Axis axisType)
{
    return QString();
}

Point MockPlot::centerPoint() const
{
    return Point();
}

const Figure* MockPlot::currentFigure() const
{
    return nullptr;
}

const Point MockPlot::pixelToCoord(const Point& pixel) const
{
    return Point();
}

const Point MockPlot::coordToPixel(const Point& coord) const
{
    return Point();
}
