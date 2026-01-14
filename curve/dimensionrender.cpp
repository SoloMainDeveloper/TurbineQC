	#include "curve/pch.h"
#include "dimensionrender.h"

DimensionRender::DimensionRender(Plot *plot) {
	_plot = plot;
	connect(_plot, &Plot::mouseMove, this, &DimensionRender::onMouseMove);
	connect(_plot, &Plot::mousePress, this, &DimensionRender::onMousePress);
	connect(_plot, &Plot::renderDimension, this, &DimensionRender::render);
	connect(this, &DimensionRender::dimensionAdded, _plot, &Plot::addDimension);
}

void DimensionRender::addRadius() {
	_circleFigure = dynamic_cast<const CircleFigure*>(_plot->currentFigure());
	_plot->setCursor(QCursor(Qt::CrossCursor));
	_dimType = DimFigure::DimType::Radius;
}

void DimensionRender::addDiameter() {
	_circleFigure = dynamic_cast<const CircleFigure*>(_plot->currentFigure());
    _plot->setCursor(QCursor(Qt::CrossCursor));
	_dimType = DimFigure::DimType::Diameter;
}

void DimensionRender::addPerimeter() {
	_curveFigure = dynamic_cast<const CurveFigure*>(_plot->currentFigure());
    _plot->setCursor(QCursor(Qt::BusyCursor));
	auto params = Function15Params(_curveFigure->isClosed());
	auto result = CurveLibrary::function15(_curveFigure->points(), params);
	_dimType = DimFigure::DimType::Perimeter;
	_perimeter = result.perimeter;
	_plot->setCursor(QCursor(Qt::CrossCursor));
}

void DimensionRender::onMouseMove(QMouseEvent *event) {
	if(_dimType == DimFigure::DimType::Radius || _dimType == DimFigure::DimType::Diameter) {
		Point mousePoint = Point(_plot->xAxis->pixelToCoord(event->pos().x()), _plot->yAxis->pixelToCoord(event->pos().y()));
		DimFigure *dimFigure = new DimFigure("CALLOUT", _dimType, mousePoint, _circleFigure);
		auto valueType = _dimType == DimFigure::DimType::Radius ? DimFigure::ValueType::Radius : DimFigure::ValueType::Diameter;
		DimFigure::Value value(valueType);
		
		if(valueType == DimFigure::ValueType::Radius) {
			value.measurement = _circleFigure->radius();
		} else {
            value.measurement = 2 * _circleFigure->radius();
		}
		
		dimFigure->addValue(value);
		render(dimFigure);
		delete dimFigure;
	} else if(_dimType == DimFigure::DimType::Perimeter) {
		Point mousePoint = _plot->pixelToCoord(Point(event->pos().x(), event->pos().y()));
		Point offsetPoint  = Point(mousePoint.x + _offsetCalloutX, mousePoint.y);
		DimFigure *dimFigure = new DimFigure("CALLOUT", _dimType, mousePoint, _curveFigure);
		DimFigure::Value value(DimFigure::ValueType::Length);
		value.measurement = _perimeter;

		dimFigure->addValue(value);

		render(dimFigure);
		delete dimFigure;
	}
}

