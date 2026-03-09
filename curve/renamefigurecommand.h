#pragma once

#include "icommand.h"

class RenameFigureCommand : public ICommand {
public:
    RenameFigureCommand() = default;
    RenameFigureCommand(QString name, QString newName);

    void run() override;
    CommandType getType() const override;
    QMap<QString, QVariant> getParameters() const override;
    void setParameters(QMap<QString, QVariant> params) override;
    QString getName() const override;
    QString getDescription() const override;

private:
    QString _name;
    QString _newName;
};
