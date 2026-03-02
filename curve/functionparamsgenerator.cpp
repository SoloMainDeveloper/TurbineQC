#include "curve/pch.h"
#include "functionparamsgenerator.h"

Function4Params FunctionParamsGenerator::params4(std::shared_ptr<ReportSettings> reportSettings, bool isClosed) {
    auto evaluationPlace = static_cast<int>(reportSettings->evaluationPlace());
    auto evaluationDirection = static_cast<int>(reportSettings->evaluationDirection());
    return Function4Params(0, evaluationPlace, evaluationDirection, isClosed);
}

Function6Params FunctionParamsGenerator::params6(std::shared_ptr<ReportSettings> reportSettings) {
    auto bestFitType = reportSettings->bestFitType();
    if(bestFitType == ReportSettings::BestFitType::OnlyTranslation) {
        return Function6Params(true, Function6Params::Algorithm::Curve, true, true, true, false);
    } else if(bestFitType == ReportSettings::BestFitType::OnlyRotation) {
        return Function6Params(false, Function6Params::Algorithm::Curve, true, false, false, true);
    } else if(bestFitType == ReportSettings::BestFitType::OnlyXTranslation) {
        return Function6Params(false, Function6Params::Algorithm::Curve, true, true, false, false);
    } else if(bestFitType == ReportSettings::BestFitType::OnlyYTranslation) {
        return Function6Params(false, Function6Params::Algorithm::Curve, true, false, true, false);
    } else if(bestFitType == ReportSettings::BestFitType::XTranslationAndRotation) {
        return Function6Params(true, Function6Params::Algorithm::Curve, true, true, false, true);
    } else if(bestFitType == ReportSettings::BestFitType::YTranslationAndRotation) {
        return Function6Params(true, Function6Params::Algorithm::Curve, true, false, true, true);
    } else {
        return Function6Params();
    }
}

Function18Params FunctionParamsGenerator::params18(Project *project, std::shared_ptr<ReportSettings> reportSettings) {
    if(reportSettings->measureType() == ReportSettings::MeasureType::Length) {
        auto params18 = Function18Params(0, reportSettings->zoneLE(), reportSettings->zoneTE());
        auto contactLineLength = Algorithms::getContactLineLength(reportSettings->nominalName(), &params18, project);
        auto zoneLEInPercent = reportSettings->zoneLE() / contactLineLength * 100;
        auto zoneTEInPercent = reportSettings->zoneTE() / contactLineLength * 100;
        return Function18Params(static_cast<int>(reportSettings->directionOfLE()), zoneLEInPercent, zoneTEInPercent);
    //return Function18Params(static_cast<int>(reportSettings->directionOfLE()), 1.853276, 1.853276);
    } else {
        return Function18Params(static_cast<int>(reportSettings->directionOfLE()), reportSettings->zoneLE(), reportSettings->zoneTE());
    }
}

Function21Params FunctionParamsGenerator::params21(std::shared_ptr<ReportSettings> reportSettings) {
    auto bestFitType = reportSettings->bestFitType();
    if(bestFitType == ReportSettings::BestFitType::OnlyTranslation) {
        return Function21Params(0, true, true, false);
    } else if(bestFitType == ReportSettings::BestFitType::OnlyRotation) {
        return Function21Params(0, false, false, true);
    } else if(bestFitType == ReportSettings::BestFitType::OnlyXTranslation) {
        return Function21Params(0, true, false, false);
    } else if(bestFitType == ReportSettings::BestFitType::OnlyYTranslation) {
        return Function21Params(0, false, true, false);
    } else if(bestFitType == ReportSettings::BestFitType::XTranslationAndRotation) {
        return Function21Params(0, true, false, true);
    } else if(bestFitType == ReportSettings::BestFitType::YTranslationAndRotation) {
        return Function21Params(0, false, true, true);
    } else {
        return Function21Params();
    }
}