#include "curve/pch.h"

#include "reportsettings.h"

QMap<QString, QString> ReportSettings::convertToQMap(std::shared_ptr<ReportSettings> reportSettings)
{
    QMap<QString, QString> params;

    params.insert("nominalName", reportSettings->nominalName());
    params.insert("measuredName", reportSettings->measuredName());

    // Calculated turbine parameters
    for(auto [type, paramList] : reportSettings->turbineParameters().asKeyValueRange()) {
        for(auto i = 0; i < paramList.size(); i++) {
            params.insert(TurbineParameter::toQMap(paramList[i], i));
        }
    }

    // Direction, zone
    params.insert("directionOfLE", QString::number(int(reportSettings->directionOfLE())));
    params.insert("zoneLE", QString::number(reportSettings->zoneLE()));
    params.insert("zoneTE", QString::number(reportSettings->zoneTE()));
    params.insert("measureType", QString::number(int(reportSettings->measureType())));

    // Pre-process
    params.insert("needSortPoints", reportSettings->needSortPoints() ? "true" : "false");
    params.insert("needRemoveEqualPoints", reportSettings->needRemoveEqualPoints() ? "true" : "false");
    params.insert("needRadiusCompensation", reportSettings->needRadiusCompensation() ? "true" : "false");
    params.insert("needUse3DVectors", reportSettings->needUse3DVectors() ? "true" : "false");
    params.insert("limitForEqualPoints", QString::number(reportSettings->limitForEqualPoints()));
    params.insert("radiusCompensation", QString::number(reportSettings->radiusCompensation()));

    // Global, LE, TE form
    params.insert("xShift", QString::number(reportSettings->xShift()));
    params.insert("yShift", QString::number(reportSettings->yShift()));
    params.insert("rotation", QString::number(reportSettings->rotation()));
    params.insert("xShiftCV", QString::number(reportSettings->xShiftCV()));
    params.insert("yShiftCV", QString::number(reportSettings->yShiftCV()));
    params.insert("rotationCV", QString::number(reportSettings->rotationCV()));
    params.insert("xShiftCC", QString::number(reportSettings->xShiftCC()));
    params.insert("yShiftCC", QString::number(reportSettings->yShiftCC()));
    params.insert("rotationCC", QString::number(reportSettings->rotationCC()));
    params.insert("profileType", QString::number(int(reportSettings->profileType())));
    params.insert("globalBestFit", QString::number(int(reportSettings->globalBestFit())));
    params.insert("bestFitOfLE", QString::number(int(reportSettings->bestFitOfLE())));
    params.insert("bestFitOfTE", QString::number(int(reportSettings->bestFitOfTE())));
    params.insert("bestFitType", QString::number(int(reportSettings->bestFitType())));
    params.insert("isLEStretch", reportSettings->isLEStretch() ? "true" : "false");
    params.insert("isTEStretch", reportSettings->isTEStretch() ? "true" : "false");
    params.insert("globalAmplification", QString::number(reportSettings->globalAmplification()));
    params.insert("amplificationOfLE", QString::number(reportSettings->amplificationOfLE()));
    params.insert("amplificationOfTE", QString::number(reportSettings->amplificationOfTE()));
    params.insert("globalAxisType", QString::number(int(reportSettings->globalAxisType())));
    params.insert("axisTypeOfLE", QString::number(int(reportSettings->axisTypeOfLE())));
    params.insert("axisTypeOfTE", QString::number(int(reportSettings->axisTypeOfTE())));
    params.insert("needMaxDia", reportSettings->needMaxDiameter() ? "true" : "false");
    params.insert("needMCL", reportSettings->needMCL() ? "true" : "false");
    params.insert("needContactLine", reportSettings->needContactLine() ? "true" : "false");
    params.insert("isLEVisible", reportSettings->isLEVisible() ? "true" : "false");
    params.insert("isTEVisible", reportSettings->isTEVisible() ? "true" : "false");
    params.insert("typeOfShowDevsLE", QString::number(int(reportSettings->typeOfShowDevsLE())));
    params.insert("typeOfShowDevsTE", QString::number(int(reportSettings->typeOfShowDevsTE())));
    params.insert("valueOfSetShowDevsLE", QString::number(reportSettings->valueOfSetShowDevsLE()));
    params.insert("valueOfSetShowDevsTE", QString::number(reportSettings->valueOfSetShowDevsTE()));
    params.insert("outputFormMode", QString::number(reportSettings->outputFormMode()));
    params.insert("evaluationPlace", QString::number(int(reportSettings->evaluationPlace())));
    params.insert("evaluationDirection", QString::number(int(reportSettings->evaluationDirection())));

    // Single report
    params.insert("needPrintWithTemplate", reportSettings->needPrintWithTemplate() ? "true" : "false");
    params.insert("reportTemplate", QString::number(int(reportSettings->reportTemplate())));
    params.insert("comment", reportSettings->comment());

    return params;
}

