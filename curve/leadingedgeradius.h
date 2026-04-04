#pragma once

#include "iturbineprofileparameter.h"

class LeadingEdgeRadius : public ITurbineProfileParameter {
public:
    LeadingEdgeRadius(double nominal, double upperTolerance, double lowerTolerance,
        QString extraParam1 = {}, QString extraParam2 = {});

    void calculateNominal(const QString& nominalProfileName, const Function18Params& params) override;
    void createMeasured(const QString& nominalProfileName, const QString& measuredProfileName, const Function18Params& params) override;

    QString reportType() const override;
    QString dimensionName() const override;

private:
    const QString _parameterNameTemplate = "%1_LE_CRadius";
    const QString _dimensionNameTemplate = "%1_LE_Radius";
};
