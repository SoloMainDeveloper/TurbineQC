#pragma once

#include <QObject>
#include "algorithms.h"
#include "markupcreator.h"

class TurbineParameter : public QObject {
    Q_OBJECT

public:
    enum class Type {
        MaxWidth = 0, MaxWidthX = 1, MaxWidthY = 2,
        CenterMassX = 3, CenterMassY = 4,
        ChordAngle = 5, ChordLength = 6,
        LEWidth = 7, TEWidth = 8,
        LERadius = 9, TERadius = 10,
        ShiftX = 11, ShiftY = 12,
        Turn = 13,
        MaxX = 14, MinX = 15,
        MaxY = 16, MinY = 17,
        PositionSize = 18,
        DistX = 19, DistY = 20,
        FormLEMin = 21, FormTEMin = 22,
        FormConvexMin = 23, FormConcaveMin = 24,
        FormLEMax = 25, FormTEMax = 26,
        FormConvexMax = 27, FormConcaveMax = 28,
        LEDeviation = 29, TEDeviation = 30,
        ThicknessFromLE = 31,
        FormLEMinMax = 32, FormTEMinMax = 33,
        FormConvexMinMax = 34, FormConcaveMinMax = 35,
        Default,
    };
    Q_ENUM(Type)

    static QMap<QString, QString> toQMap(const TurbineParameter *param, int indexFromList);
    static QMap<QString, QString> toQMapFromCRM(const QStringList &params);
    static TurbineParameter* toTurbineParameter(const QString &turbineParam);
    static QString turbineParamTypeToQString(Type type);
    static Type turbineParamTypeFromQString(const QString &name);

    Type type = Type::Default;
    double nominal;
    double measured;
    double UT;
    double LT;
    QString extraParam1;
    QString extraParam2;

    TurbineParameter(double nominal, double UT, double LT, QString extraParam1 = QString(), QString extraParam2 = QString());
    
    QString createParameterMarkup() const;

    virtual void calculateNominal(const QString &nominalCurve, const Function18Params &params, Project *project) = 0;
    virtual void createMeasured(const QString &nominalCurve, const QString &measuredCurve, const Function18Params &params, Project *project) = 0;

protected:
    QString _dimName;

    virtual QString getMarkupType() const = 0;
};

class MaxDiameter : public TurbineParameter {
public:
    MaxDiameter(double nominal, double UT, double LT, QString extraParam1 = QString(), QString extraParam2 = QString());

    void calculateNominal(const QString &nominalCurve, const Function18Params &params, Project *project) override;
    void createMeasured(const QString &nominalCurve, const QString &measuredCurve, const Function18Params &params, Project *project) override;
    QString getMarkupType() const override;
};

class XMaxDiameter : public TurbineParameter {
public:
    XMaxDiameter(double nominal, double UT, double LT, QString extraParam1 = QString(), QString extraParam2 = QString());

    void calculateNominal(const QString &nominalCurve, const Function18Params &params, Project *project) override;
    void createMeasured(const QString &nominalCurve, const QString &measuredCurve, const Function18Params &params, Project *project) override;
    
private:
    QString getMarkupType() const override;
};

class YMaxDiameter : public TurbineParameter {
public:
    YMaxDiameter(double nominal, double UT, double LT, QString extraParam1 = QString(), QString extraParam2 = QString());

    void calculateNominal(const QString &nominalCurve, const Function18Params &params, Project *project) override;
    void createMeasured(const QString &nominalCurve, const QString &measuredCurve, const Function18Params &params, Project *project) override;
    
private:
    QString getMarkupType() const override;
};

class ChordLength : public TurbineParameter {
public:
    ChordLength(double nominal, double UT, double LT, QString extraParam1 = QString(), QString extraParam2 = QString());

    void calculateNominal(const QString &nominalCurve, const Function18Params &params, Project *project) override;
    void createMeasured(const QString &nominalCurve, const QString &measuredCurve, const Function18Params &params, Project *project) override;
    
private:
    QString getMarkupType() const override;
};

class WidthLE : public TurbineParameter {
public:
    WidthLE(double nominal, double UT, double LT, QString extraParam1 = QString(), QString extraParam2 = QString());

    void calculateNominal(const QString &nominalCurve, const Function18Params &params, Project *project) override;
    void createMeasured(const QString &nominalCurve, const QString &measuredCurve, const Function18Params &params, Project *project) override;
    QString getMarkupType() const override;
};

class WidthTE : public TurbineParameter {
public:
    WidthTE(double nominal, double UT, double LT, QString extraParam1 = QString(), QString extraParam2 = QString());

    void calculateNominal(const QString &nominalCurve, const Function18Params &params, Project *project) override;
    void createMeasured(const QString &nominalCurve, const QString &measuredCurve, const Function18Params &params, Project *project) override;
    
private:
    QString getMarkupType() const override;
};

class RadiusLE : public TurbineParameter {
public:
    RadiusLE(double nominal, double UT, double LT, QString extraParam1 = QString(), QString extraParam2 = QString());

    void calculateNominal(const QString &nominalCurve, const Function18Params &params, Project *project) override;
    void createMeasured(const QString &nominalCurve, const QString &measuredCurve, const Function18Params &params, Project *project) override;
    
private:
    QString getMarkupType() const override;
};

class RadiusTE : public TurbineParameter {
public:
    RadiusTE(double nominal, double UT, double LT, QString extraParam1 = QString(), QString extraParam2 = QString());

    void calculateNominal(const QString &nominalCurve, const Function18Params &params, Project *project) override;
    void createMeasured(const QString &nominalCurve, const QString &measuredCurve, const Function18Params &params, Project *project) override;
    
private:
    QString getMarkupType() const override;
};

class MinX : public TurbineParameter {
public:
    MinX(double nominal, double UT, double LT, QString extraParam1 = QString(), QString extraParam2 = QString());

    void calculateNominal(const QString &nominalCurve, const Function18Params &params, Project *project) override;
    void createMeasured(const QString &nominalCurve, const QString &measuredCurve, const Function18Params &params, Project *project) override;
    
private:
    QString getMarkupType() const override;
};
