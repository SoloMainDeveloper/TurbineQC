#pragma once

#include <QDialog>
#include "ui_figureeditdialog.h"
#include "project.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FigureEditDialogClass; };
QT_END_NAMESPACE

class FigureEditDialog : public QDialog
{
    Q_OBJECT

public:
    FigureEditDialog(Project* mainProject, QWidget *parent = nullptr);
    ~FigureEditDialog();

public slots:
    void dialogInitialization(const QString figureName);

private:
    Ui::FigureEditDialogClass *_ui;
    Project* _project;
    QTableWidget* _curveTable;
    void updatePosition(double x, double y, double z);
    void updateDirection(double i, double j, double k);
    void updateParameter(QString parameter, double l);
};
