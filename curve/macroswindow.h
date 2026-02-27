#pragma once

#include <QMainWindow>
#include "ui_macroswindow.h"

namespace Ui {
    class MacrosWindow;
}

class MacrosWindow : public QMainWindow
{
    Q_OBJECT

public:
    MacrosWindow(QWidget *parent, Project *project, Plot *plot);
    ~MacrosWindow();

public slots:
    void initialization();
    void closeEvent(QCloseEvent *event);
    void clear();
    void save();
    void startDebug();
    void stopDebug();
    void debugNext();

signals:
    void needShow(bool needShow);

private:
    Ui::MacrosWindow *_ui;
    Project *_project;
    Plot *_plot;
    QTreeWidget *_operationList;
    QAction *removeItem;
    QAction *editItem;

    void run();
    void contextMenuEvent(QContextMenuEvent *event);
    void onRemoveItemTriggered();
    void onEditItemTriggered();
    void onMoveRecordButtonClicked();
    void onRecordIndexChanged(int index);
    void onOperationExecuted(int index, bool isSuccessful);
    void removeOperation(QTreeWidgetItem *item);
    void reindex(int indexFrom);
    void insert(QTreeWidget *tree, int index, QTreeWidgetItem *item);
    QTreeWidgetItem* createOperationItem(int index, QString operation, QString comment);

private slots:
    void addOperation(QString operation, QString comment);
    void updateRecordingButton();
};
