#include "curve/pch.h"
#include "curve/plot.h"

Plot::Plot(QWidget *parent) : QCustomPlot(parent), _project(nullptr) {
	const int axisSize = 100;

	xAxis->setRange(-axisSize, axisSize);
	yAxis->setRange(-axisSize, axisSize);
	
	setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables | QCP::iSelectItems);
}

void Plot::setProject(Project *_project) {
	this->_project = _project;

	connect(_project, &Project::figureAdded, this, &Plot::addFigure);
	connect(_project, &Project::figureRenamed, this, &Plot::renameFigure);
	connect(_project, &Project::figureAboutToBeRemoved, this, &Plot::removeFigure);
	connect(_project, &Project::figureVisibilityChanged, this, &Plot::changeFigureVisibility);
	connect(_project, &Project::curveParametersChanged, this, &Plot::changeCurveParameters);
}

void Plot::addFigure(Figure* figure) {
	createFigure(figure);
};

void Plot::renameFigure(const QString figureName, const QString newName) {
	updateFigure(newName);
};

void Plot::removeFigure(const QString figureName) {
	deleteFigure(figureName);
}

void Plot::changeFigureVisibility(const QString figureName, bool visible) {
	layer(figureName)->setVisible(visible);
	layer(figureName)->replot();
}

void Plot::changeCurveParameters(const QString curveName, bool showPoints, bool connectPoints, bool showVectors, bool closed, bool showNumbering, int numberingInterval) {
	updateFigure(curveName);
}

void Plot::createFigure(const Figure *figure) {
	auto isNewLayer = addLayer(figure->name());

	if(isNewLayer == false) {
		qCritical() << figure->name() << "already exists";
		return;
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

	deleteFigure(figureName);
	createFigure(figure);
}

void Plot::deleteFigure(const QString &figureName) {
	auto layerToDelete = layer(figureName);

	if(layerToDelete == nullptr) {
		qCritical() << figureName << "doesn't exist";
		return;
	}

	QCPAbstractPlottable *abstractPlottable;
	QCPAbstractItem *abstractItem;
	for(auto &children : layerToDelete->children()) {
		if(abstractPlottable = dynamic_cast<QCPAbstractPlottable*>(children)) {
			removePlottable(abstractPlottable);
		} else if(abstractItem = dynamic_cast<QCPAbstractItem*>(children)) {
			removeItem(abstractItem);
		}
	}

	layerToDelete->replot();
	removeLayer(layerToDelete);
}

void Plot::addCurveLayer(const CurveFigure &curveFigure) {
	auto curve = new Curve(xAxis, yAxis);

	auto brush = QBrush(curveFigure.color());
	auto pen = QPen(brush, _penWidth);
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

	auto brush = QBrush(lineFigure.color());
	auto pen = QPen(brush, _penWidth);
	line->setPen(pen);

	auto startPoint = lineFigure.start();
	auto endPoint = lineFigure.end();

	line->start->setCoords(startPoint.x, startPoint.y);
	line->end->setCoords(endPoint.x, endPoint.y);

	layer(lineFigure.name())->replot();
}

void Plot::addCircleLayer(const CircleFigure &circleFigure) {
	auto circle = new QCPItemEllipse(this);

	auto brush = QBrush(circleFigure.color());
	auto pen = QPen(brush, _penWidth);
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
