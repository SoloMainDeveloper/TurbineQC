#include "curve/pch.h"
#include "curve/plot.h"

Plot::Plot(QWidget *parent) : QCustomPlot(parent), _project(nullptr) {
    xAxis->setUpperEnding(QCPLineEnding::esLineArrow);
    yAxis->setUpperEnding(QCPLineEnding::esLineArrow);

    xAxis->setRange(0, 7);
    yAxis->setRange(0, 5);
    _replotTimer.setSingleShot(false);
    _replotTimer.setInterval(500);
    connect(&_replotTimer, &QTimer::timeout, this, [&]() {
        for(const auto &figure : _project->figures()) {
            if(figure->isVisible()) updateFigure(figure->name());
        }
    });
    setInteractions(QCP::iRangeDrag | QCP::iSelectPlottables | QCP::iSelectItems);
}

void Plot::setProject(Project *project) {
    _project = project;

    connect(project, &Project::figureAdded, this, &Plot::addFigure);
    connect(project, &Project::figureRenamed, this, &Plot::renameFigure);
    connect(project, &Project::figureAboutToBeRemoved, this, &Plot::removeFigure);
    connect(project, &Project::figureVisibilityChanged, this, &Plot::changeFigureVisibility);
    connect(project, &Project::curveParametersChanged, this, &Plot::changeCurveParameters);
    connect(project, &Project::currentFigureChanged, this, &Plot::changeCurrentFigure);
    connect(project, &Project::curveToleranceChanged, this, &Plot::changeCurveTolerance);
    connect(project, &Project::figureColorChanged, this, &Plot::changeFigureColor);
    connect(project, &Project::scaleChanged, this, &Plot::changeScale);
    connect(project, &Project::dimensionParametersChanged, this, &Plot::changeDimensionParameters);
    connect(project, &Project::dimensionValueChanged, this, &Plot::changeDimensionValue);
    connect(project, &Project::figureCoordsChanged, this, &Plot::changeFigureCoords, Qt::QueuedConnection);
    connect(project, &Project::figureEdited, this, &Plot::changeEditedFigure);

    connect(this, &Plot::plottableClick, this, &Plot::onPlottableClicked);
    connect(this, &Plot::plottableDoubleClick, this, &Plot::onPlottableDoubleClicked);
    connect(this, &Plot::itemClick, this, &Plot::onItemClicked);
    connect(this, &Plot::itemDoubleClick, this, &Plot::onItemDoubleClicked);
    connect(this, &Plot::currentFigureChanged, project, &Project::changeCurrentFigure);
    connect(this, &Plot::figureEditDialogRequested, project, &Project::requestFigureEditDialog);
    connect(this, &Plot::mouseWheel, this, &Plot::onMouseWheel);
    connect(this, &Plot::mouseMove, this, &Plot::onMouseMove);
    connect(this, &Plot::mousePress, this, &Plot::onMousePress);
    connect(this, &Plot::rescaled, _project, &Project::changeScale);
}

const Figure* Plot::currentFigure() const {
    return _project->findFigure(_project->currentFigureName());
}

void Plot::addFigure(Figure *figure) {
    loadFigure(figure);
};

void Plot::renameFigure(const QString figureName, const QString newName) {
    auto figure = _project->findFigure(newName);

    deleteFigure(figureName);
    loadFigure(figure);
}

void Plot::removeFigure(const QString figureName) {
    deleteFigure(figureName);
    _buffer.remove(figureName);
}

void Plot::changeCurrentFigure(const QString &currentFigureName, const QString &previousFigureName) {
    if(layer(currentFigureName) != nullptr && layer(previousFigureName) != nullptr) {
        moveLayer(layer(currentFigureName), layer(previousFigureName), LayerInsertMode::limAbove);
    }

    if(!previousFigureName.isNull()) {
        updateFigure(previousFigureName);
    }

    if(!currentFigureName.isNull()) {
        updateFigure(currentFigureName);
    }
}

void Plot::changeFigureVisibility(const QString figureName, bool visible) {
    if(auto figureLayer = layer(figureName)) {
        figureLayer->setVisible(visible);
        figureLayer->replot();
    }
}

void Plot::changeCurveParameters(const QString curveName, bool showPoints, bool connectPoints,
    bool showVectors, bool closed, bool showNumbering, int numberingInterval,
    double amplification, bool showTolerances, bool showDeviations, bool connectDeviations, bool highLightOut) {
    updateFigure(curveName);
}

void Plot::changeDimensionParameters(const QString dimensionName, bool showNumbers, bool showTolerances, bool freePosition) {
    updateFigure(dimensionName);
}

void Plot::changeCurveTolerance(const QString curveName) {
    updateFigure(curveName);
}

void Plot::changeFigureColor(const QString figureName) {
    updateFigure(figureName);
}

void Plot::changeFigureCoords(const QString figureName) {
    updateFigure(figureName);
}

void Plot::changeEditedFigure(const QString figureName) {
    updateFigure(figureName);
}

void Plot::cancelInteractiveOperations() {
    if(_calloutRendering == true) {
        _calloutRendering = false;
        setCursor(QCursor(Qt::ArrowCursor));
        if(_project->findFigure(_calloutDimension->name()) != nullptr) {
            updateFigure(_calloutDimension->name());
        } else {
            deleteFigure(_calloutDimension->name());
            delete _calloutDimension;
        }
    }
}

void Plot::changeDimensionValue(const QString &dimName, const DimFigure::Value &value) {
    updateFigure(dimName);
}

void Plot::zoomExtents() {
    rescale(Position::Center);
    _magnitude = _defaultScaleFactor * 100 / yAxis->range().size();
    emit rescaled(_magnitude, centerPoint());
}

void Plot::rescale(Position position) {
    rescaleAxes(true);

    auto xMin = xAxis->range().lower;
    auto xMax = xAxis->range().upper;

    auto yMin = yAxis->range().lower;
    auto yMax = yAxis->range().upper;

    if(position == Position::Right) {
        rescaleToRight(xMin, xMax, yMin, yMax);
    } else if(position == Position::Left) {
        rescaleToLeft(xMin, xMax, yMin, yMax);
    } else {
        rescaleToCenter(xMin, xMax, yMin, yMax);
    }
    layer("axes")->replot();
}

