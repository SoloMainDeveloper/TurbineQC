#include "curve/pch.h"

#include "createreportcommand.h"
#include "reportgenerator.h"

CreateReportCommand::CreateReportCommand(std::shared_ptr<ReportSettings> reportSettings) {
    _reportSettings = reportSettings;
}

void CreateReportCommand::run() {
    ReportGenerator::createReport(_reportSettings);
}

CommandType CreateReportCommand::getType() const {
    return CommandType::CreateReport;
}

QMap<QString, QVariant> CreateReportCommand::getParameters() const {
    QMap<QString, QString> settings = ReportSettings::convertToQMap(_reportSettings);
    QMap<QString, QVariant> result;
    for(auto [key, value] : settings.asKeyValueRange()) {
        result.insert(key, value);
    }
    return result;
}

void CreateReportCommand::setParameters(QMap<QString, QVariant> params) {
    QMap<QString, QString> settings;
    for(auto [key, value] : params.asKeyValueRange()) {
        settings.insert(key, value.toString());
    }
    _reportSettings = ReportSettings::convertToSettings(&settings);
}

QString CreateReportCommand::getName() const {
    return "Создать отчёт";
}

QString CreateReportCommand::getDescription() const {
    return "Create report";
}
