#pragma once

#include <QDialog>
#include "ui_shiftdialog.h"
#include "project.h"
#include "qstandarditemmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ShiftDialogClass; };
QT_END_NAMESPACE

class ShiftDialog : public QDialog
{
    Q_OBJECT

public:
    ShiftDialog(Project *project = nullptr, QWidget *parent = nullptr);
    ~ShiftDialog();

    void initialization();
    void addTreeTab(QStandardItem* tab);

private slots:
    void applyShift();

signals:
    void figureShiftRequested(QString figureName, QString x, QString y, QString z);

private:
    Ui::ShiftDialogClass *_ui;
    Project *_project;

    QStandardItemModel* _treeModel;
    QStandardItem* _treeCurves;
    QStandardItem* _treeCircles;
    QStandardItem* _treeLines;
    QStandardItem* _treePoints;
};