void Plot::rescaleToRight(double xMin, double xMax, double yMin, double yMax) {
    auto xLength = xMax - xMin;
    auto yLength = yMax - yMin;

    auto xFractionOfDistanceRight = 0.15;
    auto xFractionOfDistanceLeft = 0.35;
    auto yFractioOfDistance = 0.12;

    auto xMarginLeft = (xMax - xMin) * xFractionOfDistanceLeft;
    auto xMarginRight = (xMax - xMin) * xFractionOfDistanceRight;
    auto yMargin = (yMax - yMin) * yFractioOfDistance;

    if(xLength > yLength * _ratioXToY) {
        auto yMid = (yMin + yMax) / 2;
        auto yRangeDistance = xLength * _ratioYToX;

        xAxis->setRange(xMin - xMarginLeft, xMax + xMarginRight);
        yAxis->setRange((yMid - yRangeDistance / 2) - yMargin, (yMid + yRangeDistance / 2) + yMargin);
    } else {
        auto xMid = (xMin + xMax) / 2;
        auto xRangeDistance = yLength * _ratioXToY;

        xAxis->setRange((xMid - xRangeDistance / 2) - xMarginLeft, (xMid + xRangeDistance / 2) + xMarginRight);
        yAxis->setRange(yMin - yMargin, yMax + yMargin);
    }
}

void Plot::rescaleToLeft(double xMin, double xMax, double yMin, double yMax) {
    auto xLength = xMax - xMin;
    auto yLength = yMax - yMin;

    auto xFractionOfDistanceRight = 0.35;
    auto xFractionOfDistanceLeft = 0.15;
    auto yFractioOfDistance = 0.12;

    auto xMarginRight = (xMax - xMin) * xFractionOfDistanceRight;
    auto xMarginLeft = (xMax - xMin) * xFractionOfDistanceLeft;
    auto yMargin = (yMax - yMin) * yFractioOfDistance;

    if(xLength > yLength * _ratioXToY) {
        auto yMid = (yMin + yMax) / 2;
        auto yRangeDistance = xLength * _ratioYToX;

        xAxis->setRange(xMin - xMarginLeft, xMax + xMarginRight);
        yAxis->setRange((yMid - yRangeDistance / 2) - yMargin, (yMid + yRangeDistance / 2) + yMargin);
    } else {
        auto xMid = (xMin + xMax) / 2;
        auto xRangeDistance = yLength * _ratioXToY;

        xAxis->setRange((xMid - xRangeDistance / 2) - xMarginLeft, (xMid + xRangeDistance / 2) + xMarginRight);
        yAxis->setRange(yMin - yMargin, yMax + yMargin);
    }
}

void Plot::rescaleToCenter(double xMin, double xMax, double yMin, double yMax) {
    auto xLength = xMax - xMin;
    auto yLength = yMax - yMin;

    auto xFractionOfDistance = 0.15;
    auto yFractioOfDistance = 0.12;

    auto xMargin = (xMax - xMin) * xFractionOfDistance;
    auto yMargin = (yMax - yMin) * yFractioOfDistance;

    if(xLength > yLength * _ratioXToY) {
        auto yMid = (yMin + yMax) / 2;
        auto yRangeDistance = xLength * _ratioYToX;

        xAxis->setRange(xMin - xMargin, xMax + xMargin);
        yAxis->setRange((yMid - yRangeDistance / 2) - yMargin, (yMid + yRangeDistance / 2) + yMargin);
    } else {
        auto xMid = (xMin + xMax) / 2;
        auto xRangeDistance = yLength * _ratioXToY;

        xAxis->setRange((xMid - xRangeDistance / 2) - xMargin, (xMid + xRangeDistance / 2) + xMargin);
        yAxis->setRange(yMin - yMargin, yMax + yMargin);
    }
}

void Plot::zoomPlus() {
    zoomPlusToPoint(centerPoint());
}

void Plot::zoomPlusToPoint(const Point &point) {
    auto magnitude = _magnitude * pow(_reductionFactor, -1);
    emit rescaled(_defaultScaleFactor / magnitude, point);
}

void Plot::zoomMinus() {
    zoomMinusToPoint(centerPoint());
}

void Plot::zoomMinusToPoint(const Point &point) {
    auto magnitude = _magnitude * pow(_magnificationFactor, -1);
    emit rescaled(_defaultScaleFactor / magnitude, point);
}

Point Plot::centerPoint() const {
    return Point(xAxis->range().center(), yAxis->range().center());
}

void Plot::changeScale(double magnitude, const Point &point) {
    magnitude = _defaultScaleFactor / magnitude;
    auto factor = pow(magnitude / _magnitude, -1);
    xAxis->scaleRange(factor, point.x);
    yAxis->scaleRange(factor, point.y);
    _magnitude = magnitude;
    layer("axes")->replot();
}

const Point Plot::pixelToCoord(const Point &point) const {
    return Point(xAxis->pixelToCoord(point.x), yAxis->pixelToCoord(point.y));
}

const Point Plot::coordToPixel(const Point &point) const {
    return Point(xAxis->coordToPixel(point.x), yAxis->coordToPixel(point.y));
}

void Plot::loadFigure(const Figure *figure) {
    if(layer(figure->name()) == nullptr) {
        addLayer(figure->name());
        layer(figure->name())->setVisible(figure->isVisible());
    }
    if(!figure->isVisible())
        return;

    setCurrentLayer(figure->name());

    if(auto curveFigure = dynamic_cast<const CurveFigure*>(figure)) {
        addCurveLayer(curveFigure);
    } else if(auto lineFigure = dynamic_cast<const LineFigure*>(figure)) {
        addLineLayer(lineFigure);
    } else if(auto pointFigure = dynamic_cast<const PointFigure*>(figure)) {
        addPointLayer(pointFigure);
    } else if(auto circleFigure = dynamic_cast<const CircleFigure*>(figure)) {
        addCircleLayer(circleFigure);
    } else if(auto dimFigure = dynamic_cast<const DimFigure*>(figure)) {
        addDimLayer(dimFigure);
    } else if(auto textFigure = dynamic_cast<const TextFigure*>(figure)) {
        addTextLayer(textFigure);
    }
}

void Plot::updateFigure(const QString &figureName) {
    if(_isBuffering) {
        _buffer.insert(figureName);
        return;
    }

    if(auto figure = _project->findFigure(figureName)) {
        clearLayer(figureName);
        loadFigure(figure);
        layer(figureName)->replot();
    }
}

void Plot::deleteFigure(const QString &figureName) {
    auto layerToDelete = layer(figureName);

    clearLayer(figureName);
    layer(figureName)->replot();
    removeLayer(layerToDelete);
}

