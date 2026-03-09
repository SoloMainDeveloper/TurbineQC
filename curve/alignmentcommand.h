#pragma once

#include "icommand.h"

class AlignmentCommand : public ICommand {
public:
    AlignmentCommand() = default;
    AlignmentCommand(QString angle, QString axis, QString offsetX, QString offsetY);

    void run() override;
    CommandType getType() const override;
    QMap<QString, QVariant> getParameters() const override;
    void setParameters(QMap<QString, QVariant> params) override;
    QString getName() const override;
    QString getDescription() const override;

private:
    QString _angle;
    QString _axis;
    QString _offsetX;
    QString _offsetY;
};
