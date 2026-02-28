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

void Point::shift(double x, double y, double z) {
	this->x += x;
	this->y += y;
	this->z += z;
}

void Point::rotate(double angle, double x, double y, double z) {
	Eigen::Vector3d point(this->x, this->y, this->z);
	Eigen::Vector3d axis(0, 0, 1);
	Eigen::AngleAxisd rotation(angle * M_PI / 180, axis);
	Eigen::Vector3d translation(x, y, z);
	point -= translation;
	Eigen::Vector3d rotatedPoint = rotation * point;
	rotatedPoint += translation;

	this->x = rotatedPoint[0];
	this->y = rotatedPoint[1];
	this->z = rotatedPoint[2];
}

void Point::alignment(double angle, double offsetX, double offsetY) {
	shift(offsetX, offsetY, 0);
	rotate(angle);
	
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

void CurvePoint::shift(double x, double y, double z) {
	this->x += x;
	this->y += y;
	this->z += z;
}

void CurvePoint::rotate(double angle, double x, double y, double z) {
	Eigen::Vector3d point(this->x, this->y, this->z);
	Eigen::Vector3d normal(this->i, this->j, this->k);
	Eigen::Vector3d axis(0, 0, 1);
	Eigen::AngleAxisd rotation(angle * M_PI / 180, axis);
	Eigen::Vector3d translation(x, y, z);
	point -= translation;
	Eigen::Vector3d rotatedPoint = rotation * point;
	rotatedPoint += translation;
	Eigen::Vector3d rotatedNormal = rotation * normal;

	this->x = rotatedPoint[0];
	this->y = rotatedPoint[1];
	this->z = rotatedPoint[2];

	this->i = rotatedNormal[0];
	this->j = rotatedNormal[1];
	this->k = rotatedNormal[2];
}

void CurvePoint::alignment(double angle, double offsetX, double offsetY) {
	shift(offsetX, offsetY, 0);
	rotate(angle);
}

void CurveFigure::changePoints(QVector<CurvePoint> newPoints) {
	_points = newPoints;
}

void CurveFigure::changePoint(int index, CurvePoint updatePoint) {
	_points[index] = updatePoint;
}

void CurveFigure::assignToleranceToSegment(double upperTolerance, double lowerTolerance) {
	for(int i = 0; i < _points.length(); i++) {
		_points[i].ut = upperTolerance;
		_points[i].lt = lowerTolerance;
	}
}

const QString CurveFigure::exportToFLR(int precision) const {
	QString result;
	for(auto i = 0; i < _points.length(); i++) {
		auto point = _points[i];
		auto index = i + 1;
		result += "  , " + name() + "-" + QString::number(index) + ", 0, T, " + QString::number(0, 'f', precision) + ", "
			+ QString::number(point.ut, 'f', precision) + ", " + QString::number(point.lt, 'f', precision) + ", "
			+ QString::number(point.dev, 'f', precision) + ", 0, 0\n";
	}
	return result;
}

CurveFigure::CurveFigure() {
	this->_devMultiplier = 0;
}

CurveFigure::CurveFigure(QString name, QVector<CurvePoint> points, double devMultiplier) : Figure(name) {
	_points = points;
	_devMultiplier = devMultiplier;
}

void CurveFigure::shift(double x, double y, double z) {
	auto points = this->points();
	for(auto &point : points) {
		point.shift(x, y, z);
	}
	changePoints(points);
}

void CurveFigure::rotate(double angle, double x, double y, double z) {
	auto points = this->points();
	for(auto &point : points) {
		point.rotate(angle, x, y, z);
	}
	changePoints(points);
}

void CurveFigure::alignment(double angle, double offsetX, double offsetY) {
	auto points = this->points();
	for(auto &point : points) {
		point.alignment(angle, offsetX, offsetY);
	}
	changePoints(points);
}

void CurveFigure::edit(QMap<QString, QString> paramsChanged) {
	for(auto key : paramsChanged.keys()) {
		if(!key.contains("Point")) {
			continue;
		}
		auto index = key.split("Point")[1].toInt() - 1;
		auto pointStr = paramsChanged[key].split(",");
		auto point = CurvePoint {
			pointStr[0].toDouble(), pointStr[1].toDouble(), pointStr[2].toDouble(),
			pointStr[3].toDouble(), pointStr[4].toDouble(), pointStr[5].toDouble(),
			pointStr[6].toDouble(), pointStr[7].toDouble(), pointStr[8].toDouble()
		};
		changePoint(index, point);
	}
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

void PointFigure::shift(double x, double y, double z) {
	_point.shift(x, y, z);
}

void PointFigure::rotate(double angle, double x, double y, double z) {
	_point.rotate(angle, x, y, z);
}

void PointFigure::alignment(double angle, double offsetX, double offsetY) {
	_point.alignment(angle, offsetX, offsetY);
}

void PointFigure::edit(QMap<QString, QString> paramsChanged) {
	paramsChanged.contains("X") ? _point.x = paramsChanged.value("X").toDouble() : 0;
	paramsChanged.contains("Y") ? _point.y = paramsChanged.value("Y").toDouble() : 0;
	paramsChanged.contains("Z") ? _point.z = paramsChanged.value("Z").toDouble() : 0;
	paramsChanged.contains("I") ? _point.i = paramsChanged.value("I").toDouble() : 0;
	paramsChanged.contains("J") ? _point.j = paramsChanged.value("J").toDouble() : 0;
	paramsChanged.contains("K") ? _point.k = paramsChanged.value("K").toDouble() : 0;
}


LineFigure::LineFigure() {
	_length = qInf();
}

LineFigure::LineFigure(QString name, Point origin, Point direction, double length) : Figure(name) {
	_origin = origin;	
	_direction = direction;
	_length = length;
}

LineFigure::LineFigure(QString name, Point startPoint, Point endPoint) : Figure(name) {
	_length = hypot(startPoint.x - endPoint.x, startPoint.y - endPoint.y);
	_direction = Point((startPoint.x - endPoint.x) / _length, (startPoint.y - endPoint.y) / _length);
	_origin = Point((startPoint.x + endPoint.x) / 2, (startPoint.y + endPoint.y) / 2);
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

void LineFigure::shift(double x, double y, double z) {
	_origin.shift(x, y, z);
}

void LineFigure::rotate(double angle, double x, double y, double z) {
	_origin.rotate(angle, x, y, z);
	_direction.rotate(angle);
}

void LineFigure::alignment(double angle, double offsetX, double offsetY) {
	_origin.alignment(angle, offsetX, offsetY);
	_direction.rotate(angle);
}

void LineFigure::edit(QMap<QString, QString> paramsChanged) {
	paramsChanged.contains("X") ? _origin.x = paramsChanged.value("X").toDouble() : 0;
	paramsChanged.contains("Y") ? _origin.y = paramsChanged.value("Y").toDouble() : 0;
	paramsChanged.contains("Z") ? _origin.z = paramsChanged.value("Z").toDouble() : 0;
	paramsChanged.contains("I") ? _direction.x = paramsChanged.value("I").toDouble() : 0;
	paramsChanged.contains("J") ? _direction.y = paramsChanged.value("J").toDouble() : 0;
	paramsChanged.contains("K") ? _direction.z = paramsChanged.value("K").toDouble() : 0;
	paramsChanged.contains("Length") ? _length = paramsChanged.value("Length").toDouble() : 0;
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

void DimFigure::addValue(const Value &value) {
	for(auto i = 0; i < _values.length(); i++) {
		if(_values[i].type == value.type) {
            _values.replace(i, value);
			return;
		}
	}
	_values.append(value);
}

void DimFigure::addValues(const QVector<Value> &values) {
	_values.append(values);
}

const QVector<DimFigure::Value>& DimFigure::values() const {
	return _values;
}

const DimFigure::DimType DimFigure::dimType() const {
	return _dimType;
}

bool DimFigure::isOnlyLabel() const {
	return _onlyLabel;
}

void DimFigure::setOnlyLabel(bool onlyLabel) {
	_onlyLabel = onlyLabel;
}

bool DimFigure::isShowTols() const {
	return _showTols;
}

void DimFigure::setShowTolerances(bool showTols) {
	_showTols = showTols;
}

bool DimFigure::isFreePosition() const {
	return _freePosition;
}

void DimFigure::setFreePosition(bool freePosition) {
	_freePosition = freePosition;
}

void DimFigure::setDimType(const DimType &dimType) {
	_dimType = dimType;
}

int DimFigure::A() {
	return _a;
}

void DimFigure::setA(int A) {
	_a = A;
}

QList<DimFigure::Value*>* DimFigure::setTypeAndGetValueNames() {
	auto values = new QList<Value*>();
	switch(_a) {
		case 1:
			setDimType(DimType::Position);
			values->append({
				new Value(ValueType::X),
				new Value(ValueType::Y),
				new Value(ValueType::Z),
				new Value(ValueType::PolarRad),
				new Value(ValueType::PolarAngle),
				new Value(ValueType::Diameter),
				new Value(ValueType::Radius)
				});
			break;
		case 2:
			setDimType(DimType::Radius);
			setRenderType(RenderType::Callout);
			values->append(new Value(ValueType::Radius));
			break;
		case 3:
			setDimType(DimType::Diameter);
			setRenderType(RenderType::Callout);
			values->append(new Value(ValueType::Diameter));
			break;
		case 6:
			setDimType(DimType::Distance);
			values->append(new Value(ValueType::Length));
			break;
		case 8:
			setDimType(DimType::AngleTo);
			values->append({
				new Value(ValueType::Angle0),
				new Value(ValueType::Angle90),
				new Value(ValueType::Angle180),
				new Value(ValueType::Angle270)
				});
			break;
		case 10:
			setDimType(DimType::DistanceBetweenCurvePoints);
			values->append(new Value(ValueType::Length));
            break;
		case 11:
			setDimType(DimType::AngleBetween);
			values->append({
				new Value(ValueType::AngleTo),
				new Value(ValueType::AngleFrom)
				});
			break;
		case 16:
			setDimType(DimType::Perimeter);
			setRenderType(RenderType::Callout);
			values->append(new Value(ValueType::Length));
			break;
		case 19:
			setDimType(DimType::BestFitData);
			values->append({
				new Value(ValueType::dX),
				new Value(ValueType::dY),
				new Value(ValueType::Rotation)
				});
			break;
		case 20:
			setDimType(DimType::Form);
			setRenderType(RenderType::Table);
			values->append({
				new Value(ValueType::MinMax),
				new Value(ValueType::Form),
				new Value(ValueType::Min),
				new Value(ValueType::Max),
				new Value(ValueType::MaxAbs),
				new Value(ValueType::SupUT),
				new Value(ValueType::InfLT)
				});
			break;
		case 21:
			setDimType(DimType::TruePosition);
			values->append({
				new Value(ValueType::X),
				new Value(ValueType::Y),
				new Value(ValueType::TruePosition)
				});
			break;
		default:
			QMessageBox::critical(nullptr, "Error", "Unknown DIM type");
			break;
	}
	return values;
}

const DimFigure::RenderType DimFigure::renderType() const {
	return _renderType;
}

void DimFigure::setRenderType(const RenderType &renderType) {
	_renderType = renderType;
}

const QVector<CurvePoint> DimFigure::convertDimValueToPoints() {
	QVector<CurvePoint> points;
    for (auto i = 0; i < _values.length(); i++) {
		points.push_back(CurvePoint(_values[i].isShow, _values[i].nominal, 0, 0, 0,
			_values[i].measurement, 0, _values[i].upperTolerance, _values[i].lowerTolerance));
    }
	return points;
}

const QString DimFigure::exportToFLR(int precision) const {
	QString result;
	for(auto &value : _values) {
		result += "  , " + name() + ", 0, " + value.typeToFLR() + ", " + QString::number(value.nominal, 'f', precision) + ", "
			+ QString::number(value.upperTolerance, 'f', precision) + ", " + QString::number(value.lowerTolerance, 'f', precision) + ", "
			+ QString::number(value.measurement, 'f', precision) + ", 0, 0\n";
	}
	return result;
}

void DimFigure::shift(double x, double y, double z) {
	_labelPoint.shift(x, y, z);
}

void DimFigure::rotate(double angle, double x, double y, double z) {
	_labelPoint.rotate(angle, x, y, z);
}

void DimFigure::alignment(double angle, double offsetX, double offsetY) {
	_labelPoint.alignment(angle, offsetX, offsetY);
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

void CircleFigure::setCenterCross(bool hasCenterCross) {
	_centerCross = hasCenterCross;
}

bool CircleFigure::centerCross() const {
	return _centerCross;
}

void CircleFigure::shift(double x, double y, double z) {
	_center.shift(x, y, z);
}

void CircleFigure::rotate(double angle, double x, double y, double z) {
	_center.rotate(angle, x, y, z);
	_normal.rotate(angle);
}

void CircleFigure::alignment(double angle, double offsetX, double offsetY) {
	_center.alignment(angle, offsetX, offsetY);
	_normal.rotate(angle);
}

void CircleFigure::edit(QMap<QString, QString> paramsChanged) {
	paramsChanged.contains("X") ? _center.x = paramsChanged.value("X").toDouble() : 0;
	paramsChanged.contains("Y") ? _center.y = paramsChanged.value("Y").toDouble() : 0;
	paramsChanged.contains("Z") ? _center.z = paramsChanged.value("Z").toDouble() : 0;
	paramsChanged.contains("I") ? _normal.x = paramsChanged.value("I").toDouble() : 0;
	paramsChanged.contains("J") ? _normal.y = paramsChanged.value("J").toDouble() : 0;
	paramsChanged.contains("K") ? _normal.z = paramsChanged.value("K").toDouble() : 0;
	paramsChanged.contains("Radius") ? _radius = paramsChanged.value("Radius").toDouble() : 0;
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

DimFigure::DimFigure(const QString &name, const Point &labelPoint, const Figure *firstReference, const Figure *secondReference) : Figure(name) {
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
		false, false, false, false, false, false, convertDimValueToPoints()
	};
	return nullptr;
}

const Point & DimFigure::labelPoint() const {
	return _labelPoint;
}

void DimFigure::setLabelPoint(const Point &point) {
	_labelPoint = point;
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

DimFigure::Value::Value(DimFigure::ValueType valueType, bool newIsShow, double newMeasurement, double newNominal,
	double newUT, double newLT) {
	type = valueType;
    isShow = newIsShow;
    measurement = newMeasurement;
    nominal = newNominal;
    upperTolerance = newUT;
    lowerTolerance = newLT;
}

const QString DimFigure::Value::typeToFLR() const {
	return QString(); // TODO
}

TextFigure::TextFigure(){ }

TextFigure::TextFigure(QString name, QString text, Point position, double textSize, const Figure *reference, double imageWidth, double imageHeight, double imageZoom) {
	setName(name);
	_text = text;
    _position = position;
    _textSize = textSize;
    _reference = reference;
    _imageWidth = imageWidth;
    _imageHeight = imageHeight;
    _imageZoom = imageZoom;
}

QString TextFigure::text() const {
    return _text;
}

void TextFigure::setText(QString text) {
    _text = text;
}

Point TextFigure::position() const {
    return _position;
}

void TextFigure::setPosition(Point position) {
    _position = position;
}

double TextFigure::textSize() const {
    return _textSize;
}

void TextFigure::setTextSize(double textSize) {
    _textSize = textSize;
}

const Figure* TextFigure::reference() const {
    return _reference;
}

void TextFigure::setReference(Figure *reference) {
    _reference = reference;
}

double TextFigure::imageWidth() const {
    return _imageWidth;
}

void TextFigure::setImageWidth(double imageWidth) {
    _imageWidth = imageWidth;
}

double TextFigure::imageHeight() const {
    return _imageHeight;
}

void TextFigure::setImageHeight(double imageHeight) {
    _imageHeight = imageHeight;
}

double TextFigure::imageZoom() const {
    return _imageZoom;
}	

void TextFigure::setImageZoom(double imageZoom) {
    _imageZoom = imageZoom;
}

void TextFigure::shift(double x, double y, double z) {
    _position.shift(x, y, z);
}

void TextFigure::rotate(double angle, double x, double y, double z) {
    _position.rotate(angle);
}

void TextFigure::alignment(double angle, double offsetX, double offsetY) {
    _position.alignment(angle, offsetX, offsetY);
}

FigureSettings* TextFigure::settings() {
	auto rif = reference() ? reference()->name() : "";
	const QVector<CurvePoint> empty = {};
	return new FigureSettings { name(), "TXT", text(), rif, ColorTranslator::getIntFromColor(&color()), 0,
		position().x, position().y, position().z, 0, 0, 1, 0, 0, 0, textSize(), imageWidth(), imageHeight(), imageZoom(), 0, 0, 0, 0, 0,
		isVisible(), false, false, false, false,
		false, false, false, false, false, false, empty
	};
}
