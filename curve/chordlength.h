#pragma once

#include "iturbineprofileparameter.h"

class ChordLength : public ITurbineProfileParameter {
public:
    ChordLength(double nominal, double upperTolerance, double lowerTolerance,
        QString extraParam1 = {}, QString extraParam2 = {});

    void calculateNominal(const QString& nominalProfileName, const Function18Params& params) override;
    void createMeasured(const QString& nominalProfileName, const QString& measuredProfileName, const Function18Params& params) override;

    QString reportType() const override;
    QString dimensionName() const override;

private:
    const QString _parameter1NameTemplate = "%1_Chord_P1";
    const QString _parameter2NameTemplate = "%1_Chord_P2";
    const QString _dimensionNameTemplate = "%1_Chord";
};
