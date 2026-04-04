#include "curve/pch.h"

#include "changecurveappearancecommand.h"

void ChangeCurveAppearanceCommand::run()
{
}

CommandType ChangeCurveAppearanceCommand::getType() const
{
    return CommandType::ChangeCurveAppearance;
}

QMap<QString, QVariant> ChangeCurveAppearanceCommand::getParameters() const
{
    return QMap<QString, QVariant>();
}

void ChangeCurveAppearanceCommand::setParameters(QMap<QString, QVariant> params)
{
}

QString ChangeCurveAppearanceCommand::getName() const
{
    return tr("Change Curve Appearance");
}

QString ChangeCurveAppearanceCommand::getDescription() const
{
    return "ShowOptions ABCDEF XYZIJK";
}