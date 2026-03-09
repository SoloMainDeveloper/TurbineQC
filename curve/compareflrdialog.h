#pragma once

namespace Ui {
	class CompareFLRDialog;
};

class CompareFLRDialog : public BaseDialog {
	Q_OBJECT

public:
	explicit CompareFLRDialog();
	virtual ~CompareFLRDialog();

	void initialize() override;
	void compare();

private:
	Ui::CompareFLRDialog* _ui;
};
