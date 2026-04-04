#pragma once

#include "icommand.h"

class HideAllCommand : public ICommand {
public:
    HideAllCommand() = default;
    HideAllCommand(QString figuresType);

    void run() override;
    CommandType getType() const override;
    QMap<QString, QVariant> getParameters() const override;
    void setParameters(QMap<QString, QVariant> params) override;
    QString getName() const override;
    QString getDescription() const override;

private:
    QString _figuresType;
};
