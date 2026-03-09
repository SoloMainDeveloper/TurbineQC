#pragma once

#include "icommand.h"

class EditFigureCommand : public ICommand {
public:
    EditFigureCommand() = default;
    EditFigureCommand(QString figureName, QMap<QString, QString> &paramsChanged);

    void run() override;
    CommandType getType() const override;
    QMap<QString, QVariant> getParameters() const override;
    void setParameters(QMap<QString, QVariant> params) override;
    QString getName() const override;
    QString getDescription() const override;

private:
    QString _figureName;
    QMap<QString, QString> _paramsChanged;
};
