#pragma once

#include "icommand.h"

class PrintReportCommand : public ICommand {
public:
    PrintReportCommand() = default;
    PrintReportCommand(const QList<int> pagesToTake);
    PrintReportCommand(bool printAll);

    void run() override;
    CommandType getType() const override;
    QMap<QString, QVariant> getParameters() const override;
    void setParameters(QMap<QString, QVariant> params) override;
    QString getName() const override;
    QString getDescription() const override;

private:
    QList<int> _pagesToTake;
    bool _printAll;
};
