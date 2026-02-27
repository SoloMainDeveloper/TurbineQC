#pragma once

#include "ui_curvegraphicswidget.h"
#include "project.h"
#include <QWidget>

class CurveGraphicsWidget : public QWidget {
    Q_OBJECT

public:
    explicit CurveGraphicsWidget(QWidget *parent = nullptr);
    virtual ~CurveGraphicsWidget();

    void initialization();
    void setMargin(int margin);
    void drawCurve(const CurveFigure *firstCurve, Qt::GlobalColor firstColor = Qt::black, double firstWidth = 0.1, const CurveFigure *secondCurve = nullptr, Qt::GlobalColor secondColor = Qt::black, double secondWidth = 0.1);

private:
    Ui::CurveGraphicsWidget *_ui;
    QGraphicsScene *_scene;
    QVector<QGraphicsLineItem*> *_linesOfCurve;

    int _margin;

    void drawLines(const CurveFigure *curve, QRectF &boundingRect, Qt::GlobalColor color, double width);
    void clearScene();
};
