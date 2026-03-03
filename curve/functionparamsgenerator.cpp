#include "curve/pch.h"
#include "functionparamsgenerator.h"

Function4Params FunctionParamsGenerator::params4(Project *project, std::shared_ptr<ReportSettings> reportSettings, const Function18Params *params18, bool isClosed) {
    auto evaluationPlace = static_cast<int>(reportSettings->evaluationPlace());
    auto evaluationDirection = static_cast<int>(reportSettings->evaluationDirection());
    auto mclLength = Algorithms::getMiddleCurveLength(reportSettings->nominalName(), params18, project);
    auto nominalTolerance = mclLength / 5.0;
    return Function4Params(nominalTolerance, evaluationPlace, evaluationDirection, isClosed);
}

Function6Params FunctionParamsGenerator::params6(std::shared_ptr<ReportSettings> reportSettings, bool isClosed) {
    auto bestFitType = reportSettings->bestFitType();
    if(bestFitType == ReportSettings::BestFitType::OnlyTranslation) {
        return Function6Params(true, Function6Params::Algorithm::Curve, isClosed, true, true, false);
    } else if(bestFitType == ReportSettings::BestFitType::OnlyRotation) {
        return Function6Params(false, Function6Params::Algorithm::Curve, isClosed, false, false, true);
    } else if(bestFitType == ReportSettings::BestFitType::OnlyXTranslation) {
        return Function6Params(false, Function6Params::Algorithm::Curve, isClosed, true, false, false);
    } else if(bestFitType == ReportSettings::BestFitType::OnlyYTranslation) {
        return Function6Params(false, Function6Params::Algorithm::Curve, isClosed, false, true, false);
    } else if(bestFitType == ReportSettings::BestFitType::XTranslationAndRotation) {
        return Function6Params(true, Function6Params::Algorithm::Curve, isClosed, true, false, true);
    } else if(bestFitType == ReportSettings::BestFitType::YTranslationAndRotation) {
        return Function6Params(true, Function6Params::Algorithm::Curve, isClosed, false, true, true);
    } else if(bestFitType == ReportSettings::BestFitType::TranslationAndRotation){
        return Function6Params(true, Function6Params::Algorithm::Curve, isClosed, true, true, true);
    } else {
        return Function6Params();
    }
}

Function18Params FunctionParamsGenerator::params18(Project *project, std::shared_ptr<ReportSettings> reportSettings) {
    if(reportSettings->measureType() == ReportSettings::MeasureType::Length) {
        auto params18 = Function18Params(0, reportSettings->zoneLE(), reportSettings->zoneTE());
        auto mclLength = Algorithms::getMiddleCurveLength(reportSettings->nominalName(), &params18, project);
        auto zoneLEInPercent = reportSettings->zoneLE() / mclLength * 100;
        auto zoneTEInPercent = reportSettings->zoneTE() / mclLength * 100;
        return Function18Params(static_cast<int>(reportSettings->directionOfLE()), zoneLEInPercent, zoneTEInPercent);
    } else {
        return Function18Params(static_cast<int>(reportSettings->directionOfLE()), reportSettings->zoneLE(), reportSettings->zoneTE());
    }
}

Function21Params FunctionParamsGenerator::params21(std::shared_ptr<ReportSettings> reportSettings, bool isClosed) {
    auto bestFitType = reportSettings->bestFitType();
    if(bestFitType == ReportSettings::BestFitType::OnlyTranslation) {
        return Function21Params(0, true, true, false, isClosed);
    } else if(bestFitType == ReportSettings::BestFitType::OnlyRotation) {
        return Function21Params(0, false, false, true, isClosed);
    } else if(bestFitType == ReportSettings::BestFitType::OnlyXTranslation) {
        return Function21Params(0, true, false, false, isClosed);
    } else if(bestFitType == ReportSettings::BestFitType::OnlyYTranslation) {
        return Function21Params(0, false, true, false, isClosed);
    } else if(bestFitType == ReportSettings::BestFitType::XTranslationAndRotation) {
        return Function21Params(0, true, false, true, isClosed);
    } else if(bestFitType == ReportSettings::BestFitType::YTranslationAndRotation) {
        return Function21Params(0, false, true, true, isClosed);
    } else if(bestFitType == ReportSettings::BestFitType::TranslationAndRotation) {
        return Function21Params(0, true, true, true, isClosed);
    } else {
        return Function21Params();
    }
}