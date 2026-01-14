#include "curve/pch.h"
#include "reportdata.h"

ReportData::ReportData() {
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

    _onChordLength = false;
    _nominalChordLength = 0;
    _upTolChordLength = 0;
    _downTolChordLength = 0;
    _measuredChordLength = 0;
    _deviationChordLength = 0;

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
    _dimensionType = Dimension::Percent;

    //Pre-porcess
    _needSortPoints = false;
    _needRemoveEqualPoints = false;
    _needRadiusCompensation = false;
    _needUse3DVectors = false;
    _limitForEqualPoints = 0;

    //Global form
    _needMaxDiameter = false;
    _needMCL = false;
    _needChord = false;
    _profileType = Profile::Whole;
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
    _reportName = "";
    _reportTemplate = Template::AirfoilReport1;
}

void ReportData::setReportTemplate(Template reportTemplate) {
    _reportTemplate = reportTemplate;
}

ReportData::Template ReportData::reportTemplate() const {
    return _reportTemplate;
}

void ReportData::setProfileType(Profile profileType) {
    _profileType = profileType;
}

ReportData::Profile ReportData::profileType() const {
    return _profileType;
}

void ReportData::setNomMaxWidth(double nominal) {
    _nominalMaxWidth = nominal;
}

void ReportData::setMeasMaxWidth(double measured) {
    _measuredMaxWidth = measured;
    _onMaxWidth = true;
    _deviationMaxWidth = _measuredMaxWidth - _nominalMaxWidth;
}

void ReportData::setMaxWidthTolerances(double upTol, double downTol) {
    _upTolMaxWidth = upTol;
    _downTolMaxWidth = downTol;
}

bool ReportData::onMaxWidth() const {
    return _onMaxWidth;
}

double ReportData::nominalMaxWidth() const {
    return _nominalMaxWidth;
}

double ReportData::upTolMaxWidth() const {
    return _upTolMaxWidth;
}

double ReportData::downTolMaxWidth() const {
    return _downTolMaxWidth;
}
double ReportData::measuredMaxWidth() const {
    return _measuredMaxWidth;
}

double ReportData::deviationMaxWidth() const {
    return _deviationMaxWidth;
}

void ReportData::setNomXMaxWidth(double nominal) {
    _nominalXMaxWidth = nominal;
}

void ReportData::setMeasXMaxWidth(double measured) {
    _measuredXMaxWidth = measured;
    _onXMaxWidth = true;
    _deviationXMaxWidth = _measuredXMaxWidth - _nominalXMaxWidth;
}

void ReportData::setXMaxWidthTolerances(double upTol, double downTol) {
    _upTolXMaxWidth = upTol;
    _downTolXMaxWidth = downTol;
}

bool ReportData::onXMaxWidth() const {
    return _onXMaxWidth;
}

double ReportData::nominalXMaxWidth() const {
    return _nominalXMaxWidth;
}

double ReportData::upTolXMaxWidth() const {
    return _upTolXMaxWidth;
}

double ReportData::downTolXMaxWidth() const {
    return _downTolXMaxWidth;
}

double ReportData::measuredXMaxWidth() const {
    return _measuredXMaxWidth;
}

double ReportData::deviationXMaxWidth() const {
    return _deviationXMaxWidth;
}

void ReportData::setNomYMaxWidth(double nominal) {
    _nominalYMaxWidth = nominal;
}

void ReportData::setMeasYMaxWidth(double measured) {
    _measuredYMaxWidth = measured;
    _onYMaxWidth = true;
    _deviationYMaxWidth = _measuredYMaxWidth - _nominalYMaxWidth;
}

void ReportData::setYMaxWidthTolerances(double upTol, double downTol) {
    _upTolYMaxWidth = upTol;
    _downTolYMaxWidth = downTol;
}

bool ReportData::onYMaxWidth() const {
    return _onYMaxWidth;
}

double ReportData::nominalYMaxWidth() const {
    return _nominalYMaxWidth;
}

double ReportData::upTolYMaxWidth() const {
    return _upTolYMaxWidth;
}

double ReportData::downTolYMaxWidth() const {
    return _downTolYMaxWidth;
}

double ReportData::measuredYMaxWidth() const {
    return _measuredYMaxWidth;
}

