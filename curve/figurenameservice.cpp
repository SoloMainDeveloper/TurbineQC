#include "curve/pch.h"

#include "figurenameservice.h"
#include "settings.h"

FigureNameService::FigureProxy::FigureProxy(const char* key, const char* def)
    : settingsKey(QString("figures/") + key + "_name"), defaultValue(def)
{
}

QString FigureNameService::FigureProxy::getPrefix() const
{
    if(Settings::instance().hasValue(settingsKey)) {
        return Settings::instance().value<QString>(settingsKey);
    }
    return defaultValue;
}

QString FigureNameService::FigureProxy::operator()(const QString& figureName) const
{
    return figureName + getPrefix();
}

FigureNameService::FigureProxy::operator QString() const
{
    return getPrefix();
}

const FigureNameService::FigureProxy FigureNameService::MaxDiameter { "max_diameter", " Maximum Diameter" };
const FigureNameService::FigureProxy FigureNameService::MaxDiameterCircle { "max_diameter_circle", "_CMaxDia" };
const FigureNameService::FigureProxy FigureNameService::XMaxDiameter { "x_max_diameter", "_YD" };
const FigureNameService::FigureProxy FigureNameService::YMaxDiameter { "y_max_diameter", "_XD" };
const FigureNameService::FigureProxy FigureNameService::LeadingEdgeThickness { "leading_edge_thickness", "_LE_Thck" };
const FigureNameService::FigureProxy FigureNameService::LeadingEdgeThicknessPoint { "leading_edge_thickness_point", "_LE_Width_P" };
const FigureNameService::FigureProxy FigureNameService::LeadingEdgeRadius { "leading_edge_radius", "_LE_Radius" };
const FigureNameService::FigureProxy FigureNameService::LeadingEdgeRadiusCircle { "leading_edge_radius_circle", "_LE_CRadius" };
const FigureNameService::FigureProxy FigureNameService::TrailingEdgeThickness { "trailing_edge_thickness", "_TE_Thck" };
const FigureNameService::FigureProxy FigureNameService::TrailingEdgeThicknessPoint { "trailing_edge_thickness_point", "_TE_Width_P" };
const FigureNameService::FigureProxy FigureNameService::TrailingEdgeRadius { "trailing_edge_radius", "_TE_Radius" };
const FigureNameService::FigureProxy FigureNameService::TrailingEdgeRadiusCircle { "trailing_edge_radius_circle", "_TE_CRadius" };
const FigureNameService::FigureProxy FigureNameService::MinX { "min_x", " MinX" };
const FigureNameService::FigureProxy FigureNameService::MinXPoint { "min_x_point", "_MinX_P" };
