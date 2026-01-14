#include "curve/pch.h"
#include "figurecontrols.h"

FigureControls::FigureControls(Project* mainProject, QWidget *parent) {

    project = mainProject;

    //project signals
    connect(project, &Project::currentFigureChanged, this, &FigureControls::changeCurrentItem);
    connect(project, &Project::figureVisibilityChanged, this, &FigureControls::changeFigureVisibility);
    connect(project, &Project::curveParametersChanged, this, &FigureControls::changeCurveParameters);

    //figure controls signals
    connect(this, &FigureControls::figureChangeVisibilityRequested, project, &Project::changeFigureVisibility);
    connect(this, &FigureControls::curveChangeParametersRequested, project, &Project::changeCurveParameters);

    layout = new QVBoxLayout;
    setLayout(layout);

    visibility = new QCheckBox("Visible");
    connect(visibility, &QCheckBox::clicked, this, &FigureControls::visibilitySwitched);
    layout->addWidget(visibility);

    parameter = new QCheckBox("Show points");
    connect(parameter, &QCheckBox::clicked, this, &FigureControls::curveParametersSwitched);
    curveControls.append(parameter);

    parameter = new QCheckBox("Connect points");
    connect(parameter, &QCheckBox::clicked, this, &FigureControls::curveParametersSwitched);
    curveControls.append(parameter);

    parameter = new QCheckBox("Show vectors");
    connect(parameter, &QCheckBox::clicked, this, &FigureControls::curveParametersSwitched);
    curveControls.append(parameter);

    parameter = new QCheckBox("Closed");
    connect(parameter, &QCheckBox::clicked, this, &FigureControls::curveParametersSwitched);
    curveControls.append(parameter);

    for(auto parameter : curveControls) {
        layout->addWidget(parameter);
    }

    auto layoutNumber = new QHBoxLayout;
    parameter = new QCheckBox("Numbers");
    connect(parameter, &QCheckBox::clicked, this, &FigureControls::curveParametersSwitched);
    curveControls.append(parameter);
    layoutNumber->addWidget(parameter);
    
    numbers = new QLineEdit;
    numbers->setValidator(new QIntValidator);
    connect(numbers, &QLineEdit::editingFinished, this, &FigureControls::curveParametersSwitched);
    connect(numbers, &QLineEdit::textEdited, this, &FigureControls::curveParametersSwitched);
    layoutNumber->addWidget(numbers);

    layout->addLayout(layoutNumber);

    // Dev & Tols
    auto layoutAmplification = new QHBoxLayout;
    labelAmplification = new QLabel("Amplification");
    lineEditAmplification = new QLineEdit;
    lineEditAmplification->setValidator(new QDoubleValidator);
    connect(lineEditAmplification, &QLineEdit::editingFinished, this, &FigureControls::curveParametersSwitched);
    connect(lineEditAmplification, &QLineEdit::textEdited, this, &FigureControls::curveParametersSwitched);
    layoutAmplification->addWidget(labelAmplification);
    layoutAmplification->addWidget(lineEditAmplification);
    layout->addLayout(layoutAmplification);

    parameter = new QCheckBox("Show tols");
    connect(parameter, &QCheckBox::clicked, this, &FigureControls::curveParametersSwitched);
    curveControls.append(parameter);
    layout->addWidget(parameter);

    parameter = new QCheckBox("Show devs");
    connect(parameter, &QCheckBox::clicked, this, &FigureControls::curveParametersSwitched);
    curveControls.append(parameter);
    layout->addWidget(parameter);

    parameter = new QCheckBox("Connect devs");
    connect(parameter, &QCheckBox::clicked, this, &FigureControls::curveParametersSwitched);
    curveControls.append(parameter);
    layout->addWidget(parameter);

    parameter = new QCheckBox("Highlight out");
    connect(parameter, &QCheckBox::clicked, this, &FigureControls::curveParametersSwitched);
    curveControls.append(parameter);
    layout->addWidget(parameter);

    visibility->hide();
    for(auto item : curveControls) {
        item->hide();
    }
    numbers->hide();
    lineEditAmplification->hide();
    labelAmplification->hide();
}

