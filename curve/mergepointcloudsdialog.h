#pragma once

#include "project.h"
#include "curvegraphicswidget.h"

namespace Ui {
    class MergePointCloudsDialog;
}

class MergePointCloudsDialog : public BaseDialog {
    Q_OBJECT

public:
    explicit MergePointCloudsDialog();
    virtual ~MergePointCloudsDialog();

    void initialize() override;

private slots:
    void sort();
    void merge();
    void changeItemOfList();
    void closeEvent(QCloseEvent *event);
    void closeWindow();

private:
    Ui::MergePointCloudsDialog *_ui;
    Project *_project;
    CurveGraphicsWidget *_curveGraphics;
    QGridLayout *_containerLayout;
    QMessageBox *_message;

    QString _name;
};
