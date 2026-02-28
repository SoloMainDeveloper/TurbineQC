#include "curve/pch.h"
#include "reportsettings.h"

QMap<QString, QString> ReportSettings::convertToQMap(std::shared_ptr<ReportSettings> reportSettings) {
    QMap<QString, QString> params;

    params.insert("nominalName", reportSettings->nominalName());
    params.insert("measuredName", reportSettings->measuredName());

    params.insert("nominalMaxWidth", QString::number(reportSettings->nominalMaxWidth()));
    params.insert("measuredMaxWidth", QString::number(reportSettings->measuredMaxWidth()));
    params.insert("onMaxWidth", reportSettings->onMaxWidth() ? "true" : "false");
    params.insert("deviationMaxWidth", QString::number(reportSettings->deviationMaxWidth()));
    params.insert("upTolMaxWidth", QString::number(reportSettings->upTolMaxWidth()));
    params.insert("downTolMaxWidth", QString::number(reportSettings->downTolMaxWidth()));

    params.insert("nominalXMaxWidth", QString::number(reportSettings->nominalXMaxWidth()));
    params.insert("measuredXMaxWidth", QString::number(reportSettings->measuredXMaxWidth()));
    params.insert("onXMaxWidth", reportSettings->onXMaxWidth() ? "true" : "false");
    params.insert("deviationXMaxWidth", QString::number(reportSettings->deviationXMaxWidth()));
    params.insert("upTolXMaxWidth", QString::number(reportSettings->upTolXMaxWidth()));
    params.insert("downTolXMaxWidth", QString::number(reportSettings->downTolXMaxWidth()));

    params.insert("nominalYMaxWidth", QString::number(reportSettings->nominalYMaxWidth()));
    params.insert("measuredYMaxWidth", QString::number(reportSettings->measuredYMaxWidth()));
    params.insert("onYMaxWidth", reportSettings->onYMaxWidth() ? "true" : "false");
    params.insert("deviationYMaxWidth", QString::number(reportSettings->deviationYMaxWidth()));
    params.insert("upTolYMaxWidth", QString::number(reportSettings->upTolYMaxWidth()));
    params.insert("downTolYMaxWidth", QString::number(reportSettings->downTolYMaxWidth()));

    params.insert("nominalContactLineLength", QString::number(reportSettings->nominalContactLineLength()));
    params.insert("measuredContactLineLength", QString::number(reportSettings->measuredContactLineLength()));
    params.insert("onContactLineLength", reportSettings->onContactLineLength() ? "true" : "false");
    params.insert("deviationContactLineLength", QString::number(reportSettings->deviationContactLineLength()));
    params.insert("upTolContactLineLength", QString::number(reportSettings->upTolContactLineLength()));
    params.insert("downTolContactLineLength", QString::number(reportSettings->downTolContactLineLength()));

    params.insert("nominalLEWidth", QString::number(reportSettings->nominalLEWidth()));
    params.insert("measuredLEWidth", QString::number(reportSettings->measuredLEWidth()));
    params.insert("onLEWidth", reportSettings->onLEWidth() ? "true" : "false");
    params.insert("deviationLEWidth", QString::number(reportSettings->deviationLEWidth()));
    params.insert("upTolLEWidth", QString::number(reportSettings->upTolLEWidth()));
    params.insert("downTolLEWidth", QString::number(reportSettings->downTolLEWidth()));

    params.insert("nominalTEWidth", QString::number(reportSettings->nominalTEWidth()));
    params.insert("measuredTEWidth", QString::number(reportSettings->measuredTEWidth()));
    params.insert("onTEWidth", reportSettings->onTEWidth() ? "true" : "false");
    params.insert("deviationTEWidth", QString::number(reportSettings->deviationTEWidth()));
    params.insert("upTolTEWidth", QString::number(reportSettings->upTolTEWidth()));
    params.insert("downTolTEWidth", QString::number(reportSettings->downTolTEWidth()));

    params.insert("nominalLERadius", QString::number(reportSettings->nominalLERadius()));
    params.insert("measuredLERadius", QString::number(reportSettings->measuredLERadius()));
    params.insert("onLERadius", reportSettings->onLERadius() ? "true" : "false");
    params.insert("deviationLERadius", QString::number(reportSettings->deviationLERadius()));
    params.insert("upTolLERadius", QString::number(reportSettings->upTolLERadius()));
    params.insert("downTolLERadius", QString::number(reportSettings->downTolLERadius()));

    params.insert("nominalTERadius", QString::number(reportSettings->nominalTERadius()));
    params.insert("measuredTERadius", QString::number(reportSettings->measuredTERadius()));
    params.insert("onTERadius", reportSettings->onTERadius() ? "true" : "false");
    params.insert("deviationTERadius", QString::number(reportSettings->deviationTERadius()));
    params.insert("upTolTERadius", QString::number(reportSettings->upTolTERadius()));
    params.insert("downTolTERadius", QString::number(reportSettings->downTolTERadius()));

    //Direction, zone
    params.insert("directionOfLE", QString::number(int(reportSettings->directionOfLE())));
    params.insert("zoneLE", QString::number(reportSettings->zoneLE()));
    params.insert("zoneTE", QString::number(reportSettings->zoneTE()));
    params.insert("measureType", QString::number(int(reportSettings->measureType())));

    //Pre-process
    params.insert("needSortPoints", reportSettings->needSortPoints() ? "true" : "false");
    params.insert("needRemoveEqualPoints", reportSettings->needRemoveEqualPoints() ? "true" : "false");
    params.insert("needRadiusCompensation", reportSettings->needRadiusCompensation() ? "true" : "false");
    params.insert("needUse3DVectors", reportSettings->needUse3DVectors() ? "true" : "false");
    params.insert("limitForEqualPoints", QString::number(reportSettings->limitForEqualPoints()));
    params.insert("radiusCompensation", QString::number(reportSettings->radiusCompensation()));

    //Global, LE, TE form
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
    for(auto [type, isChecked] : reportSettings->typesOfTableValue().asKeyValueRange()) {
        auto key = "";
        if(type == DimFigure::ValueType::MinMax) {
            key = "MinMax";
        } else if(type == DimFigure::ValueType::Form) {
            key = "Form";
        } else if(type == DimFigure::ValueType::Min) {
            key = "Min";
        } else if(type == DimFigure::ValueType::Max) {
            key = "Max";
        } else if(type == DimFigure::ValueType::MaxAbs) {
            key = "MaxAbs";
        } else if(type == DimFigure::ValueType::SupUT) {
            key = "SupUT";
        } else if(type == DimFigure::ValueType::InfLT) {
            key = "InfLT";
        }
        params.insert(key, isChecked ? "true" : "false");
    }
    params.insert("evaluationPlace", QString::number(int(reportSettings->evaluationPlace())));
    params.insert("evaluationDirection", QString::number(int(reportSettings->evaluationDirection())));

    //Single report
    params.insert("needPrintWithTemplate", reportSettings->needPrintWithTemplate() ? "true" : "false");
    params.insert("reportTemplate", QString::number(int(reportSettings->reportTemplate())));
    params.insert("comment", reportSettings->comment());
    params.insert("reportPath", reportSettings->reportPath());
    params.insert("reportName", reportSettings->reportName());

    ////Part Data
    //params.insert("description", reportSettings->description());
    //params.insert("drawing", reportSettings->drawing());
    //params.insert("orderNumber", reportSettings->orderNumber());
    //params.insert("partNumber", reportSettings->partNumber());
    //params.insert("projectOperator", reportSettings->projectOperator());
    //params.insert("note", reportSettings->note());
    //params.insert("machine", reportSettings->machine());
    //params.insert("tool", reportSettings->tool());
    //params.insert("fixturing", reportSettings->fixturing());
    //params.insert("batch", reportSettings->batch());
    //params.insert("supplier", reportSettings->supplier());
    //params.insert("revision", reportSettings->revision());



    return params;
}

