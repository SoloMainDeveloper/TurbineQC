#include "curve/pch.h"
#include "curve/plot.h"

Plot::Plot(QWidget *parent) : QCustomPlot(parent), _project(nullptr) {
    xAxis->setUpperEnding(QCPLineEnding::esLineArrow);
    yAxis->setUpperEnding(QCPLineEnding::esLineArrow);

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
    connect(project, &Project::cleared, this, &Plot::clear);

    connect(this, &Plot::plottableClick, this, &Plot::onPlottableClicked);
    connect(this, &Plot::plottableDoubleClick, this, &Plot::onPlottableDoubleClicked);
    connect(this, &Plot::itemClick, this, &Plot::onItemClicked);
    connect(this, &Plot::itemDoubleClick, this, &Plot::onItemDoubleClicked);
    connect(this, &Plot::currentFigureChanged, project, &Project::changeCurrentFigure);
    connect(this, &Plot::figureEditDialogRequested, project, &Project::requestFigureEditDialog);
    connect(this, &Plot::mouseWheel, this, &Plot::onMouseWheel);
    connect(this, &Plot::scaled, _project, &Project::changeScale);

    _replotTimer = new QTimer(this);
    _replotTimer->setSingleShot(true);
    _replotTimer->setInterval(500);
    connect(_replotTimer, &QTimer::timeout, this, &Plot::replotTimerTimeout);
}

const Figure* Plot::currentFigure() const {
    return _project->findFigure(_project->currentFigureName());
}

void Plot::addFigure(Figure *figure) {
    createFigureWithoutReplot(figure);
    //rescaleToCurve(PositionType::Center);
    if(_isFirstFigure) {
        if(auto curve = dynamic_cast<CurveFigure*>(figure)) {
            _isFirstFigure = false;
            zoomExtents();
            qDebug() << "Init scale factor: " << _scaleFactor;
        }
    }
    layer(figure->name())->replot();
};

void Plot::renameFigure(const QString figureName, const QString newName) {
    auto figure = _project->findFigure(newName);

    deleteFigure(figureName);
    createFigureWithoutReplot(figure);
}

void Plot::removeFigure(const QString figureName) {
    deleteFigure(figureName);
}

void Plot::changeCurrentFigure(const QString &currentFigureName, const QString &previousFigureName) {
    if(!previousFigureName.isNull()) {
        updateFigure(previousFigureName);
    }

    if(!currentFigureName.isNull()) {
        updateFigure(currentFigureName);
    }
}

void Plot::changeFigureVisibility(const QString figureName, bool visible) {
    layer(figureName)->setVisible(visible);
    layer(figureName)->replot();
    MacrosManager::log(MacrosManager::FigureVisibilityChanged, { { "figureName", figureName },
        { "isVisible", visible ? "true" : "false" } });
}

void Plot::changeCurveParameters(const QString curveName, bool showPoints, bool connectPoints,
    bool showVectors, bool closed, bool showNumbering, int numberingInterval, double amplification,
    bool showTolerances, bool showDeviations, bool connectDeviations, bool highLightOut) {
    updateFigure(curveName);
}

void Plot::changeCurveTolerance(const QString curveName) {
    updateFigure(curveName);
}

void Plot::changeFigureColor(const QString figureName) {
    updateFigure(figureName);
}

char Plot::sign(double value) {
    return value / abs(value);
}

void Plot::addDimension(DimFigure *dimFigure) {
    _project->safeInsert(dimFigure->name(), dimFigure);
}

void Plot::changeScale(double scaleFactor, const Point &center) {
    xAxis->scaleRange(scaleFactor / _scaleFactor, center.x);
    yAxis->scaleRange(scaleFactor / _scaleFactor, center.y);
    _scaleFactor = scaleFactor;
    layer("axes")->replot();
}

void Plot::clear() {
    _scaleFactor = 1.0;
    _isFirstFigure = true;
}

