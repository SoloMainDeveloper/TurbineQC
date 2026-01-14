#include "curve/pch.h"
#include "curve/plot.h"

Plot::Plot(QWidget *parent) : QCustomPlot(parent), _project(nullptr) {
	xAxis->setUpperEnding(QCPLineEnding::esLineArrow);
	yAxis->setUpperEnding(QCPLineEnding::esLineArrow);

	setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables | QCP::iSelectItems);
}

void Plot::setProject(Project *project) {
	_project = project;

	connect(project, &Project::figureAdded, this, &Plot::addFigure);
	connect(project, &Project::figureRenamed, this, &Plot::renameFigure);
	connect(project, &Project::figureAboutToBeRemoved, this, &Plot::removeFigure);
	connect(project, &Project::figureVisibilityChanged, this, &Plot::changeFigureVisibility);
	connect(project, &Project::curveParametersChanged, this, &Plot::changeCurveParameters);
	connect(project, &Project::currentFigureChanged, this, &Plot::changeCurrentFigure);
	connect(project, &Project::curveToleranceChanged, this, &Plot::changeCurveTolerance);
	
	connect(this, &Plot::plottableClick, this, &Plot::onPlottableClicked);
	connect(this, &Plot::plottableDoubleClick, this, &Plot::onPlottableDoubleClicked);
	connect(this, &Plot::itemClick, this, &Plot::onItemClicked);
	connect(this, &Plot::itemDoubleClick, this, &Plot::onItemDoubleClicked);
	connect(this, &Plot::currentFigureChanged, project, &Project::changeCurrentFigure);
	connect(this, &Plot::figureEditDialogRequested, project, &Project::requestFigureEditDialog);
}

void Plot::addFigure(Figure *figure) {
	if(_isFirstFigure) { // added first curve
		if(auto curve = dynamic_cast<CurveFigure*>(figure)) {
			rescaleToCurve(curve);
			_isFirstFigure = false;
		}
	}
	
	createFigure(figure);
	layer(figure->name())->replot();
};

void Plot::renameFigure(const QString figureName, const QString newName) {
	auto figure = _project->findFigure(newName);

	deleteFigure(figureName);
	createFigure(figure);
}

void Plot::removeFigure(const QString figureName) {
	deleteFigure(figureName);
}

void Plot::changeCurrentFigure(const QString &currentFigureName, const QString &previousFigureName) {
	if(!previousFigureName.isNull()) {
		updateFigure(previousFigureName);
	}
	
	if(!currentFigureName.isNull()) {
		updateFigure(currentFigureName);
	}
}

void Plot::changeFigureVisibility(const QString figureName, bool visible) {
	layer(figureName)->setVisible(visible);
	layer(figureName)->replot();
}

void Plot::changeCurveParameters(const QString curveName,
		bool showPoints, bool connectPoints, bool showVectors, bool closed, bool showNumbering, int numberingInterval) {
	updateFigure(curveName);
}

void Plot::changeCurveTolerance(const QString curveName) {
	updateFigure(curveName);
}

void Plot::createFigure(const Figure *figure) {
	if(layer(figure->name()) == nullptr) {
		addLayer(figure->name());
	}

	setCurrentLayer(figure->name()); 

	layer(figure->name())->setVisible(figure->isVisible());

	if(auto curveFigure = dynamic_cast<const CurveFigure*>(figure)) {
		addCurveLayer(*curveFigure);
	} else if(auto lineFigure = dynamic_cast<const LineFigure*>(figure)) {
		addLineLayer(*lineFigure);
	} else if(auto pointFigure = dynamic_cast<const PointFigure*>(figure)) {
		addPointLayer(*pointFigure);
	} else if(auto circleFigure = dynamic_cast<const CircleFigure*>(figure)) {
		addCircleLayer(*circleFigure);
	}
}

void Plot::updateFigure(const QString &figureName) {
	auto figure = _project->findFigure(figureName);

	if(layer(figureName) == nullptr) {
		qDebug() << "Layer" << figureName << "does not exist";
		return;
	}

	clearFiguresFromLayer(figureName);
	createFigure(figure);
	layer(figureName)->replot();
}

void Plot::deleteFigure(const QString &figureName) {
	auto layerToDelete = layer(figureName);

	clearFiguresFromLayer(figureName);

	layer(figureName)->replot();

	removeLayer(layerToDelete);
}

