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
        double amplification, bool showTolerances, bool showDeviations, bool connectDeviations, bool highLightOut);

signals:

    void figureChangeVisibilityRequested(const QString figureName, bool visible);
    void curveChangeParametersRequested(const QString curveName, bool showPoints, bool connectPoints,
        bool showVectors, bool closed, bool showNumbering, int numberingInterval,
        double amplification, bool showTolerances, bool showDeviations, bool connectDeviations, bool highLightOut);

private:

    Project *project;

    QVBoxLayout *layout;

    QCheckBox *visibility;

    QList<QCheckBox*> curveControls;
    QCheckBox *parameter;
    QLineEdit *numbers;

    QLabel *labelAmplification;
    QLineEdit *lineEditAmplification;

    const Figure *currentFigure = nullptr;

    void visibilitySwitched();
    void curveParametersSwitched();
};

