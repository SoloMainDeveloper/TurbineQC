#pragma once

#include "ui_turbinewindow.h"
#include "project.h"
#include "algorithms.h"
#include "curvegraphicswidget.h"
#include "reportdata.h"
#include "reportgenerator.h"

class ReporData;

struct TurbineParams {
public:
    TurbineParams();
    ~TurbineParams();

    double nominal;
    double toleranceTop;
    double toleranceBottom;
    double atWidth;
};

namespace Ui {
    class TurbineWindow;
}

class TurbineWindow : public QDialog {
    Q_OBJECT

public:
    explicit TurbineWindow(Project *project, Plot *plot);
    virtual ~TurbineWindow();

    enum Operation {
    MaxWidth,
    MaxWidthX,
    MaxWidthY,
    ChordLength,
    WidthLE,
    WidthTE,
    RadiusLE,
    RadiusTE,
    OperationsCount
};

public slots:
    void initialization();

private:
    void setupWindow();
    void clearLayout(QLayout* layout);
    void updateParamOutputView();

    Ui::TurbineWindow *_ui;
    Project *_project;
    Plot *_plot;
    ReportData *_reportData;
    CurveGraphicsWidget *_curveGraphics;
    QGridLayout *_containerLayout;
    QDoubleValidator *_doubleValidator;
    QIntValidator *_intValidator;
    QMessageBox *_message;

    //profiles
    QListWidget *nominals;
    QListWidget *measured;
    QComboBox *directionLE;
    QLineEdit *zoneScaleLE;
    QLineEdit *zoneScaleTE;
    QComboBox *dimension;

    //prepare points
    QCheckBox *needPreparePoints;
    QCheckBox *needSortPoints;
    QCheckBox *needDeleteSimilarPoints;
    QCheckBox *needRadiusCorrection;
    QCheckBox *use3DVectors;

    QTabWidget *formTab;
    //global form
    QComboBox *profile;
    QComboBox *globalBestFit;
    QComboBox *globalBestFitType;
    QCheckBox *showMCL;
    QCheckBox *showMaxDiam;
    QCheckBox *showContactLines;
    QCheckBox *avgDeviation;
    QCheckBox *stretchLE;
    QCheckBox *stretchTE;
    QLineEdit *globalScale;
    QLineEdit *globalIncrease;
    QComboBox *globalAxis;

    //LE form
    QCheckBox *formLE;
    QComboBox *bestFitLE;
    QComboBox *showNumLE;
    QLineEdit *scaleLE;
    QLineEdit *increaseLE;
    QComboBox *axisLE;

    //TE form
    QCheckBox *formTE;
    QComboBox *bestFitTE;
    QComboBox *showNumTE;
    QLineEdit *scaleTE;
    QLineEdit *increaseTE;
    QComboBox *axisTE;

    //output form
    QCheckBox *minMax;
    QCheckBox *form;
    QCheckBox *min;
    QCheckBox *max;
    QCheckBox *maxAbs;
    QCheckBox *supUT;
    QCheckBox *infLT;
    QRadioButton *markPlaceNominal;
    QRadioButton *markPlaceMeasured;
    QRadioButton *markDirectionNominal;
    QRadioButton *markDirectionMeasured;

    //peak analysis
    QComboBox *peakAnalysis;

    //parameters
    QMap<QString, QMap<Operation, TurbineParams*>> nominalTurbineParams;
    QListWidget *paramList;
    QVBoxLayout *labelLayout;
    QVBoxLayout *lineEditLayout;
    QLineEdit *editToleranceTop;
    double toleranceTopValue;
    QLineEdit *editToleranceBottom;
    double toleranceBottomValue;
    QLineEdit *atEditLE;
    double atLEValue;
    QLineEdit *atEditTE;
    double atTEValue;

    //report
    QTabWidget *reportTab;
    QListWidget *reportList;
    QCheckBox *writeWithTemplate;
    QLineEdit *reportComment;
    QLineEdit *allProfilesToReport;

private slots:
    void run();
    void setSettings(const QString &fileName);
    void closeWindow();
    void calculateNominals();
    void onPreparePointsClick();
    void onDeleteSimilarClick();
    void onRadiusCorrectionClick();
    void onShowNumLEClick();
    void onShowNumTEClick();
    void onWriteWithTemplateClick();
    void showTurbineParams(QListWidgetItem* curr);
    void changeItemOfList();
    void closeEvent(QCloseEvent *event);

private:
    void calculateMeasured();
    void setTolerances(Operation operation, double toleranceTopValue, double toleranceBottomValue);
};
