#pragma once

#include "plot.h"

class ReportPlot : public Plot {
	Q_OBJECT

public:
	static Plot& instance() = delete;
	static void initialize(Plot* plot) = delete;
	static void setTestingInstance(Plot* testInstance) = delete;
	static void resetToRealInstance() = delete;

	ReportPlot() = default;

	void clear();
	void addFigures(const QStringList& figureNames);
	QString takeScreenshotInBase64(const QStringList& figureNamesToVisible,
		int width, int height, ReportSettings::Axis axisType);
};