void Plot::addCurveLayer(const CurveFigure &curveFigure) {
	auto curve = new Curve(this, &curveFigure, xAxis, yAxis);

	double penWidth;
	if(_project->currentFigureName() == curveFigure.name()) {
		penWidth = _currentFigurePenWidth;
	} else {
		penWidth = _penWidth;
	}

	auto brush = QBrush(curveFigure.color());
	auto pen = QPen(brush, penWidth);
	curve->setPen(pen);
	const auto curvePoints = curveFigure.points();

	for(const auto &point : curvePoints) {
		curve->addData(point.x, point.y);
	}
	
	if(curveFigure.isConnectPoints()) {
		curve->connectPoints();
	}
	
	if(curveFigure.isShowPoints()) {
		curve->showPoints();
	}

	if(curveFigure.isShowVectors()) {
		curve->showVectors();
	}

	if(curveFigure.isClosed()) {
		curve->addData(curvePoints[0].x, curvePoints[0].y);
	}

	if(curveFigure.isShowNumbering()) {
		curve->showNumbering();
	}
	
	curve->showTolerances();

	addGraph(curve->keyAxis(), curve->valueAxis());
}

void Plot::addPointLayer(const PointFigure &pointFigure) {
	auto circleFigure = CircleFigure(pointFigure.name(), Point(pointFigure.point()), Point(0, 0, 1), _pointRadius);

	auto pointColor = pointFigure.color();
	circleFigure.setColor(pointColor);

	addCircleLayer(circleFigure);
}

void Plot::addLineLayer(const LineFigure &lineFigure) {
	auto length = lineFigure.length();

	double penWidth;
	if(_project->currentFigureName() == lineFigure.name()) {
		penWidth = _currentFigurePenWidth;
	} else {
		penWidth = _penWidth;
	}

	auto brush = QBrush(lineFigure.color());
	auto pen = QPen(brush, penWidth);

	if(length == qInf()) {
		auto line = new QCPItemStraightLine(this);
		line->setPen(pen);

		auto origin = lineFigure.origin();
		auto direction = lineFigure.direction();

		line->point1->setCoords(origin.x, origin.y);
		line->point2->setCoords(origin.x + direction.x, origin.y + direction.y);
	} else {
		auto line = new QCPItemLine(this);
		line->setPen(pen);

		auto origin = lineFigure.origin();
		auto direction = lineFigure.direction();
		double halfLength = 0.5 * length;

		auto startPoint = Point(origin.x - halfLength * direction.x, origin.y - halfLength * direction.y);
		auto endPoint = Point(origin.x + halfLength * direction.x, origin.y + halfLength * direction.y);

		line->start->setCoords(startPoint.x, startPoint.y);
		line->end->setCoords(endPoint.x, endPoint.y);

	}
}

void Plot::addCircleLayer(const CircleFigure &circleFigure) {
	auto circle = new QCPItemEllipse(this);

	double penWidth;
	if(_project->currentFigureName() == circleFigure.name()) {
		penWidth = _currentFigurePenWidth;
	} else {
		penWidth = _penWidth;
	}

	auto brush = QBrush(circleFigure.color());
	auto pen = QPen(brush, penWidth);
	circle->setPen(pen);

	calculateCircleBox(circleFigure, circle);
}

void Plot::calculateCircleBox(const CircleFigure &circleFigure, QCPItemEllipse *circle) {
	auto center = circleFigure.center();
	auto radius = circleFigure.radius();

	auto topLeftAnchor = QPointF(center.x - radius, center.y + radius);
	auto bottomRightAnchor = QPointF(center.x + radius, center.y - radius);
	circle->topLeft->setCoords(topLeftAnchor);
	circle->bottomRight->setCoords(bottomRightAnchor);
}

void Plot::clearFiguresFromLayer(const QString &layerName) {
	auto layerToClear = layer(layerName);
	
	QCPAbstractPlottable *abstractPlottable;
	QCPAbstractItem *abstractItem;
	for(auto &children : layerToClear->children()) {
		if(abstractPlottable = dynamic_cast<QCPAbstractPlottable*>(children)) {
			removePlottable(abstractPlottable);
		} else if(abstractItem = dynamic_cast<QCPAbstractItem*>(children)) {
			removeItem(abstractItem);
		}
	}
}

