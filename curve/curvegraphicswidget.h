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
    void drawCurve(const Project *project, QListWidgetItem *currentItemOfFirstList, Qt::GlobalColor colorOfItemFromFisrtList = Qt::black, double widthOfItemFromFirstList = 0.1,
        QListWidgetItem *currentItemOfSecondList = nullptr, Qt::GlobalColor colorOfItemFromSecondList = Qt::black, double widthOfItemFromSecondList = 0.1);

private:
    Ui::CurveGraphicsWidget *_ui;
    QGraphicsScene *_scene;
    QVector<QGraphicsLineItem*> *_linesOfCurve;

    int _margin;

    void drawLines(const CurveFigure *curve, const QVector<CurvePoint> &points, QRectF &boundingRect, Qt::GlobalColor color, double width);
    void clearScene();
};
