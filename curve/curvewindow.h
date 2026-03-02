#pragma once

#include "figure.h"
#include "functionparams.h"
#include "ui_curvewindow.h"
#include "filesystem.h"
#include "project.h"
#include "algorithms.h"
#include "tree.h"
#include "figurecontrols.h"
#include "airfoiltolerancesdialog.h"
#include "loadingcloudwindow.h"
#include "figureeditdialog.h"
#include "turbinewindow.h"
#include "widthedgewindow.h"
#include "radiuscorrectiondialog.h"
#include "mergepointcloudswindow.h"
#include "saveprojectwindow.h"
#include "calculatedeviationsdialog.h"
#include "bestfitdialog.h"
#include "macroswindow.h"
#include "exporttoFLRdialog.h"
#include "shiftdialog.h"
#include "rotatedialog.h"
#include "alignmentdialog.h"
#include "calculatecurvedialog.h"
#include "partdatadialog.h"
#include "inserttextdialog.h"
#include "printpreviewwindow.h"

namespace Ui {
    class CurveWindow;
};

class LoadingCloudWindow;

class PrintWindow;

class CurveWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit CurveWindow(QWidget *parent = nullptr);
    void dimensionMenuInit();
    void disableDimensionMenu();
    virtual ~CurveWindow();

private:
    Ui::CurveWindow *_ui;

    Project _project;

    FigureControls* _figureControls;
    Tree *_tree;
    Plot *_plot;

    AirfoilTolerancesDialog* _airfoilTolerancesDialog;
    LoadingCloudWindow *_loadingCloudWindow;
    FigureEditDialog* _figureEditDialog;
    TurbineWindow *_turbineWindow;
    WidthEdgeWindow *_widthEdgeWindow;
    MergePointCloudsWindow *_mergeCloudsWindow;
    RadiusCorrectionDialog *_radiusCorrectionDialog;
    SaveProjectWindow *_saveProjectWindow;
    CalculateDeviationsDialog *_calculateDeviationsDialog;
    BestFitDialog *_bestFitDialog;
    MacrosWindow *_macrosWindow;
    ExportToFLRDialog *_exportToFLRDialog;
    ShiftDialog *_shiftDialog;
    RotateDialog *_rotateDialog;
    AlignmentDialog *_alignmentDialog;
    CalculateCurveDialog *_calculatecurvedialog;
    PartDataDialog *_partDataDialog;
    InsertTextDialog *_insertTextDialog;
    PrintPreviewWindow *_printWindow;

    QString _windowTitle;

    void changeWindowTitle(const QString &projectPath);
    void setDefualtWindowTitle();
    void keyPressEvent(QKeyEvent *event);
};
