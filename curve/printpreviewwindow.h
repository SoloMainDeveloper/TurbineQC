#pragma once

#include <QMainWindow>
#include "ui_printpreviewwindow.h"

class PrintPreviewWindow : public QMainWindow {
    Q_OBJECT

public:
    PrintPreviewWindow(QWidget *parent);
    ~PrintPreviewWindow();

    void initialization();

signals:
    void needShow(bool needShow);

public slots:
    void clearHandler();

private slots:
    void printHandler();
    void printItemTriggered();
    void deleteItemTriggered();
    void addPage(const QMap<QString, QString> &information);
    void removePage(int index);
    void contextMenuEvent(QContextMenuEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::PrintPreviewWindow *_ui;

    QAction *_printItem;
    QAction *_deleteItem;

    void updateReportPages(int updateIndex);
};
