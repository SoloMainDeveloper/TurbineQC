#pragma once

class ReportSettings {
public:
    static QMap<QString, QString> convertToQMap(std::shared_ptr<ReportSettings> reportSettings);
    static std::shared_ptr<ReportSettings> convertToSettings(QMap<QString, QString> *params);

    explicit ReportSettings();
    virtual ~ReportSettings();

    //Direction, zone
    enum class LEDirection {
        Auto,
        PlusX,
        PlusY,
        MinusX,
        MinusY,
    };
    enum class MeasureType {
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
    enum class GlobalBestFit {
        Whole,
        WithoutEdges,
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
    enum class EdgeBestFit {
        GlobalFit,
        NoFit,
    };
    enum class TypeOfShowDevs {
        Auto,
        FromNominal,
        Set,
        OnEdge,
    };

    //Form axis
    enum class Evaluation {
        Nominal,
        Measured
    };

    //Single report
    enum class Template {
        AirfoilReport1,
    };

    void setNominalName(QString name);
    QString nominalName() const;
    void setMeasuredName(QString name);
    QString measuredName() const;
    void setScreenshotOfGlobal(QImage screenshotOfGlobal);
    QImage screenshotOfGlobal() const;
    void setScreenshotOfLE(QImage screenshotOfLE);
    QImage screenshotOfLE() const;
    void setScreenshotOfTE(QImage screenshotOfTE);
    QImage screenshotOfTE() const;

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

    void setNomContactLineLength(double nominal);
    void setMeasContactLineLength(double measured);
    void setContactLineLengthTolerances(double upTol, double downTol);
    bool onContactLineLength() const;
    double nominalContactLineLength() const;
    double upTolContactLineLength() const;
    double downTolContactLineLength() const;
    double measuredContactLineLength() const;
    double deviationContactLineLength() const;

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

    void setZone(int zoneLE, int zoneTE, MeasureType type);
    int zoneLE() const;
    int zoneTE() const;
    MeasureType measureType() const;

    //Pre-process
    void setPreProcessSettings(bool needSortPoints, bool needRemoveEqualPoints, bool needRadiusCompensation, bool needUse3DVectors);
    bool needSortPoints() const;
    bool needRemoveEqualPoints() const;
    bool needRadiusCompensation() const;
    bool needUse3DVectors() const;
    void setLimitForEqualPoints(double limit);
    double limitForEqualPoints() const;
    void setRadiusCompensation(double radiusCompensation);
    double radiusCompensation() const;

    //Global form
    void setNeedAdditionalFigures(bool needMeasuredMaxDia, bool needMeasuredMCL, bool needMeasuredContactLine);
    bool needMaxDiameter() const;
    bool needMCL() const;
    bool needContactLine() const;

    void setProfileType(Profile profileType);
    Profile profileType() const;

    void setGlobalBestFit(GlobalBestFit bestFit);
    GlobalBestFit globalBestFit() const;

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
    void setEdgesBestFit(EdgeBestFit bestFitOfLE, EdgeBestFit bestFitOfTE);
    EdgeBestFit bestFitOfLE() const;
    EdgeBestFit bestFitOfTE() const;

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
    void setTypesOfTableValue(QMap<DimFigure::ValueType, bool> typesOfTableValue);
    QMap<DimFigure::ValueType, bool> typesOfTableValue() const;

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

    void setReportPath(const QString &reportPath);
    QString reportPath() const;
    QString reportName() const;

    //Part Data
    /*QString description() const;
    QString drawing() const;
    QString orderNumber() const;
    QString partNumber() const;
    QString projectOperator() const;
    QString note() const;
    QString machine() const;
    QString tool() const;
    QString fixturing() const;
    QString batch() const;
    QString supplier() const;
    QString revision() const;*/

    //void setPartData(QString description, QString drawing, QString orderNumber, QString partNumber, QString projectOperator, QString note,
    //    QString machine, QString tool, QString fixturing, QString batch, QString supplier, QString revision);
    //void setDescription(QString description);
    //void setDrawing(QString drawing);
    //void setOrderNumber(QString orderNumber);
    //void setPartNumber(QString partNumber);
    //void setProjectOperator(QString operatorName);
    //void setNote(QString note);
    //void setMachine(QString machine);
    //void setTool(QString tool);
    //void setFixturing(QString fixturing);
    //void setBatch(QString batch);
    //void setSupplier(QString supplier);
    //void setRevision(QString revision);

    void clear();

private:
    QString _nominalName;
    QString _measuredName;

    QImage _screenshotOfGlobal;
    QImage _screenshotOfLE;
    QImage _screenshotOfTE;

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

    bool _onContactLineLength;
    double _nominalContactLineLength;
    double _upTolContactLineLength;
    double _downTolContactLineLength;
    double _measuredContactLineLength;
    double _deviationContactLineLength;

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
    MeasureType _measureType;

    //Pre-process
    bool _needSortPoints;
    bool _needRemoveEqualPoints;
    bool _needRadiusCompensation;
    bool _needUse3DVectors;
    double _limitForEqualPoints;
    double _radiusCompensation;

    //Global form
    bool _needMaxDiameter;
    bool _needMCL;
    bool _needContactLine;
    Profile _profileType;
    GlobalBestFit _globalBestFit;
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
    EdgeBestFit _bestFitOfLE;
    EdgeBestFit _bestFitOfTE;
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
    QMap<DimFigure::ValueType, bool> _typesOfTableValue;
    Evaluation _evaluationPlace;
    Evaluation _evaluationDirection;

    //Single report
    bool _needPrintWithTemplate;
    QString _reportPath;
    QString _reportName;
    QString _comment;
    Template _reportTemplate;

    ////Part Data
    //QString _description;
    //QString _drawing;
    //QString _orderNumber;
    //QString _partNumber;
    //QString _projectOperator;
    //QString _note;
    //QString _machine;
    //QString _tool;
    //QString _fixturing;
    //QString _batch;
    //QString _supplier;
    //QString _revision;
};
