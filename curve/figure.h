#pragma once
#include "colortranslator.h"

class Point {
public:
	double x, y, z;

	Point();
	Point(double x, double y, double z = 0.0);

	void shift(double x, double y, double z);
	void rotate(double angle, double x = 0, double y = 0, double z = 0);
	void alignment(double angle, double offsetX, double offsetY);
};

class CurvePoint {
public:
	double x, y, z;
	double i, j, k;
	double dev, lt, ut;

	CurvePoint();
	CurvePoint(double x, double y, double z = 0, double i = 0, double j = 0, double k = 0, double dev = 0, double lt = 0, double ut = 0);

	void shift(double x, double y, double z);
	void rotate(double angle, double x = 0, double y = 0, double z = 0);
	void alignment(double angle, double offsetX, double offsetY);
	
	explicit operator Point();
};

struct FigureSettings {
public:
	QString name, type, rif, rif1;
	int colour, numberEach;
	double x, y, z, u, v, w, l, m, n, a, b, c, d, e, f, amplitude, UT, LT;
	bool isVisible, showPoints, connectPoints, showVectors, isClosed,
		showNumbering, showTols, useGenTols, showDev, connectDev, highlightDev;
	const QVector<CurvePoint> points;
};

class Figure {
public:
	Figure();
	Figure(QString name, bool isVisible = true);
	virtual ~Figure() = default;
	virtual FigureSettings* settings();

	const QString& name() const;
	void setName(QString name);
	bool isVisible() const;
	void setVisible(bool visibility);
	void toggleVisible();
	void setColor(QColor color);
	const QColor& color() const;
	virtual void shift(double x, double y, double z) {};
	virtual void rotate(double angle, double x = 0, double y = 0, double z = 0) {};
	virtual void alignment(double angle, double offsetX, double offsetY) {};
	virtual void edit(QMap<QString, QString> paramsChanged) {};
	virtual void updateRefToParent(QString oldParentName, QString newParentName) {};
	const QList<Figure*>& children() const;
	QList<Figure*>& childrenMutable() const;
	void addChild(Figure *child) const;
	void removeChild(Figure *child) const;


private:
	QList<Figure*> *_children;
	QString _name;
	bool _isVisible;
	QColor _color = QColorConstants::Black;
};

class CurveFigure : public Figure {
public:
	CurveFigure();
	CurveFigure(QString name, QVector<CurvePoint> points, double devMultiplier = 1.0);
	virtual FigureSettings* settings();

	const QVector<CurvePoint>& points() const;
	double devMultiplier() const; 

	bool isShowPoints() const;
	void setShowPoints(bool showPoints);
	void toggleShowPoints();
	bool isConnectPoints() const;
	void setConnectPoints(bool connectPoints);
	void toggleConnectPoints();
	bool isShowVectors() const;
	void setShowVectors(bool showVectors);
	void toggleShowVectors();
	bool isClosed() const;
	void setClosed(bool closed);
	void toggleClosed();
	bool isShowNumbering() const;
	void setShowNumbering(bool showNumbering);
	void toggleShowNumbering();
	int numberingInterval() const;
	void setNumberingInterval(int numberingInterval);
	bool isShowTolerances() const;
	void setShowTolerances(bool showTolerances);
	bool isShowDeviations() const;
	void setShowDeviations(bool showDeviations);
	bool isShowNumericalDeviations() const;
	void setShowNumericalDeviations(bool isShowNumericalDeviations);
	int numericalInterval() const;
	void setNumericalInterval(int numericalInterval);
	bool isConnectDeviations() const;
	void setConnectDeviations(bool connectDeviations);
	void setHighLightOut(bool highLightOut);
	bool isHighLightOut() const;
	void setAmplification(double amplification);
	double amplification() const;
	
	void changePoints(QVector<CurvePoint> updatePoints);
	void changePoint(int index, CurvePoint updatePoint);
	void assignToleranceToSegment(double upperTolerance, double lowerTolerance);

	const QString exportToFLR(int precision) const;

	virtual void shift(double x, double y, double z);
	virtual void rotate(double angle, double x = 0, double y = 0, double z = 0);
	virtual void alignment(double angle, double offsetX, double offsetY);
	virtual void edit(QMap<QString, QString> paramsChanged);

private:
	QVector<CurvePoint> _points;
	double _devMultiplier;

	bool _isShowPoints = false;
	bool _isConnectPoints = true;
	bool _isShowVectors = false;
	bool _isClosed = false;
	bool _isShowNumbering = false;
	int _numberingInterval = 5;
	double _amplification = 1.0;
	bool _isShowTolerances = false;
	bool _isShowDeviations = false;
	bool _isShowNumericalDeviations = false;
	int _numericalInterval = 1;
	bool _isConnectDeviations = false;
	bool _isHighLightOut = false;
	double _perimeter = 0;
	double _square = 0;
};

class LineFigure : public Figure {
public:
	LineFigure();
	LineFigure(QString name, Point position, Point direction, double length);
	LineFigure(QString name, Point startPoint, Point endPoint);
	virtual FigureSettings* settings();
	void setLength(double length);
	void setOrigin(Point origin);
	void setDirection(Point direction);
	Point origin() const;
	Point direction() const;
	double length() const;

    void setHead(const QCPLineEnding &head);
    void setTail(const QCPLineEnding &tail);
	const QCPLineEnding& head() const;
    const QCPLineEnding& tail() const;

	virtual void shift(double x, double y, double z);
	virtual void rotate(double angle, double x = 0, double y = 0, double z = 0);
	virtual void alignment(double angle, double offsetX, double offsetY);
	virtual void edit(QMap<QString, QString> paramsChanged);

private:
	Point _origin;
	Point _direction;
	double _length;
	QCPLineEnding _head = QCPLineEnding::esNone;
    QCPLineEnding _tail = QCPLineEnding::esNone;
};

