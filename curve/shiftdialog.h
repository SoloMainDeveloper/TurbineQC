#pragma once

#include "project.h"

namespace Ui {
	class ShiftDialog;
};

class ShiftDialog : public BaseDialog {
	Q_OBJECT

public:
	explicit ShiftDialog();
	virtual ~ShiftDialog();

	void initialize() override;
	void addTreeTab(QStandardItem* tab);

private slots:
	void applyShift();

signals:
	void figureShiftRequested(QString figureName, QString x, QString y, QString z);

private:
	Ui::ShiftDialog* _ui;
	Project* _project;

	QStandardItemModel* _treeModel;
	QStandardItem* _treeCurves;
	QStandardItem* _treeCircles;
	QStandardItem* _treeLines;
	QStandardItem* _treePoints;
};
