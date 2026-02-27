#pragma once

#include "figure.h"
#include "project.h"
#include "tree.h"

class FigureControls :
    public QWidget
{

    Q_OBJECT

public:

    FigureControls(Project* mainProject, QWidget *parent = nullptr);
    virtual ~FigureControls() = default;

public slots:

    void changeCurrentItem(const QString &currentFigureName);
    void changeFigureVisibility(const QString figureName, bool visible);
    void changeCurveParameters(const QString curveName, bool showPoints, bool connectPoints,
        bool showVectors, bool closed, bool showNumbering, int numberingInterval,
        double amplification, bool showTolerances, bool showDeviations, bool connectDeviations, bool highLightOutl);
    void changeDimensionParameters(const QString curveName, bool onlyLabel, bool showTolerances, bool freePosition);
signals:

    void figureChangeVisibilityRequested(const QString figureName, bool visible);
    void curveChangeParametersRequested(const QString curveName, bool showPoints, bool connectPoints,
        bool showVectors, bool closed, bool showNumbering, int numberingInterval,
        double amplification, bool showTolerances, bool showDeviations, bool connectDeviations, bool highLightOut);
    void dimensionChangeParametersRequested(const QString curveName, bool onlyLabel, bool showTolerances, bool freePosition);

private:

    Project *project;

    QVBoxLayout *layout;

    QCheckBox *visibilityCheckBox;
    QCheckBox *showPointsCheckBox;
    QCheckBox *connectPointsCheckBox;
    QCheckBox *showVectorsCheckBox;
    QCheckBox *closedCheckBox;
    QCheckBox *numbersCheckBox;
    QLineEdit *numbersLineEdit;
    QLabel *amplificationLabel;
    QLineEdit *amplificationLineEdit;
    QCheckBox *showTolsCheckBox;
    QCheckBox *showDevsCheckBox;
    QCheckBox *connectDevsCheckBox;
    QCheckBox *highLightOutCheckBox;

    QCheckBox *dimLabelOnlyCheckBox;
    QCheckBox *dimShowTolsCheckBox;
    QCheckBox *dimFreePositionCheckBox;
    
    const Figure *currentFigure = nullptr;

    void visibilitySwitched();
    void curveParametersSwitched();
    void dimensionParametersSwitched();
    void addCurveLayout();
    void addDimensionLayout();
    void setVisibleCurveLayout(bool visible);
    void setVisibleDimensionLayout(bool visible);
};