std::shared_ptr<ReportSettings> ReportSettings::convertToSettings(QMap<QString, QString> *params) {
    auto reportSettings = std::make_shared<ReportSettings>();

    reportSettings->setNominalName(params->value("nominalName"));
    reportSettings->setMeasuredName(params->value("measuredName"));

    //Calculate parameters
    if(params->value("onMaxWidth") == "true") {
        reportSettings->setNomMaxWidth(params->value("nominalMaxWidth").toDouble());
        reportSettings->setMeasMaxWidth(params->value("measuredMaxWidth").toDouble());
        reportSettings->setMaxWidthTolerances(params->value("upTolMaxWidth").toDouble(), params->value("downTolMaxWidth").toDouble());
    }

    if(params->value("onXMaxWidth") == "true") {
        reportSettings->setNomXMaxWidth(params->value("nominalXMaxWidth").toDouble());
        reportSettings->setMeasXMaxWidth(params->value("measuredXMaxWidth").toDouble());
        reportSettings->setXMaxWidthTolerances(params->value("upTolXMaxWidth").toDouble(), params->value("downTolXMaxWidth").toDouble());
    }

    if(params->value("onYMaxWidth") == "true") {
        reportSettings->setNomYMaxWidth(params->value("nominalYMaxWidth").toDouble());
        reportSettings->setMeasYMaxWidth(params->value("measuredYMaxWidth").toDouble());
        reportSettings->setYMaxWidthTolerances(params->value("upTolYMaxWidth").toDouble(), params->value("downTolYMaxWidth").toDouble());
    }

    if(params->value("onContactLineLength") == "true") {
        reportSettings->setNomContactLineLength(params->value("nominalContactLineLength").toDouble());
        reportSettings->setMeasContactLineLength(params->value("measuredContactLineLength").toDouble());
        reportSettings->setContactLineLengthTolerances(params->value("upTolContactLineLength").toDouble(), params->value("downTolContactLineLength").toDouble());
    }

    if(params->value("onLEWidth") == "true") {
        reportSettings->setNomLEWidth(params->value("nominalLEWidth").toDouble());
        reportSettings->setMeasLEWidth(params->value("measuredLEWidth").toDouble());
        reportSettings->setLEWidthTolerances(params->value("upTolLEWidth").toDouble(), params->value("downTolLEWidth").toDouble());
    }

    if(params->value("onTEWidth") == "true") {
        reportSettings->setNomTEWidth(params->value("nominalTEWidth").toDouble());
        reportSettings->setMeasTEWidth(params->value("measuredTEWidth").toDouble());
        reportSettings->setTEWidthTolerances(params->value("upTolTEWidth").toDouble(), params->value("downTolTEWidth").toDouble());
    }

    if(params->value("onLERadius") == "true") {
        reportSettings->setNomLERadius(params->value("nominalLERadius").toDouble());
        reportSettings->setMeasLERadius(params->value("measuredLERadius").toDouble());
        reportSettings->setLERadiusTolerances(params->value("upTolLERadius").toDouble(), params->value("downTolLERadius").toDouble());
    }

    if(params->value("onTERadius") == "true") {
        reportSettings->setNomTERadius(params->value("nominalTERadius").toDouble());
        reportSettings->setMeasTERadius(params->value("measuredTERadius").toDouble());
        reportSettings->setTERadiusTolerances(params->value("upTolTERadius").toDouble(), params->value("downTolTERadius").toDouble());
    }

    //Direction zone
    reportSettings->setLEDirection(static_cast<ReportSettings::LEDirection>(params->value("directionOfLE").toInt()));
    reportSettings->setZone(params->value("zoneLE").toInt(), params->value("zoneTE").toInt(),
        static_cast<ReportSettings::MeasureType>(params->value("measureType").toInt()));

    //Pre-process
    reportSettings->setPreProcessSettings(params->value("needSortPoints") == "true", params->value("needRemoveEqualPoints") == "true",
        params->value("needRadiusCompensation") == "true", params->value("needUse3DVectors") == "true");
    if(params->value("needRemoveEqualPoints") == "true") {
        reportSettings->setLimitForEqualPoints(params->value("limitForEqualPoints").toDouble());
    }
    if(params->value("needRadiusCompensation") == "true") {
        reportSettings->setRadiusCompensation(params->value("radiusCompensation").toDouble());
    }

    //Global, LE, TE form
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
    reportSettings->setTypesOfTableValue(QMap<DimFigure::ValueType, bool>{
        { static_cast<DimFigure::ValueType>(18), params->value("MinMax") == "true" },
        { static_cast<DimFigure::ValueType>(19), params->value("Form") == "true" },
        { static_cast<DimFigure::ValueType>(20), params->value("Min") == "true" },
        { static_cast<DimFigure::ValueType>(21), params->value("Max") == "true" },
        { static_cast<DimFigure::ValueType>(22), params->value("MaxAbs") == "true" },
        { static_cast<DimFigure::ValueType>(23), params->value("SupUT") == "true" },
        { static_cast<DimFigure::ValueType>(24), params->value("InfLT") == "true" },
    });
    reportSettings->setEvaluation(static_cast<ReportSettings::Evaluation>(params->value("evaluationPlace").toInt()),
        static_cast<ReportSettings::Evaluation>(params->value("evaluationDirection").toInt()));

    //Single report
    if(params->value("needPrintWithTemplate") == "true") {
        reportSettings->setNeedPrintWithTemplate(true);
        reportSettings->setReportTemplate(static_cast<ReportSettings::Template>(params->value("reportTemplate").toInt()));
        reportSettings->setComment(params->value("comment"));
    }

    ////Part Data
    //reportSettings->setPartData(params->value("description"), params->value("drawing"), params->value("orderNumber"), params->value("partNumber"),
    //    params->value("projectOperator"), params->value("note"), params->value("machine"), params->value("tool"), params->value("fixturing"),
    //    params->value("batch"), params->value("supplier"), params->value("revision"));


    return reportSettings;
}

