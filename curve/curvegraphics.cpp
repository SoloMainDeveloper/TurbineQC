#include "curve/pch.h"
#include "curvegraphics.h"

CurveGraphics::CurveGraphics(Project *project, QGraphicsView *graphicsView) {
    _project = project;
    _graphicsView = graphicsView;
    _scene = new QGraphicsScene();
    _scene->setSceneRect(0, 0, 0, 0);
    _graphicsView->setScene(_scene);
    _linesOfCurve = new QVector<QGraphicsLineItem*>;
    _margin = 0;
}

void CurveGraphics::setMargin(int margin) {
    _margin = margin;
}

void CurveGraphics::drawCurve(QListWidgetItem *currentItemOfFirstList, Qt::GlobalColor colorOfItemFromFisrtList, double widthOfItemFromFirstList,
    QListWidgetItem *currentItemOfSecondList, Qt::GlobalColor colorOfItemFromSecondList, double widthOfItemFromSecondList) {
    QRectF boundingRect;
    clearScene();

    if(currentItemOfFirstList != nullptr) {
        auto pointsOfFirstCurve = dynamic_cast<const CurveFigure*>(_project->findFigure(currentItemOfFirstList->text()))->points();

        for(auto i = 0; i < pointsOfFirstCurve.length() - 1; i++) {
            auto currentPoint = &pointsOfFirstCurve[i];
            auto nextPoint = &pointsOfFirstCurve[i + 1];
            auto *line = new QGraphicsLineItem(currentPoint->x, -currentPoint->y, nextPoint->x, -nextPoint->y);
            boundingRect = boundingRect.united(line->boundingRect());
            line->setPen(QPen(colorOfItemFromFisrtList, widthOfItemFromFirstList));
            _scene->addItem(line);
            _linesOfCurve->append(line);
        }

    }
    if(currentItemOfSecondList != nullptr) {
        auto pointsOfSecondCurve = dynamic_cast<const CurveFigure*>(_project->findFigure(currentItemOfSecondList->text()))->points();

        for(auto i = 0; i < pointsOfSecondCurve.length() - 1; i++) {
            auto currentPoint = &pointsOfSecondCurve[i];
            auto nextPoint = &pointsOfSecondCurve[i + 1];
            auto *line = new QGraphicsLineItem(currentPoint->x, -currentPoint->y, nextPoint->x, -nextPoint->y);
            boundingRect = boundingRect.united(line->boundingRect());
            line->setPen(QPen(colorOfItemFromSecondList, widthOfItemFromSecondList));
            _scene->addItem(line);
            _linesOfCurve->append(line); 
        }
    }
    _scene->setSceneRect(boundingRect);
    _graphicsView->setScene(_scene);
    _graphicsView->fitInView(boundingRect.adjusted(-_margin, -_margin, _margin, _margin), Qt::KeepAspectRatio);
}

void CurveGraphics::clearScene() {
    if(_linesOfCurve->length() != 0) {
        for(auto line : *_linesOfCurve) {
            _scene->removeItem(line);
        }
        _linesOfCurve->clear();
    }
}

CurveGraphics::~CurveGraphics() {
}
