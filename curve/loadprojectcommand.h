#pragma once

#include "icommand.h"

class LoadProjectCommand : public ICommand {
public:
    LoadProjectCommand() = default;
    LoadProjectCommand(QString filePath);

    void run() override;
    CommandType getType() const override;
    QMap<QString, QVariant> getParameters() const override;
    void setParameters(QMap<QString, QVariant> params) override;
    QString getName() const override;
    QString getDescription() const override;

private:
    QString _filePath;
};