ReportSettings::ReportSettings() {
    //Calculate parameters
    _onMaxWidth = false;
    _nominalMaxWidth = 0;
    _upTolMaxWidth = 0;
    _downTolMaxWidth = 0;
    _measuredMaxWidth = 0;
    _deviationMaxWidth = 0;

    _onXMaxWidth = false;
    _nominalXMaxWidth = 0;
    _upTolXMaxWidth = 0;
    _downTolXMaxWidth = 0;
    _measuredXMaxWidth = 0;
    _deviationXMaxWidth = 0;

    _onYMaxWidth = false;
    _nominalYMaxWidth = 0;
    _upTolYMaxWidth = 0;
    _downTolYMaxWidth = 0;
    _measuredYMaxWidth = 0;
    _deviationYMaxWidth = 0;

    _onContactLineLength = false;
    _nominalContactLineLength = 0;
    _upTolContactLineLength = 0;
    _downTolContactLineLength = 0;
    _measuredContactLineLength = 0;
    _deviationContactLineLength = 0;

    _onLEWidth = false;
    _nominalLEWidth = 0;
    _upTolLEWidth = 0;
    _downTolLEWidth = 0;
    _measuredLEWidth = 0;
    _deviationLEWidth = 0;

    _onTEWidth = false;
    _nominalTEWidth = 0;
    _upTolTEWidth = 0;
    _downTolTEWidth = 0;
    _measuredTEWidth = 0;
    _deviationTEWidth = 0;

    _onLERadius = false;
    _nominalLERadius = 0;
    _upTolLERadius = 0;
    _downTolLERadius = 0;
    _measuredLERadius = 0;
    _deviationLERadius = 0;

    _onTERadius = false;
    _nominalTERadius = 0;
    _upTolTERadius = 0;
    _downTolTERadius = 0;
    _measuredTERadius = 0;
    _deviationTERadius = 0;

    //Direction, zone
    _directionOfLE = LEDirection::Auto;
    _zoneLE = 0;
    _zoneTE = 0;
    _measureType = MeasureType::Percent;

    //Pre-process
    _needSortPoints = false;
    _needRemoveEqualPoints = false;
    _needRadiusCompensation = false;
    _needUse3DVectors = false;
    _limitForEqualPoints = 0;
    _radiusCompensation = 0;

    //Global form
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

    //LE, TE
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

    //Form axis
    _evaluationPlace = Evaluation::Nominal;
    _evaluationDirection = Evaluation::Nominal;

    //Single report
    _needPrintWithTemplate = false;
    _comment = "";
    _reportPath = "";
    _reportName = "";
    _reportTemplate = Template::AirfoilReport1;

    ////Part Data
    //_description = "";
    //_drawing = "";
    //_orderNumber = "";
    //_partNumber = "";
    //_projectOperator = "";
    //_note = "";
    //_machine = "";
    //_tool = "";
    //_fixturing = "";
    //_batch = "";
    //_supplier = "";
    //_revision = "";
}

