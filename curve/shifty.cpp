#include "curve/pch.h"

#include "shifty.h"

ShiftY::ShiftY(double nominal, double upperTolerance, double lowerTolerance, QString extraParam1, QString extraParam2)
    : ITurbineTransformParameter(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2)
{
    _type = Type::ShiftY;
}

QString ShiftY::reportType() const
{
    return "Offset Y";
}
