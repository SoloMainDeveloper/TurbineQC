#pragma once

#include "basedialog.h"

namespace Ui {
    class PartDataDialog;
};

class PartDataDialog : public BaseDialog {
    Q_OBJECT

public:
    explicit PartDataDialog();
    virtual ~PartDataDialog();

    void initialize() override;

public slots:
    void initializationByMacros(QString reportTitle = nullptr, QString description = nullptr, QString drawing = nullptr, QString orderNumber = nullptr,
        QString partNumber = nullptr, QString projectOperator = nullptr, QString note = nullptr, QString machine = nullptr, QString tool = nullptr,
        QString fixturing = nullptr, QString batch = nullptr, QString supplier = nullptr, QString revision = nullptr);

    void applyPartDataChanges();

signals:
    void partDataChangeRequested(QString reportTitle, QString description, QString drawing, QString orderNumber, QString partNumber, QString projectOperator, QString note,
        QString machine, QString tool, QString fixturing, QString batch, QString supplier, QString revision, bool needShowWindowWhenMacroRuns);

private:
    Ui::PartDataDialog *_ui;
};
