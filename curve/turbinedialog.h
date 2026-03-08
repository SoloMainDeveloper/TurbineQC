#pragma once

#include "project.h"
#include "algorithms.h"
#include "curvegraphicswidget.h"
#include "reportsettings.h"
#include "reportgenerator.h"
#include "basedialog.h"

namespace Ui {
	class TurbineDialog;
}

class TurbineDialog : public BaseDialog {
	Q_OBJECT

public:
	explicit TurbineDialog();
	virtual ~TurbineDialog();

	void initialize() override;

private slots:
	void run();
	void closeWindow();
	void closeEvent(QCloseEvent* event);
	void onCalculateNominals();
	void updateParamOutputView();
	void changeItemOfList();
	void onProfileTypeChange();
	void onNoBestFitLEClick();
	void onNoBestFitTEClick();
	void onShowNumDevLEClick();
	void onShowNumDevTEClick();
	void onPreparePointsClick();
	void onDeleteSimilarClick();
	void onRadiusCompensationClick();
	void onUse3DVectorsForCompClick();
	void onRadiusCompensationChange();
	void onNeedPrintTemplateClick();

private:
	Ui::TurbineDialog* _ui;
	std::shared_ptr<ReportSettings> _reportSettings;
	CurveGraphicsWidget* _curveGraphics;
	QGridLayout* _containerLayout;
	QDoubleValidator* _doubleValidator;
	QIntValidator* _intValidator;

	void setupWindow();
	void disableUnrealizedParams();
	void calculateNominals(const QString& curveName);
	void setSettings();
};