void Plot::zoomExtents() {
    QCPRange maxKeyRange, maxValueRange, keyRange, valueRange;
    bool foundKeyRange, foundValueRange;
    for(const auto &plottable : mPlottables) {
        if(const auto curve = dynamic_cast<Curve*>(plottable)) {
            keyRange = curve->getKeyRange(foundKeyRange);
            valueRange = curve->getValueRange(foundValueRange);

            if(foundKeyRange && foundValueRange) {
                maxKeyRange.expand(keyRange);
                maxValueRange.expand(valueRange);
            }
        }
    }

    if(keyRange.size() != 0 && valueRange.size() != 0) {
        QCPRange maxRange = maxKeyRange.size() > maxValueRange.size() ? maxKeyRange : maxValueRange;
        xAxis->setRange(maxRange);
        yAxis->setRange(maxRange);

        _scaleFactor = yAxis->range().size() / 100;
        zoomMinus();
        replotTimerTimeout();
    }
}

void Plot::zoomPlus() {
    zoomPlusToPoint(currentCenterPoint());
}

void Plot::zoomPlusToPoint(const Point &point) {
    auto newScale = _scaleFactor * (1 + _scaleStepPercent / 100);
    emit scaled(newScale, point);
}

void Plot::zoomMinus() {
    zoomMinusToPoint(currentCenterPoint());
}

void Plot::zoomMinusToPoint(const Point &point) {
    auto newScale = _scaleFactor * (1 - _scaleStepPercent / 100);
    emit scaled(newScale, point);
}

Point Plot::currentCenterPoint() const {
    return Point(xAxis->range().center(), yAxis->range().center());
}

double Plot::pointRadius() {
    assert(_minPointRadius <= _maxPointRadius);
    double radius = qMin(_maxPointRadius, _scaleFactor / _maxPointRadius);
    radius = qMax(radius, _minPointRadius);
    return radius;
}

const Point Plot::pixelToCoord(const Point &point) const {
    return Point(xAxis->pixelToCoord(point.x), yAxis->pixelToCoord(point.y));
}

const Point Plot::coordToPixel(const Point &point) const {
    return Point(xAxis->coordToPixel(point.x), yAxis->coordToPixel(point.y));
}

void Plot::createFigureWithoutReplot(const Figure *figure) {
    if(layer(figure->name()) == nullptr) {
        addLayer(figure->name());
        layer(figure->name())->setVisible(figure->isVisible());
    }

    setCurrentLayer(figure->name());

    layer(figure->name())->setVisible(figure->isVisible());

    if(auto curveFigure = dynamic_cast<const CurveFigure*>(figure)) {
        addCurveLayer(*curveFigure);
    } else if(auto lineFigure = dynamic_cast<const LineFigure*>(figure)) {
        addLineLayer(*lineFigure);
    } else if(auto pointFigure = dynamic_cast<const PointFigure*>(figure)) {
        addPointLayer(*pointFigure);
    } else if(auto circleFigure = dynamic_cast<const CircleFigure*>(figure)) {
        addCircleLayer(*circleFigure);
    } else if(auto dimFigure = dynamic_cast<const DimFigure*>(figure)) {
        emit renderDimension(dimFigure);
    }
}

void Plot::updateFigure(const QString &figureName) {
    auto figure = _project->findFigure(figureName);

    if(layer(figureName) == nullptr) {
        qDebug() << "Layer" << figureName << "does not exist";
        return;
    }

    clearFiguresFromLayer(figureName);
    createFigureWithoutReplot(figure);
    layer(figureName)->replot();
}

void Plot::deleteFigure(const QString &figureName) {
    auto layerToDelete = layer(figureName);

    clearFiguresFromLayer(figureName);

    layer(figureName)->replot();

    removeLayer(layerToDelete);

    if(_project->figures().isEmpty()) {
        _isFirstFigure = true;
    }
}

