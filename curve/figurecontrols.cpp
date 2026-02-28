#include "curve/pch.h"
#include "figurecontrols.h"

FigureControls::FigureControls(Project* mainProject, QWidget *parent) {
    project = mainProject;

    //project signals
    connect(project, &Project::currentFigureChanged, this, &FigureControls::changeCurrentItem);
    connect(project, &Project::figureVisibilityChanged, this, &FigureControls::changeFigureVisibility);
    connect(project, &Project::curveParametersChanged, this, &FigureControls::changeCurveParameters);
    connect(project, &Project::dimensionParametersChanged, this, &FigureControls::changeDimensionParameters);

    //figure controls signals
    connect(this, &FigureControls::figureChangeVisibilityRequested, project, &Project::changeFigureVisibility);
    connect(this, &FigureControls::curveChangeParametersRequested, project, &Project::changeCurveParameters);
    connect(this, &FigureControls::dimensionChangeParametersRequested, project, &Project::changeDimensionParameters);

    layout = new QVBoxLayout;
    setLayout(layout);
    addCurveLayout();
    addDimensionLayout();
    visibilityCheckBox->hide();
}

//slots

void FigureControls::changeCurrentItem(const QString &currentFigureName) {

     visibilityCheckBox->hide();
    setVisibleCurveLayout(false);
    setVisibleDimensionLayout(false);

    currentFigure = project->findFigure(currentFigureName);

    if(!currentFigure) {
        return;
    }

    visibilityCheckBox->show();
    visibilityCheckBox->setChecked(currentFigure->isVisible());

    if(const CurveFigure* currentCurve = dynamic_cast<const CurveFigure*>(currentFigure)) {
        setVisibleCurveLayout(true);

        showPointsCheckBox->setChecked(currentCurve->isShowPoints());
        connectPointsCheckBox->setChecked(currentCurve->isConnectPoints());
        showVectorsCheckBox->setChecked(currentCurve->isShowVectors());
        closedCheckBox->setChecked(currentCurve->isClosed());
        numbersCheckBox->setChecked(currentCurve->isShowNumbering());        
        numbersLineEdit->setText(QString::number(currentCurve->numberingInterval()));
        amplificationLineEdit->setText(QString::number(currentCurve->amplification()));
        showTolsCheckBox->setChecked(currentCurve->isShowTolerances());
        showDevsCheckBox->setChecked(currentCurve->isShowDeviations());
        connectDevsCheckBox->setChecked(currentCurve->isConnectDeviations());
        highLightOutCheckBox->setChecked(currentCurve->isHighLightOut());
    } else if(const DimFigure* currentDimension = dynamic_cast<const DimFigure*>(currentFigure)) {
        setVisibleDimensionLayout(true);
        dimLabelOnlyCheckBox->setChecked(currentDimension->isOnlyLabel());
        dimShowTolsCheckBox->setChecked(currentDimension->isShowTols());
        dimFreePositionCheckBox->setChecked(currentDimension->isFreePosition());
    }
}

void FigureControls::changeFigureVisibility(const QString figureName, bool visible) {
    auto current = currentFigure->name();
    if(current == figureName) {
        visibilityCheckBox->setChecked(visible);
    }
}

void FigureControls::changeCurveParameters(const QString curveName, bool showPoints, bool connectPoints,
    bool showVectors, bool closed, bool showNumbering, int numberingInterval,
    double amplification, bool showTolerances, bool showDeviations, bool connectDeviations, bool highLightOut) {

    if(currentFigure && currentFigure->name() == curveName) {
        showPointsCheckBox->setChecked(showPoints);
        connectPointsCheckBox->setChecked(connectPoints);
        showVectorsCheckBox->setChecked(showVectors);
        closedCheckBox->setChecked(closed);
        numbersCheckBox->setChecked(showNumbering);
        numbersLineEdit->setText(QString::number(numberingInterval));
        amplificationLineEdit->setText(QString::number(amplification));
        showTolsCheckBox->setChecked(showTolerances);
        showDevsCheckBox->setChecked(showDeviations);
        connectDevsCheckBox->setChecked(connectDeviations);
        highLightOutCheckBox->setChecked(highLightOut);
    }
}

void FigureControls::changeDimensionParameters(const QString name, bool onlyLabel, bool showTolerances, bool freePosition) {
    if(currentFigure && currentFigure->name() == name) {
        dimLabelOnlyCheckBox->setChecked(onlyLabel);
        dimShowTolsCheckBox->setChecked(showTolerances);
        dimFreePositionCheckBox->setChecked(freePosition);
    }
}

//signals

void FigureControls::visibilitySwitched() {
    emit figureChangeVisibilityRequested(currentFigure->name(), visibilityCheckBox->isChecked());
}
void FigureControls::curveParametersSwitched() {
    if(numbersLineEdit->text().toInt() == 0) {
        numbersLineEdit->setText("5");
    }
    emit curveChangeParametersRequested(currentFigure->name(), showPointsCheckBox->isChecked(), connectPointsCheckBox->isChecked(), showVectorsCheckBox->isChecked(),
        closedCheckBox->isChecked(), numbersCheckBox->isChecked(), numbersLineEdit->text().toInt(), amplificationLineEdit->text().toDouble(),
        showTolsCheckBox->isChecked(), showDevsCheckBox->isChecked(), connectDevsCheckBox->isChecked(), highLightOutCheckBox->isChecked());
}

void FigureControls::dimensionParametersSwitched() {
    emit dimensionChangeParametersRequested(currentFigure->name(), dimLabelOnlyCheckBox->isChecked(), 
        dimShowTolsCheckBox->isChecked(), dimFreePositionCheckBox->isChecked());
}

