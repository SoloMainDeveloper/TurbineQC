#pragma once

#include "icommand.h"

class ShiftFigureCommand : public ICommand {
public:
    ShiftFigureCommand() = default;
    ShiftFigureCommand(QString figureName, QString x, QString y, QString z);

    void run() override;
    CommandType getType() const override;
    QMap<QString, QVariant> getParameters() const override;
    void setParameters(QMap<QString, QVariant> params) override;
    QString getName() const override;
    QString getDescription() const override;

private:
    QString _figureName;
    QString _x;
    QString _y;
    QString _z;
};