void DimensionRender::onMousePress(QMouseEvent *event) {
	if(_dimType == DimFigure::DimType::Radius || _dimType == DimFigure::DimType::Diameter) {
		auto mousePoint = _plot->pixelToCoord(Point(event->pos().x(), event->pos().y()));

		QString name = _circleFigure->name();
		auto valueType = _dimType == DimFigure::DimType::Radius ? DimFigure::ValueType::Radius : DimFigure::ValueType::Diameter;
		auto value = DimFigure::Value(valueType);

		if(_dimType == DimFigure::DimType::Radius) {
			name += "-R";
			value.measurement = _circleFigure->radius();
		}
		if(_dimType == DimFigure::DimType::Diameter) {
			name += "-D";
			value.measurement = 2 * _circleFigure->radius();
		}

		DimFigure *dimFigure = new DimFigure(name, _dimType, mousePoint, _circleFigure);
		dimFigure->addValue(value);
		emit dimensionAdded(dimFigure);

		_plot->clearFiguresFromLayer("CALLOUT");
		_plot->setCursor(QCursor(Qt::ArrowCursor));
		_circleFigure = nullptr;
		_dimType = DimFigure::DimType::Distance;
	} else if(_dimType == DimFigure::DimType::Perimeter) {
		auto mousePoint = _plot->pixelToCoord(Point(event->pos().x(), event->pos().y()));
		DimFigure::Value _newValue(DimFigure::ValueType::Length);
		_newValue.measurement = _perimeter;
		QString name = _curveFigure->name() + "-L";
		auto dimFigure = new DimFigure(name, _dimType, mousePoint, _curveFigure);
		dimFigure->addValue(_newValue);
		emit dimensionAdded(dimFigure);

		_plot->clearFiguresFromLayer("CALLOUT");
		_plot->setCursor(QCursor(Qt::ArrowCursor));
		_curveFigure = nullptr;
		_dimType = DimFigure::DimType::Distance;
	}
}

