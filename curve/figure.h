#pragma once
#include "colortranslator.h"

class Point {
public:
	double x, y, z;

	Point();
	Point(double x, double y, double z = 0.0);
};

class CurvePoint {
public:
	double x, y, z;
	double i, j, k;
	double dev, lt, ut;

	CurvePoint();
	CurvePoint(double x, double y, double z = 0, double i = 0, double j = 0, double k = 0, double dev = 0, double lt = 0, double ut = 0);
	
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

private:
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
	void assignToleranceToSegment(double upperTolerance, double lowerTolerance);
	void computeToleranceClouds();

	const QVector<CurvePoint>& upperTolerance() const;
	const QVector<CurvePoint>& lowerTolerance() const;

private:
	QVector<CurvePoint> _points, _upperTolerancePoints, _lowerTolerancePoints;
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
	LineFigure(QString name, Point position, Point direction, double length = qInf());
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

private:
	Point _center;
	Point _normal;
	double _radius;
};

class DimFigure : public Figure {
public:
	enum struct ValueType {
		Length,
		Angle0, Angle90, Angle180, Angle270,
		AngleTo, AngleFrom,
		X, Y, Z, PolarRad, PolarAngle,
		Radius, Diameter,
		TruePosition,
		dX, dY, Rotation,
		MinMax, Form, Min, Max, MaxAbs, SupUT, InfLT, Sigma, Mean, RMS
	};

	enum struct DimType {
		Radius,
		Diameter,
		Distance,
		Perimeter,
		Position, TruePosition,
		AngleTo, AngleBetween,
		Form,
		BestFitData,
		Mean, Min, MinMax, RMS, Sigma
	};

	struct Value {
		Value(ValueType valueType);

		ValueType type;
		bool isShow = true;
		double measurement = 0.0;
		double nominal = 0.0;
		double upperTolerance = 0.0;
		double lowerTolerance = 0.0;
	};

	DimFigure(const QString &name, const DimType type, const Point &labelPoint, const Figure *firstReference = nullptr,
		const Figure *secondReference = nullptr);
	virtual FigureSettings* settings();
	const Point& labelPoint() const;
	const Figure* firstReference() const;
	const Figure* secondReference() const;

	void addValue(const Value &value);
	const QVector<Value>& values() const;
	const DimType dimType() const;

private:
	const QVector<CurvePoint> convertDimValueToPoints(const QVector<Value> &values);

	DimType _dimType;
	QVector<Value> _values;
	const Figure *_firstReference;
	const Figure *_secondReference;
	Point _labelPoint;
	int _a;
};
	