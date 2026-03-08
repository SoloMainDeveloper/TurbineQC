#pragma once

#include "icommand.h"

class ExportToFLRCommand : public ICommand {
public:
    ExportToFLRCommand() = default;
    ExportToFLRCommand(QString filepath, QStringList curvesToTake);

    void run() override;
    CommandType getType() const override;
    QMap<QString, QVariant> getParameters() const override;
    void setParameters(QMap<QString, QVariant> params) override;
    QString getName() const override;
    QString getDescription() const override;

private:
    QString _filepath;
    QStringList _curvesToTake;
};
