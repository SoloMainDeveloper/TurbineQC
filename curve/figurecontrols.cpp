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

    parameter = new QCheckBox("Numbers");
    connect(parameter, &QCheckBox::clicked, this, &FigureControls::curveParametersSwitched);
    curveControls.append(parameter);

    for(auto parameter : curveControls) {
        layout->addWidget(parameter);
    }

    numbers = new QLineEdit;
    numbers->setValidator(new QIntValidator);
    connect(numbers, &QLineEdit::editingFinished, this, &FigureControls::curveParametersSwitched);
    layout->addWidget(numbers);
}

//slots

void FigureControls::changeCurrentItem(const QString &currentFigureName) {

    visibility->hide();
    for(auto item : curveControls) {
        item->hide();
    }
    numbers->hide();

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

        curveControls[0]->setChecked(currentCurve->isShowPoints());
        curveControls[1]->setChecked(currentCurve->isConnectPoints());
        curveControls[2]->setChecked(currentCurve->isShowVectors());
        curveControls[3]->setChecked(currentCurve->isClosed());
        curveControls[4]->setChecked(currentCurve->isShowNumbering());
        numbers->setText(QString::number(currentCurve->numberingInterval()));
    }
}

void FigureControls::changeFigureVisibility(const QString figureName, bool visible) {
    auto current = currentFigure->name();
    if(current == figureName) {
        visibility->setChecked(visible);
    }
}

void FigureControls::changeCurveParameters(const QString curveName, bool showPoints, bool connectPoints,
    bool showVectors, bool closed, bool showNumbering, int numberingInterval) {

    auto current = currentFigure->name();
    if(current == curveName) {
        curveControls[0]->setChecked(showPoints);
        curveControls[1]->setChecked(connectPoints);
        curveControls[2]->setChecked(showVectors);
        curveControls[3]->setChecked(closed);
        curveControls[4]->setChecked(showNumbering);
        numbers->setText(QString::number(numberingInterval));
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

    emit curveChangeParametersRequested(current, showPoints, connectPoints, showVectors,
        closed, showNumbering, numberingInterval);
}