//slots

void FigureControls::changeCurrentItem(const QString &currentFigureName) {

    visibility->hide();
    for(auto item : curveControls) {
        item->hide();
    }
    numbers->hide();
    lineEditAmplification->hide();
    labelAmplification->hide();

    currentFigure = project->findFigure(currentFigureName);

    if(!currentFigure) {
        return;
    }

    visibility->show();
    visibility->setChecked(currentFigure->isVisible());

    if(const CurveFigure* currentCurve = dynamic_cast<const CurveFigure*>(currentFigure)) {
        for(auto parameter : curveControls) {
            parameter->show();
        }
        numbers->show();
        lineEditAmplification->show();
        labelAmplification->show();

        curveControls[0]->setChecked(currentCurve->isShowPoints());
        curveControls[1]->setChecked(currentCurve->isConnectPoints());
        curveControls[2]->setChecked(currentCurve->isShowVectors());
        curveControls[3]->setChecked(currentCurve->isClosed());
        curveControls[4]->setChecked(currentCurve->isShowNumbering());
        numbers->setText(QString::number(currentCurve->numberingInterval()));
        
        lineEditAmplification->setText(QString::number(currentCurve->amplification()));
        curveControls[5]->setChecked(currentCurve->isShowTolerances());
        curveControls[6]->setChecked(currentCurve->isShowDeviations());
        curveControls[7]->setChecked(currentCurve->isConnectDeviations());
        curveControls[8]->setChecked(currentCurve->isHighLightOut());
    }
}

void FigureControls::changeFigureVisibility(const QString figureName, bool visible) {
    if(currentFigure && currentFigure->name() == figureName) {
        visibility->setChecked(visible);
    }
}

void FigureControls::changeCurveParameters(const QString curveName, bool showPoints, bool connectPoints,
    bool showVectors, bool closed, bool showNumbering, int numberingInterval,
    double amplification, bool showTolerances, bool showDeviations, bool connectDeviations, bool highLightOut) {

    if(currentFigure && currentFigure->name() == curveName) {
        curveControls[0]->setChecked(showPoints);
        curveControls[1]->setChecked(connectPoints);
        curveControls[2]->setChecked(showVectors);
        curveControls[3]->setChecked(closed);
        curveControls[4]->setChecked(showNumbering);
        numbers->setText(QString::number(numberingInterval));

        lineEditAmplification->setText(QString::number(amplification));
        curveControls[5]->setChecked(showTolerances);
        curveControls[6]->setChecked(showDeviations);
        curveControls[7]->setChecked(connectDeviations);
        curveControls[8]->setChecked(highLightOut);
    }
}


//signals

void FigureControls::visibilitySwitched() {
    auto current = currentFigure->name();
    auto visible = visibility->isChecked();

    emit figureChangeVisibilityRequested(current, visible);
}
void FigureControls::curveParametersSwitched() {
    auto current = currentFigure->name();
    auto showPoints = curveControls[0]->isChecked();
    auto connectPoints = curveControls[1]->isChecked();
    auto showVectors = curveControls[2]->isChecked();
    auto closed = curveControls[3]->isChecked();
    auto showNumbering = curveControls[4]->isChecked();
    auto numberingInterval = numbers->text().toInt();

    auto amplification = lineEditAmplification->text().toDouble();
    auto showTolerances = curveControls[5]->isChecked();
    auto showDeviations = curveControls[6]->isChecked();
    auto connectDeviations = curveControls[7]->isChecked();
    auto highLightOut = curveControls[8]->isChecked();

    emit curveChangeParametersRequested(current, showPoints, connectPoints, showVectors,
        closed, showNumbering, numberingInterval, 
        amplification, showTolerances, showDeviations, connectDeviations, highLightOut);
}
