#pragma once

namespace Ui {
	class CalculateDeviationsDialog;
};

class CalculateDeviationsDialog : public BaseDialog {
	Q_OBJECT

public:
	explicit CalculateDeviationsDialog();
	virtual ~CalculateDeviationsDialog();

	void initialize() override;

	void switchResultNameLineEdit();
	void switchAdvancedSettings();
	void changeCurveType();
	void resetDialog();
	void closeEvent(QCloseEvent* event);
	void calculateDeviations();
	void updateResultNameAndClosed(QString curveName);

private:
	Ui::CalculateDeviationsDialog* _ui;
};
