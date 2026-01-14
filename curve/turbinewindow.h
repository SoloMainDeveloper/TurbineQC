#pragma once

#include <QDialog>
#include "ui_turbinewindow.h"
#include "project.h"
#include "algorithms.h"
#include "curvegraphics.h"

struct TurbineParams {
public:
    TurbineParams();
    ~TurbineParams();

    double nominal;
    double toleranceTop;
    double toleranceBottom;
    double atWidth;
};

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

namespace Ui {
    class TurbineWindow;
}

class TurbineWindow : public QDialog {
    Q_OBJECT

public:
    explicit TurbineWindow(Project *project, QWidget *parent = nullptr);
    virtual ~TurbineWindow();

public slots:
    void initialization();

private:
    void setupWindow();
    void clearLayout(QLayout* layout);
    void updateParamOutputView();

    Ui::TurbineWindow *_ui;
    Project *_project;
    CurveGraphics *_curveGraphics;
    QDoubleValidator *_doubleValidator;

    //profiles
    QListWidget *nominals;
    QListWidget *measured;
    QComboBox *directionLE;
    QLineEdit *zoneScaleLE;
    QLineEdit *zoneScaleTE;
    QComboBox *dimensionType;

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
    void closeWindow();
    void calculateNominals();
    void onPreparePointsClick();
    void showTurbineParams(QListWidgetItem* curr);
    void changeItemOfList();
    void closeEvent(QCloseEvent *event);
};
