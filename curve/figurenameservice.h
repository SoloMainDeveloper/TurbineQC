#pragma once

class FigureNameService {
private: // its needed to be private before public here
    class FigureProxy {
        QString settingsKey;
        QString defaultValue;

    public:
        FigureProxy(const char* key, const char* def);

        QString getPrefix() const;
        QString operator()(const QString& figureName) const;
        operator QString() const;
    };

public:
    FigureNameService() = default;
    ~FigureNameService() = default;

    static const class FigureProxy MaxDiameter;
    static const class FigureProxy MaxDiameterCircle;
    static const class FigureProxy XMaxDiameter;
    static const class FigureProxy YMaxDiameter;
    static const class FigureProxy LeadingEdgeThickness;
    static const class FigureProxy LeadingEdgeThicknessPoint;
    static const class FigureProxy LeadingEdgeRadius;
    static const class FigureProxy LeadingEdgeRadiusCircle;
    static const class FigureProxy TrailingEdgeThickness;
    static const class FigureProxy TrailingEdgeThicknessPoint;
    static const class FigureProxy TrailingEdgeRadius;
    static const class FigureProxy TrailingEdgeRadiusCircle;
    static const class FigureProxy MinX;
    static const class FigureProxy MinXPoint;
};
