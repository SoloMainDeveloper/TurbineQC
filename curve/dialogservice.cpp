#include "curve/pch.h"

#include "dialogservice.h"

#include "airfoiltolerancesdialog.h"
#include "loadingclouddialog.h"
#include "figureeditdialog.h"
#include "turbinedialog.h"
#include "widthedgedialog.h"
#include "radiuscorrectiondialog.h"
#include "mergepointcloudsdialog.h"
#include "saveprojectdialog.h"
#include "calculatedeviationsdialog.h"
#include "bestfitdialog.h"
#include "macrosdialog.h"
#include "exporttoFLRdialog.h"
#include "shiftdialog.h"
#include "rotatedialog.h"
#include "alignmentdialog.h"
#include "calculatecurvedialog.h"
#include "partdatadialog.h"
#include "inserttextdialog.h"
#include "printpreviewdialog.h"
#include "compareflrdialog.h"

DialogService& DialogService::instance() {
    static DialogService instance;
    return instance;
}

DialogService::DialogService() {
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
    _dialogs[DialogType::SaveProject] = new SaveProjectDialog();
    _dialogs[DialogType::Shift] = new ShiftDialog();
    _dialogs[DialogType::Turbine] = new TurbineDialog();
    _dialogs[DialogType::WidthEdge] = new WidthEdgeDialog();
    _dialogs[DialogType::Macros] = new MacrosDialog();
    _dialogs[DialogType::PrintPreview] = new PrintPreviewDialog();
}

void DialogService::show(DialogType type) {
    if(_dialogs.contains(type)) {
        _dialogs[type]->initialize();
    }
}