double ReportData::deviationYMaxWidth() const {
    return _deviationYMaxWidth;
}

void ReportData::setNomChordLength(double nominal) {
    _nominalChordLength = nominal;
}

void ReportData::setMeasChordLength(double measured) {
    _measuredChordLength = measured;
    _onChordLength = true;
    _deviationChordLength = _measuredChordLength - _nominalChordLength;
}

void ReportData::setChordLengthTolerances(double upTol, double downTol) {
    _upTolChordLength = upTol;
    _downTolChordLength = downTol;
}

bool ReportData::onChordLength() const {
    return _onChordLength;
}

double ReportData::nominalChordLength() const {
    return _nominalChordLength;
}

double ReportData::upTolChordLength() const {
    return _upTolChordLength;
}

double ReportData::downTolChordLength() const {
    return _downTolChordLength;
}

double ReportData::measuredChordLength() const {
    return _measuredChordLength;
}

double ReportData::deviationChordLength() const {
    return _deviationChordLength;
}

void ReportData::setNomLEWidth(double nominal) {
    _nominalLEWidth = nominal;
}

void ReportData::setMeasLEWidth(double measured) {
    _measuredLEWidth = measured;
    _onLEWidth = true;
    _deviationLEWidth = _measuredLEWidth - _nominalLEWidth;
}

void ReportData::setLEWidthTolerances(double upTol, double downTol) {
    _upTolLEWidth = upTol;
    _downTolLEWidth = downTol;
}

bool ReportData::onLEWidth() const {
    return _onLEWidth;
}

double ReportData::nominalLEWidth() const {
    return _nominalLEWidth;
}

double ReportData::upTolLEWidth() const {
    return _upTolLEWidth;
}

double ReportData::downTolLEWidth() const {
    return _downTolLEWidth;
}

double ReportData::measuredLEWidth() const {
    return _measuredLEWidth;
}

double ReportData::deviationLEWidth() const {
    return _deviationLEWidth;
}

void ReportData::setNomTEWidth(double nominal) {
    _nominalTEWidth = nominal;
}

void ReportData::setMeasTEWidth(double measured) {
    _measuredTEWidth = measured;
    _onTEWidth = true;
    _deviationTEWidth = _measuredTEWidth - _nominalTEWidth;
}

void ReportData::setTEWidthTolerances(double upTol, double downTol) {
    _upTolTEWidth = upTol;
    _downTolTEWidth = downTol;
}

bool ReportData::onTEWidth() const {
    return _onTEWidth;
}

double ReportData::nominalTEWidth() const {
    return _nominalTEWidth;
}

double ReportData::upTolTEWidth() const {
    return _upTolTEWidth;
}

double ReportData::downTolTEWidth() const {
    return _downTolTEWidth;
}

double ReportData::measuredTEWidth() const {
    return _measuredTEWidth;
}

double ReportData::deviationTEWidth() const {
    return _deviationTEWidth;
}

void ReportData::setNomLERadius(double nominal) {
    _nominalLERadius = nominal;
}

void ReportData::setMeasLERadius(double measured) {
    _measuredLERadius = measured;
    _onLERadius = true;
    _deviationLERadius = _measuredLERadius - _nominalLERadius;
}

void ReportData::setLERadiusTolerances(double upTol, double downTol) {
    _upTolLERadius = upTol;
    _downTolLERadius = downTol;
}

bool ReportData::onLERadius() const {
    return _onLERadius;
}

double ReportData::nominalLERadius() const {
    return _nominalLERadius;
}

double ReportData::upTolLERadius() const {
    return _upTolLERadius;
}

double ReportData::downTolLERadius() const {
    return _downTolLERadius;
}

double ReportData::measuredLERadius() const {
    return _measuredLERadius;
}

double ReportData::deviationLERadius() const {
    return _deviationLERadius;
}

void ReportData::setNomTERadius(double nominal) {
    _nominalTERadius = nominal;
}

void ReportData::setMeasTERadius(double measured) {
    _measuredTERadius = measured;
    _onTERadius = true;
    _deviationTERadius = _measuredTERadius - _nominalTERadius;
}

void ReportData::setTERadiusTolerances(double upTol, double downTol) {
    _upTolTERadius = upTol;
    _downTolTERadius = downTol;
}

