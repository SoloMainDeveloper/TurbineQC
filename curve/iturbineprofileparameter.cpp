#include "curve/pch.h"

#include "iturbineprofileparameter.h"

ITurbineProfileParameter::ITurbineProfileParameter(double nominal, double upperTolerance,
    double lowerTolerance, QString extraParam1, QString extraParam2)
    : ITurbineParameter(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2)
{
}
