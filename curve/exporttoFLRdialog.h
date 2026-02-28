#pragma once

#include <QDialog>
#include "ui_exporttoFLRdialog.h"
#include "filesystem.h"

namespace Ui {
    class ExportToFLRDialog;
};

class ExportToFLRDialog : public QDialog
{
    Q_OBJECT

public:
    ExportToFLRDialog(Project *project, QWidget *parent = nullptr);
    ~ExportToFLRDialog();

public slots:
    void initialization();

private slots:
    void exportToFLR();
    void chooseFilePath();
    void closeWindow();
    void closeEvent(QCloseEvent *event);
    void onChooseAllStateChanged();

private:
    Ui::ExportToFLRDialog *_ui;
    Project *_project;
};