bool ReportData::onTERadius() const {
    return _onTERadius;
}

double ReportData::nominalTERadius() const {
    return _nominalTERadius;
}

double ReportData::upTolTERadius() const {
    return _upTolTERadius;
}

double ReportData::downTolTERadius() const {
    return _downTolTERadius;
}

double ReportData::measuredTERadius() const {
    return _measuredTERadius;
}

double ReportData::deviationTERadius() const {
    return _deviationTERadius;
}

void ReportData::setZone(int zoneLE, int zoneTE, Dimension type) {
    _zoneLE = zoneLE;
    _zoneTE = zoneTE;
    _dimensionType = type;
}

int ReportData::zoneLE() const {
    return _zoneLE;
}

int ReportData::zoneTE() const {
    return _zoneTE;
}

ReportData::Dimension ReportData::dimension() const {
    return _dimensionType;
}

void ReportData::setNeedFigures(bool needMaxDiameter, bool needMCL, bool needChord) {
    _needMaxDiameter = needMaxDiameter;
    _needMCL = needMCL;
    _needChord = needChord;
}

bool ReportData::needMaxDiameter() const {
    return _needMaxDiameter;
}

bool ReportData::needMCL() const {
    return _needMCL;
}

bool ReportData::needChord() const {
    return _needChord;
}

void ReportData::setBestFitType(BestFitType type) {
    _bestFitType = type;
}

ReportData::BestFitType ReportData::bestFitType() const {
    return _bestFitType;
}

void ReportData::setBestFitValues(double xShift, double yShift, double rotation) {
    _xShift = xShift;
    _yShift = yShift;
    _rotation = rotation;
}

void ReportData::setBestFitValues(double xShiftCV, double yShiftCV, double rotationCV, double xShiftCC, double yShiftCC, double rotationCC) {
    _xShiftCV = xShiftCV;
    _yShiftCV = yShiftCV;
    _rotationCV = rotationCV;
    _xShiftCC = xShiftCC;
    _yShiftCC = yShiftCC;
    _rotationCC = rotationCC;
}

double ReportData::xShift() const {
    return _xShift;
}

double ReportData::yShift() const {
    return _yShift;
}

double ReportData::rotation() const {
    return _rotation;
}

double ReportData::xShiftCV() const {
    return _xShiftCV;
}

double ReportData::yShiftCV() const {
    return _yShiftCV;
}

double ReportData::rotationCV() const {
    return _rotationCV;
}

double ReportData::xShiftCC() const {
    return _xShiftCC;
}

double ReportData::yShiftCC() const {
    return _yShiftCC;
}

double ReportData::rotationCC() const {
    return _rotationCC;
}

void ReportData::setAmplification(double globalAmp, double ampOfLE, double ampOfTE) {
    _globalAmplification = globalAmp;
    _amplificationOfLE = ampOfLE;
    _amplificationOfTE = ampOfTE;
}

double ReportData::globalAmplification() const {
    return _globalAmplification;
}

double ReportData::amplificationOfLE() const {
    return _amplificationOfLE;
}

double ReportData::amplificationOfTE() const {
    return _amplificationOfTE;
}

void ReportData::setAxisDisplayType(Axis globalType, Axis typeOfLE, Axis typeOfTE) {
    _globalAxisType = globalType;
    _axisTypeOfLE = typeOfLE;
    _axisTypeOfTE = typeOfTE;
}

ReportData::Axis ReportData::globalAxisType() const {
    return _globalAxisType;
}

ReportData::Axis ReportData::axisTypeOfLE() const {
    return _axisTypeOfLE;
}

ReportData::Axis ReportData::axisTypeOfTE() const {
    return _axisTypeOfTE;
}

void ReportData::setPreProcessSettings(bool needSortPoints, bool needRemoveEqualPoints, bool needRadiusCompensation, bool needUse3DVectors) {
    _needSortPoints = needSortPoints;
    _needRemoveEqualPoints = needRemoveEqualPoints;
    _needRadiusCompensation = needRadiusCompensation;
    _needUse3DVectors = needUse3DVectors;
}

void ReportData::setLimitForEqualPoints(double limit) {
    _limitForEqualPoints = limit;
}

