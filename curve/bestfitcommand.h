#pragma once

#include "icommand.h"

class BestFitCommand : public ICommand {
public:
    BestFitCommand() = default;
    BestFitCommand(QString nominalCurveName,
        QString measuredCurveName,
        QString resultCurveName,
        QString bestFitLineName,
        const Function6Params* params);

    void run() override;
    CommandType getType() const override;
    QMap<QString, QVariant> getParameters() const override;
    void setParameters(QMap<QString, QVariant> params) override;
    QString getName() const override;
    QString getDescription() const override;

private:
    QString _nominalCurveName;
    QString _measuredCurveName;
    QString _resultCurveName;
    QString _bestFitLineName;
    const Function6Params* _params;
};
