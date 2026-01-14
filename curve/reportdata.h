#pragma once

class ReportData {
public:
    explicit ReportData();
    virtual ~ReportData();

    //Direction, zone
    enum class LEDirection {
        Auto,
        PlusX,
        PlusY,
        MinusX,
        MinusY,
    };
    enum class Dimension {
        Percent,
        Length,
    };

    //Global form
    enum class Profile {
        Whole,
        WithoutEdges,
        WithoutTE,
        WithoutEdgesLSQ,
        WithoutEdgesForm,
    };
    enum class BestFitType {
        TranslationAndRotation,
        OnlyTranslation,
        OnlyRotation,
        OnlyXTranslation,
        OnlyYTranslation,
        XTranslationAndRotation,
        YTranslationAndRotation,
    };
    enum class Axis {
        No,
        Center,
        LeftAndDown,
    };

    //LE, TE
    enum class TypeOfShowDevs {
        Auto,
        FromNominal,
        Set,
        OnEdge,
    };

    //Form axis
    enum class TypeOfTableValue {
        MinMax,
        Form,
        Min,
        Max,
        MaxAbs,
        SupUT,
        InfLT,
    };
    enum class Evaluation {
        Nominal,
        Measured
    };

    //Single report
    enum class Template {
        AirfoilReport1,
    };

    //Calculate parameters
    void setNomMaxWidth(double nominal);
    void setMeasMaxWidth(double measured);
    void setMaxWidthTolerances(double upTol, double downTol);
    bool onMaxWidth() const;
    double nominalMaxWidth() const;
    double upTolMaxWidth() const;
    double downTolMaxWidth() const;
    double measuredMaxWidth() const;
    double deviationMaxWidth() const;

    void setNomXMaxWidth(double nominal);
    void setMeasXMaxWidth(double measured);
    void setXMaxWidthTolerances(double upTol, double downTol);
    bool onXMaxWidth() const;
    double nominalXMaxWidth() const;
    double upTolXMaxWidth() const;
    double downTolXMaxWidth() const;
    double measuredXMaxWidth() const;
    double deviationXMaxWidth() const;

    void setNomYMaxWidth(double nominal);
    void setMeasYMaxWidth(double measured);
    void setYMaxWidthTolerances(double upTol, double downTol);
    bool onYMaxWidth() const;
    double nominalYMaxWidth() const;
    double upTolYMaxWidth() const;
    double downTolYMaxWidth() const;
    double measuredYMaxWidth() const;
    double deviationYMaxWidth() const;

    void setNomChordLength(double nominal);
    void setMeasChordLength(double measured);
    void setChordLengthTolerances(double upTol, double downTol);
    bool onChordLength() const;
    double nominalChordLength() const;
    double upTolChordLength() const;
    double downTolChordLength() const;
    double measuredChordLength() const;
    double deviationChordLength() const;

    void setNomLEWidth(double nominal);
    void setMeasLEWidth(double measured);
    void setLEWidthTolerances(double upTol, double downTol);
    bool onLEWidth() const;
    double nominalLEWidth() const;
    double upTolLEWidth() const;
    double downTolLEWidth() const;
    double measuredLEWidth() const;
    double deviationLEWidth() const;

    void setNomTEWidth(double nominal);
    void setMeasTEWidth(double measured);
    void setTEWidthTolerances(double upTol, double downTol);
    bool onTEWidth() const;
    double nominalTEWidth() const;
    double upTolTEWidth() const;
    double downTolTEWidth() const;
    double measuredTEWidth() const;
    double deviationTEWidth() const;

    void setNomLERadius(double nominal);
    void setMeasLERadius(double measured);
    void setLERadiusTolerances(double upTol, double downTol);
    bool onLERadius() const;
    double nominalLERadius() const;
    double upTolLERadius() const;
    double downTolLERadius() const;
    double measuredLERadius() const;
    double deviationLERadius() const;

    void setNomTERadius(double nominal);
    void setMeasTERadius(double measured);
    void setTERadiusTolerances(double upTol, double downTol);
    bool onTERadius() const;
    double nominalTERadius() const;
    double upTolTERadius() const;
    double downTolTERadius() const;
    double measuredTERadius() const;
    double deviationTERadius() const;
    
    //Direction, zone
    void setLEDirection(LEDirection direction);
    LEDirection directionOfLE() const;

    void setZone(int zoneLE, int zoneTE, Dimension type);
    int zoneLE() const;
    int zoneTE() const;
    Dimension dimension() const;

    //Pre-process
    void setPreProcessSettings(bool needSortPoints, bool needRemoveEqualPoints, bool needRadiusCompensation, bool needUse3DVectors);
    void setLimitForEqualPoints(double limit);
    bool needSortPoints() const;
    bool needRemoveEqualPoints() const;
    bool needRadiusCompensation() const;
    bool needUse3DVectors() const;
    double limitForEqualPoints() const;

    //Global form
    void setNeedFigures(bool needMaxDiameter, bool needMCL, bool needChord);
    bool needMaxDiameter() const;
    bool needMCL() const;
    bool needChord() const;

    void setProfileType(Profile profileType);
    Profile profileType() const;

