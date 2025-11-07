#include "curve/pch.h"
#include "functionparams.h"

Function1Params::Function1Params(int intermediate, double threshold, double minline, bool isClosed,
    bool isExternal, Direction material, bool needSort) {
    _isClosed = isClosed;
    _isExternal = isExternal;
    _needSort = needSort;
    _intermediate = intermediate;
    _threshold = threshold;
    _minline = minline;
    _material = material;
}

QString Function1Params::toQString() {
    QString result = "function=1\nplane=XY\nclosed=%1\nexternal=%2\nmaterial=%3\nthreshold=%4\nintermediate=%5\nminline=%6\nsort=%7";
    return result.arg(_isClosed ? "Y" : "N").arg(_isExternal ? "Y" : "N")
        .arg(_material == Direction::Left ? "L" : "R").arg(_threshold).arg(_intermediate).arg(_minline).arg(_needSort ? "Y" : "N");
}

Function2Params::Function2Params(bool needInvertSequence, bool needInvertDirection) {
    _needInvertSequence = needInvertSequence;
    _needInvertDirection = needInvertDirection;
}

QString Function2Params::toQString() {
    QString result = "function=2\nplane=XY\ninvertsequence=%1\ninvertdirection=%2";
    return result.arg(_needInvertSequence ? "Y" : "N").arg(_needInvertDirection ? "Y" : "N");
}

Function3Params::Function3Params(double radiusCorrection, bool isClosed,
    bool isExternal, Direction material, bool needSort) {
    _isClosed = isClosed;
    _isExternal = isExternal;
    _needSort = needSort;
    _radiusCorrection = radiusCorrection;
    _material = material;
}

QString Function3Params::toQString() {
    QString result = "function=3\nplane=XY\nclosed=%1\nradius_corr=%2\nexternal=%3\nmaterial=%4\nsort=%5";
    return result.arg(_isClosed ? "Y" : "N").arg(_radiusCorrection).arg(_isExternal ? "Y" : "N")
        .arg(_material == Direction::Left ? "L" : "R").arg(_needSort ? "Y" : "N");
}

Function4Params::Function4Params(float nominalTolerance, int evaluationPlace, int evaluationDirection,
    bool isClosed, bool isExternal, Direction material, bool needSort) {
    _isClosed = isClosed;
    _isExternal = isExternal;
    _needSort = needSort;
    _evaluationPlace = evaluationPlace;
    _evaluationDirection = evaluationDirection;
    _nominalTolerance = nominalTolerance;
    _material = material;
}

QString Function4Params::toQString() {
    QString result = "function=4\nplane=XY\nclosed=%1\nexternal=%2\nmaterial=%3\nsort=%4\nnomtol=%5\nevaluationplace=%6\nevaluationdirection=%7";
    return result.arg(_isClosed ? "Y" : "N").arg(_isExternal ? "Y" : "N").arg(_material == Direction::Left ? "L" : "R")
        .arg(_needSort ? "Y" : "N").arg(_nominalTolerance).arg(_evaluationPlace).arg(_evaluationDirection);
}

Function5Params::Function5Params(double scale, double magnification, bool isClosed) {
    _isClosed = isClosed;
    _scale = scale;
    _magnification = magnification;
}

QString Function5Params::toQString() {
    QString result = "function=5\nplane=XY\nclosed=%1\nscale=%2\nmagnification=%3";
    return result.arg(_isClosed ? "Y" : "N").arg(_scale).arg(_magnification);
}

Function6Params::Function6Params(int evaluationPlace, int EvaluationDirection, Algorithm method, bool needXShift, 
    bool needYShif, bool needRotation, bool needMinimize, bool isClosed, bool isExternal, Direction material) {
    _isClosed = isClosed;
    _isExternal = isExternal;
    _needXShift = needXShift;
    _needYShift = needYShif;
    _needRotation = needRotation;
    _needMinimize = needMinimize;
    _evaluationPlace = evaluationPlace;
    _evaluationDirection = EvaluationDirection;
    _material = material;
    _method = method;
}

QString Function6Params::toQString() {
    QString result = "function=5\nplane=XY\nclosed=%1\nexternal=%2\nmaterial=%3\nxshift=%4\n\
        yshift=%5\nrotation=%6\nminimize=%7\nevaluationplace=%8\nevaluationdirection=%9\nmethod=%10";
    return result.arg(_isClosed ? "Y" : "N").arg(_isExternal ? "Y" : "N").arg(_material == Direction::Left ? "L" : "R")
        .arg(_needXShift ? "Y" : "N").arg(_needYShift ? "Y" : "N").arg(_needRotation ? "Y" : "N")
        .arg(_needMinimize ? "Y" : "N").arg(_evaluationPlace).arg(_evaluationDirection).arg(_method == Algorithm::Curve ? "Curve" : "Points");
}

Function7Params::Function7Params(double angle, double xLine, double yLine, double zLine, bool isClosed) {
    _isClosed = isClosed;
    _xLine = xLine;
    _yLine = yLine;
    _zLine = zLine;
    _angle = angle;
}

