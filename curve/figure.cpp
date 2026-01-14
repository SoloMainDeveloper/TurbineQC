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

void LineFigure::setHead(const QCPLineEnding &head) {
    _head = head;
}

void LineFigure::setTail(const QCPLineEnding &tail) {
    _tail = tail;
}

const QCPLineEnding & LineFigure::head() const {
	return _head;
}

const QCPLineEnding & LineFigure::tail() const {
	return _tail;
}

const Figure* DimFigure::firstReference() const {
	return _firstReference;
}

const Figure* DimFigure::secondReference() const {
	return _secondReference;
}

void DimFigure::addValue(const Value & value) {
	_values.push_back(value);
}

const QVector<DimFigure::Value>& DimFigure::values() const {
	return _values;
}

const DimFigure::DimType DimFigure::dimType() const {
	return _dimType;
}

const QVector<CurvePoint> DimFigure::convertDimValueToPoints(const QVector<Value>& values) {
	QVector<CurvePoint> points;
    for (auto i = 0; i < values.length(); i++) {
		points.push_back(CurvePoint(values[i].isShow , values[i].nominal, 0, 0, 0,
			values[i].measurement, 0, values[i].upperTolerance, values[i].lowerTolerance));
    }
	return points;
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

bool CurveFigure::isShowTolerances() const {
	return _isShowTolerances;
}

void CurveFigure::setShowTolerances(bool showTolerances) {
	_isShowTolerances = showTolerances;
}

bool CurveFigure::isShowDeviations() const {
	return _isShowDeviations;
}

void CurveFigure::setShowDeviations(bool showDeviations) {
	_isShowDeviations = showDeviations;
}

bool CurveFigure::isShowNumericalDeviations() const {
	return _isShowNumericalDeviations;
}

void CurveFigure::setShowNumericalDeviations(bool isShowNumericalDeviations) {
	_isShowNumericalDeviations = isShowNumericalDeviations;
}

int CurveFigure::numericalInterval() const {
	return _numericalInterval;
}

void CurveFigure::setNumericalInterval(int numericalInterval) {
	_numericalInterval = numericalInterval;
}

bool CurveFigure::isConnectDeviations() const {
	return _isConnectDeviations;
}

void CurveFigure::setConnectDeviations(bool connectDeviations) {
	_isConnectDeviations = connectDeviations;
}

void CurveFigure::setHighLightOut(bool highLightOut) {
    _isHighLightOut = highLightOut;
}

bool CurveFigure::isHighLightOut() const {
	return _isHighLightOut;
}

void CurveFigure::setAmplification(double amplification) {
	_amplification = amplification;
}

double CurveFigure::amplification() const {
	return _amplification;
}

FigureSettings* CurveFigure::settings() {
	return new FigureSettings { name(), "CRV", "", "", ColorTranslator::getIntFromColor(&color()), numberingInterval(),
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
		isVisible(), isShowPoints(), isConnectPoints(), isShowVectors(), isClosed(),
		isShowNumbering(), true, false, false, false, false, points()
	};
}

FigureSettings* PointFigure::settings() {
	const QVector<CurvePoint> empty = { };
	return new FigureSettings { name(), "PNT", "", "", ColorTranslator::getIntFromColor(&color()), 0,
		point().x, point().y, point().z, point().i, point().j, point().k, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		isVisible(), false, false, false, false,
		false, false, false, false, false, false, empty
	};
}

FigureSettings* LineFigure::settings() {
	const QVector<CurvePoint> empty = {};
	return new FigureSettings { name(), "LIN", "", "", ColorTranslator::getIntFromColor(&color()), 0,
		origin().x, origin().y, origin().z, direction().x, direction().y, direction().z, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		isVisible(), false, false, false, false,
		false, false, false, false, false, false, empty
	};
}

DimFigure::DimFigure(const QString &name, const DimType type, const Point &labelPoint, const Figure *firstReference,
	const Figure *secondReference) : Figure(name) {
		_dimType = type;
		_labelPoint = labelPoint;
		_firstReference = firstReference;
		_secondReference = secondReference;
}

FigureSettings* DimFigure::settings() {
	auto point = labelPoint();
	auto rif = firstReference() ? firstReference()->name() : "";
	auto rif1 = secondReference() ? secondReference()->name() : "";
	double A;

	if(_dimType == DimType::Position) A = 1;
	else if(_dimType == DimType::Radius) A = 2;
	else if(_dimType == DimType::Diameter) A = 3;
	else if(_dimType == DimType::Distance) A = 6;
	else if(_dimType == DimType::AngleTo) A = 8;
	else if(_dimType == DimType::AngleBetween) A = 11;
	else if(_dimType == DimType::Perimeter) A = 16;
	else if(_dimType == DimType::BestFitData) A = 19;
	else if(_dimType == DimType::Form) A = 20;
	else if(_dimType == DimType::TruePosition) A = 21;
	return new FigureSettings { name(), "DIM", rif, rif1, 0, 0,
		point.x, point.y, point.z, 0, 0, 1, 0, 0, 0, A, 0, 0, 0, 0, 0, 0, 0, 0,
		isVisible(), false, false, false, false,
		false, false, false, false, false, false, convertDimValueToPoints(values())
	};
	return nullptr;
}

const Point & DimFigure::labelPoint() const {
	return _labelPoint;
}

FigureSettings* Figure::settings() {
	const QVector<CurvePoint> empty = {};
	return new FigureSettings { "ERROR FIGURE", "ERROR", "", "", ColorTranslator::getIntFromColor(&color()), 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		false, false, false, false, false,
		false, false, false, false, false, false, empty
	};
}

FigureSettings* CircleFigure::settings() {
	const QVector<CurvePoint> empty = {};
	return new FigureSettings { name(), "CIR", "", "", ColorTranslator::getIntFromColor(&color()), 0,
		center().x, center().y, center().z, normal().x, normal().y, normal().z, 0, 0, 0, radius() * 2, 0, 0, 0, 0, 0, 0, 0, 0,
		isVisible(), false, false, false, false,
		false, false, false, false, false, false, empty
	};
}

DimFigure::Value::Value(ValueType valueType) {
	type = valueType;
}