void ReportSettings::setReportTemplate(Template reportTemplate) {
    _reportTemplate = reportTemplate;
}

ReportSettings::Template ReportSettings::reportTemplate() const {
    return _reportTemplate;
}

void ReportSettings::setProfileType(Profile profileType) {
    _profileType = profileType;
}

ReportSettings::Profile ReportSettings::profileType() const {
    return _profileType;
}

void ReportSettings::setGlobalBestFit(GlobalBestFit bestFit) {
    _globalBestFit = bestFit;
}

ReportSettings::GlobalBestFit ReportSettings::globalBestFit() const {
    return _globalBestFit;
}

void ReportSettings::setNominalName(QString name) {
    _nominalName = name;
}

QString ReportSettings::nominalName() const {
    return _nominalName;
}

void ReportSettings::setMeasuredName(QString name) {
    _measuredName = name;
}

QString ReportSettings::measuredName() const {
    return _measuredName;
}

void ReportSettings::setScreenshotOfGlobal(QImage screenshotOfGlobal) {
    _screenshotOfGlobal = screenshotOfGlobal;
}

QImage ReportSettings::screenshotOfGlobal() const {
    return _screenshotOfGlobal;
}

void ReportSettings::setScreenshotOfLE(QImage screenshotOfLE) {
    _screenshotOfLE = screenshotOfLE;
}

QImage ReportSettings::screenshotOfLE() const {
    return _screenshotOfLE;
}

void ReportSettings::setScreenshotOfTE(QImage screenshotOfTE) {
    _screenshotOfTE = screenshotOfTE;
}

QImage ReportSettings::screenshotOfTE() const {
    return _screenshotOfTE;
}

void ReportSettings::setNomMaxWidth(double nominal) {
    _nominalMaxWidth = nominal;
}

void ReportSettings::setMeasMaxWidth(double measured) {
    _measuredMaxWidth = measured;
    _onMaxWidth = true;
    _deviationMaxWidth = _measuredMaxWidth - _nominalMaxWidth;
}

void ReportSettings::setMaxWidthTolerances(double upTol, double downTol) {
    _upTolMaxWidth = upTol;
    _downTolMaxWidth = downTol;
}

bool ReportSettings::onMaxWidth() const {
    return _onMaxWidth;
}

double ReportSettings::nominalMaxWidth() const {
    return _nominalMaxWidth;
}

double ReportSettings::upTolMaxWidth() const {
    return _upTolMaxWidth;
}

double ReportSettings::downTolMaxWidth() const {
    return _downTolMaxWidth;
}
double ReportSettings::measuredMaxWidth() const {
    return _measuredMaxWidth;
}

double ReportSettings::deviationMaxWidth() const {
    return _deviationMaxWidth;
}

void ReportSettings::setNomXMaxWidth(double nominal) {
    _nominalXMaxWidth = nominal;
}

