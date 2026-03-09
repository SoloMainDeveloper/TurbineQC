#pragma once

#include "icommand.h"

class ClearProjectCommand : public ICommand {
public:
    ClearProjectCommand() = default;

    void run() override;
    CommandType getType() const override;
    QMap<QString, QVariant> getParameters() const override;
    void setParameters(QMap<QString, QVariant> params) override;
    QString getName() const override;
    QString getDescription() const override;
};
