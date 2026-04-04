#pragma once

#include "iturbinetransformparameter.h"

class ShiftY : public ITurbineTransformParameter {
public:
    ShiftY(double nominal, double upperTolerance, double lowerTolerance,
        QString extraParam1 = {}, QString extraParam2 = {});

    QString reportType() const override;
};
