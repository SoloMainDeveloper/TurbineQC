#pragma once

#include <QDialog>
#include "ui_radiuscorrectiondialog.h"
#include "project.h"
#include "algorithms.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RadiusCorrectionDialogClass; };
QT_END_NAMESPACE

class RadiusCorrectionDialog : public QDialog
{
    Q_OBJECT

public:
    RadiusCorrectionDialog(Project *project = nullptr, QWidget *parent = nullptr);
    ~RadiusCorrectionDialog();

    void initialization();

private slots:
    void resetDialog();
    void closeEvent(QCloseEvent *event);
    void equalizeResultToInitial();
    void devOffsetByTwo();
    void changeCurveType();
    void calculateOffsetCurve();
    void changeResultCurveName(QString curveName);

private:
    Ui::RadiusCorrectionDialogClass *_ui;
    Project* _project;
};
