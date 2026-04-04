#pragma once

#include "iturbinetransformparameter.h"

class Turn : public ITurbineTransformParameter {
public:
    Turn(double nominal, double upperTolerance, double lowerTolerance,
        QString extraParam1 = {}, QString extraParam2 = {});

    QString reportType() const override;
};
