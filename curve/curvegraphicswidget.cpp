#include "curve/pch.h"

#include "curvegraphicswidget.h"

CurveGraphicsWidget::CurveGraphicsWidget(QWidget *parent) : QWidget(parent), _ui(new Ui::CurveGraphicsWidget) {
    _ui->setupUi(this);
    _ui->gridLayout->setContentsMargins(0, 0, 0, 0);
    initialization();
    _margin = 0;
}

void CurveGraphicsWidget::initialization() {
    _scene = new QGraphicsScene();
    _scene->setSceneRect(0, 0, 0, 0);
    _ui->graphicsView->setScene(_scene);
    _linesOfCurve = new QVector<QGraphicsLineItem*>;
}

void CurveGraphicsWidget::setMargin(int margin) {
    _margin = margin;
}

void CurveGraphicsWidget::drawCurve(const CurveFigure *firstCurve, Qt::GlobalColor firstColor, double firstWidth, const CurveFigure *secondCurve, Qt::GlobalColor secondColor, double secondWidth) {
    QRectF boundingRect;
    clearScene();

    if(firstCurve != nullptr) {
        drawLines(firstCurve, boundingRect, firstColor, firstWidth);
    }
    if(secondCurve != nullptr) {
        drawLines(secondCurve, boundingRect, secondColor, secondWidth);
    }
    _scene->setSceneRect(boundingRect);
    _ui->graphicsView->setScene(_scene);
    _ui->graphicsView->fitInView(boundingRect.adjusted(-_margin, -_margin, _margin, _margin), Qt::KeepAspectRatio);
}

void CurveGraphicsWidget::drawLines(const CurveFigure *curve, QRectF &boundingRect, Qt::GlobalColor color, double width) {
    auto points = curve->points();
    for(auto i = 0; i < points.length() - 1; i++) {
        auto currentPoint = &points[i];
        auto nextPoint = &points[i + 1];
        auto *line = new QGraphicsLineItem(currentPoint->x, -currentPoint->y, nextPoint->x, -nextPoint->y);
        boundingRect = boundingRect.united(line->boundingRect());
        line->setPen(QPen(color, width));
        _scene->addItem(line);
        _linesOfCurve->append(line);
    }
    if(curve->isClosed()) {
        auto firstPoint = &points.first();
        auto lastPoint = &points.last();
        auto *line = new QGraphicsLineItem(firstPoint->x, -firstPoint->y, lastPoint->x, -lastPoint->y);
        boundingRect = boundingRect.united(line->boundingRect());
        line->setPen(QPen(color, width));
        _scene->addItem(line);
        _linesOfCurve->append(line);
    }
}

void CurveGraphicsWidget::clearScene() {
    if(_linesOfCurve->length() != 0) {
        for(auto line : *_linesOfCurve) {
            _scene->removeItem(line);
        }
        _linesOfCurve->clear();
    }
}

CurveGraphicsWidget::~CurveGraphicsWidget() {
    delete _ui;
}
