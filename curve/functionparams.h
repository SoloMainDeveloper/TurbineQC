#pragma once

class FunctionParams {
public:
    enum class Direction {
        Right, Left
    };

    virtual QString toQString() = 0;
    QMap<QString, QString> toQMap();
};

class Function1Params : public FunctionParams {
public:
    Function1Params(int intermediate, double threshold = 0, double minline = 0, bool isClosed = false,
        bool isExternal = true, Direction material = Direction::Left, bool needSort = false);
    QString toQString() override;

private:
    bool _isClosed;
    bool _isExternal;
    bool _needSort;
    int _intermediate;
    double _threshold;
    double _minline;
    Direction _material;

};

class Function2Params : public FunctionParams {
public:
    Function2Params(bool isInvertSequence = false, bool isInvertDirection = false);
    QString toQString() override;

private:
    bool _needInvertSequence;
    bool _needInvertDirection;
};

class Function3Params : public FunctionParams {
public:
    Function3Params(double radiusCorrection, bool isClosed = false, 
        bool isExternal = true, Direction material = Direction::Left, bool needSort = false);
    QString toQString() override;

private:
    bool _isClosed;
    bool _isExternal;
    bool _needSort;
    double _radiusCorrection;
    Direction _material;
};

class Function4Params : public FunctionParams {
public:
    Function4Params(float nominalTolerance = 0, int evaluationPlace = 1, int evaluationDirection = 1, 
        bool isClosed = false, bool isExternal = true, Direction material = Direction::Left, bool needSort = false);
    QString toQString() override;

private:
    bool _isClosed;
    bool _isExternal;
    bool _needSort;
    int _evaluationPlace;
    int _evaluationDirection; 
    float _nominalTolerance; 
    Direction _material;
};

class Function5Params : public FunctionParams {
public:
    Function5Params(double scale, double magnification, bool isClosed = false);
    QString toQString() override;

private:
    bool _isClosed;
    double _scale;
    double _magnification;
};

class Function6Params : public FunctionParams {
public:
    enum class Algorithm {
        Curve, Point
    };

    Function6Params(bool needMinimize = true, Algorithm method  = Algorithm::Curve, bool isClosed = true, bool needXShift = true, 
        bool needYShift = true, bool needRotation = true, bool needHConstraint = false, double xShiftFrom = 0, double xShiftTo = 0, 
        bool needVConstraint = false, double yShiftFrom = 0, double yShiftTo = 0, bool needRConstraint = false, double rotationFrom = 0, 
        double rotationTo = 0);
    QString toQString() override;

private:
    double _xShiftFrom;
    double _xShiftTo;
    double _yShiftFrom;
    double _yShiftTo;
    double _rotationFrom;
    double _rotationTo;
    bool _isClosed;
    //bool _isExternal; defualt Y
    bool _needXShift;
    bool _needYShift;
    bool _needRotation;
    bool _needHConstraint;
    bool _needVConstraint;
    bool _needRConstraint;
    bool _needMinimize;
    //int _evaluationPlace; //defualt 1
    //int _evaluationDirection; // defualt 2
    //Direction _material; // defualt L
    Algorithm _method;
};

class Function7Params : public FunctionParams {
public:
    Function7Params(double angle, double xLine, double yLine, double zLine = 0, bool isClosed = false);
    QString toQString() override;

private:
    bool _isClosed;
    double _xLine;
    double _yLine;
    double _zLine;
    double _angle;
};

class Function8Params : public FunctionParams {
public:
    Function8Params(double diameter, double xCircle, double yCircle, double zCircle = 0, bool isClosed = false);
    QString toQString() override;

private:
    bool _isClosed;
    double _xCircle;
    double _yCircle;
    double _zCircle;
    double _diameter;
};

class Function9Params : public FunctionParams {
public:
    Function9Params(bool isClosed = false);
    QString toQString() override;

private:
    bool _isClosed;
};

class Function10Params : public FunctionParams {
public:
    Function10Params(double xStart, double yStart, double zStart, double diameter, double angle, int centerType = 0);
    QString toQString() override;

private:
    double _xStart;
    double _yStart;
    double _zStart;
    double _diameter;
    double _angle;
    int _centerType;
};

class Function11Params : public FunctionParams {
public:
    Function11Params(Direction material = Direction::Right);
    QString toQString() override;

private:
    Direction _material;
};

class Function12Params : public FunctionParams {
public:
    Function12Params(QString algorithm = "Gauss", bool isExternal = false);
    QString toQString() override;

private:
    QString _algorithm;
    bool _isExternal;
};

class Function13Params : public FunctionParams {
public:
    enum class Filter {
        Polar, Linear
    };
    enum class FilterType {
        LowPass, HighPass, Band
    };

    Function13Params(Filter filter, FilterType filterType, double filterFactor, bool isClosed = false);
    QString toQString() override;

private:
    bool _isClosed;
    double _filterFactor;
    Filter _filter;
    FilterType _filterType;
};

class Function14Params : public FunctionParams {
public:
    Function14Params(bool isClosed1 = false, bool isClosed2 = false, int mode = 1);
    QString toQString() override;

private:
    bool _isClosed1;
    bool _isClosed2;
    int _mode;
};

class Function15Params : public FunctionParams {
public:
    Function15Params(bool isClosed = false);
    QString toQString() override;

private:
    bool _isClosed;
};

class Function16Params : public FunctionParams {
public:
    Function16Params(double maxFormError, double maxCircleRadius, int algorithm = 1);
    QString toQString() override;

private:
    int _algorithm;
    double _maxFormError;
    double _maxCircleRadius;
};

class Function17Params : public FunctionParams {
public:
    enum class Sorter {
        NotSort, OnIncrease, OnDecrease
    };

    Function17Params(double diameter, Sorter sortZ = Sorter::NotSort);
    QString toQString() override;

private:
    double _diameter;
    Sorter _sortZ;
};

class Function18Params : public FunctionParams {
public:
    Function18Params(int directionLE = 0, int percentLE = 5, int percentTE = 5, int joinedSegments = 1);
    QString toQString() override;

private:
    int _leadingEdgeDirection;
    int _percentageLE;
    int _percentageTE;
    int _joinedSegmentsCount;
};

class Function19Params : public FunctionParams {
public:
    Function19Params(bool isClosed = false, bool isExternal = true, Direction material = Direction::Left, QString mode = "number", int value = 500);
    QString toQString() override;

private:
    bool _isClosed;
    bool _isExternal;
    int _value;
    QString _mode;
    Direction _material;
};

class Function21Params : public FunctionParams {
public:
    Function21Params(int limInterpMethod = 0, bool needXSift = true, bool needYShift = true, bool needRotation = true, bool isClosed = false);
    QString toQString();

private:
    bool _isClosed;
    bool _needXShift;
    bool _needYShift;
    bool _needRotation;
    int _limInterpMethod;
};

class Function31Params : public FunctionParams {
public:
    Function31Params(bool isLEStretch, bool isTEStretch, int leadingEdgeDirection = 0);
    QString toQString() override;

private:
    int _leadingEdgeDirection;
    bool _isLEStretch;
    bool _isTEStretch;
};
