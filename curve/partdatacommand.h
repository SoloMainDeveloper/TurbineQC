#pragma once

#include "icommand.h"

class PartDataCommand : public ICommand {
public:
    PartDataCommand() = default;
    PartDataCommand(
        QString reportTitle,
        QString description,
        QString drawing,
        QString orderNumber,
        QString partNumber,
        QString projectOperator,
        QString note,
        QString machine,
        QString tool,
        QString fixturing,
        QString batch,
        QString supplier,
        QString revision,
        bool needShowWindowWhenMacroRuns
    );

    void run() override;
    CommandType getType() const override;
    QMap<QString, QVariant> getParameters() const override;
    void setParameters(QMap<QString, QVariant> params) override;
    QString getName() const override;
    QString getDescription() const override;

private:
    QString _reportTitle;
    QString _description;
    QString _drawing;
    QString _orderNumber;
    QString _partNumber;
    QString _projectOperator;
    QString _note;
    QString _machine;
    QString _tool;
    QString _fixturing;
    QString _batch;
    QString _supplier;
    QString _revision;
    bool _needShowWindowWhenMacroRuns;
};
