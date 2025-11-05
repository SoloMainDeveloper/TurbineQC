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
	virtual QVector<Point> points();
	//virtual QVector<Point> getPointsForDrawing //extra needed for circles
	Figure(QString name, bool isVisible = false);

	const QString& name();
	void setName(QString name);
	bool isVisible();
	void toggleVisible();

private:
	QString _name;
	bool _isVisible;
};

class CurveFigure : public Figure {
public:
	CurveFigure(QString name, QVector<Point> points, double devMultiplier);
	QVector<Point> points() override;
	double devMultiplier() const; 

private:
	QVector<Point> _points;
	double _devMultiplier;
};

class LineFigure : public Figure {
public:
	LineFigure(QString name, Point start, Point end);
	QVector<Point> points() override;
	Point start() const;
	Point end() const;

private:
	Point _start;
	Point _end;
};

class PointFigure : public Figure {
public:
	PointFigure(QString name, Point point);
	QVector<Point> points() override;
	const Point point();

private:
	Point _point;
};

class CircleFigure : public Figure {
public:
	CircleFigure(QString name, Point centre = Point(0, 0), double radius = 0);
	QVector<Point> points() override;
	Point centre() const;
	double radius() const;

private:
	Point _centre;
	double _radius;
};