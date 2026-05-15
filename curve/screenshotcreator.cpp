#include "curve/pch.h"

#include "screenshotcreator.h"

ScreenshotCreator::ScreenshotCreator(std::shared_ptr<ReportSettings> reportSettings) : _reportSettings(reportSettings)
{
	_project = &Project::instance();
	_plot = &Plot::instance();

	_reportPlot = new ReportPlot();
	_reportPlot->setProject(new Project());
	_reportPlot->resize(800, 600);
	_reportPlot->setVisible(true);

	_nominalName = _reportSettings->nominalName();
	_measuredName = _reportSettings->measuredName();
}

void ScreenshotCreator::run(const GlobalCurveMap& curvesToMakeScreenshot)
{
	QStringList defaultCurvesToVisible;
	if (curvesToMakeScreenshot.contains(CurveType::WholeGlobal)) {
		auto [name, points] = curvesToMakeScreenshot[CurveType::WholeGlobal];
		defaultCurvesToVisible.append(name);
		if (_reportSettings->needPrintWithTemplate()) {
			makeScreenshotOfGlobal({ name });
		}
	}
	if (curvesToMakeScreenshot.contains(CurveType::GlobalWithoutTE)) {
		auto [name, points] = curvesToMakeScreenshot[CurveType::GlobalWithoutTE];
		defaultCurvesToVisible.append(name);
		if (_reportSettings->needPrintWithTemplate()) {
			makeScreenshotOfGlobal({ name });
		}
	}
	if ((curvesToMakeScreenshot.contains(CurveType::GlobalCV) || curvesToMakeScreenshot.contains(CurveType::GlobalCC)) && (!curvesToMakeScreenshot.contains(CurveType::WholeGlobal) && !curvesToMakeScreenshot.contains(CurveType::GlobalWithoutTE))) {
		auto [nameCV, pointsCV] = curvesToMakeScreenshot[CurveType::GlobalCV];
		auto [nameCC, pointsCC] = curvesToMakeScreenshot[CurveType::GlobalCC];
		defaultCurvesToVisible.append(nameCV);
		defaultCurvesToVisible.append(nameCC);
		if (_reportSettings->needPrintWithTemplate()) {
			makeScreenshotOfGlobal({ nameCV, nameCC });
		}
	}
	if (curvesToMakeScreenshot.contains(CurveType::GlobalLE) && _reportSettings->isLeadingEdgeVisible()) {
		auto [name, points] = curvesToMakeScreenshot[CurveType::GlobalLE];
		makeScreenshotOfEdge(name, CurveType::GlobalLE, _reportSettings->typeOfShowDevsLE(), _reportSettings->axisTypeOfLE());
	}
	if (curvesToMakeScreenshot.contains(CurveType::GlobalTE) && _reportSettings->isTrailingEdgeVisible()) {
		auto [name, points] = curvesToMakeScreenshot[CurveType::GlobalTE];
		makeScreenshotOfEdge(name, CurveType::GlobalTE, _reportSettings->typeOfShowDevsTE(), _reportSettings->axisTypeOfTE());
	}

	setDefaultVisibilityOnGraphics(defaultCurvesToVisible);
}

void ScreenshotCreator::makeScreenshotOfGlobal(const QStringList& globalNames)
{
	//_project->resetVisibilityForAllFigures();
	//_project->setVisibility({ globalNames });
	QStringList additionalFigureNames = getAdditionalFigureNamesToVisible();
	QStringList resultFiguresToVisible = globalNames + additionalFigureNames;

	//_plot->zoomExtents();
	//_project->setCurrentFigure(_nominalName);

	_reportPlot->clear();
	_reportPlot->addFigures(resultFiguresToVisible);
	_reportPlot->zoomExtents();

	_reportPlot->update();
	_reportPlot->replot();

	for (int i = 0; i < 5; ++i) {
		QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
	}

	QString screenshot = _reportPlot->getScreenshotInBase64(800, 450, _reportSettings->globalAxisType());
	_reportSettings->setGlobalBase64Image(screenshot);
}

