#pragma once

#include <QDialog>
#include "ui_rotatedialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RotateDialogClass; };
QT_END_NAMESPACE

class RotateDialog : public QDialog
{
    Q_OBJECT

public:
    RotateDialog(Project* project, QWidget *parent = nullptr);
    ~RotateDialog();

    void initialization();
    void addTreeTab(QStandardItem* tab);

private slots:
    void applyRotate();
    void plus90();
    void minus90();
    void updateXYZ(QString figureName);

signals:
    void figureRotateRequested(QString figureName, double angle, QString x, QString y, QString z);

private:
    Ui::RotateDialogClass *_ui;

    Project* _project;

    QStandardItemModel* _treeModel;
    QStandardItem* _treeCurves;
    QStandardItem* _treeCircles;
    QStandardItem* _treeLines;
    QStandardItem* _treePoints;
};
