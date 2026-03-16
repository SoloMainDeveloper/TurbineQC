#include "curve/pch.h"

#include "rotatedialog.h"
#include "ui_rotatedialog.h"

RotateDialog::RotateDialog() : _ui(new Ui::RotateDialog()) {
    _ui->setupUi(this);

    _ui->angleLE->setValidator(new QDoubleValidator);
    _ui->xLE->setValidator(new QDoubleValidator);
    _ui->yLE->setValidator(new QDoubleValidator);
    _ui->zLE->setValidator(new QDoubleValidator);

    _treeModel = new QStandardItemModel;

    _treeCurves = new QStandardItem(tr("Curves"));
    _treeCurves->setIcon(QIcon("icons/curve.png"));
    addTreeTab(_treeCurves);

    _treeCircles = new QStandardItem(tr("Circles"));
    _treeCircles->setIcon(QIcon("icons/circle.png"));
    addTreeTab(_treeCircles);

    _treeLines = new QStandardItem(tr("Lines"));
    _treeLines->setIcon(QIcon("icons/line.png"));
    addTreeTab(_treeLines);

    _treePoints = new QStandardItem(tr("Points"));
    _treePoints->setIcon(QIcon("icons/point.png"));
    addTreeTab(_treePoints);

    _ui->figuresTree->setModel(_treeModel);
    _ui->figuresTree->setHeaderHidden(true);
    _ui->figuresTree->expandAll();
    _ui->figuresTree->setEditTriggers(QAbstractItemView::NoEditTriggers);


    connect(_ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &RotateDialog::applyRotate);
    connect(_ui->buttonBox->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &RotateDialog::reject);

    connect(_ui->xyzFromCB, &QComboBox::textActivated, this, &RotateDialog::updateXYZ);

    connect(_ui->plus90PB, &QPushButton::clicked, this, &RotateDialog::plus90);
    connect(_ui->minus90PB, &QPushButton::clicked, this, &RotateDialog::minus90);

    connect(this, &RotateDialog::figureRotateRequested, &Project::instance(), static_cast<void(Project::*)(QString, double, QString, QString, QString)>(&Project::rotateFigure));
}

void RotateDialog::initialize() {
    _treeCurves->removeRows(0, _treeCurves->rowCount());
    _treeCircles->removeRows(0, _treeCircles->rowCount());
    _treeLines->removeRows(0, _treeLines->rowCount());
    _treePoints->removeRows(0, _treePoints->rowCount());

    auto figures = Project::instance().figures();
    for(auto &figure : figures) {
        if(dynamic_cast<CurveFigure*>(figure)) {
            _treeCurves->appendRow(new QStandardItem(figure->name()));
        } else if(dynamic_cast<LineFigure*>(figure)) {
            _treeLines->appendRow(new QStandardItem(figure->name()));
        } else if(dynamic_cast<PointFigure*>(figure)) {
            _treePoints->appendRow(new QStandardItem(figure->name()));
            _ui->xyzFromCB->addItem(figure->name());
        } else if(dynamic_cast<CircleFigure*>(figure)) {
            _treeCircles->appendRow(new QStandardItem(figure->name()));
            _ui->xyzFromCB->addItem(figure->name());
        }
    }

    auto currentFigure = Project::instance().currentFigureName();
    if(!_treeModel->findItems(currentFigure, Qt::MatchExactly | Qt::MatchRecursive).isEmpty()) {
        _ui->figuresTree->setCurrentIndex(_treeModel->indexFromItem(_treeModel->findItems(currentFigure, Qt::MatchExactly | Qt::MatchRecursive)[0]));
    }

    _ui->angleLE->setText("0");
    _ui->xyzFromCB->setCurrentIndex(-1);
    _ui->xLE->setText("0");
    _ui->yLE->setText("0");
    _ui->zLE->setText("0");

    adjustSize();
    show();
}

void RotateDialog::addTreeTab(QStandardItem* tab) {
    tab->setBackground(QBrush("#f7f7f7"));
    tab->setEnabled(false);
    _treeModel->appendRow(tab);
}

void RotateDialog::applyRotate() {
    if(_ui->figuresTree->currentIndex().data().toString().isEmpty()) {
        return;
    }
    auto angle = _ui->angleLE->text().toDouble();
    auto x = _ui->xLE->text();
    auto y = _ui->yLE->text();
    auto z = _ui->zLE->text();
    emit figureRotateRequested(_ui->figuresTree->currentIndex().data().toString(), angle, x, y, z);
}

void RotateDialog::plus90() {
    _ui->angleLE->setText(QString::number(_ui->angleLE->text().toDouble() + 90));
}

void RotateDialog::minus90() {
    _ui->angleLE->setText(QString::number(_ui->angleLE->text().toDouble() - 90));
}

void RotateDialog::updateXYZ(QString figureName) {
    _ui->xLE->setText(figureName + ".X");
    _ui->yLE->setText(figureName + ".Y");
    _ui->zLE->setText(figureName + ".Z");

    /*auto figure = _project->findFigure(figureName);
    if(auto point = dynamic_cast<const PointFigure*>(figure)) {
        _ui->xLE->setText(QString::number(point->point().x));
        _ui->yLE->setText(QString::number(point->point().y));
        _ui->zLE->setText(QString::number(point->point().z));
    } else if(auto circle = dynamic_cast<const CircleFigure*>(figure)) {
        _ui->xLE->setText(QString::number(circle->center().x));
        _ui->yLE->setText(QString::number(circle->center().y));
        _ui->zLE->setText(QString::number(circle->center().z));
    }*/
}

RotateDialog::~RotateDialog() {
    delete _ui;
}
