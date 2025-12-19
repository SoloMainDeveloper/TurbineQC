#include "curve/pch.h"
#include "curve/plot.h"

Plot::Plot(QWidget *parent) : QCustomPlot(parent), _project(nullptr) {
	const int axisSize = 100;

	xAxis->setRange(-axisSize, axisSize);
	yAxis->setRange(-axisSize, axisSize);
	
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
	connect(this, &Plot::itemClick, this, &Plot::onItemClicked);
	connect(this, &Plot::currentFigureChanged, project, &Project::changeCurrentFigure);
}

void Plot::addFigure(Figure *figure) {
	createFigure(figure);
};

void Plot::renameFigure(const QString figureName, const QString newName) {
	auto figure = _project->findFigure(newName);

	deleteFigure(figureName);
	createFigure(figure);
}

void Plot::removeFigure(const QString figureName) {
	deleteFigure(figureName);
}

void Plot::changeCurrentFigure(const QString &currentFigureName) {
	if(currentFigureName == "") return;

	updateFigure(currentFigureName);
	updateFigure(_currentFigureName);
	_currentFigureName = _project->currentFigureName();
}

void Plot::changeFigureVisibility(const QString figureName, bool visible) {
	layer(figureName)->setVisible(visible);
	layer(figureName)->replot();
}

void Plot::changeCurveParameters(const QString curveName,
		bool showPoints, bool connectPoints, bool showVectors, bool closed, bool showNumbering, int numberingInterval) {
	updateFigure(curveName);
}

void Plot::changeCurveTolerance(const QString curveName, double upperTolerance, double lowerTolerance) {
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
}

void Plot::deleteFigure(const QString &figureName) {
	auto layerToDelete = layer(figureName);

	clearFiguresFromLayer(figureName);

	layer(figureName)->replot();

	removeLayer(layerToDelete);
}

void Plot::addCurveLayer(const CurveFigure &curveFigure) {
	auto curve = new Curve(xAxis, yAxis);

	double penWidth;
	if(_project->currentFigureName() == curveFigure.name()) {
		penWidth = _currentFigurePenWidth;
	} else {
		penWidth = _penWidth;
	}

	auto brush = QBrush(curveFigure.color());
	auto pen = QPen(brush, penWidth);
	curve->setPen(pen);
	
	for(const auto &point : curveFigure.points()) {
		curve->addData(point.x, point.y);
	}
	
	if(curveFigure.isConnectPoints()) {
		curve->connectPoints();
	}
	
	if(curveFigure.isShowPoints()) {
		curve->showPoints();
	}
	
	addGraph(curve->keyAxis(), curve->valueAxis());

	layer(curveFigure.name())->replot();
}

void Plot::addPointLayer(const PointFigure &pointFigure) {
	auto circleFigure = CircleFigure(pointFigure.name(), pointFigure.point(), _pointRadius);

	auto pointColor = pointFigure.color();
	circleFigure.setColor(pointColor);

	addCircleLayer(circleFigure);
}

void Plot::addLineLayer(const LineFigure &lineFigure) {
	auto line = new QCPItemLine(this);

	double penWidth;
	if(_project->currentFigureName() == lineFigure.name()) {
		penWidth = _currentFigurePenWidth;
	} else {
		penWidth = _penWidth;
	}

	auto brush = QBrush(lineFigure.color());
	auto pen = QPen(brush, penWidth);
	line->setPen(pen);

	auto startPoint = lineFigure.start();
	auto endPoint = lineFigure.end();

	line->start->setCoords(startPoint.x, startPoint.y);
	line->end->setCoords(endPoint.x, endPoint.y);

	layer(lineFigure.name())->replot();
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
 
	layer(circleFigure.name())->replot();
}

void Plot::calculateCircleBox(const CircleFigure &circleFigure, QCPItemEllipse *circle) {
	auto center = circleFigure.centre();
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

Plot::Curve::Curve(QCPAxis *keyAxis, QCPAxis *valueAxis) : QCPCurve(keyAxis, valueAxis) {
	setLineStyle(LineStyle::lsNone);

	const auto scatterShape = QCPScatterStyle::ScatterShape::ssNone;
	const auto scatterStyle = QCPScatterStyle(scatterShape, _scatterSize);
	setScatterStyle(scatterStyle);
	setScatterSkip(_scatterSkip);
}

void Plot::Curve::connectPoints() {
	setLineStyle(LineStyle::lsLine);
}

void Plot::Curve::showPoints() {
	const auto scatterShape = QCPScatterStyle::ScatterShape::ssSquare;
	const auto scatterStyle = QCPScatterStyle(scatterShape, _scatterSize);
	setScatterStyle(scatterStyle);
}

void Plot::onItemClicked(QCPAbstractItem *item, QMouseEvent *event) {
	auto figureName = item->layer()->name();
	emit currentFigureChanged(figureName);
	updateFigure(figureName);
	_currentFigureName = figureName;
}

void Plot::onPlottableClicked(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event) {
	auto figureName = plottable->layer()->name();
	emit currentFigureChanged(figureName);
	updateFigure(figureName);
	_currentFigureName = figureName;
}
