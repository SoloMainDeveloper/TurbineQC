#pragma once

#include "icommand.h"
#include "printer.h"

class SetPrinterSettingsCommand : public ICommand {
public:
    SetPrinterSettingsCommand() = default;
    SetPrinterSettingsCommand(const Printer::PrintType printType);

    void run() override;
    CommandType getType() const override;
    QMap<QString, QVariant> getParameters() const override;
    void setParameters(QMap<QString, QVariant> params) override;
    QString getName() const override;
    QString getDescription() const override;

private:
    Printer::PrintType _printType;
};
