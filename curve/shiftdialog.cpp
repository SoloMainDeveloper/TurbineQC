#include "curve/pch.h"

#include "shiftdialog.h"
#include "ui_shiftdialog.h"

ShiftDialog::ShiftDialog() : _ui(new Ui::ShiftDialog()) {
	_ui->setupUi(this);

	_project = &Project::instance();

	_ui->xLE->setValidator(new QDoubleValidator);
	_ui->yLE->setValidator(new QDoubleValidator);
	_ui->zLE->setValidator(new QDoubleValidator);

	_treeModel = new QStandardItemModel;

	_treeCurves = new QStandardItem("Curves");
	_treeCurves->setIcon(QIcon("icons/curve.png"));
	addTreeTab(_treeCurves);

	_treeCircles = new QStandardItem("Circles");
	_treeCircles->setIcon(QIcon("icons/circle.png"));
	addTreeTab(_treeCircles);

	_treeLines = new QStandardItem("Lines");
	_treeLines->setIcon(QIcon("icons/line.png"));
	addTreeTab(_treeLines);

	_treePoints = new QStandardItem("Points");
	_treePoints->setIcon(QIcon("icons/point.png"));
	addTreeTab(_treePoints);

	_ui->figuresTree->setModel(_treeModel);
	_ui->figuresTree->setHeaderHidden(true);
	_ui->figuresTree->expandAll();
	_ui->figuresTree->setEditTriggers(QAbstractItemView::NoEditTriggers);


	connect(_ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &ShiftDialog::applyShift);
	connect(_ui->buttonBox->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &ShiftDialog::reject);

	connect(this, &ShiftDialog::figureShiftRequested, _project, static_cast<void(Project::*)(QString, QString, QString, QString)>(&Project::shiftFigure));
}

void ShiftDialog::initialize() {
	_treeCurves->removeRows(0, _treeCurves->rowCount());
	_treeCircles->removeRows(0, _treeCircles->rowCount());
	_treeLines->removeRows(0, _treeLines->rowCount());
	_treePoints->removeRows(0, _treePoints->rowCount());

	auto figures = _project->figures();
	for (auto& figure : figures) {
		if (dynamic_cast<CurveFigure*>(figure)) {
			_treeCurves->appendRow(new QStandardItem(figure->name()));
		}
		else if (dynamic_cast<LineFigure*>(figure)) {
			_treeLines->appendRow(new QStandardItem(figure->name()));
		}
		else if (dynamic_cast<PointFigure*>(figure)) {
			_treePoints->appendRow(new QStandardItem(figure->name()));
		}
		else if (dynamic_cast<CircleFigure*>(figure)) {
			_treeCircles->appendRow(new QStandardItem(figure->name()));
		}
	}

	auto currentFigure = _project->currentFigureName();
	if (!_treeModel->findItems(currentFigure, Qt::MatchExactly | Qt::MatchRecursive).isEmpty()) {
		_ui->figuresTree->setCurrentIndex(_treeModel->indexFromItem(_treeModel->findItems(currentFigure, Qt::MatchExactly | Qt::MatchRecursive)[0]));
	}

	_ui->xLE->setText("0");
	_ui->yLE->setText("0");
	_ui->zLE->setText("0");

	adjustSize();
	show();
}

void ShiftDialog::addTreeTab(QStandardItem* tab) {
	tab->setBackground(QBrush("#f7f7f7"));
	tab->setEnabled(false);
	_treeModel->appendRow(tab);
}

void ShiftDialog::applyShift() {
	if (_ui->figuresTree->currentIndex().data().toString().isEmpty()) {
		return;
	}
	auto x = _ui->xLE->text();
	auto y = _ui->yLE->text();
	auto z = _ui->zLE->text();
	emit figureShiftRequested(_ui->figuresTree->currentIndex().data().toString(), x, y, z);
}

ShiftDialog::~ShiftDialog() {
	delete _ui;
}
