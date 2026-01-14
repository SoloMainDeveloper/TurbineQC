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
    MacrosWindow(QWidget *parent, Project *project);
    ~MacrosWindow();

public slots:
    void initialization();
    void addOperation(QString operation, QString comment);
    void closeEvent(QCloseEvent *event);
    void clear();
    void save();
    void loadMacros();
    void toggleRecording();

signals:
    void needShow(bool needShow);

private:
    Ui::MacrosWindow *_ui;
    Project *_project;
    QTreeWidget *_operationList;
    QAction *removeItem;

    void runMacros();
    void contextMenuEvent(QContextMenuEvent *event);
    void onRemoveItemTriggered();
    void removeOperation(QTreeWidgetItem* item);
    void reindex(int indexFrom);
};