bool ReportData::needSortPoints() const {
    return _needSortPoints;
}

bool ReportData::needRemoveEqualPoints() const {
    return _needRemoveEqualPoints;
}

bool ReportData::needRadiusCompensation() const {
    return _needRadiusCompensation;
}

bool ReportData::needUse3DVectors() const {
    return _needUse3DVectors;
}

double ReportData::limitForEqualPoints() const {
    return _limitForEqualPoints;
}

void ReportData::setLEDirection(LEDirection direction) {
    _directionOfLE = direction;
}

ReportData::LEDirection ReportData::directionOfLE() const {
    return _directionOfLE;
}

void ReportData::setStretch(bool isLEStretch, bool isTEStretch) {
    _isLEStretch = isLEStretch;
    _isTEStretch = isTEStretch;
}

bool ReportData::isLEStretch() const {
    return _isLEStretch;
}

bool ReportData::isTEStretch() const {
    return _isTEStretch;
}

void ReportData::setComment(const QString &comment) {
    _comment = comment;
}

QString ReportData::comment() const {
    return _comment;
}

void ReportData::setVisibilityEdges(bool isLEVisible, bool isTEVisible) {
    _isLEVisible = isLEVisible;
    _isTEVisible = isTEVisible;
}

bool ReportData::isLEVisible() const {
    return _isLEVisible;
}

bool ReportData::isTEVisible() const {
    return _isTEVisible;
}

void ReportData::setTypeOfShowDevs(TypeOfShowDevs typeOfLE, TypeOfShowDevs typeOfTE) {
    _typeOfShowDevsLE = typeOfLE;
    _typeOfShowDevsTE = typeOfTE;
}

ReportData::TypeOfShowDevs ReportData::typeOfShowDevsLE() const {
    return _typeOfShowDevsLE;
}

ReportData::TypeOfShowDevs ReportData::typeOfShowDevsTE() const {
    return _typeOfShowDevsTE;
}


void ReportData::setValueOfSetShowDevs(int valueOfLE, int valueOfTE) {
    _valueOfSetShowDevsLE = valueOfLE;
    _valueOfSetShowDevsTE = valueOfTE;
}

int ReportData::valueOfSetShowDevsLE() const {
    return _valueOfSetShowDevsLE;
}

int ReportData::valueOfSetShowDevsTE() const {
    return _valueOfSetShowDevsTE;
}

void ReportData::setNeedPrintWithTemplate(bool needPrintWithTemplate) {
    _needPrintWithTemplate = needPrintWithTemplate;
}

bool ReportData::needPrintWithTemplate() const {
    return _needPrintWithTemplate;
}

void ReportData::setTypesOfTableValue(QMap<TypeOfTableValue, bool> typesOfTableValue) {
    _typesOfTableValue = typesOfTableValue;
}

QMap<ReportData::TypeOfTableValue, bool> ReportData::typesOfTableValue() const {
    return _typesOfTableValue;
}

void ReportData::setEvaluation(Evaluation evaluationPlace, Evaluation evaluationDirection) {
    _evaluationPlace = evaluationPlace;
    _evaluationDirection = evaluationDirection;
}

ReportData::Evaluation ReportData::evaluationPlace() const {
    return _evaluationPlace;
}

ReportData::Evaluation ReportData::evaluationDirection() const {
    return _evaluationDirection;
}

void ReportData::setReportName(const QString &reportName) {
    _reportName = reportName;
}

QString ReportData::reportName() const {
    return _reportName;
}

void ReportData::clear() {
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

    _onChordLength = false;
    _nominalChordLength = 0;
    _upTolChordLength = 0;
    _downTolChordLength = 0;
    _measuredChordLength = 0;
    _deviationChordLength = 0;

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
    _dimensionType = Dimension::Percent;

    //Pre-porcess
    _needSortPoints = false;
    _needRemoveEqualPoints = false;
    _needRadiusCompensation = false;
    _needUse3DVectors = false;
    _limitForEqualPoints = 0;

    //Global form
    _needMaxDiameter = false;
    _needMCL = false;
    _needChord = false;
    _profileType = Profile::Whole;
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
    _reportName = "";
    _reportTemplate = Template::AirfoilReport1;
}

ReportData::~ReportData() {
}