    void setBestFitType(BestFitType type);
    BestFitType bestFitType() const;
    void setBestFitValues(double xShift, double yShift, double rotation);
    void setBestFitValues(double xShiftCV, double yShiftCV, double rotationCV, double xShiftCC, double yShiftCC, double rotationCC);
    double xShift() const;
    double yShift() const;
    double rotation() const;
    double xShiftCV() const;
    double yShiftCV() const;
    double rotationCV() const;
    double xShiftCC() const;
    double yShiftCC() const;
    double rotationCC() const;

    void setAmplification(double globalAmp, double ampOfLE, double ampOfTE);
    double globalAmplification() const;

    void setAxisDisplayType(Axis globalType, Axis typeOfLE, Axis typeOfTE);
    Axis globalAxisType() const;

    void setStretch(bool isLEStrech, bool isTEStretch);
    bool isLEStretch() const;
    bool isTEStretch() const;
    
    //LE, TE
    double amplificationOfLE() const;
    double amplificationOfTE() const;

    Axis axisTypeOfLE() const;
    Axis axisTypeOfTE() const;

    void setVisibilityEdges(bool isLEVisible, bool isTEVisible);
    bool isLEVisible() const;
    bool isTEVisible() const;

    void setTypeOfShowDevs(TypeOfShowDevs typeOfLE, TypeOfShowDevs typeOfTE);
    TypeOfShowDevs typeOfShowDevsLE() const;
    TypeOfShowDevs typeOfShowDevsTE() const;

    void setValueOfSetShowDevs(int valueOfLE, int valueOfTE);
    int valueOfSetShowDevsLE() const;
    int valueOfSetShowDevsTE() const;
    
    //Form axis
    void setTypesOfTableValue(QMap<TypeOfTableValue, bool> typesOfTableValue);
    QMap<TypeOfTableValue, bool> typesOfTableValue() const;

    void setEvaluation(Evaluation evaluationPlace, Evaluation evaluationDirection);
    Evaluation evaluationPlace() const;
    Evaluation evaluationDirection() const;

    //Single report
    void setNeedPrintWithTemplate(bool needPrintWithTemplate);
    bool needPrintWithTemplate() const;

    void setReportTemplate(Template reportTemplate);
    Template reportTemplate() const;

    void setComment(const QString &comment);
    QString comment() const;

    void setReportName(const QString &reportName);
    QString reportName() const;


    void clear();

private:
    //Calculate parameters
    bool _onMaxWidth;
    double _nominalMaxWidth;
    double _upTolMaxWidth;
    double _downTolMaxWidth;
    double _measuredMaxWidth;
    double _deviationMaxWidth;

    bool _onXMaxWidth;
    double _nominalXMaxWidth;
    double _upTolXMaxWidth;
    double _downTolXMaxWidth;
    double _measuredXMaxWidth;
    double _deviationXMaxWidth;

    bool _onYMaxWidth;
    double _nominalYMaxWidth;
    double _upTolYMaxWidth;
    double _downTolYMaxWidth;
    double _measuredYMaxWidth;
    double _deviationYMaxWidth;

    bool _onChordLength;
    double _nominalChordLength;
    double _upTolChordLength;
    double _downTolChordLength;
    double _measuredChordLength;
    double _deviationChordLength;

    bool _onLEWidth;
    double _nominalLEWidth;
    double _upTolLEWidth;
    double _downTolLEWidth;
    double _measuredLEWidth;
    double _deviationLEWidth;

    bool _onTEWidth;
    double _nominalTEWidth;
    double _upTolTEWidth;
    double _downTolTEWidth;
    double _measuredTEWidth;
    double _deviationTEWidth;

    bool _onLERadius;
    double _nominalLERadius;
    double _upTolLERadius;
    double _downTolLERadius;
    double _measuredLERadius;
    double _deviationLERadius;

    bool _onTERadius;
    double _nominalTERadius;
    double _upTolTERadius;
    double _downTolTERadius;
    double _measuredTERadius;
    double _deviationTERadius;

    //Direction, zone
    LEDirection _directionOfLE;
    int _zoneLE;
    int _zoneTE;
    Dimension _dimensionType;

    //Pre-process
    bool _needSortPoints;
    bool _needRemoveEqualPoints;
    bool _needRadiusCompensation;
    bool _needUse3DVectors;
    double _limitForEqualPoints;

    //Global form
    bool _needMaxDiameter;
    bool _needMCL;
    bool _needChord;
    Profile _profileType;
    BestFitType _bestFitType;
    double _xShift;
    double _yShift;
    double _rotation;
    double _xShiftCV;
    double _yShiftCV;
    double _rotationCV;
    double _xShiftCC;
    double _yShiftCC;
    double _rotationCC;
    double _globalAmplification;
    Axis _globalAxisType;
    bool _isLEStretch;
    bool _isTEStretch;

    //LE, TE
    bool _isLEVisible;
    bool _isTEVisible;
    Axis _axisTypeOfLE;
    Axis _axisTypeOfTE;
    double _amplificationOfLE;
    double _amplificationOfTE;
    TypeOfShowDevs _typeOfShowDevsLE;
    TypeOfShowDevs _typeOfShowDevsTE;
    int _valueOfSetShowDevsLE;
    int _valueOfSetShowDevsTE;
    
    //Form axis
    QMap<TypeOfTableValue, bool> _typesOfTableValue;
    Evaluation _evaluationPlace;
    Evaluation _evaluationDirection;

    //Single report
    bool _needPrintWithTemplate;
    QString _reportName;
    QString _comment;
    Template _reportTemplate;
};
