#include "curve/pch.h"

#include "turn.h"

Turn::Turn(double nominal, double upperTolerance, double lowerTolerance, QString extraParam1, QString extraParam2)
    : ITurbineTransformParameter(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2)
{
    _type = Type::Turn;
}

QString Turn::reportType() const
{
    return "Rotation";
}
