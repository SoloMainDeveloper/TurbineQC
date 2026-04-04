#pragma once

#include "icommand.h"

class RadiusCorrection3DCommand : public ICommand {
public:
    RadiusCorrection3DCommand() = default;
    RadiusCorrection3DCommand(const QString& nominalCurveName, const QString& measuredCurveName, const QString& resultCurveName,
        const Function42Params* params, double radiusCorrection);

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
    const Function42Params* _params;
    double _radiusCorrection;
};