std::shared_ptr<ReportSettings> ReportSettings::convertToSettings(QMap<QString, QString>* params)
{
    auto reportSettings = std::make_shared<ReportSettings>();

    reportSettings->setNominalName(params->value("nominalName"));
    reportSettings->setMeasuredName(params->value("measuredName"));

    // Calculate parameters
    for(auto [type, turbineParam] : params->asKeyValueRange()) {
        if(!type.contains("Param ")) {
            continue;
        }

        reportSettings->appendTurbineParameter(TurbineParameter::toTurbineParameter(turbineParam));
    }

    // Direction zone
    reportSettings->setLEDirection(static_cast<ReportSettings::LEDirection>(params->value("directionOfLE").toInt()));
    reportSettings->setZone(params->value("zoneLE").toInt(), params->value("zoneTE").toInt(),
        static_cast<ReportSettings::MeasureType>(params->value("measureType").toInt()));

    // Pre-process
    reportSettings->setPreProcessSettings(params->value("needSortPoints") == "true", params->value("needRemoveEqualPoints") == "true",
        params->value("needRadiusCompensation") == "true", params->value("needUse3DVectors") == "true");
    if(params->value("needRemoveEqualPoints") == "true") {
        reportSettings->setLimitForEqualPoints(params->value("limitForEqualPoints").toDouble());
    }
    if(params->value("needRadiusCompensation") == "true") {
        reportSettings->setRadiusCompensation(params->value("radiusCompensation").toDouble());
    }

    // Global, LE, TE form
    reportSettings->setBestFitValues(params->value("xShift").toDouble(), params->value("yShift").toDouble(), params->value("rotation").toDouble());
    reportSettings->setBestFitValues(params->value("xShiftCV").toDouble(), params->value("yShiftCV").toDouble(), params->value("rotationCV").toDouble(),
        params->value("xShiftCC").toDouble(), params->value("yShiftCC").toDouble(), params->value("rotationCC").toDouble());
    reportSettings->setProfileType(static_cast<ReportSettings::Profile>(params->value("profileType").toInt()));
    reportSettings->setGlobalBestFit(static_cast<ReportSettings::GlobalBestFit>(params->value("globalBestFit").toInt()));
    reportSettings->setEdgesBestFit(
        static_cast<ReportSettings::EdgeBestFit>(params->value("bestFitOfLE").toInt()),
        static_cast<ReportSettings::EdgeBestFit>(params->value("bestFitOfTE").toInt()));
    reportSettings->setBestFitType(static_cast<ReportSettings::BestFitType>(params->value("bestFitType").toInt()));
    reportSettings->setStretch(params->value("isLEStretch") == "true", params->value("isTEStretch") == "true");
    reportSettings->setAmplification(params->value("globalAmplification").toDouble(),
        params->value("amplificationOfLE").toDouble(), params->value("amplificationOfTE").toDouble());
    reportSettings->setAxisDisplayType(
        static_cast<ReportSettings::Axis>(params->value("globalAxisType").toInt()),
        static_cast<ReportSettings::Axis>(params->value("axisTypeOfLE").toInt()),
        static_cast<ReportSettings::Axis>(params->value("axisTypeOfTE").toInt()));
    reportSettings->setNeedAdditionalFigures(params->value("needMaxDia") == "true", params->value("needMCL") == "true", params->value("needContactLine") == "true");
    reportSettings->setVisibilityEdges(params->value("isLEVisible") == "true", params->value("isTEVisible") == "true");
    reportSettings->setTypeOfShowDevs(
        static_cast<ReportSettings::TypeOfShowDevs>(params->value("typeOfShowDevsLE").toInt()),
        static_cast<ReportSettings::TypeOfShowDevs>(params->value("typeOfShowDevsTE").toInt()));
    reportSettings->setValueOfSetShowDevs(params->value("valueOfSetShowDevsLE").toInt(), params->value("valueOfSetShowDevsTE").toInt());
    reportSettings->setOutputFormMode(params->value("outputFormMode").toInt());
    reportSettings->setEvaluation(static_cast<ReportSettings::Evaluation>(params->value("evaluationPlace").toInt()),
        static_cast<ReportSettings::Evaluation>(params->value("evaluationDirection").toInt()));

    // Single report
    if(params->value("needPrintWithTemplate") == "true") {
        reportSettings->setNeedPrintWithTemplate(true);
        reportSettings->setReportTemplate(static_cast<ReportSettings::Template>(params->value("reportTemplate").toInt()));
        reportSettings->setComment(params->value("comment"));
    }

    return reportSettings;
}

