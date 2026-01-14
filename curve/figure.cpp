#include "curve/pch.h"
#include "figure.h"

Point::Point() : Point(0.0, 0.0) {
	// empty
}

Point::Point(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

CurvePoint::CurvePoint() : CurvePoint(0.0, 0.0) {
	// empty
}

CurvePoint::CurvePoint(double x, double y, double z, double i, double j, double k, double dev, double lt, double ut) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->i = i;
	this->j = j;
	this->k = k;
	this->dev = dev;
	this->lt = lt;
	this->ut = ut;
}

CurvePoint::operator Point() {
	return Point(x, y, z);
}

void CurveFigure::changePoints(QVector<CurvePoint> newPoints) {
	_points = newPoints;
}

void CurveFigure::assignToleranceToSegment(double upperTolerance, double lowerTolerance) {
	for(int i = 0; i < _points.length(); i++) {
		_points[i].ut = upperTolerance;
		_points[i].lt = lowerTolerance;
	}
}

void CurveFigure::computeToleranceClouds() {
	_lowerTolerancePoints.clear();
	_upperTolerancePoints.clear();

	for(auto point = _points.begin(); point != _points.end(); point++) {
		auto upperOffsetPoint = *point;
		upperOffsetPoint.x += point->i * point->ut;
		upperOffsetPoint.y += point->j * point->ut;
		upperOffsetPoint.z += point->k * point->ut;
		_upperTolerancePoints.push_back(upperOffsetPoint);

		auto lowerOffsetPoint = *point;
		if(point->lt >= 0) { // fix bug. It should be negative, but curve send positive
			lowerOffsetPoint.x += point->i * point->lt;
			lowerOffsetPoint.y += point->j * point->lt;
			lowerOffsetPoint.z += point->j * point->lt;
		} else {
			lowerOffsetPoint.x += point->i * point->lt;
			lowerOffsetPoint.y += point->j * point->lt;
			lowerOffsetPoint.z += point->k * point->lt;
		}
		
		_lowerTolerancePoints.push_back(lowerOffsetPoint);
	}
}

const QVector<CurvePoint>& CurveFigure::upperTolerance() const {
	return _upperTolerancePoints;
}

const QVector<CurvePoint>& CurveFigure::lowerTolerance() const {
	return _lowerTolerancePoints;
}

CurveFigure::CurveFigure() {
	this->_devMultiplier = 0;
}

CurveFigure::CurveFigure(QString name, QVector<CurvePoint> points, double devMultiplier) : Figure(name) {
	_points = points;
	_devMultiplier = devMultiplier;
}

const QVector<CurvePoint>& CurveFigure::points() const {
	return _points;
}

double CurveFigure::devMultiplier() const {
	return _devMultiplier;
}

PointFigure::PointFigure(QString name, CurvePoint point) : Figure(name) {
	_point = point;
}

CurvePoint PointFigure::point() const {
	return _point;
}

LineFigure::LineFigure() {
	_length = qInf();
}

LineFigure::LineFigure(QString name, Point origin, Point direction, double length) : Figure(name) {
	_origin = origin;	
	_direction = direction;
	_length = length;
}

void LineFigure::setLength(double length) {
	if(length >= 0) {
		_length = length;
    }
}

void LineFigure::setOrigin(Point origin) {
	_origin = origin;
}

void LineFigure::setDirection(Point direction) {
	_direction = direction;
}

Point LineFigure::origin() const {
	return _origin;
}

Point LineFigure::direction() const {
	return _direction;
}

double LineFigure::length() const {
	return _length;
}

DimFigure::DimFigure(QString name, PointFigure *start, PointFigure *end) {
	this->setName(name);
	_start = start;
	_end = end;
}

PointFigure* DimFigure::start() const {
	return _start;
}

PointFigure* DimFigure::end() const {
	return _end;
}

CircleFigure::CircleFigure() {
	this->_radius = 0;
}

CircleFigure::CircleFigure(QString name, Point center, Point normal, double radius) : Figure(name) {
	_center = center;
	_normal = normal;
	_radius = radius;
}

Point CircleFigure::center() const {
	return _center;
}

Point CircleFigure::normal() const {
	return _normal;
}

double CircleFigure::radius() const {
	return _radius;
}

Figure::Figure() {
	_isVisible = true;
}

