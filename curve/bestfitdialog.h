#pragma once

#include "ui_bestfitdialog.h"
#include "project.h"
#include "algorithms.h"
#include "functionparams.h"

class BestFitDialog : public QDialog {
    Q_OBJECT

public:
    explicit BestFitDialog(Project *project);
    virtual ~BestFitDialog();

    void initialization();

private slots:
    void changelistOfMeasuredCurves();
    void changeMethod();
    void onClosed();
    void onXShift();
    void onYShift();
    void onRotation();
    void onConstraints();
    void onHorizontalConstraint();
    void onVerticalConstraint();
    void onRotationConstraint();
    void checkSettings();
    void closeEvent(QCloseEvent *event);
    void closeDialog();

private:
    Ui::BestFitDialog *_ui;
    Project *_project;
    QMessageBox *_errorMessage;
    QMessageBox *_questionMessage;

    Function6Params::Algorithm _algorithm;
    bool _isClosed;
    bool _needXShift;
    bool _needYShift;
    bool _needRotation;
    bool _needHConstraint;
    bool _needVConstraint;
    bool _needRConstraint;

    void calculateBestFit(const QString &nominalCurveName, const QString &measuredCurveName, const QString &resultCurveName);
    bool isCorrectNumericLines();
};
