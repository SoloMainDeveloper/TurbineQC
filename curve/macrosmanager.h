#pragma once
#include "turbinewindow.h"
#include "filesystem.h"
#include "loadingcloudwindow.h"

class MacrosTranslator : public QObject {
    Q_OBJECT

public:
    enum OperationCRM {
        LoadCRV, PartData, CollectCross, Regen2D, Alignment,
        ShiftCurves1, RotateCurves1, BestFit2D, Airfoil, InsertBFPos,
        ShowOptions, EditDim, SaveFLR, PrinterSettings, PrintFromViewer
    };
    Q_ENUM(OperationCRM)

    static QStringList splitCRM(const QString &text);
    static OperationCRM operationCRMFromString(QString macrosType);
};

class MacrosManager : public QObject {
    Q_OBJECT
public:
    enum Operation {
        LoadCloud, LoadProject, SaveProject, ClearProject,
        ConstantTolerance, EdgesTolerance, GetWidthOfEdges,
        MergeScans, RadiusCorrection, CalculateDeviations, BestFit,
        CreateDimension,
        RenameFigure, RemoveFigure, FigureVisibilityChanged,
        CreateReport, ChangeFigureColor, ExportToFLR,
        ShiftFigure, RotateFigure, Alignment,
        CalculateCurve, RegenerateCurve, EditFigure,
        CreateMaxCircle, CreateMiddleCurve, CreateContactLine,
        ChangeFigureVisibility, ChangeCurveParameters, ChangeDimensionParameters,
        PartData, InsertText,
        Unknown
    };
    Q_ENUM(Operation)

    enum MacrosType {
        CRM,
        CRMM
    };
    Q_ENUM(MacrosType)


    static MacrosManager& instance();
    static void save(const QString fileName);
    static void load();
    static QList<QPair<Operation, QMap<QString, QString>>>* translate(MacrosType type, const QString &operationText);
    static void clear();
    static void run(Project *project, Plot *plot);
    static void debugNext(Project *project, Plot *plot);
    static void stopDebug();
    static bool tryExecuteOperation(Project *project, Plot *plot, int index);
    static void log(const Operation operation, const QMap<QString, QString> &params = QMap<QString, QString>());
    template<typename Func, typename... Args>
    static void executeWithoutLogging(Func func, Args&&... args);
    static void insert(int index, QString operationText);
    static void remove(int index);
    static bool isRecording();
    static void editOperation(int index, QString newOperationText);
    static int recordIndex();
    static void setRecordIndex(int newIndex);

    static Operation fromString(QString operation);
    static QString toString(Operation operation);
    static MacrosType macrosTypeFromString(QString macrosType);

public slots:
    static void toggleRecording();
    static void setRecording(bool needRecording);

signals:
    void operationLogged(QString operation, QString comment);
    void recordingToggled();
    void recordIndexChanged(int index);
    void operationExecuted(int index, bool isSuccessful);

private:
    MacrosManager() {
    }
    static QString createOperationText(const Operation operation, const QMap<QString, QString> &params = QMap<QString, QString>());

    static bool _isRecording;
    static int _recordIndex;
    static int _debugIndex;
    static QStringList* _macros;
    static QMap<QString, Operation> stringToEnumMap;
};

template<typename Func, typename ...Args>
inline void MacrosManager::executeWithoutLogging(Func func, Args && ...args) {
    auto isRecording = MacrosManager::isRecording();
    setRecording(false);
    try {
        func(std::forward<Args>(args)...); //execute function
    } catch(...) {
        setRecording(isRecording);
        throw;
    }
    setRecording(isRecording);
}
