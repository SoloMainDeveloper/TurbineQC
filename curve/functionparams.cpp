#include "curve/pch.h"
#include "functionparams.h"

Function1Params::Function1Params(int intermediate, double threshold, double minline, bool isClosed,
    bool isExternal, Direction material, bool needSort) {
    _isClosed = isClosed;
    _isExternal = isExternal;
    _material = material;
    _threshold = threshold;
    _intermediate = intermediate;
    _minline = minline;
    _needSort = needSort;
}

QString Function1Params::toQString() {
    QString result = "function=1\nPlane=XY\nclosed=%1\nexternal=%2\nmaterial=%3\nthreshold=%4\nintermediate=%5\nminline=%6\nsort=%7";
    return result.arg(_isClosed ? "N" : "Y").arg(_isExternal ? "N" : "Y")
        .arg(_material == Direction::Left ? "L" : "R").arg(_threshold).arg(_intermediate).arg(_minline).arg(_needSort ? "Y" : "N");
}

Function7Params::Function7Params(bool isClosed, double xLine, double yLine, double zLine, double angle) {
    _isClosed = isClosed;
    _xLine = xLine;
    _yLine = yLine;
    _zLine = zLine;
    _angle = angle;
}

QString Function7Params::toQString() {
    QString result = "function=7\nplane=XY\nclosed=%1\nxline=%2\nyline=%3\nzline=%4\nangle=%5";
    return result.arg(_isClosed ? "N" : "Y").arg(_xLine).arg(_yLine).arg(_zLine).arg(_angle);
}

Function8Params::Function8Params(bool isClosed, double xCircle, double yCircle, double zCircle, int diameter) {
    _isClosed = isClosed;
    _xCircle = xCircle;
    _yCircle = yCircle;
    _zCircle = zCircle;
    _diameter = diameter;
}

QString Function8Params::toQString() {
    QString result = "function=8\nplane=XY\nclosed=%1\nxcircle=%2\nycircle=%3\nzcircle=%4\ndiameter=%5";
    return result.arg(_isClosed ? "N" : "Y").arg(_xCircle).arg(_yCircle).arg(_zCircle).arg(_diameter);
}

Function18Params::Function18Params(int directionLE, int percentLE, int percentTE, int joinedSegments) {
    _leadingEdgeDirection = (directionLE >= 0 && directionLE < 5) ? directionLE : 0; //default is 0
    _percentageLE = (percentLE >= 0 && percentLE <= 100) ? percentLE : 5; //default is 5
    _percentageTE = (percentTE >= 0 && percentTE <= 100) ? percentTE : 5; // default is 5
    _joinedSegmentsCount = joinedSegments > 0 ? joinedSegments : 1; //default is 1
}

QString Function18Params::toQString() {
    QString result = "function=18\nplane=XY\nle_dir=%1\nle_perc=%2\nte_perc=%3\njoined_segments=%4";
    return result.arg(_leadingEdgeDirection).arg(_percentageLE).arg(_percentageTE).arg(_joinedSegmentsCount);
}