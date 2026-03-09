#pragma once

#include "project.h"

namespace Ui {
	class RadiusCorrectionDialog;
};

class RadiusCorrectionDialog : public BaseDialog {
	Q_OBJECT

public:
	explicit RadiusCorrectionDialog();
	virtual ~RadiusCorrectionDialog();

	void initialize() override;

private slots:
	void resetDialog();
	void closeEvent(QCloseEvent* event);
	void equalizeResultToInitial();
	void devOffsetByTwo();
	void changeCurveType();
	void calculateOffsetCurve();
	void updateResultNameAndClosed(QString curveName);

private:
	Ui::RadiusCorrectionDialog* _ui;
	Project* _project;
};
