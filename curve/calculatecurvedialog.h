#pragma once

namespace Ui {
	class CalculateCurveDialog;
};

class CalculateCurveDialog : public BaseDialog {
	Q_OBJECT

public:
	explicit CalculateCurveDialog();
	virtual ~CalculateCurveDialog();

	void initialize();

private slots:
	void updateResultNameAndClosed(QString curveName);
	void calculateCurve();

private:
	Ui::CalculateCurveDialog* _ui;
};