void ReportSettings::setMeasXMaxWidth(double measured) {
    _measuredXMaxWidth = measured;
    _onXMaxWidth = true;
    _deviationXMaxWidth = _measuredXMaxWidth - _nominalXMaxWidth;
}

void ReportSettings::setXMaxWidthTolerances(double upTol, double downTol) {
    _upTolXMaxWidth = upTol;
    _downTolXMaxWidth = downTol;
}

bool ReportSettings::onXMaxWidth() const {
    return _onXMaxWidth;
}

double ReportSettings::nominalXMaxWidth() const {
    return _nominalXMaxWidth;
}

double ReportSettings::upTolXMaxWidth() const {
    return _upTolXMaxWidth;
}

double ReportSettings::downTolXMaxWidth() const {
    return _downTolXMaxWidth;
}

double ReportSettings::measuredXMaxWidth() const {
    return _measuredXMaxWidth;
}

double ReportSettings::deviationXMaxWidth() const {
    return _deviationXMaxWidth;
}

void ReportSettings::setNomYMaxWidth(double nominal) {
    _nominalYMaxWidth = nominal;
}

void ReportSettings::setMeasYMaxWidth(double measured) {
    _measuredYMaxWidth = measured;
    _onYMaxWidth = true;
    _deviationYMaxWidth = _measuredYMaxWidth - _nominalYMaxWidth;
}

void ReportSettings::setYMaxWidthTolerances(double upTol, double downTol) {
    _upTolYMaxWidth = upTol;
    _downTolYMaxWidth = downTol;
}

bool ReportSettings::onYMaxWidth() const {
    return _onYMaxWidth;
}

double ReportSettings::nominalYMaxWidth() const {
    return _nominalYMaxWidth;
}

double ReportSettings::upTolYMaxWidth() const {
    return _upTolYMaxWidth;
}

double ReportSettings::downTolYMaxWidth() const {
    return _downTolYMaxWidth;
}

double ReportSettings::measuredYMaxWidth() const {
    return _measuredYMaxWidth;
}

double ReportSettings::deviationYMaxWidth() const {
    return _deviationYMaxWidth;
}

void ReportSettings::setNomContactLineLength(double nominal) {
    _nominalContactLineLength = nominal;
}

void ReportSettings::setMeasContactLineLength(double measured) {
    _measuredContactLineLength = measured;
    _onContactLineLength = true;
    _deviationContactLineLength = _measuredContactLineLength - _nominalContactLineLength;
}

void ReportSettings::setContactLineLengthTolerances(double upTol, double downTol) {
    _upTolContactLineLength = upTol;
    _downTolContactLineLength = downTol;
}

bool ReportSettings::onContactLineLength() const {
    return _onContactLineLength;
}

double ReportSettings::nominalContactLineLength() const {
    return _nominalContactLineLength;
}

double ReportSettings::upTolContactLineLength() const {
    return _upTolContactLineLength;
}

double ReportSettings::downTolContactLineLength() const {
    return _downTolContactLineLength;
}

double ReportSettings::measuredContactLineLength() const {
    return _measuredContactLineLength;
}

double ReportSettings::deviationContactLineLength() const {
    return _deviationContactLineLength;
}

void ReportSettings::setNomLEWidth(double nominal) {
    _nominalLEWidth = nominal;
}

void ReportSettings::setMeasLEWidth(double measured) {
    _measuredLEWidth = measured;
    _onLEWidth = true;
    _deviationLEWidth = _measuredLEWidth - _nominalLEWidth;
}

void ReportSettings::setLEWidthTolerances(double upTol, double downTol) {
    _upTolLEWidth = upTol;
    _downTolLEWidth = downTol;
}

bool ReportSettings::onLEWidth() const {
    return _onLEWidth;
}

double ReportSettings::nominalLEWidth() const {
    return _nominalLEWidth;
}

double ReportSettings::upTolLEWidth() const {
    return _upTolLEWidth;
}

double ReportSettings::downTolLEWidth() const {
    return _downTolLEWidth;
}

double ReportSettings::measuredLEWidth() const {
    return _measuredLEWidth;
}

double ReportSettings::deviationLEWidth() const {
    return _deviationLEWidth;
}

void ReportSettings::setNomTEWidth(double nominal) {
    _nominalTEWidth = nominal;
}

void ReportSettings::setMeasTEWidth(double measured) {
    _measuredTEWidth = measured;
    _onTEWidth = true;
    _deviationTEWidth = _measuredTEWidth - _nominalTEWidth;
}

void ReportSettings::setTEWidthTolerances(double upTol, double downTol) {
    _upTolTEWidth = upTol;
    _downTolTEWidth = downTol;
}

bool ReportSettings::onTEWidth() const {
    return _onTEWidth;
}

double ReportSettings::nominalTEWidth() const {
    return _nominalTEWidth;
}

double ReportSettings::upTolTEWidth() const {
    return _upTolTEWidth;
}

double ReportSettings::downTolTEWidth() const {
    return _downTolTEWidth;
}

double ReportSettings::measuredTEWidth() const {
    return _measuredTEWidth;
}

