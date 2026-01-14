#pragma once

#include "ui_mergepointcloudswindow.h"
#include "project.h"
#include "algorithms.h"
#include "curvegraphicswidget.h"

namespace Ui {
    class MergePointCloudsWindow;
}

class MergePointCloudsWindow : public QDialog {
    Q_OBJECT

public:
    explicit MergePointCloudsWindow(Project *project = nullptr, QWidget *parent = nullptr);
    virtual ~MergePointCloudsWindow();

public slots:
    void windowInitialization();

private slots:
    void sort();
    void merge();
    void changeItemOfList();
    void closeEvent(QCloseEvent *event);
    void closeWindow();

private:
    Ui::MergePointCloudsWindow *_ui;
    Project *_project;
    CurveGraphicsWidget *_curveGraphics;
    QGridLayout *_containerLayout;
    QMessageBox *_message;

    QString _name;
};
