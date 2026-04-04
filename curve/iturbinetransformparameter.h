#pragma once

#include "iturbineparameter.h"

class ITurbineTransformParameter : public ITurbineParameter {
    Q_OBJECT

public:
    static void createMeasuredTransforms(const QList<ITurbineTransformParameter*>& transforms,
        const QString& nominalProfileName, const QString& parentName);
    static QString commonDimensionName();

    ITurbineTransformParameter(double nominal, double upperTolerance,
        double lowerTolerance, QString extraParam1 = {}, QString extraParam2 = {});

    virtual QString reportType() const = 0;

    void setMeasured(double measured);

private:
    const static QString _dimensionNameTemplate;

    static QString _commonDimensionName;
};