void Plot::addCurveLayer(const CurveFigure &curveFigure) {
    auto curve = new Curve(this, &curveFigure, xAxis, yAxis);

    double penWidth;
    if(_project->currentFigureName() == curveFigure.name()) {
        penWidth = _currentFigurePenWidth;
    } else {
        penWidth = _penWidth;
    }

    auto brush = QBrush(curveFigure.color());
    auto pen = QPen(brush, penWidth);
    curve->setPen(pen);
    const auto curvePoints = curveFigure.points();

    for(const auto &point : curvePoints) {
        curve->addData(point.x, point.y);
    }

    if(curveFigure.isConnectPoints()) {
        curve->connectPoints();
    }

    if(curveFigure.isShowPoints()) {
        curve->showPoints();
    }

    if(curveFigure.isShowVectors()) {
        curve->showVectors();
    }

    if(curveFigure.isClosed()) {
        curve->addData(curvePoints[0].x, curvePoints[0].y);
    }

    if(curveFigure.isShowNumbering()) {
        curve->showNumbering();
    }

    if(curveFigure.isShowTolerances()) {
        curve->showTolerances();
    }

    if(curveFigure.isShowDeviations()) {
        curve->showDeviations();
    }

    if(curveFigure.isShowNumericalDeviations()) {
        curve->showNumericalDeviations(curveFigure.numericalInterval());
    }

    if(curveFigure.isConnectDeviations()) {
        curve->connectDeviations();
    }

    addGraph(curve->keyAxis(), curve->valueAxis());
}

void Plot::addPointLayer(const PointFigure &pointFigure) {
    auto circleFigure = CircleFigure(pointFigure.name(), Point(pointFigure.point()), Point(0, 0, 1), _pointRadius);

    auto &pointColor = pointFigure.color();
    circleFigure.setColor(pointColor);

    addCircleLayer(circleFigure);
}

void Plot::addLineLayer(const LineFigure &lineFigure) {
    auto length = lineFigure.length();

    double penWidth;
    if(_project->currentFigureName() == lineFigure.name()) {
        penWidth = _currentFigurePenWidth;
    } else {
        penWidth = _penWidth;
    }

    auto brush = QBrush(lineFigure.color());
    auto pen = QPen(brush, penWidth);

    if(length == qInf()) {
        auto line = new QCPItemStraightLine(this);
        line->setPen(pen);

        auto origin = lineFigure.origin();
        auto direction = lineFigure.direction();

        line->point1->setCoords(origin.x, origin.y);
        line->point2->setCoords(origin.x + direction.x, origin.y + direction.y);
    } else {
        auto line = new QCPItemLine(this);
        line->setPen(pen);

        auto origin = lineFigure.origin();
        auto direction = lineFigure.direction();
        double halfLength = 0.5 * length;

        auto startPoint = Point(origin.x - halfLength * direction.x, origin.y - halfLength * direction.y);
        auto endPoint = Point(origin.x + halfLength * direction.x, origin.y + halfLength * direction.y);

        line->start->setCoords(startPoint.x, startPoint.y);
        line->end->setCoords(endPoint.x, endPoint.y);

        line->setHead(lineFigure.head());
        line->setTail(lineFigure.tail());
    }
}

void Plot::addCircleLayer(const CircleFigure &circleFigure) {
    auto circle = new QCPItemEllipse(this);

    double penWidth;
    if(_project->currentFigureName() == circleFigure.name()) {
        penWidth = _currentFigurePenWidth;
    } else {
        penWidth = _penWidth;
    }

    auto brush = QBrush(circleFigure.color());
    auto pen = QPen(brush, penWidth);
    circle->setPen(pen);

    calculateCircleBox(circleFigure, circle);
}

void Plot::calculateCircleBox(const CircleFigure &circleFigure, QCPItemEllipse *circle) {
    auto center = circleFigure.center();
    auto radius = circleFigure.radius();

    auto topLeftAnchor = QPointF(center.x - radius, center.y + radius);
    auto bottomRightAnchor = QPointF(center.x + radius, center.y - radius);
    circle->topLeft->setCoords(topLeftAnchor);
    circle->bottomRight->setCoords(bottomRightAnchor);
}

void Plot::replotTimerTimeout() {
    const auto &currentCursor = cursor();
    setCursor(QCursor(Qt::BusyCursor));
    for(auto &figure : _project->figures()) {
        if(dynamic_cast<PointFigure*>(figure) != nullptr || dynamic_cast<DimFigure*>(figure) != nullptr) {
            updateFigure(figure->name());
        }
    }
    setCursor(currentCursor);
}

