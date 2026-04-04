#pragma once

#include "iturbinetransformparameter.h"

class ShiftX : public ITurbineTransformParameter {
public:
    ShiftX(double nominal, double upperTolerance, double lowerTolerance,
        QString extraParam1 = {}, QString extraParam2 = {});

    QString reportType() const override;
};