void Plot::addCurveLayer(const CurveFigure *curveFigure) {
    drawCurve(curveFigure);
    drawCurveVectors(curveFigure);
    drawCurveNumbering(curveFigure);
    drawCurveTolerances(curveFigure);
    drawCurveDeviations(curveFigure);
}

void Plot::addPointLayer(const PointFigure *pointFigure) {
    auto circleFigure = new CircleFigure(pointFigure->name(), Point(pointFigure->point()), Point(0, 0, 1), _pointRadius);
    auto &pointColor = pointFigure->color();
    circleFigure->setColor(pointColor);
    circleFigure->setCenterCross(false);
    addCircleLayer(circleFigure);
    delete circleFigure;
}

void Plot::addLineLayer(const LineFigure *lineFigure) {
    auto length = lineFigure->length();

    double penWidth;
    if(_project->currentFigureName() == lineFigure->name()) {
        penWidth = _currentFigurePenWidth;
    } else {
        penWidth = _penWidth;
    }

    auto brush = QBrush(lineFigure->color());
    auto pen = QPen(brush, penWidth);

    if(length == qInf()) {
        auto line = new QCPItemStraightLine(this);
        line->setPen(pen);

        auto origin = lineFigure->origin();
        auto direction = lineFigure->direction();

        line->point1->setCoords(origin.x, origin.y);
        line->point2->setCoords(origin.x + direction.x, origin.y + direction.y);
    } else {
        auto line = new QCPItemLine(this);
        line->setPen(pen);

        auto origin = lineFigure->origin();
        auto direction = lineFigure->direction();
        double halfLength = 0.5 * length;

        auto startPoint = Point(origin.x - halfLength * direction.x, origin.y - halfLength * direction.y);
        auto endPoint = Point(origin.x + halfLength * direction.x, origin.y + halfLength * direction.y);

        line->start->setCoords(startPoint.x, startPoint.y);
        line->end->setCoords(endPoint.x, endPoint.y);

        line->setHead(lineFigure->head());
        line->setTail(lineFigure->tail());
    }
}

void Plot::addCircleLayer(const CircleFigure *circleFigure) {
    auto circle = new QCPItemEllipse(this);

    double penWidth;
    if(_project->currentFigureName() == circleFigure->name()) {
        penWidth = _currentFigurePenWidth;
    } else {
        penWidth = _penWidth;
    }
    
    auto brush = QBrush(circleFigure->color());
    auto pen = QPen(brush, penWidth);
    circle->setPen(pen);

    auto center = circleFigure->center();
    auto radius = circleFigure->radius();
    auto topLeftAnchor = QPointF(center.x - radius, center.y + radius);
    auto bottomRightAnchor = QPointF(center.x + radius, center.y - radius);
    circle->topLeft->setCoords(topLeftAnchor);
    circle->bottomRight->setCoords(bottomRightAnchor);

    if(circleFigure->centerCross()) {
        auto centerPoint = circleFigure->center();
        auto radius = circleFigure->radius();
        auto lineLength = qMin(_project->scaleFactor() / radius, radius);
        auto horizontalLine = new LineFigure(circleFigure->name(), centerPoint, Point(1, 0), lineLength);
        auto verticalLine = new LineFigure(circleFigure->name(), centerPoint, Point(0, 1), lineLength);
        horizontalLine->setColor(circleFigure->color());
        verticalLine->setColor(circleFigure->color());
        loadFigure(horizontalLine);
        loadFigure(verticalLine);
        delete horizontalLine, verticalLine;
    }
}

void Plot::addDimLayer(const DimFigure *dimFigure) {
    switch(dimFigure->dimType()) {
        case(DimFigure::DimType::Perimeter):
        case(DimFigure::DimType::Radius):
        case(DimFigure::DimType::Diameter):
        {
            drawCallout(dimFigure);
            break;
        };
        case(DimFigure::DimType::Form):
        case(DimFigure::DimType::Position):
        case(DimFigure::DimType::BestFitData):
        {
            drawTable(dimFigure);
            break;
        };
        case(DimFigure::DimType::DistanceBetweenCurvePoints):
        case(DimFigure::DimType::Distance):
        {
            /*drawDistanceBetweenCurvePoints(dimFigure);
            break;*/
        };
    }
}

void Plot::addTextLayer(const TextFigure *textFigure) {
    if(textFigure->text().indexOf("IMG:") == 0) {
        drawImage(textFigure);
    } else {
        drawText(textFigure);
    }
}

LineFigure* Plot::createLineFigure(const QString & name, const Point & startPoint, const Point & endPoint, const QCPLineEnding & head, const QCPLineEnding & tail, const QColor & color) {
    auto directionPoint = Point(startPoint.x - endPoint.x, startPoint.y - endPoint.y, startPoint.z - endPoint.z);
    auto length = hypot(directionPoint.x, directionPoint.y);
    auto normalizedDirection = Point(directionPoint.x / length, directionPoint.y / length, directionPoint.z / length);

    length = hypot(startPoint.x - endPoint.x, startPoint.y - endPoint.y);
    auto middlePoint = Point((startPoint.x + endPoint.x) / 2, (startPoint.y + endPoint.y) / 2, (startPoint.z + endPoint.z) / 2);
    auto lineFigure = new LineFigure(name, middlePoint, normalizedDirection, length);
    lineFigure->setColor(color);
    lineFigure->setHead(head);
    lineFigure->setTail(tail);

    return lineFigure;
}

const Point Plot::findNearestFigurePoint(const Point &point, const Figure *figure) {
    Point result;
    if(auto circleFigure = dynamic_cast<const CircleFigure*>(figure)) {
        const double &radius = circleFigure->radius();
        const Point &centerPoint = circleFigure->center();
        const double length = hypot(point.x - centerPoint.x, point.y - centerPoint.y);
        const double k = radius / length;
        const double dx = point.x - centerPoint.x;
        const double dy = point.y - centerPoint.y;
        result.x = centerPoint.x + k * dx;
        result.y = centerPoint.y + k * dy;
    } else if(auto curveFigure = dynamic_cast<const CurveFigure*>(figure)) {
        double length, minLength = DBL_MAX;
        CurvePoint nearestPoint;
        for(const auto &curvePoint : curveFigure->points()) {
            length = hypot(curvePoint.x - point.x, curvePoint.y - point.y);
            if(length < minLength) {
                minLength = length;
                nearestPoint = curvePoint;
            }
        }
        result.x = nearestPoint.x;
        result.y = nearestPoint.y;
    } else if(auto pointFigure = dynamic_cast<const PointFigure*>(figure)) {
        const CurvePoint &point = pointFigure->point();
        const double length = hypot(point.x - point.x, point.y - point.y);
        const double k = _pointRadius / length;
        const double dx = point.x - point.x;
        const double dy = point.y - point.y;
        result.x = point.x + k * dx;
        result.y = point.y + k * dy;
    } else if(auto lineFigure = dynamic_cast<const LineFigure*>(figure)) {
        const Point &origin = lineFigure->origin();
        const Point &direction = lineFigure->direction();
        const double dx = point.x - origin.x;
        const double dy = point.y - origin.y;
        const double t = (dx * direction.x + dy * direction.y) / (direction.x * direction.x + direction.y * direction.y);
        result.x = origin.x + t * direction.x;
        result.y = origin.y + t * direction.y;
    }
    return result;
}

