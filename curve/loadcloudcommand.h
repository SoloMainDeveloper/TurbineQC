#pragma once

#include "icommand.h"

class LoadCloudCommand : public ICommand {
public:
    LoadCloudCommand() = default;
    LoadCloudCommand(QString filePath, QString name, QString sep, int skipStart,
        int skipAfter, QString columnNames, QString colummNumbers, QString decimal);

    void run() override;
    CommandType getType() const override;
    QMap<QString, QVariant> getParameters() const override;
    void setParameters(QMap<QString, QVariant> params) override;
    QString getName() const override;
    QString getDescription() const override;

private:
    QString _filePath;
    QString _name;
    QString _sep;
    int _skipStart;
    int _skipAfter;
    QString _columnNames;
    QString _columnNumbers;
    QString _decimal;
};
