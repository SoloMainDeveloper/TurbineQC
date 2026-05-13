#pragma once

#include "iturbineprofileparameter.h"

class MinX : public ITurbineProfileParameter {
public:
    MinX(double nominal, double upperTolerance, double lowerTolerance,
        QString extraParam1 = {}, QString extraParam2 = {});

    void calculateNominal(const QString& nominalProfileName, const Function18Params& params) override;
    void createMeasured(const QString& nominalProfileName, const QString& measuredProfileName, const Function18Params& params) override;

    QString reportType() const override;
    QString dimensionName() const override;
};