double ReportSettings::deviationTEWidth() const {
    return _deviationTEWidth;
}

void ReportSettings::setNomLERadius(double nominal) {
    _nominalLERadius = nominal;
}

void ReportSettings::setMeasLERadius(double measured) {
    _measuredLERadius = measured;
    _onLERadius = true;
    _deviationLERadius = _measuredLERadius - _nominalLERadius;
}

void ReportSettings::setLERadiusTolerances(double upTol, double downTol) {
    _upTolLERadius = upTol;
    _downTolLERadius = downTol;
}

bool ReportSettings::onLERadius() const {
    return _onLERadius;
}

double ReportSettings::nominalLERadius() const {
    return _nominalLERadius;
}

double ReportSettings::upTolLERadius() const {
    return _upTolLERadius;
}

double ReportSettings::downTolLERadius() const {
    return _downTolLERadius;
}

double ReportSettings::measuredLERadius() const {
    return _measuredLERadius;
}

double ReportSettings::deviationLERadius() const {
    return _deviationLERadius;
}

void ReportSettings::setNomTERadius(double nominal) {
    _nominalTERadius = nominal;
}

void ReportSettings::setMeasTERadius(double measured) {
    _measuredTERadius = measured;
    _onTERadius = true;
    _deviationTERadius = _measuredTERadius - _nominalTERadius;
}

void ReportSettings::setTERadiusTolerances(double upTol, double downTol) {
    _upTolTERadius = upTol;
    _downTolTERadius = downTol;
}

bool ReportSettings::onTERadius() const {
    return _onTERadius;
}

double ReportSettings::nominalTERadius() const {
    return _nominalTERadius;
}

double ReportSettings::upTolTERadius() const {
    return _upTolTERadius;
}

double ReportSettings::downTolTERadius() const {
    return _downTolTERadius;
}

double ReportSettings::measuredTERadius() const {
    return _measuredTERadius;
}

double ReportSettings::deviationTERadius() const {
    return _deviationTERadius;
}

void ReportSettings::setZone(int zoneLE, int zoneTE, MeasureType type) {
    _zoneLE = zoneLE;
    _zoneTE = zoneTE;
    _measureType = type;
}

int ReportSettings::zoneLE() const {
    return _zoneLE;
}

int ReportSettings::zoneTE() const {
    return _zoneTE;
}

ReportSettings::MeasureType ReportSettings::measureType() const {
    return _measureType;
}

void ReportSettings::setNeedAdditionalFigures(bool needMaxDiam, bool needMCL, bool needContactLine) {
    _needMaxDiameter = needMaxDiam;
    _needMCL = needMCL;
    _needContactLine = needContactLine;
}

bool ReportSettings::needMaxDiameter() const {
    return _needMaxDiameter;
}

bool ReportSettings::needMCL() const {
    return _needMCL;
}

bool ReportSettings::needContactLine() const {
    return _needContactLine;
}

void ReportSettings::setBestFitType(BestFitType type) {
    _bestFitType = type;
}

ReportSettings::BestFitType ReportSettings::bestFitType() const {
    return _bestFitType;
}

void ReportSettings::setBestFitValues(double xShift, double yShift, double rotation) {
    _xShift = xShift;
    _yShift = yShift;
    _rotation = rotation;
}

void ReportSettings::setBestFitValues(double xShiftCV, double yShiftCV, double rotationCV, double xShiftCC, double yShiftCC, double rotationCC) {
    _xShiftCV = xShiftCV;
    _yShiftCV = yShiftCV;
    _rotationCV = rotationCV;
    _xShiftCC = xShiftCC;
    _yShiftCC = yShiftCC;
    _rotationCC = rotationCC;
}

double ReportSettings::xShift() const {
    return _xShift;
}

double ReportSettings::yShift() const {
    return _yShift;
}

double ReportSettings::rotation() const {
    return _rotation;
}

double ReportSettings::xShiftCV() const {
    return _xShiftCV;
}

double ReportSettings::yShiftCV() const {
    return _yShiftCV;
}

double ReportSettings::rotationCV() const {
    return _rotationCV;
}

double ReportSettings::xShiftCC() const {
    return _xShiftCC;
}

double ReportSettings::yShiftCC() const {
    return _yShiftCC;
}

double ReportSettings::rotationCC() const {
    return _rotationCC;
}

void ReportSettings::setAmplification(double globalAmp, double ampOfLE, double ampOfTE) {
    _globalAmplification = globalAmp;
    _amplificationOfLE = ampOfLE;
    _amplificationOfTE = ampOfTE;
}

double ReportSettings::globalAmplification() const {
    return _globalAmplification;
}

void ReportSettings::setEdgesBestFit(EdgeBestFit bestFitOfLE, EdgeBestFit bestFitOfTE) {
    _bestFitOfLE = bestFitOfLE;
    _bestFitOfTE = bestFitOfTE;
}

