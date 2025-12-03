#pragma once

#include "ui_loadingcloudwindow.h"
#include "curvewindow.h"
#include "filesystem.h"
#include "project.h"
#include "tree.h"

namespace Ui { 
    class LoadingCloudWindow; 
};

class LoadingCloudWindow : public QDialog {
    Q_OBJECT

public:
    explicit LoadingCloudWindow(Project *project, QWidget *parent = nullptr);
    virtual ~LoadingCloudWindow();

private slots:
    void selectFile();
    void selectClipboard();
    void findFile();
    void openFile();
    void pasteData();
    void loadCloud();
    void closeEvent(QCloseEvent *event);
    void closeWindow();

private:
    Ui::LoadingCloudWindow *_ui;

    QStringList _fileData;
    QString _name;

    Project *_project;
    QGroupBox *_groupOfBtnWidgets;
    QHBoxLayout *_btnWidgetsLayout;
    QPushButton *_pasteDataBtn;
};