const void Plot::findNearestCurvePoint(const Point &point, const CurveFigure *curveFigure, CurvePoint &result, int &resultIndex) const {
    double length, minLength = DBL_MAX;
    int index = 0;
    for(const auto &curvePoint : curveFigure->points()) {
        length = hypot(curvePoint.x - point.x, curvePoint.y - point.y);
        if(length < minLength) {
            minLength = length;
            result = curvePoint;
            resultIndex = index + 1;
        }
        index++;
    }
}


void Plot::drawCallout(const DimFigure *dimFigure) {
    const Point &labelPoint = dimFigure->labelPoint();
    auto &value = dimFigure->values()[0];
    auto reference = _project->findFigure(dimFigure->firstReference());

    if(reference == nullptr) {
        qDebug() << "Callout reference not found. DimFigure: " << dimFigure->name();
        return;
    }

    auto &endPoint = findNearestFigurePoint(labelPoint, reference);
    drawCalloutLine(dimFigure->name(), dimFigure->color(), labelPoint, endPoint);

    auto labelText = new QCPItemText(this);
    QString text = QString("%1 %2")
        .arg(getTextByValueType(value.type))
        .arg(QString::number(value.measurement, 'f', _project->precision()));

    labelText->setColor(dimFigure->color());
    if(dimFigure->isOnlyLabel()) {
        text = dimFigure->name();
    }
    if(dimFigure->isShowTols()) {
        text += QString("*\n (%1 %2 %3)").arg(QString::number(value.nominal, 'f', _project->precision()))
            .arg(QString::number(value.upperTolerance, 'f', _project->precision()))
            .arg(QString::number(value.lowerTolerance, 'f', _project->precision()));
        labelText->setColor(QColorConstants::Red);
    }

    auto offsetPointPx = coordToPixel(labelPoint);
    char sideSign = (labelPoint.x - endPoint.x) / abs(labelPoint.x - endPoint.x);
    offsetPointPx.x += sideSign * _offsetCalloutPx;
    auto offsetPoint = pixelToCoord(offsetPointPx);

    auto widthTextPx = QFontMetrics(labelText->font()).horizontalAdvance(text);
    auto labelPointPx = coordToPixel(labelPoint);
    labelText->setTextAlignment(Qt::AlignLeft);
    labelPointPx.x += sideSign * (widthTextPx / 2 + _offsetCalloutPx);
    labelText->position->setPixelPosition(QPointF(labelPointPx.x, labelPointPx.y));
    labelText->setText(text);
    auto font = labelText->font();
    font.setBold(currentFigure() == dimFigure);
    labelText->setFont(font);
    layer(dimFigure->name())->replot();
}

void Plot::drawCalloutLine(const QString &name, const QColor &color, const Point &startPoint, const Point &endPoint) {
    auto offsetPointPx = coordToPixel(startPoint);
    char sideSign = (startPoint.x - endPoint.x) / abs(startPoint.x - endPoint.x);
    offsetPointPx.x += sideSign * _offsetCalloutPx;
    auto offsetPoint = pixelToCoord(offsetPointPx);
    auto directLineFigure = createLineFigure(name, startPoint, endPoint,
        QCPLineEnding::esNone, QCPLineEnding::esLineArrow, color);
    auto mainLineFigure = createLineFigure(name, offsetPoint, startPoint,
        QCPLineEnding::esNone, QCPLineEnding::esNone, color);
    loadFigure(mainLineFigure);
    loadFigure(directLineFigure);
    delete directLineFigure, mainLineFigure;
}

void Plot::drawText(const TextFigure *textFigure) {
    auto text = textFigure->text();
    if(text.indexOf("IMG:") == 0) {
        text = "Not found";
    }

    const auto &color = textFigure->color();
    const auto &position = textFigure->position();
    auto reference = textFigure->reference();
    auto itemText = new QCPItemText(this);
    auto font = QFont();
    font.setPointSizeF(font.pointSizeF() + textFigure->textSize());

    itemText->setText(text);
    itemText->setFont(font);
    itemText->setColor(color);

    auto textWidthPx = QFontMetrics(font).horizontalAdvance(text);
    auto offsetPointPx = coordToPixel(position);
    if(reference != nullptr) {
        auto &endPoint = findNearestFigurePoint(position, _project->findFigure(reference));
        char signSide = (position.x - endPoint.x) / abs(position.x - endPoint.x);
        offsetPointPx.x += signSide * ((float)textWidthPx / 2 + _offsetCalloutPx);
        itemText->position->setPixelPosition(toQPointF(offsetPointPx));
        drawCalloutLine(textFigure->name(), color, position, endPoint);
    } else {
        offsetPointPx.x += textWidthPx / 2;
        itemText->position->setCoords(toQPointF(pixelToCoord(offsetPointPx)));
    }
}

void Plot::drawImage(const TextFigure *textFigure) {
    auto path = textFigure->text().sliced(4);
    auto pixmap = QPixmap(path);
    if(pixmap.isNull()) {
        drawText(textFigure);
        return;
    }
    const auto &position = textFigure->position();
    const auto &width = textFigure->imageWidth();
    const auto &height = textFigure->imageHeight();
    const auto &zoom = textFigure->imageZoom();
    auto image = new QCPItemPixmap(this);

    image->setPixmap(pixmap);
    image->topLeft->setCoords(position.x, position.y);
    image->bottomRight->setCoords(position.x + zoom * width, position.y - zoom * height);
    image->setScaled(true, Qt::AspectRatioMode::IgnoreAspectRatio, Qt::TransformationMode::FastTransformation);

    if(textFigure->reference() != nullptr) {
        auto &endPoint = findNearestFigurePoint(position, _project->findFigure(textFigure->reference()));
        drawCalloutLine(textFigure->name(), textFigure->color(), position, endPoint);
    }
}

