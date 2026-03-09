#pragma once

#include "icommand.h"
#include <QMap>
#include <functional>

class CommandFactory {
public:
    using CommandCreator = std::function<std::shared_ptr<ICommand>()>;

    static CommandFactory& instance();

    // Регистрация типа команды
    template<typename T>
    void registerCommand() {
        T* cmd = new T();
        CommandType type = cmd->getType();  // Получаем enum
        delete cmd;

        _creators[type] = []() {
            return std::make_shared<T>();
        };
    }

    // Создание команды по типу
    std::shared_ptr<ICommand> createCommand(CommandType type);

    // Создание команды из JSON
    std::shared_ptr<ICommand> createFromJson(const QJsonObject& json);

    // Получение списка всех зарегистрированных типов
    QList<CommandType> getRegisteredTypes() const;

private:
    CommandFactory() = default;
    QMap<CommandType, CommandCreator> _creators;
};
