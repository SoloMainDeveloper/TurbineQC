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

CurveFigure::CurveFigure(QString name, QVector<Point> points, double devMultiplier) : Figure(name) {
	_points = points;
	_devMultiplier = devMultiplier;
}

QVector<Point> CurveFigure::points() {
	return _points;
}

double CurveFigure::devMultiplier() const {
	return _devMultiplier;
}

PointFigure::PointFigure(QString name, Point point) : Figure(name) {
	_point = point;
}

QVector<Point> PointFigure::points() {
	return QVector<Point> { _point };
}

const Point PointFigure::point() {
	return _point;
}

LineFigure::LineFigure(QString name, Point start, Point end) : Figure(name) {
	_start = start;
	_end = end;
}

QVector<Point> LineFigure::points() {
	return QVector<Point> { _start, _end };
}

Point LineFigure::start() const {
	return _start;
}

Point LineFigure::end() const {
	return _end;
}

CircleFigure::CircleFigure(QString name, Point centre, double radius) : Figure(name) {
	_centre = centre;
	_radius = radius;
}

QVector<Point> CircleFigure::points() {
	return QVector<Point> { _centre };
}

Point CircleFigure::centre() const {
	return _centre;
}

double CircleFigure::radius() const {
	return _radius;
}

QVector<Point> Figure::points() {
	throw _EXCEPTION_; //not implemented figure
}

Figure::Figure(QString name, bool isVisible) {
	_name = name;
	_isVisible = isVisible;
}

const QString& Figure::name() {
	return _name;
}

void Figure::setName(QString name) {
	_name = name;
}

bool Figure::isVisible() {
	return _isVisible;
}

void Figure::toggleVisible() {
	_isVisible = !_isVisible;
}
