#include "curve/pch.h"

#include "iturbinetransformparameter.h"
#include "shiftx.h"
#include "shifty.h"
#include "turn.h"

const QString ITurbineTransformParameter::_dimensionNameTemplate = "%1_Best-fit_Data";

QString ITurbineTransformParameter::_commonDimensionName;

void ITurbineTransformParameter::createMeasuredTransforms(const QList<ITurbineTransformParameter*>& transforms,
    const QString& nominalProfileName, const QString& parentName)
{
    _commonDimensionName = _dimensionNameTemplate.arg(nominalProfileName);
    auto labelPoint = Point(0.0, 0.0, 0.0);

    auto dimFigure = new DimFigure(_commonDimensionName, labelPoint, parentName);
    dimFigure->setDimType(DimFigure::DimType::BestFitData);

    for(auto transformParameter : transforms) {
        if(auto shiftX = dynamic_cast<ShiftX*>(transformParameter)) {
            dimFigure->addValue(DimFigure::Value(DimFigure::ValueType::dX, true,
                shiftX->measured(), shiftX->nominal(), shiftX->upperTolerance(), shiftX->lowerTolerance()));
        }
        if(auto shiftY = dynamic_cast<ShiftY*>(transformParameter)) {
            dimFigure->addValue(DimFigure::Value(DimFigure::ValueType::dY, true,
                shiftY->measured(), shiftY->nominal(), shiftY->upperTolerance(), shiftY->lowerTolerance()));
        }
        if(auto turn = dynamic_cast<Turn*>(transformParameter)) {
            dimFigure->addValue(DimFigure::Value(DimFigure::ValueType::Rotation, true,
                turn->measured(), turn->nominal(), turn->upperTolerance(), turn->lowerTolerance()));
        }
    }

    Project::instance().safeInsert(_commonDimensionName, dimFigure);
}

QString ITurbineTransformParameter::commonDimensionName()
{
    return _commonDimensionName;
}

ITurbineTransformParameter::ITurbineTransformParameter(double nominal,
    double upperTolerance, double lowerTolerance, QString extraParam1, QString extraParam2)
    : ITurbineParameter(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2)
{
}

void ITurbineTransformParameter::setMeasured(double measured)
{
    _measured = measured;
}
