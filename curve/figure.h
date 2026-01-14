#pragma once

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
	QColor _color;
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

private:
	Point _origin;
	Point _direction;
	double _length;
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

class DimFigure : public LineFigure {
public:
	DimFigure(QString name, PointFigure *start, PointFigure *end);
	virtual FigureSettings* settings();
	PointFigure* start() const;
	PointFigure* end() const;

private:
	PointFigure *_start;
	PointFigure *_end;
};