Figure::Figure(QString name, bool isVisible) {
	_name = name;
	_isVisible = isVisible;
}

const QString& Figure::name() const {
	return _name;
}

void Figure::setName(QString name) {
	_name = name;
}

bool Figure::isVisible() const {
	return _isVisible;
}

void Figure::setVisible(bool visible) {
	_isVisible = visible;
}

void Figure::toggleVisible() {
	_isVisible = !_isVisible;
}

void Figure::setColor(QColor color) {
    _color = color;
}

const QColor& Figure::color() const {
    return _color;
}

bool CurveFigure::isShowPoints() const {
	return _isShowPoints;
}

void CurveFigure::setShowPoints(bool showPoints) {
	_isShowPoints = showPoints;
}

void CurveFigure::toggleShowPoints() {
	_isShowPoints = !_isShowPoints;
}

bool CurveFigure::isConnectPoints() const {
	return _isConnectPoints;
}

void CurveFigure::setConnectPoints(bool connectPoints) {
	_isConnectPoints = connectPoints;
}

void CurveFigure::toggleConnectPoints() {
	_isConnectPoints = !_isConnectPoints;
}

bool CurveFigure::isShowVectors() const {
	return _isShowVectors;
}

void CurveFigure::setShowVectors(bool showVectors) {
	_isShowVectors = showVectors;
}

void CurveFigure::toggleShowVectors() {
	_isShowVectors = !_isShowVectors;
}

bool CurveFigure::isClosed() const {
	return _isClosed;
}

void CurveFigure::setClosed(bool closed) {
	_isClosed = closed;
}

void CurveFigure::toggleClosed() {
	_isClosed = !_isClosed;
}

bool CurveFigure::isShowNumbering() const {
	return _isShowNumbering;
}

void CurveFigure::setShowNumbering(bool showNumbering) {
	_isShowNumbering = showNumbering;
}

void CurveFigure::toggleShowNumbering() {
	_isShowNumbering = !_isShowNumbering;
}

int CurveFigure::numberingInterval() const {
	return _numberingInterval;
}

void CurveFigure::setNumberingInterval(int numberingInterval) {
	_numberingInterval = numberingInterval;
}

FigureSettings* CurveFigure::settings() {
	return new FigureSettings { name(), "CRV", "", "", 0, numberingInterval(), //colour = 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
		isVisible(), isShowPoints(), isConnectPoints(), isShowVectors(), isClosed(),
		isShowNumbering(), true, false, false, false, false, points()
	};
}

FigureSettings* PointFigure::settings() {
	const QVector<CurvePoint> empty = { };
	return new FigureSettings { name(), "PNT", "", "", 0, 0,
		point().x, point().y, point().z, point().i, point().j, point().k, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		isVisible(), false, false, false, false,
		false, false, false, false, false, false, empty
	};
}

FigureSettings* LineFigure::settings() {
	const QVector<CurvePoint> empty = {};
	return new FigureSettings { name(), "LIN", "", "", 0, 0,
		origin().x, origin().y, origin().z, direction().x, direction().y, direction().z, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		isVisible(), false, false, false, false,
		false, false, false, false, false, false, empty
	};
}

FigureSettings* DimFigure::settings() {
	const QVector<CurvePoint> point = { CurvePoint(1, 0, 0, 0, 0, length(), 0, 0, 0) };
	return new FigureSettings { name(), "DIM", start()->name(), end()->name(), 0, 0,
		origin().x, origin().y, origin().z, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0,
		isVisible(), false, false, false, false,
		false, false, false, false, false, false, point
	};
}

FigureSettings* Figure::settings() {
	const QVector<CurvePoint> empty = {};
	return new FigureSettings { "ERROR FIGURE", "ERROR", "", "", 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		false, false, false, false, false,
		false, false, false, false, false, false, empty
	};
}

FigureSettings* CircleFigure::settings() {
	const QVector<CurvePoint> empty = {};
	return new FigureSettings { name(), "CIR", "", "", 0, 0,
		center().x, center().y, center().z, normal().x, normal().y, normal().z, 0, 0, 0, radius() * 2, 0, 0, 0, 0, 0, 0, 0, 0,
		isVisible(), false, false, false, false,
		false, false, false, false, false, false, empty
	};
}
