#pragma once
#include "turbinewindow.h"
#include "filesystem.h"
#include "loadingcloudwindow.h"

class MacrosManager : public QObject {
    Q_OBJECT
public:
    enum Operation {
        LoadCloud, LoadProject, SaveProject, ClearProject,
        AssignTolerance, GetWidthOfEdges,
        MergeScans, RadiusCorrection, CalculateDeviations, BestFit,
        CreateDimension,
        RenameFigure, RemoveFigure, FigureVisibilityChanged,
        Unknown
    };

    Q_ENUM(Operation)

    static MacrosManager& instance();

    static void saveRecord(const QString fileName);
    static void clear();
    static void runMacros(Project *project);
    static void log(const Operation operation, const QMap<QString, QString> &params = QMap<QString, QString>(), QString comment = "");
    static void remove(int index);
    static bool isRecording();

    static Operation fromString(QString operation);
    static QString toString(Operation operation);

public slots:
    static void toggleRecording();

signals:
    void operationLogged(QString operation, QString comment);
    void recordingToggled();

private:
    MacrosManager() { }

    static bool _isRecording;
    static int _operationsCount;
    static QStringList* _macros;
    static QMap<QString, Operation> stringToEnumMap;
};