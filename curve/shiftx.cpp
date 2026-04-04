#include "curve/pch.h"

#include "shiftx.h"

ShiftX::ShiftX(double nominal, double upperTolerance, double lowerTolerance, QString extraParam1, QString extraParam2)
    : ITurbineTransformParameter(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2)
{
    _type = Type::ShiftX;
}

QString ShiftX::reportType() const
{
    return "Offset X";
}