void Plot::rescaleToCurve(const CurveFigure *curveFigure) {
	auto curvePoints = curveFigure->points();
	double minX = curvePoints[0].x, maxX = curvePoints[0].x;
	double minY = curvePoints[0].y, maxY = curvePoints[0].y;

	for(auto &point : curvePoints) {
		if(point.x < minX) minX = point.x;
		if(point.y < minY) minY = point.y;
		if(point.x > maxX) maxX = point.x;
		if(point.y > maxY) maxY = point.y;
	}

	const double reductionWidthFactor = 1 - ((_rightMarginCurve + _leftMarginCurve) / 100);
	const double reductionHeightFactor = 1 - ((_topMarginCurve + _bottomMarginCurve) / 100);

	double width = (maxX - minX) / reductionWidthFactor;
	double height = (maxY - minY) / reductionHeightFactor;

	double leftMargin = width * _leftMarginCurve / 100;
	double rightMargin = width * _rightMarginCurve / 100;
	double topMargin = height * _topMarginCurve / 100;
	double bottomMargin = height * _bottomMarginCurve / 100;

	xAxis->setRange(minX - leftMargin, maxX + rightMargin);
	yAxis->setRange(minY - bottomMargin, maxY + topMargin);
}

Plot::Curve::Curve(Plot *plot, const CurveFigure *curveFigure, QCPAxis *keyAxis, QCPAxis *valueAxis) : QCPCurve(keyAxis, valueAxis) {
	setLineStyle(LineStyle::lsNone);

	const auto scatterShape = QCPScatterStyle::ScatterShape::ssNone;
	const auto scatterStyle = QCPScatterStyle(scatterShape, _scatterSize);
	setScatterStyle(scatterStyle);
	setScatterSkip(_scatterSkip);
	
	_plot = plot;
	_curveFigure = curveFigure;
}

void Plot::Curve::connectPoints() {
	setLineStyle(LineStyle::lsLine);
}

void Plot::Curve::showPoints() {
	const auto scatterShape = QCPScatterStyle::ScatterShape::ssSquare;
	const auto scatterStyle = QCPScatterStyle(scatterShape, _scatterSize);
	setScatterStyle(scatterStyle);
}

void Plot::Curve::showTolerances() {
	auto curveName = _curveFigure->name();
	
	auto upperTolerancePoints = _curveFigure->upperTolerance();
	auto lowerTolerancePoints = _curveFigure->lowerTolerance();
	auto color = QColorConstants::DarkGreen;

	if(upperTolerancePoints.isEmpty() == false) {
		auto upperCurveFigure = new CurveFigure(curveName, upperTolerancePoints);
		upperCurveFigure->setColor(color);
		upperCurveFigure->setClosed(_curveFigure->isClosed());
		_plot->createFigure(upperCurveFigure);
	}

	if(lowerTolerancePoints.isEmpty() == false) {
		auto lowerCurveFigure = new CurveFigure(curveName, lowerTolerancePoints);
		lowerCurveFigure->setColor(color);
		lowerCurveFigure->setClosed(_curveFigure->isClosed());
		_plot->createFigure(lowerCurveFigure);
	}
}

void Plot::Curve::showVectors() {
	auto pen = QPen(QColorConstants::Red);
	for(auto &point : _curveFigure->points()) {
		auto line = new QCPItemLine(parentPlot());
		
		line->setPen(pen);
		line->setHead(QCPLineEnding::esLineArrow);
		line->start->setCoords(point.x, point.y);
		line->end->setCoords(point.x + point.i, point.y + point.j);
	}
}

void Plot::Curve::showNumbering() {
	auto points = _curveFigure->points();
	auto numberingInterval = _curveFigure->numberingInterval();
	const double offsetX = 0, offsetY = -0.05;

	for(int i = 0; i < points.size(); i += numberingInterval) {
		auto point = points[i];
		auto label = new QCPItemText(parentPlot());
		label->setText(QString::number(i + 1));
		label->position->setCoords(point.x + offsetX, point.y + offsetY);
	}
}

void Plot::onItemClicked(QCPAbstractItem *item, QMouseEvent *event) {
	auto figureName = item->layer()->name();
	emit currentFigureChanged(figureName);
	updateFigure(figureName);
}

void Plot::onItemDoubleClicked(QCPAbstractItem* item, QMouseEvent* event) {
	auto figureName = item->layer()->name();
	emit figureEditDialogRequested(figureName);
}

void Plot::onPlottableClicked(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event) {
	auto figureName = plottable->layer()->name();
	emit currentFigureChanged(figureName);
	updateFigure(figureName);
}

void Plot::onPlottableDoubleClicked(QCPAbstractPlottable* plottable, int dataIndex, QMouseEvent* event) {
	auto figureName = plottable->layer()->name();
	emit figureEditDialogRequested(figureName);
}