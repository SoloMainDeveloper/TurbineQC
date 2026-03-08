#pragma once

namespace Ui {
    class RotateDialog;
};

class RotateDialog : public BaseDialog {
    Q_OBJECT

public:
    explicit RotateDialog();
    virtual ~RotateDialog();

    void initialize() override;
    void addTreeTab(QStandardItem* tab);

private slots:
    void applyRotate();
    void plus90();
    void minus90();
    void updateXYZ(QString figureName);

signals:
    void figureRotateRequested(QString figureName, double angle, QString x, QString y, QString z);

private:
    Ui::RotateDialog *_ui;

    QStandardItemModel* _treeModel;
    QStandardItem* _treeCurves;
    QStandardItem* _treeCircles;
    QStandardItem* _treeLines;
    QStandardItem* _treePoints;
};
