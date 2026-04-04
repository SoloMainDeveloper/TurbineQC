#pragma once

#include "iturbineparameter.h"

class ITurbineProfileParameter : public ITurbineParameter {
    Q_OBJECT

public:
    ITurbineProfileParameter(double nominal, double upperTolerance, double lowerTolerance,
        QString extraParam1 = {}, QString extraParam2 = {});

    virtual void calculateNominal(const QString& nominalProfileName,
        const Function18Params& params) = 0;
    virtual void createMeasured(const QString& nominalProfileName,
        const QString& measuredProfileName, const Function18Params& params) = 0;
    virtual QString dimensionName() const = 0;

    virtual QString reportType() const override = 0;

protected:
    QString _dimensionName;
};
