#pragma once

#include <QDialog>
#include "ui_calculatecurvedialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CalculateCurveDialogClass; };
QT_END_NAMESPACE

class CalculateCurveDialog : public QDialog
{
    Q_OBJECT

public:
    CalculateCurveDialog(Project *project = nullptr, QWidget *parent = nullptr);
    ~CalculateCurveDialog();

    void initialization();

private slots:
    void updateResultNameAndClosed(QString curveName);
    void calculateCurve();

private:
    Ui::CalculateCurveDialogClass *_ui;

    Project* _project;
};
