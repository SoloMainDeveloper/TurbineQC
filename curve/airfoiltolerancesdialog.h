#pragma once

#include "project.h"
#include "curvelibrary.h"
#include "curvegraphicswidget.h"
#include "basedialog.h"

namespace Ui {
	class AirfoilTolerancesDialog;
}

class AirfoilTolerancesDialog : public BaseDialog {
	Q_OBJECT

public:
	explicit AirfoilTolerancesDialog();
	virtual ~AirfoilTolerancesDialog();

public slots:
	void initialize() override;
	void changeCurrentMode();
	void changeItemOfList();
	void assignTolerances();

signals:
	void curveChangeToleranceRequested(const QString curveName, QVector<CurvePoint> curveWithTolerances);

private:
	Ui::AirfoilTolerancesDialog* _ui;

	CurveGraphicsWidget* _curveGraphics;
	QGridLayout* _containerLayout;

	QStackedLayout* _stackedLayoutModes;

	QDialogButtonBox* _constantToleranceButtonBox;
	QDialogButtonBox* _edgesButtonBox;

	QWidget* _constantTolerancesWidget;
	QVBoxLayout* _constantTolerancesLayout;
	QLabel* _toleranceLabel;
	QLineEdit* _upperToleranceLineEdit;
	QLineEdit* _lowerToleranceLineEdit;

	QWidget* _edgesWidget;
	QVBoxLayout* _edgesLayout;
	QLabel* _leadingEdgeToleranceLabel;
	QLabel* _trailingEdgeToleranceLabel;
	QLabel* _highEdgeToleranceLabel;
	QLabel* _lowEdgeToleranceLabel;
	QLineEdit* _leadingEdgeUpperToleranceLineEdit;
	QLineEdit* _leadingEdgeLowerToleranceLineEdit;
	QLineEdit* _trailingEdgeUpperToleranceLineEdit;
	QLineEdit* _trailingEdgeLowerToleranceLineEdit;
	QLineEdit* _highEdgeUpperToleranceLineEdit;
	QLineEdit* _highEdgeLowerToleranceLineEdit;
	QLineEdit* _lowEdgeUpperToleranceLineEdit;
	QLineEdit* _lowEdgeLowerToleranceLineEdit;

	QGridLayout* _edgesGroupBoxLayout;
	QGroupBox* _edgesGroupBox;
	QLabel* _leadingEdgeDirectionLabel;
	QLabel* _edgesPercentsLabel;
	QComboBox* _leadingEdgeDirectionComboBox;
	QLineEdit* _leadingEdgePercentLineEdit;
	QLineEdit* _trailingEdgePercentLineEdit;

	QString _currentCurveName;
	const CurveFigure* _currentCurve = nullptr;

	void addToleranceBlock(QVBoxLayout* layout, QString blockName, QLabel* label, QLineEdit* upperLineEdit, QLineEdit* lowerLineEdit);
	void addButtonBox(QVBoxLayout* layout, QDialogButtonBox* buttonBox);
	void widgetsInitializaion();
	void checkCurrentItem(QDialogButtonBox* buttonBox);
	void edgesLayoutSetEnabled(bool state);
};
