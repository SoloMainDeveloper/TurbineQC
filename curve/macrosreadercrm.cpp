#include "curve/pch.h"

#include "macrosreadercrm.h"
#include "macrostranslator.h"

QList<std::shared_ptr<ICommand>>* MacrosReaderCRM::read(const QString filepath)
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly)) {
        throw new std::runtime_error("Cannot open file for reading CRM macros");
    }

    QByteArray data = file.readAll();
    QString operationText = QString::fromUtf8(data);
    operationText.remove('\r');

    QList<std::shared_ptr<ICommand>>* commands = MacrosTranslator::translateCRM(operationText);

    file.close();

    return commands;
}

QString MacrosReaderCRM::availableExtension()
{
    return QString("crm");
}
