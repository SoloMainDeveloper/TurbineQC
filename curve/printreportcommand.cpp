#include "curve/pch.h"

#include "printreportcommand.h"
#include "printer.h"

PrintReportCommand::PrintReportCommand(const QList<int> pagesToTake) {
    _pagesToTake = pagesToTake;
}

PrintReportCommand::PrintReportCommand(bool printAll) {
    _printAll = printAll;
}

void PrintReportCommand::run() {
    auto& printer = Printer::instance();
    if(_printAll) {
        printer.printAll();
    } else {
        printer.print(_pagesToTake);
    }
}

CommandType PrintReportCommand::getType() const {
    return CommandType::PrintReport;
}

QMap<QString, QVariant> PrintReportCommand::getParameters() const {
    QVariantList pages;
    for(int page : _pagesToTake) {
        pages.append(page);
    }
    return {
        { "pages", pages },
        { "printAll", _printAll }
    };
}

void PrintReportCommand::setParameters(QMap<QString, QVariant> params) {
    if(params.contains("pages")) {
        QVariantList pages = params["pages"].toList();
        _pagesToTake.clear();
        for(const QVariant& v : pages) {
            _pagesToTake.append(v.toInt());
        }
    }
    _printAll = params["printAll"].toBool();
}

QString PrintReportCommand::getName() const {
    return "Печать отчёта";
}

QString PrintReportCommand::getDescription() const {
    return "Print report";
}