QMap<QString, QString> ReportSettings::translateAirfoilSettings(QList<QStringList> lines)
{
    QMap<QString, QString> params;
    auto calcParamsPart = getAirfoilPart(lines, "Par0", "Par");
    auto globalFormPart = getAirfoilPart(lines, "FormGlobal", "Print");
    params.insert({ { "nominalName", lines[0][2] },
        { "measuredName", lines[0][3] },
        { "directionOfLE", lines[0][4] },
        { "zoneLE", lines[0][5] },
        { "zoneTE", lines[0][6] },
        { "outputFormMode", lines[0][7] },
        { "measureType", lines[0][8] },
        { "evaluationPlace", lines[0][9] },
        { "evaluationDirection", lines[0][10] },
        // pre-process
        { "needSortPoints", lines[1][3] == "1" ? "true" : "false" },
        { "needRemoveEqualPoints", lines[1][4] == "1" ? "true" : "false" },
        { "limitForEqualPoints", lines[1][5] },
        { "needRadiusCompensation", lines[1][6] == "1" ? "true" : "false" },
        { "radiusCompensation", lines[1][7] },
        { "needUse3DVectors", lines[1][8] == "1" ? "true" : "false" } });

    // params to calculate
    for(auto i = 0; i < calcParamsPart.size(); i++) {
        params.insert(TurbineParameter::toQMapFromCRM(calcParamsPart[i]));
    }

    // global, LE, TE form
    params.insert("profileType", globalFormPart[0][2]);
    params.insert("globalBestFit", globalFormPart[0][3]);
    params.insert("globalAmplification", globalFormPart[0][4]);
    params.insert("globalAxisType", globalFormPart[0][5]);
    params.insert("isLEStretch", globalFormPart[0][8] == "1" ? "true" : "false");
    params.insert("isTEStretch", globalFormPart[0][9] == "1" ? "true" : "false");
    params.insert("bestFitType", globalFormPart[1][2]);
    params.insert("needMCL", globalFormPart[1][3] == "1" ? "true" : "false");
    params.insert("needMaxDia", globalFormPart[1][4] == "1" ? "true" : "false");
    params.insert("needContactLine", globalFormPart[1][5] == "1" ? "true" : "false");
    params.insert("isLEVisible", globalFormPart[2][2] == "1" ? "true" : "false");
    params.insert("bestFitOfLE", globalFormPart[2][3]);
    params.insert("amplificationOfLE", globalFormPart[2][4]);
    params.insert("axisTypeOfLE", globalFormPart[2][5]);
    params.insert("typeOfShowDevsLE", globalFormPart[2][9]);

    params.insert("isTEVisible", globalFormPart[4][2] == "1" ? "true" : "false");
    params.insert("bestFitOfTE", globalFormPart[4][3]);
    params.insert("amplificationOfTE", globalFormPart[4][4]);
    params.insert("axisTypeOfTE", globalFormPart[4][5]);
    params.insert("typeOfShowDevsTE", globalFormPart[4][9]);

    params.insert("needPrintWithTemplate", globalFormPart[7][2] == "1" ? "true" : "false");
    params.insert("reportTemplate", "0");
    params.insert("comment", globalFormPart[7].size() == 5 ? globalFormPart[7][4] : QString());
    return params;
}

