#include "curve/pch.h"

#include "commandfactory.h"

CommandFactory& CommandFactory::instance() {
    static CommandFactory factory;
    return factory;
}

std::shared_ptr<ICommand> CommandFactory::createCommand(CommandType type) {
    if(_creators.contains(type)) {
        return _creators[type]();
    }
    return nullptr;
}

std::shared_ptr<ICommand> CommandFactory::createFromJson(const QJsonObject& json) {
    if(!json.contains("type") || !json["type"].isString()) {
        qWarning() << "Invalid command JSON: missing type";
        return nullptr;
    }

    QString typeStr = json["type"].toString();
    CommandType type = stringToCommandType(typeStr);
    auto command = createCommand(type);

    if(command) {
        command->fromJson(json);
    }

    return command;
}

QList<CommandType> CommandFactory::getRegisteredTypes() const {
    return _creators.keys();
}
