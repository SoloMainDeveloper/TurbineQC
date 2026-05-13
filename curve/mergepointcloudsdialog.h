#pragma once

#include "curvegraphicswidget.h"
#include "project.h"

namespace Ui {
    class MergePointCloudsDialog;
} // namespace Ui

class MergePointCloudsDialog : public BaseDialog {
    Q_OBJECT

public:
    explicit MergePointCloudsDialog();
    virtual ~MergePointCloudsDialog();

    void initialize() override;

private slots:
    void sort();
    void merge();
    void changeItemOfFirstList(QListWidgetItem* currentFirstScan);
    void changeItemOfSecondList(QListWidgetItem* currentSecondScan);
    void closeEvent(QCloseEvent* event);
    void closeWindow();

private:
    Ui::MergePointCloudsDialog* _ui;
    Project* _project;
    CurveGraphicsWidget* _curveGraphics;
    QGridLayout* _containerLayout;
    QMessageBox* _message;

    QString _name;

    void updateFigures();
    void autoSelectSimilarInSecondList(const QString& selectedFromFirst);
    int calculatePrefixMatchLength(const QString& str1, const QString& str2);
    QString findMostSimilarByPrefix(const QString& selectedItem, const QStringList& items);
};