void Plot::createRadiusDimension() {
    auto circleFigure = dynamic_cast<const CircleFigure*>(currentFigure());
    _calloutDimension = new DimFigure(circleFigure->name() + "-R", Point(0, 0), circleFigure->name());
    auto value = DimFigure::Value(DimFigure::ValueType::Radius);
    value.measurement = circleFigure->radius();
    _calloutDimension->addValue(value);
    _calloutDimension->setDimType(DimFigure::DimType::Radius);
    setCursor(QCursor(Qt::CrossCursor));
    _calloutRendering = true;
    addLayer(_calloutDimension->name());
}

void Plot::createDiameterDimension() {
    auto circleFigure = dynamic_cast<const CircleFigure*>(currentFigure());
    _calloutDimension = new DimFigure(circleFigure->name() + "-D", Point(0, 0), circleFigure->name());
    auto value = DimFigure::Value(DimFigure::ValueType::Diameter);
    value.measurement = circleFigure->radius() * 2;
    _calloutDimension->addValue(value);
    _calloutDimension->setDimType(DimFigure::DimType::Diameter);
    setCursor(QCursor(Qt::CrossCursor));
    _calloutRendering = true;
    addLayer(_calloutDimension->name());
}

void Plot::createPerimeterDimension(double perimeter) {
    auto curveFigure = dynamic_cast<const CurveFigure*>(currentFigure());
    _calloutDimension = new DimFigure(curveFigure->name() + "-L", Point(0, 0), curveFigure->name());
    auto value = DimFigure::Value(DimFigure::ValueType::Length);
    value.measurement = perimeter;
    _calloutDimension->addValue(value);
    _calloutDimension->setDimType(DimFigure::DimType::Perimeter);
    setCursor(QCursor(Qt::CrossCursor));
    _calloutRendering = true;
    addLayer(_calloutDimension->name());
}

void Plot::clearLayer(const QString &layerName) {
    auto layerToClear = layer(layerName);
    if(layerToClear == nullptr) return;

    QCPAbstractPlottable *abstractPlottable;
    QCPAbstractItem *abstractItem;
    for(auto &children : layerToClear->children()) {
        if(abstractPlottable = dynamic_cast<QCPAbstractPlottable*>(children)) {
            removePlottable(abstractPlottable);
        } else if(abstractItem = dynamic_cast<QCPAbstractItem*>(children)) {
            removeItem(abstractItem);
        }
    }
}

void Plot::setGridVisible(bool enabled) {
    xAxis->grid()->setVisible(enabled);
    yAxis->grid()->setVisible(enabled);
}

void Plot::setAxesVisible(bool enabled) {
    xAxis->setVisible(enabled);
    yAxis->setVisible(enabled);
}

void Plot::setBuffering(bool enabled) {
    _isBuffering = enabled;

    if(_isBuffering == false && _buffer.size() > 0) {
        unloadBuffer();
    }
}

void Plot::unloadBuffer() {
    for(const auto &figure : _buffer) {
        if(figure != nullptr) {
            updateFigure(figure);
        }
    }
    _buffer.clear();
}

void Plot::onItemClicked(QCPAbstractItem *item, QMouseEvent *event) {
    auto figureName = item->layer()->name();
    if(_project->findFigure(figureName) == nullptr) return;
    emit currentFigureChanged(figureName);
    updateFigure(figureName);
}

void Plot::onItemDoubleClicked(QCPAbstractItem* item, QMouseEvent* event) {
    auto figureName = item->layer()->name();
    if(_project->findFigure(figureName) == nullptr) return;
    emit figureEditDialogRequested(figureName);
}

void Plot::onPlottableClicked(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event) {
    auto figureName = plottable->layer()->name();
    if(_project->findFigure(figureName) == nullptr) return;
    emit currentFigureChanged(figureName);
    updateFigure(figureName);
}

void Plot::onPlottableDoubleClicked(QCPAbstractPlottable* plottable, int dataIndex, QMouseEvent* event) {
    auto figureName = plottable->layer()->name();
    if(_project->findFigure(figureName) == nullptr) return;
    emit figureEditDialogRequested(figureName);
}

QImage Plot::getScreenshot(int width, int height, ReportSettings::Axis axisType) {
    QPixmap screenshot(width, height);
    QCPPainter painter(&screenshot);
    if(axisType == ReportSettings::Axis::Center) {
        //to do
    } else if(axisType == ReportSettings::Axis::LeftAndDown) {
        xAxis->grid()->setVisible(false);
        yAxis->grid()->setVisible(false);
        toPainter(&painter, width, height);
        xAxis->grid()->setVisible(true);
        yAxis->grid()->setVisible(true);
    } else {
        xAxis->setVisible(false);
        yAxis->setVisible(false);
        toPainter(&painter, width, height);
        xAxis->setVisible(true);
        yAxis->setVisible(true);
    }
    painter.end();
    return screenshot.toImage();
}

void Plot::onMouseWheel(QWheelEvent *event) {
    if(event->angleDelta().y() > 0) {
        Point mousePoint = pixelToCoord(Point(event->position().x(), event->position().y()));
        zoomPlusToPoint(mousePoint);
    } else if(event->angleDelta().y() < 0) {
        Point mousePoint = pixelToCoord(Point(event->position().x(), event->position().y()));
        zoomMinusToPoint(mousePoint);
    }
    _replotTimer.start();
}

void Plot::onMouseMove(QMouseEvent *event) {
    if(_calloutRendering) {
        auto mousePoint = pixelToCoord(Point(event->pos().x(), event->pos().y()));
        _calloutDimension->setLabelPoint(mousePoint);
        clearLayer(_calloutDimension->name());
        drawCallout(_calloutDimension);
    }
}

void Plot::onMousePress(QMouseEvent *event) {
    if(_calloutRendering) {
        _project->safeInsert(_calloutDimension->name(), _calloutDimension);
        _calloutRendering = false;
        setCursor(QCursor(Qt::ArrowCursor));
    }
}

