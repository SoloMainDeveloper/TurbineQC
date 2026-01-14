#pragma once

#include "ui_saveprojectwindow.h"
#include "project.h"
#include "filesystem.h"

namespace Ui {
    class SaveProjectWindow;
}

class SaveProjectWindow : public QDialog {
    Q_OBJECT

public:
    SaveProjectWindow(Project *project, QWidget *parent = nullptr);
    ~SaveProjectWindow();

public slots:
    void initialization();

private slots:
    void closeWindow();
    void closeEvent(QCloseEvent *event);
    void showBoxCRV();
    void saveProject();
    void openExplorer();
    void projectNameChanged();

private:
    Ui::SaveProjectWindow *_ui;
    Project *_project;
};
