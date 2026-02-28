#pragma once

#include "ui_turbinewindow.h"
#include "project.h"
#include "algorithms.h"
#include "curvegraphicswidget.h"
#include "reportsettings.h"
#include "reportgenerator.h"

class ReporData;

namespace Ui {
    class TurbineWindow;
}

class TurbineWindow : public QDialog {
    Q_OBJECT

public:
    enum class Operation {
        MaxWidth,
        MaxWidthX,
        MaxWidthY,
        ContactLineLength,
        WidthLE,
        WidthTE,
        RadiusLE,
        RadiusTE,
        OperationsCount
    };

    enum class CurveType {
        Nominal,
        Measured,
    };

    explicit TurbineWindow(Project *project, Plot *plot);
    virtual ~TurbineWindow();

public slots:
    void initialization();

private slots:
    void run();
    void closeWindow();
    void closeEvent(QCloseEvent *event);
    void calculateNominals();
    void updateParamOutputView();
    void changeItemOfList();
    void onNoBestFitLEClick();
    void onNoBestFitTEClick();
    void onShowNumDevLEClick();
    void onShowNumDevTEClick();
    void onPreparePointsClick();
    void onDeleteSimilarClick();
    void onRadiusCorrectionClick();
    void onUpperValueChange();
    void onNeedPrintTemplateClick();

private:
    Ui::TurbineWindow *_ui;
    Project *_project;
    Plot *_plot;
    std::shared_ptr<ReportSettings> _reportSettings;
    CurveGraphicsWidget *_curveGraphics;
    QGridLayout *_containerLayout;
    QDoubleValidator *_doubleValidator;
    QIntValidator *_intValidator;
    QMessageBox *_message;

    void setupWindow();
    void disableUnrealizedParams();
    QMap<Operation, double> calculate(const QString &curveName, CurveType curveType);
    void setNominalParams(const QMap<Operation, double> &params);
    void setMeasuredParams(const QMap<Operation, double> &params);
    void calculateMeasured();
    void setSettings(const QString &filePath);
    void setTolerances(Operation operation);
};
