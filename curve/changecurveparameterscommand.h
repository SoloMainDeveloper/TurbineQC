#pragma once

#include "icommand.h"

class ChangeCurveParametersCommand : public ICommand {
public:
    ChangeCurveParametersCommand() = default;
    ChangeCurveParametersCommand(const QString &figureName, bool showPoints, bool connectPoints,
        bool showVectors, bool closed, bool showNumbering, int numberingInterval, double amplification,
        bool showTolerances, bool showDeviations, bool connectDeviations, bool highLightOut);

    void run() override;
    CommandType getType() const override;
    QMap<QString, QVariant> getParameters() const override;
    void setParameters(QMap<QString, QVariant> params) override;
    QString getName() const override;
    QString getDescription() const override;

private:
    QString _figureName;
    bool _showPoints;
    bool _connectPoints;
    bool _showVectors;
    bool _closed;
    bool _showNumbering;
    int _numberingInterval;
    double _amplification;
    bool _showTolerances;
    bool _showDeviations;
    bool _connectDeviations;
    bool _highLightOut;
};
