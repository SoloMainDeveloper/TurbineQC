#include "curve/pch.h"
#include "figureeditdialog.h"

FigureEditDialog::FigureEditDialog(Project* mainProject, QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::FigureEditDialogClass())
{
    _ui->setupUi(this);

    _project = mainProject;

    _curveTable = new QTableWidget;
    layout()->addWidget(_curveTable);

    connect(_project, &Project::figureEditDialogRequested, this, &FigureEditDialog::dialogInitialization);
}

void FigureEditDialog::dialogInitialization(const QString figureName) {
    _curveTable->clear();
    while(QLayoutItem* item = _ui->dynamicGLayout->takeAt(0)) {
        auto widget = item->widget();
        delete widget;
    }
    resize(600, 400);
    _curveTable->hide();

    setWindowTitle("Edit " + figureName);

    auto figure = _project->findFigure(figureName);
    auto color = figure->color();

    _ui->nameLE->setText(figureName);
    _ui->colorPB->setStyleSheet("background-color: " + color.name() + ";");

    if(const CurveFigure* curve = dynamic_cast<const CurveFigure*>(figure)) {
        _curveTable->show();
        auto points = curve->points();
        auto pointsCount = points.length();
        _curveTable->setRowCount(pointsCount);
        _curveTable->setColumnCount(9);
        _curveTable->setHorizontalHeaderLabels({ "X", "Y", "Z", "I", "J", "K", "Dev.", "UT", "LT" });
        _curveTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        _curveTable->horizontalHeader()->setStretchLastSection(true);

        for(auto i = 0; i < pointsCount; i++) {
            auto currentPoint = points[i];
            auto parameters = QVector<double>{ currentPoint.x, currentPoint.y, currentPoint.z,
                currentPoint.i, currentPoint.j, currentPoint.k, currentPoint.dev, currentPoint.ut, currentPoint.lt };
            for(auto j = 0; j < 9; j++) {
                auto text = QString::number(parameters[j], 'f', 6);
                QTableWidgetItem* item = new QTableWidgetItem;
                item->setText(text);
                _curveTable->setItem(i, j, item);
            }
        }
    } else if(const PointFigure* point = dynamic_cast<const PointFigure*>(figure)) {
        auto coordinates = point->point();
        updatePosition(coordinates.x, coordinates.y, coordinates.z);
        adjustSize();
        adjustSize();
    } else if(const CircleFigure* circle = dynamic_cast<const CircleFigure*>(figure)) {
        auto center = circle->center();
        auto normal = circle->normal();
        auto radius = circle->radius();

        updatePosition(center.x, center.y, center.z);
        updateDirection(normal.x, normal.y, normal.z);
        updateParameter("Radius", radius);

        adjustSize();
        adjustSize();
    } else if(const LineFigure* line = dynamic_cast<const LineFigure*>(figure)) {
        auto origin = line->origin();
        auto direction = line->direction();
        auto length = line->length();

        updatePosition(origin.x, origin.y, origin.z);
        updateDirection(direction.x, direction.y, direction.z);
        updateParameter("Length", length);

        adjustSize();
        adjustSize();
    }

    exec();
}

void FigureEditDialog::updatePosition(double x, double y, double z) {
    QLabel* text = new QLabel("X, Y, Z: ");
    QLineEdit* xLineEdit = new QLineEdit;
    xLineEdit->setValidator(new QDoubleValidator);
    xLineEdit->setText(QString::number(x, 'f', 6));
    QLineEdit* yLineEdit = new QLineEdit;
    yLineEdit->setValidator(new QDoubleValidator);
    yLineEdit->setText(QString::number(y, 'f', 6));
    QLineEdit* zLineEdit = new QLineEdit;
    zLineEdit->setValidator(new QDoubleValidator);
    zLineEdit->setText(QString::number(z, 'f', 6));

    _ui->dynamicGLayout->addWidget(text, 0, 0);
    _ui->dynamicGLayout->addWidget(xLineEdit, 0, 1);
    _ui->dynamicGLayout->addWidget(yLineEdit, 0, 2);
    _ui->dynamicGLayout->addWidget(zLineEdit, 0, 3);
}

void FigureEditDialog::updateDirection(double i, double j, double k) {
    QLabel* textIJK = new QLabel("I, J, K: ");
    QLineEdit* iLineEdit = new QLineEdit;
    iLineEdit->setValidator(new QDoubleValidator);
    iLineEdit->setText(QString::number(i, 'f', 6));
    QLineEdit* jLineEdit = new QLineEdit;
    jLineEdit->setValidator(new QDoubleValidator);
    jLineEdit->setText(QString::number(j, 'f', 6));
    QLineEdit* kLineEdit = new QLineEdit;
    kLineEdit->setValidator(new QDoubleValidator);
    kLineEdit->setText(QString::number(k, 'f', 6));

    _ui->dynamicGLayout->addWidget(textIJK, 1, 0);
    _ui->dynamicGLayout->addWidget(iLineEdit, 1, 1);
    _ui->dynamicGLayout->addWidget(jLineEdit, 1, 2);
    _ui->dynamicGLayout->addWidget(kLineEdit, 1, 3);
}

void FigureEditDialog::updateParameter(QString parameter, double l) { //for length or radius
    QLabel* textParameter = new QLabel(parameter + ": ");
    QLineEdit* parameterLineEdit = new QLineEdit;
    parameterLineEdit->setValidator(new QDoubleValidator);
    parameterLineEdit->setText(QString::number(l, 'f', 6));
    _ui->dynamicGLayout->addWidget(textParameter, 2, 0);
    _ui->dynamicGLayout->addWidget(parameterLineEdit, 2, 1);
}

FigureEditDialog::~FigureEditDialog()
{
    delete _curveTable;
    delete _ui;
}