void DimensionRender::render(const DimFigure *dimFigure) {
	auto &name = dimFigure->name();
	auto &labelPoint = dimFigure->labelPoint();
	auto &labelPointPx = _plot->coordToPixel(labelPoint);
	const auto &dimType = dimFigure->dimType();
	const auto &values = dimFigure->values();
	_plot->clearFiguresFromLayer(name);
	QFont font = QFont();
	bool isCurrentFigure = _plot->currentFigure() == dimFigure;
	font.setBold(isCurrentFigure);

	switch(dimType) {
		case DimFigure::DimType::Radius: case DimFigure::DimType::Diameter: {
			const CircleFigure *circleFigure = dynamic_cast<const CircleFigure*>(dimFigure->firstReference());
			const Point &centerPoint = circleFigure->center();
			Point offsetPoint = labelPoint;
			char signSide = 1;

			if(labelPoint.x >= centerPoint.x) {
				offsetPoint.x -= _offsetCalloutX; // right
				signSide = -1;
			} else {
				offsetPoint.x += _offsetCalloutX; // left
			}

			auto &nearestPoint = findNearestCirclePoint(offsetPoint, circleFigure);
			renderCallout(name, labelPoint, offsetPoint, nearestPoint);

			auto label = new QCPItemText(_plot);
			QString text = values[0].type == DimFigure::ValueType::Radius ? "R " : "D ";
			text += QString::number(values[0].measurement, 'f', 3);
			label->setText(text);
			label->setFont(font);

			double labelWidthPx = QFontMetrics(font).horizontalAdvance(text);
			double labelPointPx = _plot->xAxis->coordToPixel(labelPoint.x);
			double labelX = _plot->xAxis->pixelToCoord(labelPointPx - signSide * labelWidthPx / 2);

			label->position->setCoords(labelX, labelPoint.y);
			break;
		}
		case DimFigure::DimType::Perimeter: {
			auto curveFigure = dynamic_cast<const CurveFigure*>(dimFigure->firstReference());
			auto nearestPoint = findNearestCurvePoint(labelPoint, curveFigure);
			Point offsetPoint = Point(labelPoint.x + _offsetCalloutX, labelPoint.y);
			renderCallout(name, labelPoint, offsetPoint, nearestPoint);

			auto label = new QCPItemText(_plot);
			QString text = "L ";
			text += QString::number(values[0].measurement, 'f', 3);
			label->setText(text);
			label->setFont(font);

			double deltaPx = QFontMetrics(font).horizontalAdvance(text);
			auto labelX = _plot->xAxis->pixelToCoord(labelPointPx.x - deltaPx / 2);
			label->position->setCoords(labelX, labelPoint.y);
			break;
		}
		case DimFigure::DimType::Distance: {
			auto firstPointFigure = dynamic_cast<const PointFigure*>(dimFigure->firstReference());
			auto secondPointFigure = dynamic_cast<const PointFigure*>(dimFigure->secondReference());
			const auto &firstDimPoint = (Point)firstPointFigure->point();
			const auto &secondDimPoint = (Point)secondPointFigure->point();
			const auto pointRadius = _plot->pointRadius();
			auto label = new QCPItemText(_plot);
			auto distance = QString::number(dimFigure->values()[0].measurement, 'f', 3);
			label->setText(distance);
			label->setFont(font);

			auto totalRect = QRectF(toQPointF(firstDimPoint), toQPointF(secondDimPoint)).normalized();
			auto widthPx = QFontMetrics(font).horizontalAdvance(distance);
			auto heightPx = QFontMetrics(font).height();
			auto topLeft = toQPointF(_plot->pixelToCoord(Point(labelPointPx.x - widthPx / 2.0, labelPointPx.y + heightPx / 2.0)));
			auto bottomRight = toQPointF(_plot->pixelToCoord(Point(labelPointPx.x + widthPx / 2.0, labelPointPx.y - heightPx / 2.0)));
			auto textRect = QRectF(topLeft, bottomRight).normalized();
			auto totalArea = totalRect.width() * totalRect.height();
			auto textArea = textRect.width() * textRect.height();
			
			auto k = (firstDimPoint.y - secondDimPoint.y) / (firstDimPoint.x - secondDimPoint.x);
			auto b = firstDimPoint.y - k * firstDimPoint.x;
			auto isEnoughSpace = (textArea / totalArea) <= 0.25;
			const LineFigure *firstLine, *secondLine;
			if(isEnoughSpace) {
				label->position->setCoords(labelPoint.x, labelPoint.y);

				Point firstPointOnRect, secondPointOnRect;
				const auto &points = intersectionLineAndRect(k, b, textRect);
				const auto &keys = points.keys();
				const auto &values = points.values();
				if(hypot(firstDimPoint.x - keys[0], firstDimPoint.y - values[0]) < hypot(firstDimPoint.x - keys[1], firstDimPoint.y - values[1])) {
					firstPointOnRect = Point(keys[0], values[0]);
					secondPointOnRect = Point(keys[1], values[1]);
				} else {
					firstPointOnRect = Point(keys[1], values[1]);
					secondPointOnRect = Point(keys[0], values[0]);
				}

				firstLine = createLineFigure(name, firstPointOnRect, firstDimPoint, QCPLineEnding::esNone, QCPLineEnding::esLineArrow);
				secondLine = createLineFigure(name, secondPointOnRect, secondDimPoint, QCPLineEnding::esNone, QCPLineEnding::esLineArrow);
			} else {
				label->position->setCoords(_plot->xAxis->pixelToCoord(labelPointPx.x - widthPx / 2.0 - _labelOffsetPx), labelPoint.y);
				auto direction = Point(firstDimPoint.x - secondDimPoint.x, firstDimPoint.y - secondDimPoint.y);
				direction = normalizeVector(direction);
				auto firstStartPoint = Point(firstDimPoint.x + direction.x, firstDimPoint.y + direction.y);
				firstLine = createLineFigure(name, firstStartPoint, firstDimPoint, QCPLineEnding::esNone, QCPLineEnding::esLineArrow);
				direction.x *= -1;
				direction.y *= -1;
				auto secondStartPoint = Point(secondDimPoint.x + direction.x, secondDimPoint.y + direction.y);
				secondLine = createLineFigure(name, secondStartPoint, secondDimPoint, QCPLineEnding::esNone, QCPLineEnding::esLineArrow);
			}
			_plot->createFigureWithoutReplot(firstLine);
			_plot->createFigureWithoutReplot(secondLine);
		}
	}
	_plot->layer(name)->replot();
}

