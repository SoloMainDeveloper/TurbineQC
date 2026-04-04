#include "curve/pch.h"

#include "icommand.h"

QString ICommand::getName() const
{
    return "Unknown Command";
}

QString ICommand::getDescription() const
{
    return "No description";
}

QJsonObject ICommand::toJson() const
{
    QJsonObject json;
    json["type"] = commandTypeToString(getType());
    json["name"] = getName();

    QJsonObject params;
    auto parameters = getParameters();
    for(auto [key, value] : parameters.asKeyValueRange()) {
        params[key] = QJsonValue::fromVariant(value);
    }
    json["parameters"] = params;

    return json;
}

void ICommand::fromJson(const QJsonObject& json)
{
    if(json.contains("parameters") && json["parameters"].isObject()) {
        QJsonObject paramsObj = json["parameters"].toObject();
        setParameters(paramsObj.toVariantMap());
    }
}
