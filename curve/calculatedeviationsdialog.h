#pragma once

#include "ui_calculatedeviationsdialog.h"
#include "project.h"
#include "algorithms.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CalculateDeviationsDialogClass; };
QT_END_NAMESPACE

class CalculateDeviationsDialog : public QDialog
{
    Q_OBJECT

public:
    CalculateDeviationsDialog(Project *project, QWidget *parent = nullptr);
    ~CalculateDeviationsDialog();

    void initialization();
    void switchResultNameLineEdit();
    void switchAdvancedSettings();
    void changeCurveType();
    void resetDialog();
    void closeEvent(QCloseEvent *event);
    void calculateDeviations();
    void changeResultName();

private:
    Ui::CalculateDeviationsDialogClass *_ui;
    Project *_project;
};
