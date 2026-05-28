#include "curve/pch.h"

#include "project.h"
#include "reportplot.h"
#include "reportsettings.h"

QString ReportPlot::takeScreenshotInBase64(const QStringList& figureNamesToVisible,
    int width, int height, ReportSettings::Axis axisType)
{
    // clear();

    // addFigures(figureNamesToVisible);

    // zoomExtents();

    QString screenshot = getScreenshotInBase64(width, height, axisType);

    return screenshot;
}

void ReportPlot::clear()
{
    clearPlottables();
    clearItems();
}

void ReportPlot::addFigures(const QStringList& figureNames)
{
    Project* project = &Project::instance();

    for(QString name : figureNames) {
        const Figure* figure = project->findFigure(name);
        loadFigure(figure);
    }
}
