#include "curve/pch.h"

#include "setprintersettingscommand.h"

SetPrinterSettingsCommand::SetPrinterSettingsCommand(const Printer::PrintType printType) {
    _printType = printType;
}

void SetPrinterSettingsCommand::run() {
    Printer::instance().setPrinterSettings(_printType);
}

CommandType SetPrinterSettingsCommand::getType() const {
    return CommandType::SetPrinterSettings;
}

QMap<QString, QVariant> SetPrinterSettingsCommand::getParameters() const {
    return {
        { "printType", Printer::printTypeToQString(_printType) },
    };
}

void SetPrinterSettingsCommand::setParameters(QMap<QString, QVariant> params) {
    _printType = Printer::qStringToPrintType(params["printType"].toString());
}

QString SetPrinterSettingsCommand::getName() const {
    return "Установить тип печати";
}

QString SetPrinterSettingsCommand::getDescription() const {
    return "Set printer settings";
}
