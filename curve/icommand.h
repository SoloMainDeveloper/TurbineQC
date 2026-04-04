#pragma once

#include "commandtype.h"

class ICommand : public QObject {
    Q_OBJECT

public:
    virtual ~ICommand() = default;

    virtual void run() = 0;
    virtual CommandType getType() const = 0;
    virtual QMap<QString, QVariant> getParameters() const = 0;
    virtual void setParameters(QMap<QString, QVariant> params) = 0;

    virtual QString getName() const;
    virtual QString getDescription() const;
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);
};
