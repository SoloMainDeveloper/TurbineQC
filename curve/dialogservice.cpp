#include "curve/pch.h"

#include "dialogservice.h"

#include "airfoiltolerancesdialog.h"
#include "alignmentdialog.h"
#include "bestfitdialog.h"
#include "calculatecurvedialog.h"
#include "calculatedeviationsdialog.h"
#include "compareflrdialog.h"
#include "exporttoFLRdialog.h"
#include "figureeditdialog.h"
#include "inserttextdialog.h"
#include "loadingclouddialog.h"
#include "macrosdialog.h"
#include "mergepointcloudsdialog.h"
#include "partdatadialog.h"
#include "printpreviewdialog.h"
#include "radiuscorrectiondialog.h"
#include "rotatedialog.h"
#include "shiftdialog.h"
#include "turbinedialog.h"
#include "widthedgedialog.h"

DialogService& DialogService::instance()
{
    static DialogService instance;
    return instance;
}

DialogService::DialogService()
{
    _dialogs[DialogType::AirfoilTolerances] = new AirfoilTolerancesDialog();
    _dialogs[DialogType::Alignment] = new AlignmentDialog();
    _dialogs[DialogType::BestFit] = new BestFitDialog();
    _dialogs[DialogType::CalculateCurve] = new CalculateCurveDialog();
    _dialogs[DialogType::CalculateDeviations] = new CalculateDeviationsDialog();
    _dialogs[DialogType::CompareFLR] = new CompareFLRDialog();
    _dialogs[DialogType::ExportToFLR] = new ExportToFLRDialog();
    _dialogs[DialogType::FigureEdit] = new FigureEditDialog();
    _dialogs[DialogType::InsertText] = new InsertTextDialog();
    _dialogs[DialogType::LoadCloud] = new LoadingCloudDialog();
    _dialogs[DialogType::MergePointCloud] = new MergePointCloudsDialog();
    _dialogs[DialogType::PartData] = new PartDataDialog();
    _dialogs[DialogType::RadiusCorrection] = new RadiusCorrectionDialog();
    _dialogs[DialogType::Rotate] = new RotateDialog();
    _dialogs[DialogType::Shift] = new ShiftDialog();
    _dialogs[DialogType::Turbine] = new TurbineDialog();
    _dialogs[DialogType::WidthEdge] = new WidthEdgeDialog();
    _dialogs[DialogType::Macros] = new MacrosDialog();
    _dialogs[DialogType::PrintPreview] = new PrintPreviewDialog();
}

void DialogService::show(DialogType type)
{
    if(_dialogs.contains(type)) {
        _dialogs[type]->initialize();
    }
}

void DialogService::showInfo(QString title, QString info)
{
    QMessageBox::information(nullptr, title, info);
}

void DialogService::showWarning(QString title, QString message)
{
    QMessageBox::warning(nullptr, title, message);
}
