#pragma once

#include "icommand.h"

class RemoveFigureCommand : public ICommand {
public:
    RemoveFigureCommand() = default;
    RemoveFigureCommand(QString figureName);

    void run() override;
    CommandType getType() const override;
    QMap<QString, QVariant> getParameters() const override;
    void setParameters(QMap<QString, QVariant> params) override;
    QString getName() const override;
    QString getDescription() const override;

private:
    QString _figureName;
};