ReportSettings::ReportSettings()
{
    // Direction, zone
    _directionOfLE = LEDirection::Auto;
    _zoneLE = 0;
    _zoneTE = 0;
    _measureType = MeasureType::Percent;

    // Pre-process
    _needSortPoints = false;
    _needRemoveEqualPoints = false;
    _needRadiusCompensation = false;
    _needUse3DVectors = false;
    _limitForEqualPoints = 0;
    _radiusCompensation = 0;

    // Global form
    _needMaxDiameter = false;
    _needMCL = false;
    _needContactLine = false;
    _profileType = Profile::Whole;
    _globalBestFit = GlobalBestFit::Whole;
    _bestFitType = BestFitType::TranslationAndRotation;
    _xShift = 0;
    _yShift = 0;
    _rotation = 0;
    _xShiftCV = 0;
    _yShiftCV = 0;
    _rotationCV = 0;
    _xShiftCC = 0;
    _yShiftCC = 0;
    _rotationCC = 0;
    _globalAmplification = 0;
    _globalAxisType = Axis::No;
    _isLEStretch = false;
    _isTEStretch = false;

    // LE, TE
    _bestFitOfLE = EdgeBestFit::GlobalFit;
    _bestFitOfTE = EdgeBestFit::GlobalFit;
    _amplificationOfLE = 0;
    _amplificationOfTE = 0;
    _axisTypeOfLE = Axis::No;
    _axisTypeOfTE = Axis::No;
    _isLEVisible = false;
    _isTEVisible = false;
    _typeOfShowDevsLE = TypeOfShowDevs::Auto;
    _typeOfShowDevsTE = TypeOfShowDevs::Auto;
    _valueOfSetShowDevsLE = 0;
    _valueOfSetShowDevsTE = 0;

    // Form axis
    _evaluationPlace = Evaluation::Nominal;
    _evaluationDirection = Evaluation::Nominal;

    // Single report
    _needPrintWithTemplate = false;
    _comment = "";
    _reportTemplate = Template::AirfoilReport1;
}

void ReportSettings::setReportTemplate(Template reportTemplate)
{
    _reportTemplate = reportTemplate;
}

ReportSettings::Template ReportSettings::reportTemplate() const
{
    return _reportTemplate;
}

void ReportSettings::setProfileType(Profile profileType)
{
    _profileType = profileType;
}

ReportSettings::Profile ReportSettings::profileType() const
{
    return _profileType;
}

void ReportSettings::setGlobalBestFit(GlobalBestFit bestFit)
{
    _globalBestFit = bestFit;
}

ReportSettings::GlobalBestFit ReportSettings::globalBestFit() const
{
    return _globalBestFit;
}

void ReportSettings::setNominalName(QString name)
{
    _nominalName = name;
}

QString ReportSettings::nominalName() const
{
    return _nominalName;
}

void ReportSettings::setMeasuredName(QString name)
{
    _measuredName = name;
}

QString ReportSettings::measuredName() const
{
    return _measuredName;
}

void ReportSettings::setScreenshotOfGlobal(QImage screenshotOfGlobal)
{
    _screenshotOfGlobal = screenshotOfGlobal;
}

QImage ReportSettings::screenshotOfGlobal() const
{
    return _screenshotOfGlobal;
}

void ReportSettings::setScreenshotOfLE(QImage screenshotOfLE)
{
    _screenshotOfLE = screenshotOfLE;
}

QImage ReportSettings::screenshotOfLE() const
{
    return _screenshotOfLE;
}

void ReportSettings::setScreenshotOfTE(QImage screenshotOfTE)
{
    _screenshotOfTE = screenshotOfTE;
}

QImage ReportSettings::screenshotOfTE() const
{
    return _screenshotOfTE;
}

void ReportSettings::clearTurbineParameters()
{
    _turbineParameters.clear();
}

void ReportSettings::setZone(int zoneLE, int zoneTE, MeasureType type)
{
    _zoneLE = zoneLE;
    _zoneTE = zoneTE;
    _measureType = type;
}

int ReportSettings::zoneLE() const
{
    return _zoneLE;
}

int ReportSettings::zoneTE() const
{
    return _zoneTE;
}

