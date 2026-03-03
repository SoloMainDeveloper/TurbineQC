#include "curve/pch.h"
#include "printer.h"

QStringList Printer::_printPages;
Printer::PrintType Printer::_printType = PrintType::HTML; //add UI to choose printType

Printer& Printer::instance() {
    static Printer printer;
    return printer;
}

void Printer::addPage(const QString &page, const QMap<QString, QString> &information) {
    _printPages.append(page);
    emit instance().pageAdded(information);
}

void Printer::removePage(int index) {
    _printPages.removeAt(index);
    emit instance().pageRemoved(index);
    MacrosManager::log(MacrosManager::RemovePage, { { "index", QString::number(index) } });
}

void Printer::clear() {
    _printPages.clear();
    emit instance().printerPagesCleared();
    MacrosManager::log(MacrosManager::ClearReport);
}

QString Printer::printTypeToQString(PrintType type) {
    return QMetaEnum::fromType<PrintType>().valueToKey(static_cast<int>(type));
}

Printer::PrintType Printer::qStringToPrintType(const QString &type) {
    return static_cast<PrintType>(QMetaEnum::fromType<Printer::PrintType>().keyToValue(type.toLatin1()));
}

void Printer::setPrinterSettings(const PrintType type) {
    //_printType = type;
    _printType = PrintType::HTML;
    MacrosManager::log(MacrosManager::SetPrinterSettings, { { "printType", printTypeToQString(type) } });
}

void Printer::printAll() {
    QStringList pagesToTake;
    for(auto i = 0; i < _printPages.length(); i++) {
        pagesToTake.append(QString::number(i));
    }
    print(pagesToTake);
}

void Printer::print(const QStringList &pagesToTake) {
    auto filePath = QFileDialog::getSaveFileName(nullptr, "Save report", "", "HTML Files (*.html)");
    if(filePath.isEmpty()) return;

    switch(_printType) {
        case PrintType::HTML:
        {
            QFile file(filePath);
            file.open(QIODevice::WriteOnly | QIODevice::Truncate);
            QTextStream in(&file);
            QStringList fullMarkup;
            for(auto i = 0; i < pagesToTake.size(); i++) {
                auto index = pagesToTake[i].toInt();
                fullMarkup.append(_printPages[index]);
            }
            in << MarkupCreator::index.arg(fullMarkup.join(""));
            file.close();
            break;
        }
        case PrintType::PDF:
            break;
        default:
            break;
    }
    MacrosManager::log(MacrosManager::PrintReport, { { "pagesToTake" , pagesToTake.join(",")} });
}
