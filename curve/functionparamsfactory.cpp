#include "curve/pch.h"

#include "algorithms.h"
#include "functionparamsfactory.h"
#include "iturbineparameter.h"

using OptionGlobalBestFit = ReportSettings::OptionGlobalBestFit;

FunctionParamsFactory::FunctionParamsFactory(const std::shared_ptr<ReportSettings> reportSettings)
{
    _project = &Project::instance();

    _reportSettings = reportSettings;
}

Function4Params FunctionParamsFactory::params4(const Function18Params& params18, bool isClosed)
{
    auto evaluationPlace = static_cast<int>(_reportSettings->evaluationPlace());
    auto evaluationDirection = static_cast<int>(_reportSettings->evaluationDirection());
    double length = Algorithms::getMiddleCurveLength(_reportSettings->nominalName(), params18);
    double nominalTolerance = length / 5.0;

    return Function4Params(nominalTolerance, evaluationPlace, evaluationDirection, isClosed);
}

Function6Params FunctionParamsFactory::params6ForGlobalFit()
{
    bool needMinimize = true;
    bool isClosed = true;
    auto method = Function6Params::Method::Curve;

    Function6Params function6Params = createParams6UsingOption(needMinimize, isClosed, method);

    setConstraintsToParams(function6Params);

    return function6Params;
}

Function6Params FunctionParamsFactory::params6ForLocalFit()
{
    bool needMinimize = true;
    bool isClosed = false;
    auto method = Function6Params::Method::Curve;

    Function6Params function6Params = createParams6UsingOption(needMinimize, isClosed, method);

    return function6Params;
}

Function6Params FunctionParamsFactory::createParams6UsingOption(bool needMinimize, bool isClosed, Function6Params::Method method)
{
    OptionGlobalBestFit optionGlobalBestFit = _reportSettings->optionGlobalBestFit();

    Function6Params function6Params;

    switch(optionGlobalBestFit) {
        case OptionGlobalBestFit::TranslationAndRotation: {
            function6Params = Function6Params(needMinimize, method, isClosed, true, true, true);
            break;
        }
        case OptionGlobalBestFit::XTranslationAndRotation: {
            function6Params = Function6Params(needMinimize, method, isClosed, true, false, true);
            break;
        }
        case OptionGlobalBestFit::YTranslationAndRotation: {
            function6Params = Function6Params(needMinimize, method, isClosed, false, true, true);
            break;
        }
        case OptionGlobalBestFit::OnlyTranslation: {
            function6Params = Function6Params(needMinimize, method, isClosed, true, true, false);
            break;
        }
        case OptionGlobalBestFit::OnlyXTranslation: {
            function6Params = Function6Params(needMinimize, method, isClosed, true, false, false);
            break;
        }
        case OptionGlobalBestFit::OnlyYTranslation: {
            function6Params = Function6Params(needMinimize, method, isClosed, false, true, false);
            break;
        }
        case OptionGlobalBestFit::OnlyRotation: {
            function6Params = Function6Params(needMinimize, method, isClosed, false, false, true);
            break;
        }
    }

    return function6Params;
}

Function18Params FunctionParamsFactory::params18()
{
    auto leadingEdgeDirection = static_cast<int>(_reportSettings->leadingEdgeDirection());
    double leadingEdgeZone = _reportSettings->leadingEdgeZone();
    double trailingEdgeZone = _reportSettings->trailingEdgeZone();

    if(_reportSettings->measureType() == ReportSettings::MeasureType::Length) {
        auto params18 = Function18Params(leadingEdgeDirection, leadingEdgeZone, trailingEdgeZone);
        double length = Algorithms::getMiddleCurveLength(_reportSettings->nominalName(), params18);
        double leadingEdgeZoneInPercent = _reportSettings->leadingEdgeZone() / length * 100;
        double trailingEdgeZoneInPercent = _reportSettings->trailingEdgeZone() / length * 100;

        return Function18Params(leadingEdgeDirection, leadingEdgeZoneInPercent, trailingEdgeZoneInPercent);
    }
    else {
        return Function18Params(leadingEdgeDirection, leadingEdgeZone, trailingEdgeZone);
    }
}

