#include "curve/pch.h"

#include "commandfactory.h"
#include "macrosreaderjson.h"

QList<std::shared_ptr<ICommand>>* MacrosReaderJSON::read(const QString filepath)
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly)) {
        throw new std::runtime_error("Cannot open file for reading JSON macros");
    }

    QString content = QString::fromUtf8(file.readAll());
    content.remove('\r');

    QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8());

    QList<std::shared_ptr<ICommand>>* result = new QList<std::shared_ptr<ICommand>>();
    if(doc.isArray()) {
        for(const auto& value : doc.array()) {
            if(value.isObject()) {
                auto command = CommandFactory::instance().createFromJson(value.toObject());
                result->append(command);
            }
        }
    }

    file.close();

    return result;
}

QString MacrosReaderJSON::availableExtension()
{
    return QString("json");
}