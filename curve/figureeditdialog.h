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

signals:
    void dimensionValueChanged(const QString &dimName, const DimFigure::Value &value);

public slots:
    void dialogInitialization(const QString figureName);
    void chooseColor();
    void changeDimensionValue(const QString &dimName, const DimFigure::Value &value);
    void changeCurrentDimension(const QString &dimName);

private:
    Ui::FigureEditDialogClass *_ui;
    Project *_project;
    QTableWidget *_table;
    QString _figureName;
    const Figure* _figure;
    QSet<int> _rowsChanged;
    QMap<QString, QString> _paramsChanged;
    QDoubleValidator *_doubleValidator;

    QVector<QLayout*> _layouts;
    QVector<QWidget*> _layoutWidgets;
    QLineEdit *_positionXLE, *_positionYLE, *_positionZLE;
    QLineEdit *_directionILE, *_directionJLE, *_directionKLE;
    QLineEdit *_parameterLE;
    QLineEdit *_firstRefLE, *_secondRefLE;

    void addPositionLayout(double x, double y, double z);
    void addDirectionLayout(double i, double j, double k);
    void addParameterLayout(const QString &name, const QString &value, const QValidator* validator = nullptr);

   const QString convertValueTypeToString(const DimFigure::ValueType &value);
    void fillDimTable(const QVector<DimFigure::Value> &values);
    void resetFields();
    void applyChanges();
    void tableValueChanged(int row, int column);
    void figureNameChanged();
};