#pragma once

#include "ui_printpreviewdialog.h"

class PrintPreviewDialog : public BaseDialog {
    Q_OBJECT

public:
    PrintPreviewDialog();
    ~PrintPreviewDialog();

    void initialize() override;

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
    Ui::PrintPreviewDialog *_ui;

    QAction *_printItem;
    QAction *_deleteItem;

    void updateReportPages(int updateIndex);
};
