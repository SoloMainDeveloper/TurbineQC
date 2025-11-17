#pragma once

class Point {
public:
	double x, y, z;
	double i, j, k;
	double dev, lt, ut;

	Point(double x, double y, double z = 0, double i = 0, double j = 0, double k = 0, double dev = 0, double lt = 0, double ut = 0);
	Point() : Point(0, 0) {
	};
};

class Figure {
public:
	virtual QVector<Point> points() const;
	//virtual QVector<Point> getPointsForDrawing //extra needed for circles
	Figure();
	Figure(QString name, bool isVisible = false);

	const QString& name() const;
	void setName(QString name);
	bool isVisible() const;
	void setVisible(bool visibility);
	void toggleVisible();
	void setColor(QColor& color);
	const QColor& color() const;

private:
	QString _name;
	bool _isVisible = false;
	QColor _color;
};

class CurveFigure : public Figure {
public:
	CurveFigure();
	CurveFigure(QString name, QVector<Point> points, double devMultiplier = 0);
	QVector<Point> points() const override;
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

private:
	QVector<Point> _points;
	double _devMultiplier;

	bool _isShowPoints = false;
	bool _isConnectPoints = false;
	bool _isShowVectors = false;
	bool _isClosed = false;
	bool _isShowNumbering = false;
	int _numberingInterval = 5;
};

class LineFigure : public Figure {
public:
	LineFigure();
	LineFigure(QString name, Point start, Point end);
	QVector<Point> points() const override;
	Point start() const;
	Point end() const;

private:
	Point _start;
	Point _end;
};

class PointFigure : public Figure {
public:
	PointFigure();
	PointFigure(QString name, Point point);
	QVector<Point> points() const override;
	const Point& point() const;

private:
	Point _point;
};

class CircleFigure : public Figure {
public:
	CircleFigure();
	CircleFigure(QString name, Point centre = Point(0, 0), double radius = 0);
	QVector<Point> points() const override;
	Point centre() const;
	double radius() const;

private:
	Point _centre;
	double _radius;
};
