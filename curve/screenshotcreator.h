#pragma once

#include "plot.h"
#include "reportgenerator.h"
#include "reportplot.h"

using TypeOfShowDevs = ReportSettings::TypeOfShowDevs;
using Axis = ReportSettings::Axis;
using CurveType = FigureCreator::CurveType;
using GlobalCurveMap = QMap<CurveType, QPair<QString, QVector<CurvePoint>>>;

class ScreenshotCreator {
public:
	explicit ScreenshotCreator(std::shared_ptr<ReportSettings> reportSettings);
	virtual ~ScreenshotCreator() = default;

	void run(const GlobalCurveMap& curvesToMakeScreenshot);

private:
	Plot* _plot;
	ReportPlot* _reportPlot;
	Project* _project;
	std::shared_ptr<ReportSettings> _reportSettings;

	QString _nominalName;
	QString _measuredName;

	void makeScreenshotOfGlobal(const QStringList& globalNames);
	QStringList getAdditionalFigureNamesToVisible();
	void makeScreenshotOfEdge(const QString& edgeName, CurveType curveType, TypeOfShowDevs devsType, Axis axisType);
	CurveFigure* createNumericalDeviations(const QString& edgeName, TypeOfShowDevs devsType, CurveType curveType);
	void setDefaultVisibilityOnGraphics(const QStringList& visibleFigures);
};
