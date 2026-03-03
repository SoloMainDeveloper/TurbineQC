#pragma once

#include "ui_turbinewindow.h"
#include "project.h"
#include "algorithms.h"
#include "curvegraphicswidget.h"
#include "reportsettings.h"
#include "reportgenerator.h"

namespace Ui {
    class TurbineWindow;
}

class TurbineWindow : public QDialog {
    Q_OBJECT

public:
    explicit TurbineWindow(Project *project, Plot *plot);
    virtual ~TurbineWindow();

public slots:
    void initialization();

private slots:
    void run();
    void closeWindow();
    void closeEvent(QCloseEvent *event);
    void onCalculateNominals();
    void updateParamOutputView();
    void changeItemOfList();
    void onProfileTypeChange();
    void onNoBestFitLEClick();
    void onNoBestFitTEClick();
    void onShowNumDevLEClick();
    void onShowNumDevTEClick();
    void onPreparePointsClick();
    void onDeleteSimilarClick();
    void onRadiusCompensationClick();
    void onUse3DVectorsForCompClick();
    void onRadiusCompensationChange();
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

    void setupWindow();
    void disableUnrealizedParams();
    void calculateNominals(const QString &curveName);
    void setSettings();
};