void FigureControls::addCurveLayout() {
    visibilityCheckBox = new QCheckBox("Visible");
    connect(visibilityCheckBox, &QCheckBox::clicked, this, &FigureControls::visibilitySwitched);
    layout->addWidget(visibilityCheckBox);

    showPointsCheckBox = new QCheckBox("Show points");
    connect(showPointsCheckBox, &QCheckBox::clicked, this, &FigureControls::curveParametersSwitched);
    layout->addWidget(showPointsCheckBox);

    connectPointsCheckBox = new QCheckBox("Connect points");
    connect(connectPointsCheckBox, &QCheckBox::clicked, this, &FigureControls::curveParametersSwitched);
    layout->addWidget(connectPointsCheckBox);

    showVectorsCheckBox = new QCheckBox("Show vectors");
    connect(showVectorsCheckBox, &QCheckBox::clicked, this, &FigureControls::curveParametersSwitched);
    layout->addWidget(showVectorsCheckBox);

    closedCheckBox = new QCheckBox("Closed");
    connect(closedCheckBox, &QCheckBox::clicked, this, &FigureControls::curveParametersSwitched);
    layout->addWidget(closedCheckBox);

    auto numberLayout = new QHBoxLayout;
    numbersCheckBox = new QCheckBox("Numbers");
    connect(numbersCheckBox, &QCheckBox::clicked, this, &FigureControls::curveParametersSwitched);
    numberLayout->addWidget(numbersCheckBox);
    numbersLineEdit = new QLineEdit;
    numbersLineEdit->setValidator(new QIntValidator(0, 9999));
    connect(numbersLineEdit, &QLineEdit::editingFinished, this, &FigureControls::curveParametersSwitched);
    //connect(numbersLineEdit, &QLineEdit::textEdited, this, &FigureControls::curveParametersSwitched);
    numberLayout->addWidget(numbersLineEdit);
    layout->addLayout(numberLayout);

    // Dev & Tols
    auto layoutAmplification = new QHBoxLayout;
    amplificationLabel = new QLabel("Amplification");
    amplificationLineEdit = new QLineEdit;
    amplificationLineEdit->setValidator(new QDoubleValidator);
    connect(amplificationLineEdit, &QLineEdit::editingFinished, this, &FigureControls::curveParametersSwitched);
    //connect(amplificationLineEdit, &QLineEdit::textEdited, this, &FigureControls::curveParametersSwitched);
    layoutAmplification->addWidget(amplificationLabel);
    layoutAmplification->addWidget(amplificationLineEdit);
    layout->addLayout(layoutAmplification);

    showTolsCheckBox = new QCheckBox("Show tols");
    connect(showTolsCheckBox, &QCheckBox::clicked, this, &FigureControls::curveParametersSwitched);
    layout->addWidget(showTolsCheckBox);

    showDevsCheckBox = new QCheckBox("Show devs");
    connect(showDevsCheckBox, &QCheckBox::clicked, this, &FigureControls::curveParametersSwitched);
    layout->addWidget(showDevsCheckBox);

    connectDevsCheckBox = new QCheckBox("Connect devs");
    connect(connectDevsCheckBox, &QCheckBox::clicked, this, &FigureControls::curveParametersSwitched);
    layout->addWidget(connectDevsCheckBox);

    highLightOutCheckBox = new QCheckBox("Highlight out");
    connect(highLightOutCheckBox, &QCheckBox::clicked, this, &FigureControls::curveParametersSwitched);
    layout->addWidget(highLightOutCheckBox);
    setVisibleCurveLayout(false);
}

void FigureControls::addDimensionLayout() {
    dimLabelOnlyCheckBox = new QCheckBox("Label only");
    connect(dimLabelOnlyCheckBox, &QCheckBox::clicked, this, &FigureControls::dimensionParametersSwitched);
    layout->addWidget(dimLabelOnlyCheckBox);
    dimShowTolsCheckBox = new QCheckBox("Show Tols");
    connect(dimShowTolsCheckBox, &QCheckBox::clicked, this, &FigureControls::dimensionParametersSwitched);
    layout->addWidget(dimShowTolsCheckBox);
    dimFreePositionCheckBox = new QCheckBox("Free position");
    connect(dimFreePositionCheckBox, &QCheckBox::clicked, this, &FigureControls::dimensionParametersSwitched);
    layout->addWidget(dimFreePositionCheckBox);
    setVisibleDimensionLayout(false);
}

void FigureControls::setVisibleCurveLayout(bool visible) {
    showPointsCheckBox->setVisible(visible);
    connectPointsCheckBox->setVisible(visible);
    showVectorsCheckBox->setVisible(visible);
    closedCheckBox->setVisible(visible);
    numbersCheckBox->setVisible(visible);
    numbersLineEdit->setVisible(visible);
    amplificationLabel->setVisible(visible);
    amplificationLineEdit->setVisible(visible);
    showTolsCheckBox->setVisible(visible);
    showDevsCheckBox->setVisible(visible);
    connectDevsCheckBox->setVisible(visible);
    highLightOutCheckBox->setVisible(visible);
}

void FigureControls::setVisibleDimensionLayout(bool visible) {
    dimLabelOnlyCheckBox->setVisible(visible);
    dimShowTolsCheckBox->setVisible(visible);

    dimFreePositionCheckBox->setVisible(false);
    if(auto dimFigure = dynamic_cast<const DimFigure*>(currentFigure)) {
        if(dimFigure->renderType() == DimFigure::RenderType::DistanceBetweenCurvePoints) {
            dimFreePositionCheckBox->setVisible(visible);
        }
    }
}
