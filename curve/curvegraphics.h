#pragma once

#include "project.h"

class CurveGraphics {
public:
    explicit CurveGraphics(Project *project, QGraphicsView *graphicsView);
    virtual ~CurveGraphics();

    void setMargin(int margin);
    void drawCurve(QListWidgetItem *currentItemOfFirstList, Qt::GlobalColor colorOfItemFromFisrtList = Qt::black, double widthOfItemFromFirstList = 0.1,
        QListWidgetItem *currentItemOfSecondList = nullptr, Qt::GlobalColor colorOfItemFromSecondList = Qt::black, double widthOfItemFromSecondList = 0.1);

private:
    Project *_project;
    QGraphicsView *_graphicsView;
    QGraphicsScene *_scene;
    QVector<QGraphicsLineItem*> *_linesOfCurve;

    int _margin;

    void clearScene();
};
