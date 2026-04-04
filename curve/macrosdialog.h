#pragma once

#include "basedialog.h"

namespace Ui {
    class MacrosDialog;
} // namespace Ui

class MacrosDialog : public BaseDialog {
    Q_OBJECT

public:
    explicit MacrosDialog();
    virtual ~MacrosDialog();

    void initialize() override;

public slots:
    void clear();
    void save();
    void load();
    void startDebug();
    void stopDebug();
    void debugNext();

private:
    Ui::MacrosDialog* _ui;
    MacrosManager* _macrosManager;

    QTreeWidget* _operationList;
    bool _isDebugging;

    QAction* removeItem;
    QAction* editItem;
    QAction* executeCurrentItem;
    QAction* moveOperationUp;
    QAction* moveOperationDown;

    void run();
    void contextMenuEvent(QContextMenuEvent* event);
    void onRemoveItemTriggered();
    void onEditItemTriggered();
    void onExecuteCurrentItemTriggered();
    void onMoveOperationUpItemTriggered();
    void onMoveOperationDownItemTriggered();

    void onOperationLogged(std::shared_ptr<ICommand> command);
    void onRecordingToggled();
    void onMoveRecordButtonClicked();
    void onRecordIndexChanged(int index);
    void onOperationExecuted(int index, bool isSuccessful);
    void onOperationSkipped(int index);
    void keyPressEvent(QKeyEvent* event);

    void removeOperation(int index);
    void reindex(int indexFrom, int indexTo);
    void reindex(int indexFrom);
    void insert(int index, QTreeWidgetItem* item);
    QTreeWidgetItem* createOperationItem(int index, QString commandName);
};
