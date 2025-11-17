#include "curve/pch.h"
#include "figure.h"

Point::Point(double x, double y, double z, double i, double j, double k, double dev, double lt, double ut) {
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

CurveFigure::CurveFigure() {
	this->_devMultiplier = 0;
}

CurveFigure::CurveFigure(QString name, QVector<Point> points, double devMultiplier) : Figure(name) {
	_points = points;
	_devMultiplier = devMultiplier;
}

QVector<Point> CurveFigure::points() const {
	return _points;
}

double CurveFigure::devMultiplier() const {
	return _devMultiplier;
}

PointFigure::PointFigure() {
}

PointFigure::PointFigure(QString name, Point point) : Figure(name) {
	_point = point;
}

QVector<Point> PointFigure::points() const {
	return QVector<Point> { _point };
}

const Point& PointFigure::point() const {
	return _point;
}

LineFigure::LineFigure() {
}

LineFigure::LineFigure(QString name, Point start, Point end) : Figure(name) {
	_start = start;
	_end = end;
}

QVector<Point> LineFigure::points() const {
	return QVector<Point> { _start, _end };
}

Point LineFigure::start() const {
	return _start;
}

Point LineFigure::end() const {
	return _end;
}

CircleFigure::CircleFigure() {
	this->_radius = 0;
}

CircleFigure::CircleFigure(QString name, Point centre, double radius) : Figure(name) {
	_centre = centre;
	_radius = radius;
}

QVector<Point> CircleFigure::points() const {
	return QVector<Point> { _centre };
}

Point CircleFigure::centre() const {
	return _centre;
}

double CircleFigure::radius() const {
	return _radius;
}

QVector<Point> Figure::points() const {
	throw _EXCEPTION_; //not implemented figure
}

Figure::Figure() {
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

void Figure::setColor(QColor& color) {
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
