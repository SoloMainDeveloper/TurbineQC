#pragma once

#include "icommand.h"

class MergeScansCommand : public ICommand {
public:
    MergeScansCommand() = default;
    MergeScansCommand(QString firstCurveName, QString secondCurveName, QString resultName, double threshold, bool needSorted);

    void run() override;
    CommandType getType() const override;
    QMap<QString, QVariant> getParameters() const override;
    void setParameters(QMap<QString, QVariant> params) override;
    QString getName() const override;
    QString getDescription() const override;

private:
    QString _firstCurveName;
    QString _secondCurveName;
    QString _resultName;
    double _threshold;
    bool _needSorted;
};
