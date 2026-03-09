#include "curve/pch.h"

#include "printreportcommand.h"
#include "printer.h"

PrintReportCommand::PrintReportCommand(const QList<int> pagesToTake) {
    _pagesToTake = pagesToTake;
}

void PrintReportCommand::run() {
    Printer::instance().print(_pagesToTake);
}

CommandType PrintReportCommand::getType() const {
    return CommandType::PrintReport;
}

QMap<QString, QVariant> PrintReportCommand::getParameters() const {
    QVariantList pages;
    for(int page : _pagesToTake) {
        pages.append(page);
    }
    return { { "pages", pages } };
}

void PrintReportCommand::setParameters(QMap<QString, QVariant> params) {
    if(params.contains("pages")) {
        QVariantList pages = params["pages"].toList();
        _pagesToTake.clear();
        for(const QVariant& v : pages) {
            _pagesToTake.append(v.toInt());
        }
    }
}

QString PrintReportCommand::getName() const {
    return "Печать отчёта";
}

QString PrintReportCommand::getDescription() const {
    return "Print report";
}
