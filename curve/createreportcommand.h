#pragma once

#include "icommand.h"
#include "reportsettings.h"

class CreateReportCommand : public ICommand {
public:
    CreateReportCommand() = default;
    CreateReportCommand(std::shared_ptr<ReportSettings> reportSettings);

    void run() override;
    CommandType getType() const override;
    QMap<QString, QVariant> getParameters() const override;
    void setParameters(QMap<QString, QVariant> params) override;
    QString getName() const override;
    QString getDescription() const override;

private:
    std::shared_ptr<ReportSettings> _reportSettings;
};
