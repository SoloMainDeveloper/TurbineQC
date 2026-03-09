#pragma once

#include "project.h"

namespace Ui {
	class AlignmentDialog;
};

class AlignmentDialog : public BaseDialog {
	Q_OBJECT

public:
	explicit AlignmentDialog();
	virtual ~AlignmentDialog();

	void initialize() override;

private slots:
	void applyAlignment();
	void changeAngle();
	void changeOffsetX();
	void changeOffsetY();

signals:
	void alignmentRequested(QString angle, QString axis, QString offsetX, QString offsetY);

private:
	Ui::AlignmentDialog* _ui;

	Project* _project;

	bool _angleMacrosFlag = false;
	bool _offsetXMacrosFlag = false;
	bool _offsetYMacrosFlag = false;
};
