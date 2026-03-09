#pragma once

#include "icommand.h"

class InsertBestFitPositionCommand : public ICommand {
public:
    InsertBestFitPositionCommand() = default;
    InsertBestFitPositionCommand(QString figureName, QString parentName, double x, double y, double z, bool isShowX, bool isShowY, bool isShowR);

    void run() override;
    CommandType getType() const override;
    QMap<QString, QVariant> getParameters() const override;
    void setParameters(QMap<QString, QVariant> params) override;
    QString getName() const override;
    QString getDescription() const override;

private:
    QString _figureName;
    QString _parentName;
    double _x;
    double _y;
    double _z;
    bool _isShowX;
    bool _isShowY;
    bool _isShowR;
};
