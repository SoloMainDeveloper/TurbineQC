#pragma once

#include "icommand.h"

#include "turbinedialog.h"
#include "filesystem.h"
#include "loadingclouddialog.h"
#include "printer.h"

class MacrosManager : public QObject {
    Q_OBJECT

public:
    MacrosManager();
    static MacrosManager& instance();

    QJsonArray toJson();
    void fromJson(const QJsonArray& json);

    void clear();
    void run();
    void executeOne(int index);
    void debugNext();
    void skipOne();
    void stopDebug();

    void log(std::shared_ptr<ICommand> command);
    std::shared_ptr<ICommand> getCommand(int index);
    void insert(int index, std::shared_ptr<ICommand> command);

    void remove(int index);
    bool isRecording();
    int recordIndex();
    void setRecordIndex(int newIndex);

    template<typename Func, typename... Args>
    void executeWithoutLogging(Func func, Args&&... args);
    bool tryExecuteOperation(int index);

public slots:
    void toggleRecording();
    void setRecording(bool needRecording);
    void swapOperations(int index1, int index2);

signals:
    void operationLogged(QString operation, QString comment);
    void recordingToggled();
    void recordIndexChanged(int index);
    void operationExecuted(int index, bool isSuccessful);
    void operationSkipped(int index);

private:
    bool _isRecording;
    int _recordIndex;
    int _debugIndex;
    QList<std::shared_ptr<ICommand>>* _macros;

    void registerAllCommands();
};

template<typename Func, typename ...Args>
inline void MacrosManager::executeWithoutLogging(Func func, Args && ...args) {
    auto isRecording = MacrosManager::isRecording();
    setRecording(false);
    try {
        func(std::forward<Args>(args)...);
    } catch(...) {
        setRecording(isRecording);
        throw;
    }
    setRecording(isRecording);
}