ReportSettings::EdgeBestFit ReportSettings::bestFitOfLE() const {
    return _bestFitOfLE;
}

ReportSettings::EdgeBestFit ReportSettings::bestFitOfTE() const {
    return _bestFitOfTE;
}

double ReportSettings::amplificationOfLE() const {
    return _amplificationOfLE;
}

double ReportSettings::amplificationOfTE() const {
    return _amplificationOfTE;
}

void ReportSettings::setAxisDisplayType(Axis globalType, Axis typeOfLE, Axis typeOfTE) {
    _globalAxisType = globalType;
    _axisTypeOfLE = typeOfLE;
    _axisTypeOfTE = typeOfTE;
}

ReportSettings::Axis ReportSettings::globalAxisType() const {
    return _globalAxisType;
}

ReportSettings::Axis ReportSettings::axisTypeOfLE() const {
    return _axisTypeOfLE;
}

ReportSettings::Axis ReportSettings::axisTypeOfTE() const {
    return _axisTypeOfTE;
}

void ReportSettings::setPreProcessSettings(bool needSortPoints, bool needRemoveEqualPoints, bool needRadiusCompensation, bool needUse3DVectors) {
    _needSortPoints = needSortPoints;
    _needRemoveEqualPoints = needRemoveEqualPoints;
    _needRadiusCompensation = needRadiusCompensation;
    _needUse3DVectors = needUse3DVectors;
}

void ReportSettings::setLimitForEqualPoints(double limit) {
    _limitForEqualPoints = limit;
}

bool ReportSettings::needSortPoints() const {
    return _needSortPoints;
}

bool ReportSettings::needRemoveEqualPoints() const {
    return _needRemoveEqualPoints;
}

bool ReportSettings::needRadiusCompensation() const {
    return _needRadiusCompensation;
}

bool ReportSettings::needUse3DVectors() const {
    return _needUse3DVectors;
}

double ReportSettings::limitForEqualPoints() const {
    return _limitForEqualPoints;
}

void ReportSettings::setRadiusCompensation(double radiusCompensation) {
    _radiusCompensation = radiusCompensation;
}

double ReportSettings::radiusCompensation() const {
    return _radiusCompensation;
}

void ReportSettings::setLEDirection(LEDirection direction) {
    _directionOfLE = direction;
}

ReportSettings::LEDirection ReportSettings::directionOfLE() const {
    return _directionOfLE;
}

void ReportSettings::setStretch(bool isLEStretch, bool isTEStretch) {
    _isLEStretch = isLEStretch;
    _isTEStretch = isTEStretch;
}

bool ReportSettings::isLEStretch() const {
    return _isLEStretch;
}

bool ReportSettings::isTEStretch() const {
    return _isTEStretch;
}

void ReportSettings::setComment(const QString &comment) {
    _comment = comment;
}

QString ReportSettings::comment() const {
    return _comment;
}

void ReportSettings::setVisibilityEdges(bool isLEVisible, bool isTEVisible) {
    _isLEVisible = isLEVisible;
    _isTEVisible = isTEVisible;
}

bool ReportSettings::isLEVisible() const {
    return _isLEVisible;
}

bool ReportSettings::isTEVisible() const {
    return _isTEVisible;
}

void ReportSettings::setTypeOfShowDevs(TypeOfShowDevs typeOfLE, TypeOfShowDevs typeOfTE) {
    _typeOfShowDevsLE = typeOfLE;
    _typeOfShowDevsTE = typeOfTE;
}

ReportSettings::TypeOfShowDevs ReportSettings::typeOfShowDevsLE() const {
    return _typeOfShowDevsLE;
}

ReportSettings::TypeOfShowDevs ReportSettings::typeOfShowDevsTE() const {
    return _typeOfShowDevsTE;
}


void ReportSettings::setValueOfSetShowDevs(int valueOfLE, int valueOfTE) {
    _valueOfSetShowDevsLE = valueOfLE;
    _valueOfSetShowDevsTE = valueOfTE;
}

int ReportSettings::valueOfSetShowDevsLE() const {
    return _valueOfSetShowDevsLE;
}

int ReportSettings::valueOfSetShowDevsTE() const {
    return _valueOfSetShowDevsTE;
}

void ReportSettings::setNeedPrintWithTemplate(bool needPrintWithTemplate) {
    _needPrintWithTemplate = needPrintWithTemplate;
}

bool ReportSettings::needPrintWithTemplate() const {
    return _needPrintWithTemplate;
}

void ReportSettings::setTypesOfTableValue(QMap<DimFigure::ValueType, bool> typesOfTableValue) {
    _typesOfTableValue = typesOfTableValue;
}

QMap<DimFigure::ValueType, bool> ReportSettings::typesOfTableValue() const {
    return _typesOfTableValue;
}

void ReportSettings::setEvaluation(Evaluation evaluationPlace, Evaluation evaluationDirection) {
    _evaluationPlace = evaluationPlace;
    _evaluationDirection = evaluationDirection;
}