ReportSettings::MeasureType ReportSettings::measureType() const
{
    return _measureType;
}

void ReportSettings::setNeedAdditionalFigures(bool needMaxDia, bool needMCL, bool needContactLine)
{
    _needMaxDiameter = needMaxDia;
    _needMCL = needMCL;
    _needContactLine = needContactLine;
}

bool ReportSettings::needMaxDiameter() const
{
    return _needMaxDiameter;
}

bool ReportSettings::needMCL() const
{
    return _needMCL;
}

bool ReportSettings::needContactLine() const
{
    return _needContactLine;
}

void ReportSettings::setBestFitType(BestFitType type)
{
    _bestFitType = type;
}

ReportSettings::BestFitType ReportSettings::bestFitType() const
{
    return _bestFitType;
}

void ReportSettings::setBestFitValues(double xShift, double yShift, double rotation)
{
    _xShift = xShift;
    _yShift = yShift;
    _rotation = rotation;
}

void ReportSettings::setBestFitValues(double xShiftCV, double yShiftCV, double rotationCV, double xShiftCC, double yShiftCC, double rotationCC)
{
    _xShiftCV = xShiftCV;
    _yShiftCV = yShiftCV;
    _rotationCV = rotationCV;
    _xShiftCC = xShiftCC;
    _yShiftCC = yShiftCC;
    _rotationCC = rotationCC;
}

double ReportSettings::xShift() const
{
    return _xShift;
}

double ReportSettings::yShift() const
{
    return _yShift;
}

double ReportSettings::rotation() const
{
    return _rotation;
}

double ReportSettings::xShiftCV() const
{
    return _xShiftCV;
}

double ReportSettings::yShiftCV() const
{
    return _yShiftCV;
}

double ReportSettings::rotationCV() const
{
    return _rotationCV;
}

double ReportSettings::xShiftCC() const
{
    return _xShiftCC;
}

double ReportSettings::yShiftCC() const
{
    return _yShiftCC;
}

double ReportSettings::rotationCC() const
{
    return _rotationCC;
}

void ReportSettings::setAmplification(double globalAmp, double ampOfLE, double ampOfTE)
{
    _globalAmplification = globalAmp;
    _amplificationOfLE = ampOfLE;
    _amplificationOfTE = ampOfTE;
}

double ReportSettings::globalAmplification() const
{
    return _globalAmplification;
}

void ReportSettings::setEdgesBestFit(EdgeBestFit bestFitOfLE, EdgeBestFit bestFitOfTE)
{
    _bestFitOfLE = bestFitOfLE;
    _bestFitOfTE = bestFitOfTE;
}

ReportSettings::EdgeBestFit ReportSettings::bestFitOfLE() const
{
    return _bestFitOfLE;
}

ReportSettings::EdgeBestFit ReportSettings::bestFitOfTE() const
{
    return _bestFitOfTE;
}

double ReportSettings::amplificationOfLE() const
{
    return _amplificationOfLE;
}

double ReportSettings::amplificationOfTE() const
{
    return _amplificationOfTE;
}

void ReportSettings::setAxisDisplayType(Axis globalType, Axis typeOfLE, Axis typeOfTE)
{
    _globalAxisType = globalType;
    _axisTypeOfLE = typeOfLE;
    _axisTypeOfTE = typeOfTE;
}

ReportSettings::Axis ReportSettings::globalAxisType() const
{
    return _globalAxisType;
}

ReportSettings::Axis ReportSettings::axisTypeOfLE() const
{
    return _axisTypeOfLE;
}

ReportSettings::Axis ReportSettings::axisTypeOfTE() const
{
    return _axisTypeOfTE;
}

void ReportSettings::setPreProcessSettings(bool needSortPoints, bool needRemoveEqualPoints, bool needRadiusCompensation, bool needUse3DVectors)
{
    _needSortPoints = needSortPoints;
    _needRemoveEqualPoints = needRemoveEqualPoints;
    _needRadiusCompensation = needRadiusCompensation;
    _needUse3DVectors = needUse3DVectors;
}

void ReportSettings::setLimitForEqualPoints(double limit)
{
    _limitForEqualPoints = limit;
}

bool ReportSettings::needSortPoints() const
{
    return _needSortPoints;
}

