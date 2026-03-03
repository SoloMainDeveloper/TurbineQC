#include "curve/pch.h"
#include "resultcompareflr.h"

ResultCompareFLR::ResultCompareFLR(int paramsTotal, int dimsFailsTotal, double dimsFailsDevitationSum, int pointsFailsTotal,
    double pointsFailsDevitationSum, double maxDeviationFail) {
        _paramsTotal = paramsTotal;
        _dimsFailsTotal = dimsFailsTotal;
        _dimsFailsDevitationSum = dimsFailsDevitationSum;
        _pointsFailsTotal = pointsFailsTotal;
        _pointsFailsDevitationSum = pointsFailsDevitationSum;
        _maxDeviationFail = maxDeviationFail;
        _failsTotal = dimsFailsTotal + pointsFailsTotal;
}

ResultCompareFLR::~ResultCompareFLR() {
}

int ResultCompareFLR::paramsTotal() {
    return _paramsTotal;
}

int ResultCompareFLR::failsTotal() {
    return _failsTotal;
}

int ResultCompareFLR::dimsFailsTotal() {
    return _dimsFailsTotal;
}

int ResultCompareFLR::pointsFailsTotal() {
    return _pointsFailsTotal;
}

double ResultCompareFLR::dimsFailsDevitationSum() {
    return _dimsFailsDevitationSum;
}

double ResultCompareFLR::pointsFailsDevitationSum() {
    return _pointsFailsDevitationSum;
}

double ResultCompareFLR::getAverageDimDevFail() {
    return _dimsFailsTotal == 0 ? 0 : _dimsFailsDevitationSum / _dimsFailsTotal;
}

double ResultCompareFLR::getAveragePointDevFail() {
    return _pointsFailsTotal == 0 ? 0 : _pointsFailsDevitationSum / _pointsFailsTotal;
}

double ResultCompareFLR::maxDeviationFail() {
    return _maxDeviationFail;
}

ResultCompare2Params::ResultCompare2Params(QString name1, QString name2, double value1, double value2) {
    _name1 = name1;
    _name2 = name2;
    _value1 = value1;
    _value2 = value2;
    _diff = abs(value1 - value2);
    _isComparable = name1 == name2;
}

bool ResultCompare2Params::isDiffBiggerThanPrecision(double precision) {
    return _diff > precision;
}

QString ResultCompare2Params::toQString(double precision) {
    auto result = _name1 + "\t\t" + _name2 + "\t\t" + QString::number(_diff);
    result += isDiffBiggerThanPrecision(precision) ? "\t\tFAIL" : "";
    return result;
}

QString ResultCompare2Params::name1() {
    return _name1;
}

QString ResultCompare2Params::name2() {
    return _name2;
}

double ResultCompare2Params::value1() {
    return _value1;
}

double ResultCompare2Params::value2() {
    return _value2;
}

double ResultCompare2Params::diff() {
    return _diff;
}

bool ResultCompare2Params::isComparable() {
    return _isComparable;
}
