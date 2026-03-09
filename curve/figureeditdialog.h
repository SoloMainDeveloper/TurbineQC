#pragma once

#include "project.h"

namespace Ui {
    class FigureEditDialog;
};

class FigureEditDialog : public BaseDialog {
    Q_OBJECT

public:
    explicit FigureEditDialog();
    virtual ~FigureEditDialog();

    void initialize() override;

    //signals:
    //    void dimensionValueChanged(const QString &dimName, const DimFigure::Value &value);

public slots:
    void chooseColor();

private:
    Ui::FigureEditDialog* _ui;
    Project* _project;
    QString _figureName;
    const Figure* _figure;
    QSet<int> _tableCellChanged;
    QMap<QString, QString> _paramsChanged;
    QDoubleValidator* _doubleValidator;

    void validateFields(const QList<QLineEdit*>& fields, const QValidator* validator = nullptr);
    const QString convertValueTypeToString(const DimFigure::ValueType& value);
    void fillDimTable(const QVector<DimFigure::Value>& values);
    void applyChanges();
    void tableCellChanged(int row, int column);
    void figureNameChanged();
};