bool ReportSettings::needRemoveEqualPoints() const
{
    return _needRemoveEqualPoints;
}

bool ReportSettings::needRadiusCompensation() const
{
    return _needRadiusCompensation;
}

bool ReportSettings::needUse3DVectors() const
{
    return _needUse3DVectors;
}

double ReportSettings::limitForEqualPoints() const
{
    return _limitForEqualPoints;
}

void ReportSettings::setRadiusCompensation(double radiusCompensation)
{
    _radiusCompensation = radiusCompensation;
}

double ReportSettings::radiusCompensation() const
{
    return _radiusCompensation;
}

void ReportSettings::setLEDirection(LEDirection direction)
{
    _directionOfLE = direction;
}

ReportSettings::LEDirection ReportSettings::directionOfLE() const
{
    return _directionOfLE;
}

void ReportSettings::setStretch(bool isLEStretch, bool isTEStretch)
{
    _isLEStretch = isLEStretch;
    _isTEStretch = isTEStretch;
}

bool ReportSettings::isLEStretch() const
{
    return _isLEStretch;
}

bool ReportSettings::isTEStretch() const
{
    return _isTEStretch;
}

void ReportSettings::setComment(const QString& comment)
{
    _comment = comment;
}

QString ReportSettings::comment() const
{
    return _comment;
}

void ReportSettings::appendTurbineParameter(TurbineParameter* parameter)
{
    _turbineParameters[parameter->type].append(parameter);
}

QMap<TurbineParameter::Type, QList<TurbineParameter*>>& ReportSettings::turbineParameters()
{
    return _turbineParameters;
}

void ReportSettings::setVisibilityEdges(bool isLEVisible, bool isTEVisible)
{
    _isLEVisible = isLEVisible;
    _isTEVisible = isTEVisible;
}

bool ReportSettings::isLEVisible() const
{
    return _isLEVisible;
}

bool ReportSettings::isTEVisible() const
{
    return _isTEVisible;
}

void ReportSettings::setTypeOfShowDevs(TypeOfShowDevs typeOfLE, TypeOfShowDevs typeOfTE)
{
    _typeOfShowDevsLE = typeOfLE;
    _typeOfShowDevsTE = typeOfTE;
}

ReportSettings::TypeOfShowDevs ReportSettings::typeOfShowDevsLE() const
{
    return _typeOfShowDevsLE;
}

ReportSettings::TypeOfShowDevs ReportSettings::typeOfShowDevsTE() const
{
    return _typeOfShowDevsTE;
}

void ReportSettings::setValueOfSetShowDevs(int valueOfLE, int valueOfTE)
{
    _valueOfSetShowDevsLE = valueOfLE;
    _valueOfSetShowDevsTE = valueOfTE;
}

int ReportSettings::valueOfSetShowDevsLE() const
{
    return _valueOfSetShowDevsLE;
}

int ReportSettings::valueOfSetShowDevsTE() const
{
    return _valueOfSetShowDevsTE;
}

void ReportSettings::setNeedPrintWithTemplate(bool needPrintWithTemplate)
{
    _needPrintWithTemplate = needPrintWithTemplate;
}

bool ReportSettings::needPrintWithTemplate() const
{
    return _needPrintWithTemplate;
}

void ReportSettings::setOutputFormMode(int outputFormMode)
{
    _outputFormMode = outputFormMode;
}

int ReportSettings::outputFormMode() const
{
    return _outputFormMode;
}

void ReportSettings::setEvaluation(Evaluation evaluationPlace, Evaluation evaluationDirection)
{
    _evaluationPlace = evaluationPlace;
    _evaluationDirection = evaluationDirection;
}

ReportSettings::Evaluation ReportSettings::evaluationPlace() const
{
    return _evaluationPlace;
}

ReportSettings::Evaluation ReportSettings::evaluationDirection() const
{
    return _evaluationDirection;
}

QList<QStringList> ReportSettings::getAirfoilPart(QList<QStringList> lines, QString startWith, QString endWith)
{
    int start = 0;
    int end = 0;
    for(auto i = 0; i < lines.size(); i++) {
        if(lines[i][1] == startWith)
            start = i;
        if(lines[i][1].startsWith(endWith))
            end = i;
    }
    return lines.mid(start, end - start + 1);
}