void Plot::drawTable(const DimFigure *dimFigure) {
    const auto referenceFigure = _project->findFigure(dimFigure->firstReference());
    if(referenceFigure == nullptr) {
        qDebug() << "Table reference not found. DimFigure: " << dimFigure->name();
        return;
    }

    const int _columnWidthPx = 130;
    const int _rowHeightPx = 20;
    const int _separatorHeightPx = 3;
    const Point &labelPoint = dimFigure->labelPoint();
    const QString &name = dimFigure->name();
    const QVector<DimFigure::Value> &values = dimFigure->values();

    QVector<DimFigure::Value> visibleValues;
    for(const DimFigure::Value &value : values) {
        if(value.isShow) {
            visibleValues.append(value);
        }
    }

    QFont font = QFont();
    font.setBold(currentFigure() == dimFigure);

    Point targetPoint = findNearestFigurePoint(labelPoint, referenceFigure);
    auto connectLine = new LineFigure(name, targetPoint, labelPoint);
    loadFigure(connectLine);
    delete connectLine;
    char sideSign = ((targetPoint.x - labelPoint.x) / abs(targetPoint.x - labelPoint.x)) == 1 ? -1 : 0;
    Point labelPointPx = coordToPixel(labelPoint);

    auto topLine = new LineFigure(name, pixelToCoord(labelPointPx),
        pixelToCoord(Point(labelPointPx.x + sideSign * _columnWidthPx, labelPointPx.y)));
    loadFigure(topLine);
    delete topLine;

    Point referencePointPx = Point(labelPointPx.x + sideSign * _columnWidthPx, labelPointPx.y);
    const QFontMetrics fontMetrics(font);
    int heightPx = fontMetrics.height(), widthPx = 0;
    QString text;

    QCPItemText *nameLabel = new QCPItemText(this);
    nameLabel->setFont(font);
    nameLabel->position->setPixelPosition(QPointF(referencePointPx.x + _columnWidthPx / 2, labelPointPx.y - heightPx / 2));
    nameLabel->setText(name);

    if(dimFigure->isOnlyLabel()) return;

    for(auto i = 0; i < visibleValues.length(); i++) {
        const DimFigure::Value value = visibleValues[i];
        QCPItemRect *rect = new QCPItemRect(this);
        rect->setBrush(QBrush(QColor::fromRgb(230, 230, 230)));
        rect->topLeft->setPixelPosition(toQPointF(referencePointPx));
        rect->bottomRight->setPixelPosition(QPointF(referencePointPx.x + _columnWidthPx, referencePointPx.y + _rowHeightPx));

        QCPItemText *keyLabel = new QCPItemText(this);
        keyLabel->setFont(font);
        text = DimFigure::valueTypeToString(value.type);
        widthPx = fontMetrics.horizontalAdvance(text);
        keyLabel->position->setPixelPosition(QPointF(referencePointPx.x + _labelOffsetPx + widthPx / 2, referencePointPx.y + _rowHeightPx / 2));
        keyLabel->setText(text);
        keyLabel->setColor(dimFigure->color());

        QCPItemText *valueLabel = new QCPItemText(this);
        valueLabel->setFont(font);
        text = QString::number(value.measurement, 'f', _project->precision());
        widthPx = fontMetrics.horizontalAdvance(text);
        valueLabel->position->setPixelPosition(QPointF(referencePointPx.x + _columnWidthPx - widthPx / 2 - _labelOffsetPx, referencePointPx.y + _rowHeightPx / 2));
        valueLabel->setText(text);
        valueLabel->setColor(dimFigure->color());

        if(dimFigure->isShowTols()) {
            QCPItemText *toleranceLabel = new QCPItemText(this);
            toleranceLabel->setFont(font);
            text = QString("(%1 %2 %3)").arg(QString::number(value.nominal, 'f', _project->precision()))
                .arg(QString::number(value.upperTolerance, 'f', _project->precision()))
                .arg(QString::number(value.lowerTolerance, 'f', _project->precision()));
            widthPx = fontMetrics.horizontalAdvance(text);
            toleranceLabel->position->setPixelPosition(QPointF(referencePointPx.x + _columnWidthPx + widthPx / 2 + _labelOffsetPx, referencePointPx.y + _rowHeightPx / 2));
            toleranceLabel->setText(text);
            toleranceLabel->setColor(dimFigure->color());
        }

        referencePointPx.y += _rowHeightPx;
        if(i < visibleValues.length() - 1) {
            QCPItemRect *rect = new QCPItemRect(this);
            rect->setBrush(QBrush(QColorConstants::White));
            rect->topLeft->setPixelPosition(toQPointF(referencePointPx));
            rect->bottomRight->setPixelPosition(QPointF(referencePointPx.x + _columnWidthPx, referencePointPx.y + _rowHeightPx));
            referencePointPx.y += _separatorHeightPx;
        }
    }
    /*const auto precision = _project->precision();
    auto table = new QCPItemTable(this, dimFigure->name());
    table->basePosition->setCoords(toQPointF(dimFigure->labelPoint()));
    const auto figure = _project->findFigure(dimFigure->firstReference());
    auto targetPosition = findNearestFigurePoint(dimFigure->labelPoint(), figure);
    table->targetPosition->setCoords(toQPointF(targetPosition));
    table->setIsCurrentFigure(currentFigure() == dimFigure);
    table->setOnlyLabel(dimFigure->isOnlyLabel());
    table->setColor(dimFigure->color());

    for(const auto &value : dimFigure->values()) {
        if(value.isShow == false) continue;
        table->addData(DimFigure::valueTypeToString(value.type), QString::number(value.measurement, 'f', precision));
    }*/
}

