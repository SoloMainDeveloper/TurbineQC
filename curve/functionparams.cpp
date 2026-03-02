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

Function1Params::Function1Params(const QMap<QString, QString>* data) : Function1Params(data->value("intermediate").toDouble(), data->value("threshold").toDouble(),
    data->value("minline").toDouble(), data->value("closed") == "Y", data->value("external") == "Y",
    data->value("material") == "L" ? FunctionParams::Direction::Left : FunctionParams::Direction::Right, data->value("needSort") == "Y") { }

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

Function3Params::Function3Params(const QMap<QString, QString>* data) : Function3Params(data->value("radius_corr").toDouble(),
    data->value("closed") == "Y", data->value("external") == "Y", data->value("material") == "L" ? FunctionParams::Direction::Left :
    FunctionParams::Direction::Right, data->value("sort") == "Y") { }

QString Function3Params::toQString() {
    QString result = "function=3\nplane=XY\nclosed=%1\nexternal=%2\nmaterial=%3\nradius_corr=%4\nsort=%5\nalgorithm=envelope";
    return result.arg(_isClosed ? "Y" : "N").arg(_isExternal ? "Y" : "N")
        .arg(_material == Direction::Left ? "L" : "R").arg(_radiusCorrection).arg(_needSort ? "Y" : "N");
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

Function4Params::Function4Params(const QMap<QString, QString>* data) : Function4Params(
    data->value("nomtol").toDouble(), data->value("evaluationplace").toInt(), data->value("evaluationdirection").toInt(), data->value("closed") == "Y",
    data->value("external") == "Y", data->value("material") == "L" ? FunctionParams::Direction::Left : FunctionParams::Direction::Right,
    data->value("sort") == "Y") {}

QString Function4Params::toQString() {
    QString result = "function=4\nplane=XY\nclosed=%1\nexternal=%2\nmaterial=%3\nsort=%4\nnomtol=%5\nevaluationplace=%6\nevaluationdirection=%7";
    return result.arg(_isClosed ? "Y" : "N").arg(_isExternal ? "Y" : "N").arg(_material == Direction::Left ? "L" : "R")
        .arg(_needSort ? "Y" : "N").arg(QString::number(_nominalTolerance, 'f', 9)).arg(_evaluationPlace).arg(_evaluationDirection);
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

Function6Params::Function6Params(bool needMinimize, Algorithm method, bool isClosed, bool needXShift, bool needYShif, 
    bool needRotation, bool needHConstraint, double xShiftFrom, double xShiftTo, bool needVConstraint, double yShiftFrom, 
    double yShiftTo, bool needRConstraint, double rotationFrom, double rotationTo) {
    _xShiftFrom = xShiftFrom;
    _xShiftTo = xShiftTo;
    _yShiftFrom = yShiftFrom;
    _yShiftTo = yShiftTo;
    _rotationFrom = rotationFrom;
    _rotationTo = rotationTo;
    _isClosed = isClosed;
    //_isExternal = isExternal;
    _needXShift = needXShift;
    _needYShift = needYShif;
    _needRotation = needRotation;
    _needVConstraint = needVConstraint;
    _needHConstraint = needHConstraint;
    _needRConstraint = needRConstraint;
    _needMinimize = needMinimize;
    //_evaluationPlace = evaluationPlace;
    //_evaluationDirection = EvaluationDirection;
    //_material = material;
    _method = method;
}

Function6Params::Function6Params(const QMap<QString, QString>* data) : Function6Params(data->value("minimize") == "Y",
    data->value("method") == "curve" ? Function6Params::Algorithm::Curve : Function6Params::Algorithm::Point, data->value("closed") == "Y",
    data->value("xshift") == "Y", data->value("yshift") == "Y", data->value("rotation") == "Y", data->value("needHconstraint") == "Y",
    data->value("xshiftfrom").toDouble(), data->value("xshiftto").toDouble(), data->value("needVconstraint") == "Y", data->value("yshiftfrom").toDouble(),
    data->value("yshiftto").toDouble(), data->value("needRconstraint") == "Y", data->value("rotationfrom").toDouble(), data->value("rotationto").toDouble()) { }

QString Function6Params::toQString() {
    /*QString result = "function=6\nplane=XY\nclosed=%1\nexternal=%2\nmaterial=%3\nxshift=%4\n\
        yshift=%5\nrotation=%6\nminimize=%7\nevaluationplace=%8\nevaluationdirection=%9\nmethod=%10";*/
    QString result = "function=6\nplane=XY\nminimize=%1\nmethod=%2\nclosed=%3\nxshift=%4\nyshift=%5\nrotation=%6";

    if(_needHConstraint) {
        result += QString("\nxshiftfrom=%7\nxshiftto=%8").arg(QString::number(_xShiftFrom)).arg(QString::number(_xShiftTo));
    }
    if(_needVConstraint) {
        result += QString("\nyshiftfrom=%7\nyshiftto=%8").arg(QString::number(_yShiftFrom)).arg(QString::number(_yShiftTo));
    } 
    if(_needRConstraint) {
        result += QString("\nrotationfrom=%7\nrotationto=%8").arg(QString::number(_rotationFrom)).arg(QString::number(_rotationTo));
    }
    result += "\nunits=inch";

    return result.arg(_needMinimize ? "Y" : "N").arg(_method == Algorithm::Curve ? "curve" : "point").arg(_isClosed ? "Y" : "N")
        .arg(_needXShift ? "Y" : "N").arg(_needYShift ? "Y" : "N").arg(_needRotation ? "Y" : "N");
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
    return result.arg(_isClosed ? "Y" : "N").arg(QString::number(_xLine, 'f', 9)).arg(QString::number(_yLine, 'f', 9))
        .arg(QString::number(_zLine, 'f', 9)).arg(QString::number(_angle, 'f', 9));
}

Function8Params::Function8Params(double diameter, double xCircle, double yCircle, double zCircle, bool isClosed) {
    _isClosed = isClosed;
    _xCircle = xCircle;
    _yCircle = yCircle;
    _zCircle = zCircle;
    _diameter = diameter;
}

QString Function8Params::toQString() {
    QString result = "function=8\nplane=XY\nclosed=%1\nxcircle=%2\nycircle=%3\nzcircle=%4\ndiameter=%5";
    return result.arg(_isClosed ? "Y" : "N").arg(QString::number(_xCircle, 'f', 9)).arg(QString::number(_yCircle, 'f', 9))
        .arg(QString::number(_zCircle, 'f', 9)).arg(_diameter);
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
    QString result = "function=13\nplane=XY\nclosed=%1\nfilter=%2\nfilter_type=%3\nfilter_factor=%4";
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

Function14Params::Function14Params(bool isClosed1, bool isClosed2, int mode) {
    _isClosed1 = isClosed1;
    _isClosed2 = isClosed2;
    _mode = mode;
}

QString Function14Params::toQString() {
    QString result = "function=14\nplane=XY\nclosed1=%1\nclosed2=%2\nmode=1";
    return result.arg(_isClosed1 ? "Y" : "N").arg(_isClosed2 ? "Y" : "N").arg(_mode);
}

Function15Params::Function15Params(bool isClosed) {
    _isClosed = isClosed;
}

QString Function15Params::toQString() {
    QString result = "function=15\nplane=XY\nclosed=%1";
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

Function18Params::Function18Params(int directionLE, double percentLE, double percentTE, int joinedSegments) {
    _leadingEdgeDirection = (directionLE >= 0 && directionLE < 5) ? directionLE : 0; //default is 0
    _percentageLE = (percentLE >= 0 && percentLE <= 100) ? percentLE : 5; //default is 5
    _percentageTE = (percentTE >= 0 && percentTE <= 100) ? percentTE : 5; // default is 5
    _joinedSegmentsCount = joinedSegments > 0 ? joinedSegments : 1; //default is 1
}

Function18Params::Function18Params(const QMap<QString, QString>* data) : Function18Params(data->value("directionLE").toInt(), data->value("percentLE").toInt(),
    data->value("percentTE").toInt(), data->value("joinedSegments").toInt()) { }

QString Function18Params::toQString() {
    QString result = "function=18\nplane=XY\nle_dir=%1\nle_perc=%2\nte_perc=%3\njoined_segments=%4";
    return result.arg(_leadingEdgeDirection).arg(_percentageLE).arg(_percentageTE).arg(_joinedSegmentsCount);
}

Function19Params::Function19Params(bool isClosed, bool isExternal, Direction material, QString mode, int value) {
    _isClosed = isClosed;
    _isExternal = isExternal;
    _value = value;
    _material = material;
    _mode = mode;
}

Function19Params::Function19Params(const QMap<QString, QString>* data) : Function19Params(data->value("closed") == "Y", data->value("external") == "Y",
    data->value("material") == "L" ? FunctionParams::Direction::Left : FunctionParams::Direction::Right, data->value("mode"), data->value("value").toInt()) {}

QString Function19Params::toQString() {
    QString result = "function=19\nplane=XY\nclosed=%1\nexternal=%2\nmaterial=%3\nmode=%4\nvalue=%5";
    return result.arg(_isClosed ? "Y" : "N").arg(_isExternal ? "Y" : "N").arg(_material == Direction::Left ? "L" : "R").arg(_mode).arg(_value);
}

Function21Params::Function21Params(int limInterpMethod, bool needXShift, bool needYShift, bool needRotation, bool isClosed) {
    _limInterpMethod = limInterpMethod;
    _needXShift = needXShift;
    _needYShift = needYShift;
    _needRotation = needRotation;
    _isClosed = isClosed;
}

QString Function21Params::toQString() {
    QString result = "function=21\nplane=XY\nclosed=%1\nlim_interp_method=%2\nxshift=%3\nyshift=%4\nrotation=%5\nunits=inch";
    return result.arg(_isClosed ? "Y" : "N").arg(_limInterpMethod).arg(_needXShift ? "Y" : "N").arg(_needYShift ? "Y" : "N").arg(_needRotation ? "Y" : "N");
}

Function31Params::Function31Params(bool isLEStretch, bool isTEStretch, int leadingEdgeDirection) {
    _leadingEdgeDirection = leadingEdgeDirection;
    _isLEStretch = isLEStretch;
    _isTEStretch = isTEStretch;
}

QString Function31Params::toQString() {
    QString result = "function=31\nplane=XY\nle_dir=%1\nle_stretch=%2\nte_stretch=%3";
    return result.arg(_leadingEdgeDirection).arg(_isLEStretch ? "Y" : "N").arg(_isTEStretch ? "Y" : "N");
}

Function42Params::Function42Params(bool isClosed, bool needVectors, bool needColumnA, bool needColumnR, bool needColumnD) {
    _isClosed = isClosed;
    _needVectors = needVectors;
    _needColumnA = needColumnA;
    _needColumnR = needColumnR;
    _needColumnD = needColumnD;
}

QString Function42Params::toQString() {
    QString result = "function=42\nClosed=%1\nVectors=%2\nColumn_A=%3\nColumn_R=%4\nColumn_D=%5";
    return result.arg(_isClosed ? "Y" : "N").arg(_needVectors ? "Y" : "N")
        .arg(_needColumnA ? "Y" : "N").arg(_needColumnR ? "Y" : "N").arg(_needColumnD ? "Y" : "N");
}

QMap<QString, QString> FunctionParams::toQMap() {
    auto parsedText = toQString().split('\n');
    auto params = QMap<QString, QString>();
    for(auto i = 0; i < parsedText.size(); i++) {
        auto pair = parsedText[i].split('=');
        params.insert(pair[0], pair[1]);
    }
    return params;
}
