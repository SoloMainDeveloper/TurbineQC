#pragma once

namespace Ui {
	class ExportToFLRDialog;
};

class ExportToFLRDialog : public BaseDialog {
	Q_OBJECT

public:
	explicit ExportToFLRDialog();
	virtual ~ExportToFLRDialog();

public slots:
	void initialize() override;

private slots:
	void exportToFLR();
	void chooseFilePath();
	void closeWindow();
	void closeEvent(QCloseEvent* event);
	void onChooseAllStateChanged();

private:
	Ui::ExportToFLRDialog* _ui;
};
