#include "curve/pch.h"

#include "chordlength.h"
#include "iturbineparameter.h"
#include "leadingedgeradius.h"
#include "leadingedgewidth.h"
#include "maxdiameter.h"
#include "minx.h"
#include "shiftx.h"
#include "shifty.h"
#include "trailingedgeradius.h"
#include "trailingedgewidth.h"
#include "turn.h"
#include "xmaxdiameter.h"
#include "ymaxdiameter.h"

const QString ITurbineParameter::parameterKeyTemplate = "Param %1 %2";

const QString ITurbineParameter::parameterValueTemplate = "Type:%1,Nom:%2,UT:%3,LT:%4,ex1:%5,ex2:%6";

QMap<QString, QString> ITurbineParameter::toQMap(const ITurbineParameter* parameter, int indexFromList)
{
    auto typeIndex = static_cast<int>(parameter->type());
    QString key = parameterKeyTemplate
                      .arg(QString::number(typeIndex))
                      .arg(QString::number(indexFromList));
    QString value = parameterValueTemplate
                        .arg(turbineParamTypeToQString(parameter->type()))
                        .arg(QString::number(parameter->nominal()))
                        .arg(QString::number(parameter->upperTolerance()))
                        .arg(QString::number(parameter->lowerTolerance()))
                        .arg(parameter->extraParam1())
                        .arg(parameter->extraParam2());

    QMap<QString, QString> result = { { key, value } };

    return result;
}

QMap<QString, QString> ITurbineParameter::toQMapFromCRM(const QStringList& parameters)
{
    if(parameters[2] == "0") {
        return QMap<QString, QString>();
    }

    QMap<QString, QString> paramsOfSameType;

    auto index = parameters[1].mid(3).toInt();
    auto type = static_cast<ITurbineParameter::Type>(index);

    QStringList nominals = parameters[3].split(";");
    QStringList extraParams1 = parameters[6].split(";");

    for(auto j = 0; j < nominals.size(); j++) {
        QString key = parameterKeyTemplate
                          .arg(QString::number(index))
                          .arg(QString::number(j));
        QString value = parameterValueTemplate
                            .arg(ITurbineParameter::turbineParamTypeToQString(type))
                            .arg(nominals[j])
                            .arg(parameters[4])
                            .arg(parameters[5])
                            .arg(extraParams1[j])
                            .arg(parameters[7]);

        paramsOfSameType.insert(key, value);
    }

    return paramsOfSameType;
}

ITurbineParameter* ITurbineParameter::toTurbineParameter(const QString& turbineParameter)
{
    QMap<QString, QString> parameterPairs;
    QStringList properties = turbineParameter.split(",");

    for(auto& property : properties) {
        QStringList pair = property.split(":");
        parameterPairs.insert(pair[0], pair[1]);
    }

    Type parameterType = turbineParamTypeFromQString(parameterPairs["Type"]);

    auto nominal = parameterPairs["Nom"].toDouble();
    auto upperTolerance = parameterPairs["UT"].toDouble();
    auto lowerTolerance = parameterPairs["LT"].toDouble();
    QString extraParam1 = parameterPairs["ex1"];
    QString extraParam2 = parameterPairs["ex2"];

    switch(parameterType) {
        case Type::MaxWidth:
            return new MaxDiameter(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2);
        case Type::MaxWidthX:
            return new XMaxDiameter(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2);
        case Type::MaxWidthY:
            return new YMaxDiameter(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2);
        case Type::ChordLength:
            return new ChordLength(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2);
        case Type::LEWidth:
            return new LeadingEdgeWidth(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2);
        case Type::TEWidth:
            return new TrailingEdgeWidth(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2);
        case Type::LERadius:
            return new LeadingEdgeRadius(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2);
        case Type::TERadius:
            return new TrailingEdgeRadius(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2);
        case Type::ShiftX:
            return new ShiftX(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2);
        case Type::ShiftY:
            return new ShiftY(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2);
        case Type::Turn:
            return new Turn(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2);
        case Type::MinX:
            return new MinX(nominal, upperTolerance, lowerTolerance, extraParam1, extraParam2);
        default:
            return nullptr;
    }
}

QString ITurbineParameter::turbineParamTypeToQString(Type type)
{
    return QMetaEnum::fromType<Type>().valueToKey(static_cast<int>(type));
}

ITurbineParameter::Type ITurbineParameter::turbineParamTypeFromQString(const QString& name)
{
    return static_cast<Type>(QMetaEnum::fromType<Type>().keyToValue(name.toLatin1()));
}

ITurbineParameter::ITurbineParameter(double nominal,
    double upperTolerance, double lowerTolerance,
    QString extraParam1, QString extraParam2)
{
    _nominal = nominal;
    _upperTolerance = upperTolerance;
    _lowerTolerance = lowerTolerance;
    _extraParam1 = extraParam1;
    _extraParam2 = extraParam2;

    _measured = 0.0;
}

ITurbineParameter::Type ITurbineParameter::type() const
{
    return _type;
}

double ITurbineParameter::nominal() const
{
    return _nominal;
}

double ITurbineParameter::measured() const
{
    return _measured;
}

double ITurbineParameter::upperTolerance() const
{
    return _upperTolerance;
}

double ITurbineParameter::lowerTolerance() const
{
    return _lowerTolerance;
}

QString ITurbineParameter::extraParam1() const
{
    return _extraParam1;
}

QString ITurbineParameter::extraParam2() const
{
    return _extraParam2;
}
