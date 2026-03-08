#pragma once

#include "icommand.h"

class RegenerateCurveCommand : public ICommand {
public:
    RegenerateCurveCommand() = default;
    RegenerateCurveCommand(QString figureName, QString newfigureName, const Function19Params* params);

    void run() override;
    CommandType getType() const override;
    QMap<QString, QVariant> getParameters() const override;
    void setParameters(QMap<QString, QVariant> params) override;
    QString getName() const override;
    QString getDescription() const override;

private:
    QString _figureName;
    QString _newFigureName;
    const Function19Params* _params;
};