ReportSettings::Evaluation ReportSettings::evaluationPlace() const {
    return _evaluationPlace;
}

ReportSettings::Evaluation ReportSettings::evaluationDirection() const {
    return _evaluationDirection;
}

void ReportSettings::setReportPath(const QString &reportPath) {
    _reportPath = reportPath;
    _reportName = reportPath.split("/").last();
}

QString ReportSettings::reportPath() const {
    return _reportPath;
}

QString ReportSettings::reportName() const {
    return _reportName;
}

//QString ReportSettings::description() const {
//    return _description;
//}
//
//QString ReportSettings::drawing() const {
//    return _drawing;
//}
//
//QString ReportSettings::orderNumber() const {
//    return _orderNumber;
//}
//
//QString ReportSettings::partNumber() const {
//    return _partNumber;
//}
//
//QString ReportSettings::projectOperator() const {
//    return _projectOperator;
//}
//
//QString ReportSettings::note() const {
//    return _note;
//}
//
//QString ReportSettings::machine() const {
//    return _machine;
//}
//
//QString ReportSettings::tool() const {
//    return _tool;
//}
//
//QString ReportSettings::fixturing() const {
//    return _fixturing;
//}
//
//QString ReportSettings::batch() const {
//    return _batch;
//}
//
//QString ReportSettings::supplier() const {
//    return _supplier;
//}
//
//QString ReportSettings::revision() const {
//    return _revision;
//}

//void ReportSettings::setPartData(QString description, QString drawing, QString orderNumber, QString partNumber, QString projectOperator, QString note,
//    QString machine, QString tool, QString fixturing, QString batch, QString supplier, QString revision) {
//    _description = description;
//    _drawing = drawing;
//    _orderNumber = orderNumber;
//    _partNumber = partNumber;
//    _projectOperator = projectOperator;
//    _note = note;
//    _machine = machine;
//    _tool = tool;
//    _fixturing = fixturing;
//    _batch = batch;
//    _supplier = supplier;
//    _revision = revision;
//}

void ReportSettings::clear() {
    //Calculate parameters
    _onMaxWidth = false;
    _nominalMaxWidth = 0;
    _upTolMaxWidth = 0;
    _downTolMaxWidth = 0;
    _measuredMaxWidth = 0;
    _deviationMaxWidth = 0;

    _onXMaxWidth = false;
    _nominalXMaxWidth = 0;
    _upTolXMaxWidth = 0;
    _downTolXMaxWidth = 0;
    _measuredXMaxWidth = 0;
    _deviationXMaxWidth = 0;

    _onYMaxWidth = false;
    _nominalYMaxWidth = 0;
    _upTolYMaxWidth = 0;
    _downTolYMaxWidth = 0;
    _measuredYMaxWidth = 0;
    _deviationYMaxWidth = 0;

    _onContactLineLength = false;
    _nominalContactLineLength = 0;
    _upTolContactLineLength = 0;
    _downTolContactLineLength = 0;
    _measuredContactLineLength = 0;
    _deviationContactLineLength = 0;

    _onLEWidth = false;
    _nominalLEWidth = 0;
    _upTolLEWidth = 0;
    _downTolLEWidth = 0;
    _measuredLEWidth = 0;
    _deviationLEWidth = 0;

    _onTEWidth = false;
    _nominalTEWidth = 0;
    _upTolTEWidth = 0;
    _downTolTEWidth = 0;
    _measuredTEWidth = 0;
    _deviationTEWidth = 0;

    _onLERadius = false;
    _nominalLERadius = 0;
    _upTolLERadius = 0;
    _downTolLERadius = 0;
    _measuredLERadius = 0;
    _deviationLERadius = 0;

    _onTERadius = false;
    _nominalTERadius = 0;
    _upTolTERadius = 0;
    _downTolTERadius = 0;
    _measuredTERadius = 0;
    _deviationTERadius = 0;

    //Direction, zone
    _directionOfLE = LEDirection::Auto;
    _zoneLE = 0;
    _zoneTE = 0;
    _measureType = MeasureType::Percent;

    //Pre-porcess
    _needSortPoints = false;
    _needRemoveEqualPoints = false;
    _needRadiusCompensation = false;
    _needUse3DVectors = false;
    _limitForEqualPoints = 0;
    _radiusCompensation = 0;
    
    //Global form
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

    //LE, TE
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

    //Form axis
    _evaluationPlace = Evaluation::Nominal;
    _evaluationDirection = Evaluation::Nominal;

    //Single report
    _needPrintWithTemplate = false;
    _comment = "";
    _reportPath = "";
    _reportName = "";
    _reportTemplate = Template::AirfoilReport1;

    ////Part Data
    //_description = "";
    //_drawing = "";
    //_orderNumber = "";
    //_partNumber = "";
    //_projectOperator = "";
    //_note = "";
    //_machine = "";
    //_tool = "";
    //_fixturing = "";
    //_batch = "";
    //_supplier = "";
    //_revision = "";
}

ReportSettings::~ReportSettings() {
}