QString Function7Params::toQString() {
    QString result = "function=7\nplane=XY\nclosed=%1\nxline=%2\nyline=%3\nzline=%4\nangle=%5";
    return result.arg(_isClosed ? "Y" : "N").arg(_xLine).arg(_yLine).arg(_zLine).arg(_angle);
}

Function8Params::Function8Params(int diameter, double xCircle, double yCircle, double zCircle, bool isClosed) {
    _isClosed = isClosed;
    _xCircle = xCircle;
    _yCircle = yCircle;
    _zCircle = zCircle;
    _diameter = diameter;
}

QString Function8Params::toQString() {
    QString result = "function=8\nplane=XY\nclosed=%1\nxcircle=%2\nycircle=%3\nzcircle=%4\ndiameter=%5";
    return result.arg(_isClosed ? "Y" : "N").arg(_xCircle).arg(_yCircle).arg(_zCircle).arg(_diameter);
}

Function9Params::Function9Params(bool isClosed) {
    _isClosed = isClosed;
}

QString Function9Params::toQString() {
    QString result = "function=9\nplane=XY\nclosed=%1";
    return result.arg(_isClosed ? "Y" : "N");
}

Function10Params::Function10Params(double xStart, double yStart, double zStart, double diameter, double angle, int centerType) {
    _xStart = xStart;
    _yStart = yStart;
    _zStart = zStart;
    _diameter = diameter;
    _angle = angle;
    _centerType = centerType;
}

QString Function10Params::toQString() {
    QString result = "function=10\nplane=XY\nxstart=%1\nystart=%2\nzstart=%3\ndiameter=%4\nangle=%5\ncentertype=%6";
    return result.arg(_xStart).arg(_yStart).arg(_zStart).arg(_diameter).arg(_angle).arg(_centerType);
}

Function11Params::Function11Params(Direction material) {
    _material = material;
}

QString Function11Params::toQString() {
    QString result = "function=11\nplane=XY\nmaterial=%1";
    return result.arg(_material == Direction::Left ? "L" : "R");
}

Function12Params::Function12Params(QString algorithm, bool isExternal) {
    _algorithm = algorithm;
    _isExternal = isExternal;
}

QString Function12Params::toQString() {
    QString result = "function=12\nplane=XY\nalgorithm=%1\nexternal=%2";
    return result.arg(_algorithm).arg(_isExternal ? "Y" : "N");
}

Function13Params::Function13Params(Filter filter, FilterType filterType, double filterFactor, bool isClosed) {
    _isClosed = isClosed;
    _filterFactor = filterFactor;
    _filter = filter;
    _filterType = filterType;
}

QString Function13Params::toQString() {
    QString result = "function=13\nclosed=%1\nplane=XY\nfilter=%2\nfilter_type=%3\nfilter_factor=%4";
    QString filterTypeValue = "";
    switch(_filterType) {
        case FilterType::LowPass:
            filterTypeValue = "low_pass";
            break;
        case FilterType::HighPass:
            filterTypeValue = "high_pass";
            break;
        case FilterType::Band:
            filterTypeValue = "band";
            break;
    }
    return result.arg(_isClosed ? "Y" : "N").arg(_filter == Filter::Polar ? "polar" : "linear").arg(filterTypeValue).arg(_filterFactor);
}

Function14Params::Function14Params(bool isClosed) {
    _isClosed = isClosed;
}

QString Function14Params::toQString() {
    QString result = "function=14\nclosed=%1\nplane=XY";
    return result.arg(_isClosed ? "Y" : "N");
}

Function15Params::Function15Params(bool isClosed) {
    _isClosed = isClosed;
}

QString Function15Params::toQString() {
    QString result = "function=15\nclosed=%1\nplane=XY";
    return result.arg(_isClosed ? "Y" : "N");
}

Function16Params::Function16Params(double maxFormError, double maxCircleRadius, int algorithm) {
    _algorithm = algorithm;
    _maxFormError = maxFormError;
    _maxCircleRadius = maxCircleRadius;
}

QString Function16Params::toQString() {
    QString result = "function=16\nalgorithm=%1\nmaxformerror=%2\nmaxcircleradius=%3";
    return result.arg(_algorithm).arg(_maxFormError).arg(_maxCircleRadius);
}

Function17Params::Function17Params(double diameter, Sorter sortZ) {
    _diameter = diameter;
    _sortZ = sortZ;
}

QString Function17Params::toQString() {
    QString result = "function=17\ndiameter=%1\nsortz=%2";
    QString sortZValue = "";
    switch(_sortZ) {
        case Sorter::NotSort:
            sortZValue = "N";
                break;
        case Sorter::OnIncrease:
            sortZValue = "I";
            break;
        case Sorter::OnDecrease:
            sortZValue = "D";
            break;
    }
    return result.arg(_diameter).arg(sortZValue);
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