Function21Params FunctionParamsFactory::params21ForGlobalFit()
{
    bool isClosed = false;
    int limInterpMethod = 0;

    Function21Params function21Params = createParams21UsingOption(isClosed, limInterpMethod);

    setConstraintsToParams(function21Params);

    return function21Params;
}

Function21Params FunctionParamsFactory::params21ForLocalFit()
{
    bool isClosed = false;
    int limInterpMethod = 0;

    Function21Params function21Params = createParams21UsingOption(isClosed, limInterpMethod);

    return function21Params;
}

Function21Params FunctionParamsFactory::createParams21UsingOption(bool isClosed, int limInterpMethod)
{
    OptionGlobalBestFit optionGlobalBestFit = _reportSettings->optionGlobalBestFit();

    Function21Params function21Params;

    switch(optionGlobalBestFit) {
        case OptionGlobalBestFit::TranslationAndRotation: {
            function21Params = Function21Params(isClosed, limInterpMethod, true, true, true);
            break;
        }
        case OptionGlobalBestFit::XTranslationAndRotation: {
            function21Params = Function21Params(isClosed, limInterpMethod, true, false, true);
            break;
        }
        case OptionGlobalBestFit::YTranslationAndRotation: {
            function21Params = Function21Params(isClosed, limInterpMethod, false, true, true);
            break;
        }
        case OptionGlobalBestFit::OnlyTranslation: {
            function21Params = Function21Params(isClosed, limInterpMethod, true, true, false);
            break;
        }
        case OptionGlobalBestFit::OnlyXTranslation: {
            function21Params = Function21Params(isClosed, limInterpMethod, true, false, false);
            break;
        }
        case OptionGlobalBestFit::OnlyYTranslation: {
            function21Params = Function21Params(isClosed, limInterpMethod, false, true, false);
            break;
        }
        case OptionGlobalBestFit::OnlyRotation: {
            function21Params = Function21Params(isClosed, limInterpMethod, false, false, true);
            break;
        }
    }

    return function21Params;
}

template<class T>
inline void FunctionParamsFactory::setConstraintsToParams(T& functionParams)
{
    using ParameterType = ITurbineParameter::Type;

    auto& turbineParams = _reportSettings->turbineParameters();
    auto xShift = turbineParams.value(ParameterType::ShiftX, QList<ITurbineParameter*>());
    auto yShift = turbineParams.value(ParameterType::ShiftY, QList<ITurbineParameter*>());
    auto turn = turbineParams.value(ParameterType::Turn, QList<ITurbineParameter*>());

    if(!xShift.isEmpty()) {
        ITurbineParameter* xShiftParameter = xShift.first();
        bool needXShift = xShiftParameter->extraParam2().toInt() == 1
            ? true
            : false;

        if(needXShift) {
            double xShiftLT = xShiftParameter->lowerTolerance();
            double xShiftUT = xShiftParameter->upperTolerance();

            functionParams.setHorizontalConstraint(xShiftLT, xShiftUT);
        }
    }

    if(!yShift.isEmpty()) {
        ITurbineParameter* yShiftParameter = yShift.first();
        bool needYShift = yShiftParameter->extraParam2().toInt() == 1
            ? true
            : false;

        if(needYShift) {
            double yShiftLT = yShiftParameter->lowerTolerance();
            double yShiftUT = yShiftParameter->upperTolerance();

            functionParams.setVerticalConstraint(yShiftLT, yShiftUT);
        }
    }

    if(!turn.isEmpty()) {
        ITurbineParameter* turnParameter = turn.first();
        bool needTurn = turnParameter->extraParam2().toInt() == 1
            ? true
            : false;

        if(needTurn) {
            double turnLT = turnParameter->lowerTolerance();
            double turnUT = turnParameter->upperTolerance();

            functionParams.setRotationConstraint(turnLT, turnUT);
        }
    }
}