class PointFigure : public Figure {
public:
	PointFigure() = default;
	PointFigure(QString name, CurvePoint point);
	virtual FigureSettings* settings();
	CurvePoint point() const;

	virtual void shift(double x, double y, double z);
	virtual void rotate(double angle, double x = 0, double y = 0, double z = 0);
	virtual void alignment(double angle, double offsetX, double offsetY);
	virtual void edit(QMap<QString, QString> paramsChanged);

private:
	CurvePoint _point;
};

class CircleFigure : public Figure {
public:
	CircleFigure();
	CircleFigure(QString name, Point center, Point normal, double radius);
	virtual FigureSettings* settings();
	Point center() const;
	Point normal() const;
	double radius() const;
    void setCenterCross(bool centerCross);
	bool centerCross() const;

	virtual void shift(double x, double y, double z);
	virtual void rotate(double angle, double x = 0, double y = 0, double z = 0);
	virtual void alignment(double angle, double offsetX, double offsetY);
	virtual void edit(QMap<QString, QString> paramsChanged);

private:
	Point _center;
	Point _normal;
	double _radius;
	bool _centerCross = true;
};

class DimFigure : public Figure {
	Q_GADGET
public:
	enum ValueType {
		Length,
		Angle0, Angle90, Angle180, Angle270,
		AngleTo, AngleFrom,
		X, Y, Z, PolarRad, PolarAngle,
		Radius, Diameter,
		TruePosition,
		dX, dY, Rotation,
		MinMax, Form, Min, Max, MaxAbs, SupUT, InfLT,
		Perimeter, Sigma, Mean, RMS
	};

	Q_ENUM(ValueType);

	enum struct DimType {
		Radius,
		Diameter,
		Distance,
		DistanceBetweenCurvePoints,
		Perimeter,
		Position, TruePosition,
		AngleTo, AngleBetween,
		Form,
		BestFitData,
		Mean, Min, MinMax, RMS, Sigma
	};

	struct Value {
		Value(ValueType valueType, bool newIsShow = true, double newMeasurement = 0.0, double newNominal = 0.0, 
			double newUT = 0.0, double newLT = 0.0);

		ValueType type;
		const QString typeToFLR() const;
		bool isShow = true;
		double measurement = 0.0;
		double nominal = 0.0;
		double upperTolerance = 0.0;
		double lowerTolerance = 0.0;
	};

	DimFigure(const QString &name, const DimType type, const Point &labelPoint, const QString firstReference = QString(),
		const QString secondReference = QString());
	DimFigure(const QString &name, const Point &labelPoint, const QString firstReference = QString(), const QString secondReference = QString());
	virtual FigureSettings* settings();
	const Point& labelPoint() const;
	void setLabelPoint(const Point &point);
	const QString firstReference() const;
	const QString secondReference() const;
	void setFirstReference(const QString &firstReference);
    void setSecondReference(const QString &secondReference);
	void addValue(const Value &value);
	void addValues(const QVector<Value> &value);
	const QVector<Value>& values() const;
	const DimType dimType() const;
	const QVector<CurvePoint> convertDimValueToPoints();
	const QString exportToFLR(int precision) const ;
	bool isOnlyLabel() const;
	void setOnlyLabel(bool onlyLabel);
    bool isShowTols() const;
	void setShowTolerances(bool showTols);
	bool isFreePosition() const;
	void setFreePosition(bool freePosition);
    void setDimType(const DimType &dimType);
	int A();
	void setA(int A);
	void setValues(QList<CurvePoint> points);
	static ValueType valueTypeFromString(QString macrosType);
	static QString valueTypeToString(ValueType value);

	virtual void shift(double x, double y, double z);
	virtual void rotate(double angle, double x = 0, double y = 0, double z = 0);
	virtual void alignment(double angle, double offsetX, double offsetY);
	virtual void edit(QMap<QString, QString> paramsChanged);
	virtual void updateRefToParent(QString oldParentName, QString newParentName);

private:

	DimType _dimType;
	QVector<Value> _values;
	QString _firstReference;
	QString _secondReference;
	Point _labelPoint;
	int _a;
	bool _onlyLabel = false;
	bool _showTols = false;
	bool _freePosition = false;
};
	
class TextFigure : public Figure {
public:
	TextFigure();
    TextFigure(QString name = "", QString text = "", Point position = Point(0, 0, 0), double textSize = 0, QString reference = "",
		double imageWidth = 0, double imageHeight = 0, double imageZoom = 0);

    virtual FigureSettings* settings();

    QString text() const;
    void setText(QString text);
    Point position() const;
    void setPosition(Point position);
    double textSize() const;
    void setTextSize(double textSize);
    const QString reference() const;
    void setReference(const QString reference);
    double imageWidth() const;
    void setImageWidth(double imageWidth);
    double imageHeight() const;
    void setImageHeight(double imageHeight);
    double imageZoom() const;
    void setImageZoom(double imageZoom);

    virtual void shift(double x, double y, double z);
    virtual void rotate(double angle, double x = 0, double y = 0, double z = 0);
    virtual void alignment(double angle, double offsetX, double offsetY);
    virtual void edit(QMap<QString, QString> paramsChanged);
	virtual void updateRefToParent(QString oldParentName, QString newParentName);

private:
	QString _text = "";
	Point _position = Point(0, 0, 0);
	double _textSize = 0;
	QString _reference;
	double _imageWidth = 0;
    double _imageHeight = 0;
	double _imageZoom = 0;
};