void DimensionRender::renderCallout(const QString &name, const Point &startPoint, const Point &offsetPoint, const Point &endPoint) {
	auto directLineFigure = createLineFigure(name, offsetPoint, endPoint, QCPLineEnding::esNone, QCPLineEnding::esLineArrow);
	auto mainLineFigure = createLineFigure(name, startPoint, offsetPoint);

	_plot->createFigureWithoutReplot(mainLineFigure);
	_plot->createFigureWithoutReplot(directLineFigure);
	delete directLineFigure, mainLineFigure;
}

const Point DimensionRender::normalizeVector(const Point &vector) const {
	double vectorLength = hypot(vector.x, vector.y);
	return Point(vector.x / vectorLength, vector.y / vectorLength);
}

const LineFigure* DimensionRender::createLineFigure(const QString &name, const Point &startPoint, const Point &endPoint, const QCPLineEnding &head, const QCPLineEnding &tail) const {
	auto directionPoint = Point(startPoint.x - endPoint.x, startPoint.y - endPoint.y, startPoint.z - endPoint.z);
	auto normalizedDirection = normalizeVector(directionPoint);

	auto length = hypot(startPoint.x - endPoint.x, startPoint.y - endPoint.y);
	auto middlePoint = Point((startPoint.x + endPoint.x) / 2, (startPoint.y + endPoint.y) / 2, (startPoint.z + endPoint.z) / 2);
	auto lineFigure = new LineFigure(name, middlePoint, normalizedDirection, length);
	lineFigure->setHead(head);
    lineFigure->setTail(tail);

	return lineFigure;
}

const Point DimensionRender::findNearestCirclePoint(const Point &startPoint, const CircleFigure *circleFigure) const {
	const double &radius = circleFigure->radius();
	const Point &centerPoint = circleFigure->center();

	double length = hypot(startPoint.x - centerPoint.x, startPoint.y - centerPoint.y);
    double k = radius / length;
	double dx = startPoint.x - centerPoint.x;
	double dy = startPoint.y - centerPoint.y;

	return Point(centerPoint.x + k * dx, centerPoint.y + k * dy);
}

const Point DimensionRender::findNearestCurvePoint(const Point &startPoint, const CurveFigure *curveFigure) const {
	double length, minLength = DBL_MAX;
	CurvePoint nearestPoint;

	for(const auto &point : curveFigure->points()) {
		length = hypot(point.x - startPoint.x, point.y - startPoint.y);
		if(length < minLength) {
			minLength = length;
			nearestPoint = point;
		}
	}
	return Point(nearestPoint.x, nearestPoint.y);
}

const QHash<double, double> DimensionRender::intersectionLineAndRect(const double k, const double b, const QRectF &rect) const {
	auto points = QHash<double, double>();

	bool hasIntersectionWithTop = ((rect.top() - b) / k) >= rect.left() && ((rect.top() - b) / k) <= rect.right();
	bool hasIntersectionWithBottom = ((rect.bottom() - b) / k) >= rect.left() && ((rect.bottom() - b) / k) <= rect.right();

	bool hasIntersectionWithRight = (k * rect.right() + b) >= rect.bottom() && (k * rect.right() + b) <= rect.top();
	bool hastIntersectionWithLeft = (k * rect.left() + b) >= rect.bottom() && (k * rect.left() + b) <= rect.top();

	double x, y;
	if(hasIntersectionWithTop) {
		x = (rect.top() - b) / k;
		y = rect.top();
        points.insert(x, y);
	}

    if(hasIntersectionWithBottom) {
		x = (rect.bottom() - b) / k;
		y = rect.bottom();
		points.insert(x, y);
    }

    if(hasIntersectionWithRight) {
		x = rect.right();
		y = k * rect.right() + b;
		points.insert(x, y);
    }

    if(hastIntersectionWithLeft) {
		x = rect.left();
		y = k * rect.left() + b;
		points.insert(x, y);
    }

	return points;
}

const QPointF DimensionRender::toQPointF(const Point &point) const {
	return QPointF(point.x, point.y);
}
