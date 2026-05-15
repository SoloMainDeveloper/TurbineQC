#include "curve/pch.h"

#include "reportplot.h"
#include "project.h"
#include "reportsettings.h"

ReportPlot::ReportPlot() : Plot(new QWidget)
{
}

QString ReportPlot::takeScreenshotInBase64(const QStringList& figureNamesToVisible,
	int width, int height, ReportSettings::Axis axisType)
{
	//clear();

	//addFigures(figureNamesToVisible);

	//zoomExtents();

	QString screenshot = getScreenshotInBase64(width, height, axisType);

	return screenshot;
}

void ReportPlot::clear()
{
	clearGraphs();
	clearItems();
}

void ReportPlot::addFigures(const QStringList& figureNames)
{
	Project* project = &Project::instance();

	for (QString name : figureNames) {
		const Figure* figure = project->findFigure(name);
		loadFigure(figure);
	}
}