void Plot::drawDistanceBetweenCurvePoints(const DimFigure *dimFigure) {
    // TODO: Rewrite function
    const auto firstPointFigure = dynamic_cast<const PointFigure*>(_project->findFigure(dimFigure->firstReference()));
    const auto secondPointFigure = dynamic_cast<const PointFigure*>(_project->findFigure(dimFigure->secondReference()));
    if(firstPointFigure == nullptr || secondPointFigure == nullptr) {
        qDebug() << "Distance between curve points references not found. DimFigure: " << dimFigure->name();
        return;
    }

    QFont font = QFont();
    font.setBold(currentFigure() == dimFigure);
    auto &name = dimFigure->name();
    const auto &firstDimPoint = (Point)firstPointFigure->point();
    const auto &secondDimPoint = (Point)secondPointFigure->point();
    auto labelPoint = Algorithms::getMiddlePoint(firstDimPoint, secondDimPoint);
    auto &labelPointPx = coordToPixel(labelPoint);

    auto label = new QCPItemText(this);
    label->position->setCoords(labelPoint.x, labelPoint.y);
    auto labelText = QString::number(dimFigure->values()[0].measurement, 'f', _project->precision());

    label->setColor(dimFigure->color());
    if(dimFigure->isOnlyLabel()) {
        labelText = name;
    }
    if(dimFigure->isShowTols()) {
        labelText += QString("*\n (%1 %2 %3)").arg(QString::number(dimFigure->values()[0].nominal, 'f', _project->precision()))
            .arg(QString::number(dimFigure->values()[0].upperTolerance, 'f', _project->precision()))
            .arg(QString::number(dimFigure->values()[0].lowerTolerance, 'f', _project->precision()));
        label->setColor(QColorConstants::Red);
    }
    label->setText(labelText);
    label->setFont(font);

    auto totalRect = QRectF(toQPointF(firstDimPoint), toQPointF(secondDimPoint)).normalized();
    auto widthPx = QFontMetrics(font).horizontalAdvance(labelText);
    auto heightPx = QFontMetrics(font).height();
    auto topLeft = toQPointF(pixelToCoord(Point(labelPointPx.x - widthPx / 2.0, labelPointPx.y + heightPx / 2.0)));
    auto bottomRight = toQPointF(pixelToCoord(Point(labelPointPx.x + widthPx / 2.0, labelPointPx.y - heightPx / 2.0)));
    auto textRect = QRectF(topLeft, bottomRight).normalized();
    auto totalArea = totalRect.width() * totalRect.height();
    if(totalArea == 0) return;

    auto textArea = textRect.width() * textRect.height();

    double k;
    if((firstDimPoint.x - secondDimPoint.x) == 0) k = 0;
    else k = (firstDimPoint.y - secondDimPoint.y) / (firstDimPoint.x - secondDimPoint.x);
    auto b = firstDimPoint.y - k * firstDimPoint.x;
    auto isEnoughSpace = (textArea / totalArea) <= 0.25;
    LineFigure *firstLine, *secondLine;

    if(isEnoughSpace) {
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
        
        firstLine = createLineFigure(name, firstPointOnRect, firstDimPoint,
            QCPLineEnding::esNone, QCPLineEnding::esLineArrow, dimFigure->color());
        secondLine = createLineFigure(name, secondPointOnRect, secondDimPoint,
            QCPLineEnding::esNone, QCPLineEnding::esLineArrow, dimFigure->color());
    } else {
        auto direction = Point(firstDimPoint.x - secondDimPoint.x, firstDimPoint.y - secondDimPoint.y);
        auto length = hypot(direction.x, direction.y);
        direction.x /= length;
        direction.y /= length;
        auto firstStartPoint = Point(firstDimPoint.x + direction.x, firstDimPoint.y + direction.y);
        firstLine = createLineFigure(name, firstStartPoint, firstDimPoint,
            QCPLineEnding::esNone, QCPLineEnding::esLineArrow, dimFigure->color());
        direction.x *= -1;
        direction.y *= -1;
        auto secondStartPoint = Point(secondDimPoint.x + direction.x, secondDimPoint.y + direction.y);
        secondLine = createLineFigure(name, secondStartPoint, secondDimPoint,
            QCPLineEnding::esNone, QCPLineEnding::esLineArrow, dimFigure->color());
    }
    firstLine->setColor(dimFigure->color());
    secondLine->setColor(dimFigure->color());
    loadFigure(firstLine);
    loadFigure(secondLine);
    delete firstLine, secondLine;
}