QStringList ScreenshotCreator::getAdditionalFigureNamesToVisible()
{
	auto dummyNames = ReportGenerator::getTemplateInterimNames(_nominalName, _measuredName);
	auto preparedMeasName = dummyNames[ReportGenerator::InterimName::MeasuredCurve];

	using AdditionalName = ReportGenerator::AdditionalName;
	auto templateAdditionalNames = ReportGenerator::getTemplateAdditionalNames(_nominalName, preparedMeasName);
	QStringList figuresToVisible;

	if (_reportSettings->needMCL()) {
		figuresToVisible.append(templateAdditionalNames[AdditionalName::NominalMCL]);
		figuresToVisible.append(templateAdditionalNames[AdditionalName::MeasuredMCL]);
	}
	if (_reportSettings->needMaxDiameter()) {
		figuresToVisible.append(templateAdditionalNames[AdditionalName::NominalMaxDia]);
		figuresToVisible.append(templateAdditionalNames[AdditionalName::MeasuredMaxDia]);
	}
	if (_reportSettings->needContactLine()) {
		figuresToVisible.append(templateAdditionalNames[AdditionalName::NominalCntctLine]);
		figuresToVisible.append(templateAdditionalNames[AdditionalName::MeasuredCntctLine]);
	}

	return figuresToVisible;
}

void ScreenshotCreator::makeScreenshotOfEdge(const QString& edgeName, CurveType curveType, TypeOfShowDevs devsType, Axis axisType)
{
	//_project->resetVisibilityForAllFigures();
	//_project->setVisibility({ edgeName });
	//CurveFigure* deviations = createNumericalDeviations(edgeName, devsType, curveType);
	//_plot->zoomExtents();

	_reportPlot->clear();
	_reportPlot->addFigures({ edgeName });
	_reportPlot->zoomExtents();

	QStringList additionalFiguresToVisible;

	if (_reportSettings->needMCL()) {
		auto dummyNames = ReportGenerator::getTemplateInterimNames(_nominalName, _measuredName);
		auto preparedMeasName = dummyNames[ReportGenerator::InterimName::MeasuredCurve];
		auto templateAdditionalNames = ReportGenerator::getTemplateAdditionalNames(_nominalName, preparedMeasName);
		using AdditionalName = ReportGenerator::AdditionalName;
		additionalFiguresToVisible.append(templateAdditionalNames[AdditionalName::NominalMCL]);
		additionalFiguresToVisible.append(templateAdditionalNames[AdditionalName::MeasuredMCL]);
		_reportPlot->addFigures(additionalFiguresToVisible);
	}

	//_project->setCurrentFigure(_nominalName);

	QString screenshot = _reportPlot->getScreenshotInBase64(400, 225, axisType);
	curveType == CurveType::GlobalLE
		? _reportSettings->setLeadingEdgeBase64Image(screenshot)
		: _reportSettings->setTrailingEdgeBase64Image(screenshot);

	//_project->removeFigure(deviations->name());
}

CurveFigure* ScreenshotCreator::createNumericalDeviations(const QString& edgeName, TypeOfShowDevs devsType, CurveType curveType)
{
	auto edge = dynamic_cast<const CurveFigure*>(_project->findFigure(edgeName));

	ARGUMENT_ASSERT(edge, "Create numerical deviations: curve's not found");

	auto edgePoints = edge->points();
	QVector<CurvePoint> pointsWithInterval;
	if (devsType == ReportSettings::TypeOfShowDevs::Auto) {
		auto pointsInterval = 4;
		if (edgePoints.length() > 50) {
			pointsInterval = edgePoints.length() / 12;
		}
		for (auto i = 0; i < edgePoints.length(); i += pointsInterval) {
			pointsWithInterval.append(edgePoints[i]);
		}
	}
	else if (devsType == ReportSettings::TypeOfShowDevs::Set) {
		auto pointsInterval = curveType == CurveType::GlobalLE ? _reportSettings->valueOfSetShowDevsLE() + 1 : _reportSettings->valueOfSetShowDevsTE() + 1;
		for (auto i = 0; i < edgePoints.length(); i += pointsInterval) {
			pointsWithInterval.append(edgePoints[i]);
		}
	}
	else if (devsType == ReportSettings::TypeOfShowDevs::OnEdge) {
		pointsWithInterval.append(edgePoints[edgePoints.length() / 2]);
	}
	auto curveWithIntervalName = QString("%1_interval").arg(edgeName);
	auto curveWithInterval = new CurveFigure(curveWithIntervalName, pointsWithInterval);
	curveWithInterval->setConnectPoints(false);
	curveWithInterval->setShowPoints(true);
	curveWithInterval->setShowNumericalDeviations(true);
	_project->safeInsert(curveWithIntervalName, curveWithInterval, false);
	return curveWithInterval;
}

void ScreenshotCreator::setDefaultVisibilityOnGraphics(const QStringList& visibleFigures)
{
	_project->resetVisibilityForAllFigures();
	_project->setVisibility(visibleFigures);
	_plot->zoomExtents();
	QStringList names = getAdditionalFigureNamesToVisible();
	names.append(_nominalName);
	_project->setVisibility({ names });
	_project->setCurrentFigure(_nominalName);
}
