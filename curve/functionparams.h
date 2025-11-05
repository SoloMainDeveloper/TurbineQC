#pragma once

class FunctionParams {
public:
    virtual QString toQString() = 0;
};

class Function1Params : public FunctionParams {
public:
    enum Direction {
        Right, Left
    };

    Function1Params(int intermediate, double threshold = 0, double minline = 0, bool isClosed = false,
        bool isExternal = false, Direction material = Direction::Left, bool needSort = false);
    QString toQString() override;

private:
    bool _isClosed;
    bool _isExternal;
    Direction _material;
    double _threshold;
    int _intermediate;
    double _minline;
    bool _needSort;

};

class Function2Params : public FunctionParams {
public:
    Function2Params();
    QString toQString() override;

private:
    //properties
};

class Function3Params : public FunctionParams {
public:
    Function3Params();
    QString toQString() override;

private:
    //properties
};

class Function4Params : public FunctionParams {
public:
    Function4Params();
    QString toQString() override;

private:
    //properties
};

class Function5Params : public FunctionParams {
public:
    Function5Params();
    QString toQString() override;

private:
    //properties
};

class Function6Params : public FunctionParams {
public:
    Function6Params();
    QString toQString() override;

private:
    //properties
};

class Function7Params : public FunctionParams {
public:
    Function7Params();
    QString toQString() override;

private:
    //properties
};

class Function8Params : public FunctionParams {
public:
    Function8Params();
    QString toQString() override;

private:
    //properties
};

class Function9Params : public FunctionParams {
public:
    Function9Params();
    QString toQString() override;

private:
    //properties
};

class Function10Params : public FunctionParams {
public:
    Function10Params();
    QString toQString() override;

private:
    //properties
};

class Function11Params : public FunctionParams {
public:
    Function11Params();
    QString toQString() override;

private:
    //properties
};

class Function12Params : public FunctionParams {
public:
    Function12Params();
    QString toQString() override;

private:
    //properties
};

class Function13Params : public FunctionParams {
public:
    Function13Params();
    QString toQString() override;

private:
    //properties
};

class Function14Params : public FunctionParams {
public:
    Function14Params();
    QString toQString() override;

private:
    //properties
};

class Function15Params : public FunctionParams {
public:
    Function15Params();
    QString toQString() override;

private:
    //properties
};

class Function16Params : public FunctionParams {
public:
    Function16Params();
    QString toQString() override;

private:
    //properties
};

class Function17Params : public FunctionParams {
public:
    Function17Params();
    QString toQString() override;

private:
    //properties
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
