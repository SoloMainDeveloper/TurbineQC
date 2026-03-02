#pragma once

#include "ui_figureeditdialog.h"
#include "project.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FigureEditDialogClass; };
QT_END_NAMESPACE

class FigureEditDialog : public QDialog
{
    Q_OBJECT

public:
    FigureEditDialog(Project* mainProject, QWidget *parent = nullptr);
    ~FigureEditDialog();

//signals:
//    void dimensionValueChanged(const QString &dimName, const DimFigure::Value &value);

public slots:
    void dialogInitialization(const QString figureName);
    void chooseColor();

private:
    Ui::FigureEditDialogClass *_ui;
    Project *_project;
    QString _figureName;
    const Figure* _figure;
    QSet<int> _tableCellChanged;
    QMap<QString, QString> _paramsChanged;
    QDoubleValidator *_doubleValidator;

    void validateFields(const QList<QLineEdit*> &fields, const QValidator* validator = nullptr);
    const QString convertValueTypeToString(const DimFigure::ValueType &value);
    void fillDimTable(const QVector<DimFigure::Value> &values);
    void applyChanges();
    void tableCellChanged(int row, int column);
    void figureNameChanged();
};