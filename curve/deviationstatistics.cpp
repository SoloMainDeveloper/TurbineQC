#include "curve/pch.h"

#include "deviationstatistics.h"

DeviationStatistics::DeviationStatistics(const QList<CurvePoint>& points)
{
    for(CurvePoint point : points) {
        _deviations.append(point.dev);
        _upperTolerances.append(point.ut);
        _lowerTolerances.append(point.lt);
    }

    _min = *std::min_element(_deviations.begin(), _deviations.end());
    _max = *std::max_element(_deviations.begin(), _deviations.end());
}

double DeviationStatistics::min() const
{
    return _min;
}

double DeviationStatistics::max() const
{
    return _max;
}

double DeviationStatistics::minMax() const
{
    double minMax = abs(_max - _min);

    return minMax;
}

double DeviationStatistics::form() const
{
    double form = std::max(abs(_min), abs(_max)) * 2;

    return form;
}

double DeviationStatistics::maxAbs() const
{
    double maxAbs = std::max(abs(_min), abs(_max));

    return maxAbs;
}

double DeviationStatistics::superiorUpperTolerance() const
{
    double maxUpperShift = -1e9;

    for(int i = 0; i < _deviations.size(); ++i) {
        double upperShift = _deviations[i] - _upperTolerances[i];

        if(upperShift > maxUpperShift) {
            maxUpperShift = upperShift;
        }
    }

    double superiorUpperTolerance = std::max(0.0, maxUpperShift);

    return superiorUpperTolerance;
}

double DeviationStatistics::inferiorLowerTolerance() const
{
    double minLowerShift = 1e9;

    for(int i = 0; i < _deviations.size(); ++i) {
        double lowerShift = _deviations[i] - _lowerTolerances[i];

        if(lowerShift < minLowerShift) {
            minLowerShift = lowerShift;
        }
    }

    double inferiorLowerTolerance = std::min(0.0, minLowerShift);

    return inferiorLowerTolerance;
}
