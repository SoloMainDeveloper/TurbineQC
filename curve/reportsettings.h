#pragma once

#include "iturbineparameter.h"

class ReportSettings {
public:
    static QMap<QString, QString> convertToQMap(std::shared_ptr<ReportSettings> reportSettings);
    static std::shared_ptr<ReportSettings> convertToSettings(QMap<QString, QString>* params);
    static QMap<QString, QString> translateAirfoilSettings(QList<QStringList> lines);

    explicit ReportSettings();
    virtual ~ReportSettings() = default;

    // Direction, zone
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

    // Global form
    enum class Profile {
        Whole,
        WithoutEdges,
        WithoutTE,
        WithoutEdgesLSQ,
        WithoutEdgesForm,
    };

    enum class GlobalBestFit {
        NoFit,
        WholeLSQ,
        WithoutEdgesLSQ,
        WithoutTELSQ,
        TwoPointsAt10Percent,
        FitInTolBand,
        MinForm,
    };

    enum class OptionGlobalBestFit {
        TranslationAndRotation,
        OnlyTranslation,
        OnlyRotation,
        OnlyXTranslation,
        OnlyYTranslation,
        XTranslationAndRotation,
        YTranslationAndRotation,
    };

    enum class Axis {
        LeftAndDown,
        Center,
        No,
    };

    // LE, TE
    enum class EdgeBestFit {
        GlobalFit = 0,
        FreeFitForm = 1,
        NoFit = 9,
    };
    enum class TypeOfShowDevs {
        Auto,
        FromNominal,
        Set,
        OnEdge,
    };

    // Form axis
    enum class Evaluation {
        Nominal = 1,
        Measured = 2,
    };

    // Single report
    enum class Template {
        AirfoilReport1,
    };

    void setNominalName(QString name);
    QString nominalName() const;
    void setMeasuredName(QString name);
    QString measuredName() const;

    void setGlobalBase64Image(const QString& base64Image);
    QString globalBase64Image() const;
    void setLeadingEdgeBase64Image(const QString& base64Image);
    QString leadingEdgeBase64Image() const;
    void setTrailingEdgeBase64Image(const QString& base64Image);
    QString trailingEdgeBase64Image() const;

    // Calculate parameters
    void appendTurbineParameter(ITurbineParameter* parameter);
    QMap<ITurbineParameter::Type, QList<ITurbineParameter*>>& turbineParameters();
    void clearTurbineParameters();

    // Direction, zone
    void setLEDirection(LEDirection direction);
    LEDirection leadingEdgeDirection() const;

    void setZone(int leadingEdgeZone, int trailingEdgeZone, MeasureType type);
    int leadingEdgeZone() const;
    int trailingEdgeZone() const;
    MeasureType measureType() const;

    // Pre-process
    void setPreProcessSettings(bool needSortPoints, bool needRemoveEqualPoints, bool needRadiusCompensation, bool needUse3DVectors);
    bool needSortPoints() const;
    bool needRemoveEqualPoints() const;
    bool needRadiusCompensation() const;
    bool needUse3DVectors() const;
    void setLimitForEqualPoints(double limit);
    double limitForEqualPoints() const;
    void setRadiusCompensation(double radiusCompensation);
    double radiusCompensation() const;

    // Global form
    void setNeedAdditionalFigures(bool needMaxDia, bool needMCL, bool needContactLine);
    bool needMaxDiameter() const;
    bool needMCL() const;
    bool needContactLine() const;

    void setProfileType(Profile profileType);
    Profile profileType() const;

    void setGlobalBestFit(GlobalBestFit bestFit);
    GlobalBestFit globalBestFit() const;

    void setBestFitType(OptionGlobalBestFit type);
    OptionGlobalBestFit optionGlobalBestFit() const;
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

    // LE, TE
    void setEdgesBestFit(EdgeBestFit leadingEdgeBestFit, EdgeBestFit trailingEdgeBestFit);
    EdgeBestFit leadingEdgeBestFit() const;
    EdgeBestFit trailingEdgeBestFit() const;

    double leadingEdgeAmplification() const;
    double trailingEdgeAmplification() const;

    Axis axisTypeOfLE() const;
    Axis axisTypeOfTE() const;

    void setVisibilityEdges(bool isLeadingEdgeVisible, bool isTrailingEdgeVisible);
    bool isLeadingEdgeVisible() const;
    bool isTrailingEdgeVisible() const;

    void setTypeOfShowDevs(TypeOfShowDevs typeOfLE, TypeOfShowDevs typeOfTE);
    TypeOfShowDevs typeOfShowDevsLE() const;
    TypeOfShowDevs typeOfShowDevsTE() const;

    void setValueOfSetShowDevs(int valueOfLE, int valueOfTE);
    int valueOfSetShowDevsLE() const;
    int valueOfSetShowDevsTE() const;

    // Form axis
    void setOutputFormMode(int number);
    int outputFormMode() const;

    void setEvaluation(Evaluation evaluationPlace, Evaluation evaluationDirection);
    Evaluation evaluationPlace() const;
    Evaluation evaluationDirection() const;

    // Single report
    void setNeedPrintWithTemplate(bool needPrintWithTemplate);
    bool needPrintWithTemplate() const;

    void setReportTemplate(Template reportTemplate);
    Template reportTemplate() const;

    void setComment(const QString& comment);
    QString comment() const;

private:
    static QList<QStringList> getAirfoilPart(QList<QStringList> lines, QString startWith, QString endWith);

    QString _nominalName;
    QString _measuredName;

    QString _globalBase64Image;
    QString _leadingEdgeBase64Image;
    QString _trailingEdgeBase64Image;

    // Calculate parameters
    QMap<ITurbineParameter::Type, QList<ITurbineParameter*>> _turbineParameters;

    // Direction, zone
    LEDirection _directionOfLE;
    int _zoneLE;
    int _zoneTE;
    MeasureType _measureType;

    // Pre-process
    bool _needSortPoints;
    bool _needRemoveEqualPoints;
    bool _needRadiusCompensation;
    bool _needUse3DVectors;
    double _limitForEqualPoints;
    double _radiusCompensation;

    // Global form
    bool _needMaxDiameter;
    bool _needMCL;
    bool _needContactLine;
    Profile _profileType;
    GlobalBestFit _globalBestFit;
    OptionGlobalBestFit _bestFitType;
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

    // LE, TE
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

    // Form axis
    int _outputFormMode;
    Evaluation _evaluationPlace;
    Evaluation _evaluationDirection;

    // Single report
    bool _needPrintWithTemplate;
    QString _comment;
    Template _reportTemplate;
};