void Plot::clearFiguresFromLayer(const QString &layerName) {
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

void Plot::rescaleToCurve(Position position) {
    this->rescaleAxes(true);

    auto xMin = this->xAxis->range().lower;
    auto xMax = this->xAxis->range().upper;
    auto xDistance = xMax - xMin;

    auto yMin = this->yAxis->range().lower;
    auto yMax = this->yAxis->range().upper;
    auto yDistance = yMax - yMin;
    auto yMid = (yMin + yMax) / 2;
    auto yRangeDistance = xDistance * 5 / 7;

    if(position == Position::Right) {
        auto xFractionOfDistanceRight = 0.15;
        auto xFractionOfDistanceLeft = 0.35;
        auto yFractioOfDistance = 0.12;

        auto xMarginRight = (xMax - xMin) * xFractionOfDistanceRight;
        auto xMarginLeft = (xMax - xMin) * xFractionOfDistanceLeft;
        auto yMargin = (yMax - yMin) * yFractioOfDistance;

        if(xDistance > yDistance * 3 / 2) {
            this->xAxis->setRange(xMin - xMarginLeft, xMax + xMarginRight);
            this->yAxis->setRange((yMid - yRangeDistance / 2) - yMargin, (yMid + yRangeDistance / 2) + yMargin);
        } else {
            this->xAxis->setRange(xMin - xMarginLeft, xMax + xMarginRight);
            this->yAxis->setRange(yMin - yMargin, yMax + yMargin);
        }
    } else if(position == Position::Left) {
        auto xFractionOfDistanceRight = 0.35;
        auto xFractionOfDistanceLeft = 0.15;
        auto yFractioOfDistance = 0.12;

        auto xMarginRight = (xMax - xMin) * xFractionOfDistanceRight;
        auto xMarginLeft = (xMax - xMin) * xFractionOfDistanceLeft;
        auto yMargin = (yMax - yMin) * yFractioOfDistance;

        if(xDistance > yDistance * 3 / 2) {
            this->xAxis->setRange(xMin - xMarginLeft, xMax + xMarginRight);
            this->yAxis->setRange((yMid - yRangeDistance / 2) - yMargin, (yMid + yRangeDistance / 2) + yMargin);
        } else {
            this->xAxis->setRange(xMin - xMarginLeft, xMax + xMarginRight);
            this->yAxis->setRange(yMin - yMargin, yMax + yMargin);
        }
    } else {
        auto xFractionOfDistance = 0.15;
        auto yFractioOfDistance = 0.12;

        auto xMargin = (xMax - xMin) * xFractionOfDistance;
        auto yMargin = (yMax - yMin) * yFractioOfDistance;

        if(xDistance > yDistance * 3 / 2) {
            this->xAxis->setRange(xMin - xMargin, xMax + xMargin);
            this->yAxis->setRange((yMid - yRangeDistance / 2) - yMargin, (yMid + yRangeDistance / 2) + yMargin);
        } else {
            this->xAxis->setRange(xMin - xMargin, xMax + xMargin);
            this->yAxis->setRange(yMin - yMargin, yMax + yMargin);
        }
    }
    this->replot();
}

void Plot::makeScreenshot(const QString &filePath, int width, int height, ReportData::Axis axisType) {
    if(axisType == ReportData::Axis::Center) {
        //to do
    } else if(axisType == ReportData::Axis::LeftAndDown) {
        this->xAxis->grid()->setVisible(false);
        this->yAxis->grid()->setVisible(false);
        this->savePng(filePath, width, height);
        this->xAxis->grid()->setVisible(true);
        this->yAxis->grid()->setVisible(true);
    } else {
        this->xAxis->setVisible(false);
        this->yAxis->setVisible(false);
        this->savePng(filePath, width, height);
        this->xAxis->setVisible(true);
        this->yAxis->setVisible(true);
    }
}

double Plot::defaultFontSize() const {
    return _defaultFontSize;
}

double Plot::scaleCoefficient() const {
    return _magnitude * _defaultScaleFactor;
}

Point Plot::centerPoint() const {
    return Point(xAxis->range().center(), yAxis->range().center());
}

Plot::Curve::Curve(Plot *plot, const CurveFigure *curveFigure, QCPAxis *keyAxis, QCPAxis *valueAxis) : QCPCurve(keyAxis, valueAxis) {
    setLineStyle(LineStyle::lsNone);

    const auto scatterShape = QCPScatterStyle::ScatterShape::ssNone;
    const auto scatterStyle = QCPScatterStyle(scatterShape, _scatterSize);
    setScatterStyle(scatterStyle);
    setScatterSkip(_scatterSkip);

    _plot = plot;
    _curveFigure = curveFigure;
}

void Plot::Curve::connectPoints() {
    setLineStyle(LineStyle::lsLine);
}

void Plot::Curve::showPoints() {
    const auto scatterShape = QCPScatterStyle::ScatterShape::ssSquare;
    const auto scatterStyle = QCPScatterStyle(scatterShape, _scatterSize);
    setScatterStyle(scatterStyle);
}

void Plot::Curve::showTolerances() {
    const auto &curveName = _curveFigure->name();
    const auto color = QColorConstants::DarkGreen;
    const auto amplification = _curveFigure->amplification();
    QVector<CurvePoint> upperTolerancePoints, lowerTolerancePoints;

    if(amplification == 0) return;

    for(const auto &point : _curveFigure->points()) {
        if(point.ut != 0) {
            auto upperOffsetPoint = point;
            upperOffsetPoint.x += point.i * point.ut * amplification;
            upperOffsetPoint.y += point.j * point.ut * amplification;
            upperOffsetPoint.z += point.k * point.ut * amplification;
            upperTolerancePoints.push_back(upperOffsetPoint);
        }

        if(point.lt != 0) {
            auto lowerOffsetPoint = point;
            lowerOffsetPoint.x += point.i * point.lt * amplification;
            lowerOffsetPoint.y += point.j * point.lt * amplification;
            lowerOffsetPoint.z += point.k * point.lt * amplification;
            lowerTolerancePoints.push_back(lowerOffsetPoint);
        }
    }

    if(upperTolerancePoints.isEmpty() == false) {
        auto upperCurveFigure = new CurveFigure(curveName, upperTolerancePoints);
        upperCurveFigure->setColor(color);
        upperCurveFigure->setClosed(_curveFigure->isClosed());
        _plot->createFigureWithoutReplot(upperCurveFigure);
    }

    if(lowerTolerancePoints.isEmpty() == false) {
        auto lowerCurveFigure = new CurveFigure(curveName, lowerTolerancePoints);
        lowerCurveFigure->setColor(color);
        lowerCurveFigure->setClosed(_curveFigure->isClosed());
        _plot->createFigureWithoutReplot(lowerCurveFigure);
    }
}

void Plot::Curve::showDeviations() {
    Point direction, middlePoint;
    double length, amplification = _curveFigure->amplification();
    QString curveName = _curveFigure->name();
    bool isHighLightOut = _curveFigure->isHighLightOut();
    for(const auto &point : _curveFigure->points()) {
        if(point.dev == 0) continue;
        direction = Point(point.i, point.j);
        middlePoint = Point(point.x + (point.i * point.dev * amplification) / 2, point.y + (point.j * point.dev * amplification) / 2);

        length = abs(point.dev * amplification);
        const auto lineFigure = new LineFigure(curveName, middlePoint, direction, length);

        lineFigure->setColor(QColorConstants::Red);
        if(point.dev < 0 && isHighLightOut) {
            lineFigure->setColor(QColorConstants::Blue);
        }

        _plot->createFigureWithoutReplot(lineFigure);
    }
}

void Plot::Curve::connectDeviations() {
    auto amplification = _curveFigure->amplification();
    if(amplification == 0) return;

    auto &points = _curveFigure->points();
    auto isHighLight = _curveFigure->isHighLightOut();

    auto offsetPoint = points[0];
    offsetPoint.x += points[0].i * points[0].dev * amplification;
    offsetPoint.y += points[0].j * points[0].dev * amplification;
    offsetPoint.z += points[0].k * points[0].dev * amplification;

    QVector<CurvePoint> devPoints = { offsetPoint };
    QColor color;
    for(int i = 1; i < points.size(); i++) {
        auto point = points[i];
        if(point.dev == 0) continue;

        point.x += point.i * point.dev * amplification;
        point.y += point.j * point.dev * amplification;
        point.z += point.k * point.dev * amplification;

        if(Plot::sign(point.dev) != Plot::sign(devPoints.last().dev)) {
            color = QColorConstants::Blue;
            if(isHighLight) {
                color = Plot::sign(devPoints.last().dev) < 0 ? QColorConstants::Blue : QColorConstants::Red;
            }

            auto curveFigure = new CurveFigure(_curveFigure->name(), devPoints);
            curveFigure->setColor(color);
            _plot->createFigureWithoutReplot(curveFigure);
            devPoints = { devPoints.last(), point };
        } else {
            devPoints.push_back(point);
        }
    }

    // for last segment
    color = QColorConstants::Blue;
    if(isHighLight) {
        color = Plot::sign(devPoints.last().dev) < 0 ? QColorConstants::Blue : QColorConstants::Red;
    }

    auto curveFigure = new CurveFigure(_curveFigure->name(), devPoints);
    curveFigure->setColor(color);
    _plot->createFigureWithoutReplot(curveFigure);
}

void Plot::Curve::showVectors() {
    auto pen = QPen(QColorConstants::Red);
    for(auto &point : _curveFigure->points()) {
        if(point.i == 0 && point.j == 0) continue;
        auto line = new QCPItemLine(parentPlot());

        line->setPen(pen);
        line->setHead(QCPLineEnding::esLineArrow);
        line->start->setCoords(point.x, point.y);
        line->end->setCoords(point.x + point.i, point.y + point.j);
    }
}

void Plot::Curve::showNumbering() {
    auto numberingInterval = _curveFigure->numberingInterval();
    if(numberingInterval == 0) return;

    auto &points = _curveFigure->points();
    const double offsetX = 0, offsetY = -0.05;

    for(int i = 0; i < points.size(); i += numberingInterval) {
        auto &point = points[i];
        auto label = new QCPItemText(_plot);
        label->setText(QString::number(i + 1));
        label->position->setCoords(point.x + offsetX, point.y + offsetY);
    }
}

void Plot::Curve::showNumericalDeviations(int devNumericalInterval) {
    const double offsetX = -0.05, offsetY = 0.1;
    auto &points = _curveFigure->points();

    for(int i = 0; i < points.size(); i += devNumericalInterval) {
        auto &point = points[i];
        auto label = new QCPItemText(_plot);
        auto font = label->font();
        font.setPointSize(8);
        label->setFont(font);
        label->setText(QString::number(point.dev, 'f', 3));
        label->position->setCoords(point.x + offsetX, point.y + offsetY);
    }
}

void Plot::onItemClicked(QCPAbstractItem *item, QMouseEvent *event) {
    auto figureName = item->layer()->name();
    if(figureName == "CALLOUT") return;
    emit currentFigureChanged(figureName);
    updateFigure(figureName);
}

void Plot::onItemDoubleClicked(QCPAbstractItem* item, QMouseEvent* event) {
    auto figureName = item->layer()->name();
    if(figureName == "CALLOUT") return;
    emit figureEditDialogRequested(figureName);
}

void Plot::onPlottableClicked(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event) {
    auto figureName = plottable->layer()->name();
    if(figureName == "CALLOUT") return;
    emit currentFigureChanged(figureName);
    updateFigure(figureName);
}

void Plot::onPlottableDoubleClicked(QCPAbstractPlottable* plottable, int dataIndex, QMouseEvent* event) {
    auto figureName = plottable->layer()->name();
    if(figureName == "CALLOUT") return;
    emit figureEditDialogRequested(figureName);
}

void Plot::onMouseWheel(QWheelEvent *event) {
    if(event->angleDelta().y() > 0) {
        Point mousePoint = pixelToCoord(Point(event->position().x(), event->position().y()));
        zoomPlusToPoint(mousePoint);
    } else if(event->angleDelta().y() < 0) {
        Point mousePoint = pixelToCoord(Point(event->position().x(), event->position().y()));
        zoomMinusToPoint(mousePoint);
    }
    _replotTimer->start();
}