const QHash<double, double> Plot::intersectionLineAndRect(const double k, const double b, const QRectF &rect) const {
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

QPointF Plot::toQPointF(const Point &point) const {
    return QPointF(point.x, point.y);
}

const QString Plot::getTextByValueType(const DimFigure::ValueType &valueType) const {
    QMap<DimFigure::ValueType, QString> texts = {
        { DimFigure::ValueType::X, "X" },
        { DimFigure::ValueType::Y, "Y" },
        { DimFigure::ValueType::Z, "Z" },
        { DimFigure::ValueType::PolarRad, "PR" },
        { DimFigure::ValueType::PolarAngle, "PA" },
        { DimFigure::ValueType::Diameter, "D" },
        { DimFigure::ValueType::Radius, "R" },
        { DimFigure::ValueType::MinMax, "MinMax" },
        { DimFigure::ValueType::Form, "Form" },
        { DimFigure::ValueType::Min, "Min" },
        { DimFigure::ValueType::Max, "Max" },
        { DimFigure::ValueType::MaxAbs, "MaxAbs" },
        { DimFigure::ValueType::SupUT, "SupUT" },
        { DimFigure::ValueType::InfLT, "InfLT" },
        { DimFigure::ValueType::Length, "L" },
    };
    return texts[valueType];
}

void Plot::setCurveDecoration(const CurveFigure *curveFigure, QCPCurve *curve) {
    double penWidth = _penWidth;
    if(_project->currentFigureName() == curveFigure->name()) {
        penWidth = _currentFigurePenWidth;
    }

    const auto brush = QBrush(curveFigure->color());
    const auto pen = QPen(brush, penWidth);
    curve->setPen(pen);

    curve->setLineStyle(QCPCurve::LineStyle::lsNone);
    if(curveFigure->isConnectPoints()) {
        curve->setLineStyle(QCPCurve::LineStyle::lsLine);
    }

    if(curveFigure->isShowPoints()) {
        const int scatterSize = 3;
        const auto scatterShape = QCPScatterStyle::ScatterShape::ssSquare;
        const auto scatterStyle = QCPScatterStyle(scatterShape, scatterSize);
        curve->setScatterStyle(scatterStyle);
    }

    if(curveFigure->isClosed()) {
        const auto &firstPoint = curveFigure->points()[0];
        curve->addData(firstPoint.x, firstPoint.y);
    }
}

void Plot::drawCurve(const CurveFigure *curveFigure) {
    auto curve = new QCPCurve(xAxis, yAxis);
    const auto &curvePoints = curveFigure->points();
    for(const auto &point : curvePoints) {
        curve->addData(point.x, point.y);
    }

    setCurveDecoration(curveFigure, curve);
    addGraph(curve->keyAxis(), curve->valueAxis());
}

void Plot::drawCurveVectors(const CurveFigure *curveFigure) {
    if(curveFigure->isShowVectors()) {
        const auto pen = QPen(QColorConstants::Red);
        for(const auto &point : curveFigure->points()) {
            if(point.i == 0 && point.j == 0) continue;
            auto line = new QCPItemLine(this);

            line->setPen(pen);
            line->setHead(QCPLineEnding::esLineArrow);
            line->start->setCoords(point.x, point.y);
            line->end->setCoords(point.x + point.i, point.y + point.j);
        }
    }
}

void Plot::drawCurveNumbering(const CurveFigure *curveFigure) {
    if(curveFigure->isShowNumbering()) {
        const auto &points = curveFigure->points();
        const auto numberingInterval = curveFigure->numberingInterval();
        if(numberingInterval > points.size()) return;

        double textHeight = QFontMetrics(QFont()).height();
        QPointF labelPointPx;
        QCPItemText *label;
        for(int i = 0; i < points.size(); i += numberingInterval) {
            label = new QCPItemText(this);
            label->setText(QString::number(i + 1));

            label->position->setCoords(QPointF(points[i].x, points[i].y));
            labelPointPx = label->position->pixelPosition();
            labelPointPx.setY(labelPointPx.y() - textHeight / 2);
            label->position->setPixelPosition(labelPointPx);
        }
    }
}

void Plot::drawCurveTolerances(const CurveFigure *curveFigure) {
    const auto amplification = curveFigure->amplification();
    if(curveFigure->isShowTolerances() && amplification != 0) {
        const auto &curveName = curveFigure->name();
        const auto color = QColorConstants::DarkGreen;

        QVector<CurvePoint> upperTolerancePoints, lowerTolerancePoints;
        CurvePoint upperOffsetPoint, lowerOffsetPoint;
        for(const auto &point : curveFigure->points()) {
            if(point.ut != 0) {
                upperOffsetPoint = point;
                upperOffsetPoint.x += point.i * point.ut * amplification;
                upperOffsetPoint.y += point.j * point.ut * amplification;
                upperOffsetPoint.z += point.k * point.ut * amplification;
                upperTolerancePoints.push_back(upperOffsetPoint);
            }

            if(point.lt != 0) {
                lowerOffsetPoint = point;
                lowerOffsetPoint.x += point.i * point.lt * amplification;
                lowerOffsetPoint.y += point.j * point.lt * amplification;
                lowerOffsetPoint.z += point.k * point.lt * amplification;
                lowerTolerancePoints.push_back(lowerOffsetPoint);
            }
        }

        if(upperTolerancePoints.isEmpty() == false) {
            auto upperCurveFigure = new CurveFigure(curveName, upperTolerancePoints);
            upperCurveFigure->setColor(color);
            upperCurveFigure->setClosed(curveFigure->isClosed());
            loadFigure(upperCurveFigure);
        }

        if(lowerTolerancePoints.isEmpty() == false) {
            auto lowerCurveFigure = new CurveFigure(curveName, lowerTolerancePoints);
            lowerCurveFigure->setColor(color);
            lowerCurveFigure->setClosed(curveFigure->isClosed());
            loadFigure(lowerCurveFigure);
        }
    }
}

void Plot::drawCurveDeviations(const CurveFigure *curveFigure) {
    // TODO: Rewrite function to one loop
    if(curveFigure->isShowDeviations()) {
        Point direction, middlePoint;
        double length, amplification = curveFigure->amplification();
        const QString &curveName = curveFigure->name();
        bool isHighLightOut = curveFigure->isHighLightOut();
        for(const auto &point : curveFigure->points()) {
            if(point.dev != 0) {
                direction = Point(point.i, point.j);
                middlePoint = Point(point.x + (point.i * point.dev * amplification) / 2, point.y + (point.j * point.dev * amplification) / 2);

                length = abs(point.dev * amplification);
                const auto lineFigure = new LineFigure(curveName, middlePoint, direction, length);

                lineFigure->setColor(QColorConstants::Red);
                if(point.dev < 0 && isHighLightOut) {
                    lineFigure->setColor(QColorConstants::Blue);
                }

                loadFigure(lineFigure);
                delete lineFigure;
            }
        }
    }

    const auto amplification = curveFigure->amplification();
    if(curveFigure->isConnectDeviations() && amplification != 0) {
        // TODO: rewrite
        const auto &points = curveFigure->points();
        auto isHighLight = curveFigure->isHighLightOut();

        auto offsetPoint = points[0];
        offsetPoint.x += points[0].i * points[0].dev * amplification;
        offsetPoint.y += points[0].j * points[0].dev * amplification;
        offsetPoint.z += points[0].k * points[0].dev * amplification;

        QVector<CurvePoint> devPoints = { offsetPoint };
        QColor color;
        char pointSign, devPointSign;
        for(int i = 1; i < points.size(); i++) {
            auto point = points[i];
            if(point.dev == 0) continue;

            point.x += point.i * point.dev * amplification;
            point.y += point.j * point.dev * amplification;
            point.z += point.k * point.dev * amplification;

            pointSign = point.dev / abs(point.dev);
            devPointSign = devPoints.last().dev / abs(devPoints.last().dev);

            if(pointSign != devPointSign) {
                color = QColorConstants::Blue;
                if(isHighLight) {
                    color = devPointSign == -1 ? QColorConstants::Blue : QColorConstants::Red;
                }

                auto figure = new CurveFigure(curveFigure->name(), devPoints);
                figure->setColor(color);
                loadFigure(figure);
                devPoints = { devPoints.last(), point };
                delete figure;
            } else {
                devPoints.push_back(point);
            }
        }

        // for last segment
        color = QColorConstants::Blue;
        if(isHighLight) {
            devPointSign = devPoints.last().dev / abs(devPoints.last().dev);
            color = devPointSign == -1 ? QColorConstants::Blue : QColorConstants::Red;
        }

        auto figure = new CurveFigure(curveFigure->name(), devPoints);
        figure->setColor(color);
        loadFigure(figure);
        delete figure;
    }

    if(curveFigure->isShowNumericalDeviations()) {
        // TODO: rewrite
        const double offsetX = -0.05, offsetY = 0.1;
        const int devNumericalInterval = curveFigure->numericalInterval();
        const auto &points = curveFigure->points();

        for(int i = 0; i < points.size(); i += devNumericalInterval) {
            auto &point = points[i];
            auto label = new QCPItemText(this);
            auto font = label->font();
            font.setPointSize(8);
            label->setFont(font);
            label->setText(QString::number(point.dev, 'f', 3));
            label->position->setCoords(point.x + offsetX, point.y + offsetY);
        }
    }
}