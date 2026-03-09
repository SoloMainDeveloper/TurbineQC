#pragma once

#include "dialogtype.h"

class BaseDialog : public QDialog {
	Q_OBJECT

public:
	explicit BaseDialog(QWidget* parent = nullptr);
	virtual ~BaseDialog() = default;
	virtual void initialize() = 0;
};

