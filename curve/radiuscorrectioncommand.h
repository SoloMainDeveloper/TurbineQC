#pragma once

#include "icommand.h"

class RadiusCorrectionCommand : public ICommand {
public:
    RadiusCorrectionCommand() = default;
    RadiusCorrectionCommand(QString figureName, QString newFigureName, const Function3Params* params);

    void run() override;
    CommandType getType() const override;
    QMap<QString, QVariant> getParameters() const override;
    void setParameters(QMap<QString, QVariant> params) override;
    QString getName() const override;
    QString getDescription() const override;

private:
    QString _figureName;
    QString _newFigureName;
    const Function3Params* _params;
};
