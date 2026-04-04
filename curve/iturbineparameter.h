#pragma once

class ITurbineParameter : public QObject {
    Q_OBJECT

public:
    enum class Type {
        MaxWidth = 0,
        MaxWidthX = 1,
        MaxWidthY = 2,
        CenterMassX = 3,
        CenterMassY = 4,
        ChordAngle = 5,
        ChordLength = 6,
        LEWidth = 7,
        TEWidth = 8,
        LERadius = 9,
        TERadius = 10,
        ShiftX = 11,
        ShiftY = 12,
        Turn = 13,
        MaxX = 14,
        MinX = 15,
        MaxY = 16,
        MinY = 17,
        PositionSize = 18,
        DistX = 19,
        DistY = 20,
        FormLEMin = 21,
        FormTEMin = 22,
        FormConvexMin = 23,
        FormConcaveMin = 24,
        FormLEMax = 25,
        FormTEMax = 26,
        FormConvexMax = 27,
        FormConcaveMax = 28,
        LEDeviation = 29,
        TEDeviation = 30,
        ThicknessFromLE = 31,
        FormLEMinMax = 32,
        FormTEMinMax = 33,
        FormConvexMinMax = 34,
        FormConcaveMinMax = 35,
        Default,
    };
    Q_ENUM(Type)

    static QMap<QString, QString> toQMap(const ITurbineParameter* param, int indexFromList);
    static QMap<QString, QString> toQMapFromCRM(const QStringList& parameters);
    static ITurbineParameter* toTurbineParameter(const QString& turbineParameter);
    static QString turbineParamTypeToQString(Type type);
    static Type turbineParamTypeFromQString(const QString& name);

    ITurbineParameter(double nominal, double upperTolerance, double lowerTolerance,
        QString extraParam1 = {}, QString extraParam2 = {});
    virtual ~ITurbineParameter() = default;

    Type type() const;
    double nominal() const;
    double measured() const;
    double upperTolerance() const;
    double lowerTolerance() const;
    QString extraParam1() const;
    QString extraParam2() const;

    virtual QString reportType() const = 0;

protected:
    Type _type = Type::Default;
    double _nominal;
    double _measured;
    double _upperTolerance;
    double _lowerTolerance;
    QString _extraParam1;
    QString _extraParam2;

private:
    static const QString parameterKeyTemplate;
    static const QString parameterValueTemplate;
};
