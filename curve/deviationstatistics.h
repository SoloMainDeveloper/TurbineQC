#pragma once

class DeviationStatistics {
public:
    explicit DeviationStatistics(const QList<CurvePoint>& points);
    ~DeviationStatistics() = default;

    double min() const;
    double max() const;
    double minMax() const;
    double form() const;
    double maxAbs() const;
    double superiorUpperTolerance() const;
    double inferiorLowerTolerance() const;

private:
    QList<double> _deviations;
    QList<double> _upperTolerances;
    QList<double> _lowerTolerances;

    double _min;
    double _max;
};
