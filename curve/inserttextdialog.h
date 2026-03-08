#pragma once

namespace Ui {
	class InsertTextDialog;
};

class InsertTextDialog : public BaseDialog {
	Q_OBJECT

public:
	explicit InsertTextDialog();
	virtual ~InsertTextDialog();

	void initialize() override;

public slots:
	void onPlotClick(const QPointF& point);

private:
	int findFreeIndex();
	void insertText();
	void openImage();
	void selectCoords();

	Ui::InsertTextDialog* _ui;
	bool _pointSelecting = false;
};
