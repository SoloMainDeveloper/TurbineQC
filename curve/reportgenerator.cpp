#include "curve/pch.h"
#include "reportgenerator.h"

void ReportGenerator::createReport(Project *project, Plot *plot, std::shared_ptr<ReportSettings> reportSettings) {
    auto nominalCurve = dynamic_cast<const CurveFigure*>(project->findFigure(reportSettings->nominalName()));
    auto measuredCurve = dynamic_cast<const CurveFigure*>(project->findFigure(reportSettings->measuredName()));
    ARGUMENT_ASSERT(nominalCurve && measuredCurve, "Report generator: curve's not found");

    MacrosManager::executeWithoutLogging([&]() {
        auto curveAnalyzer = CurveAnalyzer(project, reportSettings);
        auto figureCreator = FigureCreator(project, reportSettings);
        auto screenshotCreator = ScreenshotCreator(project, plot, reportSettings);

        auto analyzedGlobalCurves = curveAnalyzer.run();
        figureCreator.run(analyzedGlobalCurves);
        screenshotCreator.run(analyzedGlobalCurves);

        if(reportSettings->needPrintWithTemplate()) {
            auto creatingMarkup = MarkupCreator(project, reportSettings);
            auto reportMarkup = creatingMarkup.run(analyzedGlobalCurves);
            auto time = creatingMarkup.reportCreationTime();
            Printer::addPage(reportMarkup, {
                { "markup", reportMarkup },
                { "nominalCurve", nominalCurve->name() },
                { "measuredCurve", measuredCurve->name() },
                { "time", time } });
        }
    });
    MacrosManager::log(MacrosManager::CreateReport, ReportSettings::convertToQMap(reportSettings));
}

QMap<ReportGenerator::GlobalName, QString> ReportGenerator::getTemplateGlobalNames(const QString &nominalName, const QString &measuredName) {
    auto globalName = QString("Global_%1").arg(nominalName);
    auto globalCVName = QString("GlobalCV_%1").arg(nominalName);
    auto globalCCName = QString("GlobalCC_%1").arg(nominalName);
    auto globalLEName = QString("LE_%1").arg(nominalName);
    auto globalTEName = QString("TE_%1").arg(nominalName);
    return {
        { GlobalName::GlobalCurve, globalName },
        { GlobalName::GlobalCV, globalCVName },
        { GlobalName::GlobalCC, globalCCName },
        { GlobalName::GlobalLE, globalLEName },
        { GlobalName::GlobalTE, globalTEName }
    };
}

QMap<ReportGenerator::FormName, QString> ReportGenerator::getTemplateFormNames(const QString &nominalName) {
    auto globalFormName = QString("%1_Form").arg(nominalName);
    auto globalCVFormName = QString("%1_Convex_Form").arg(nominalName);
    auto globalCCFormName = QString("%1_Concav_Form").arg(nominalName);
    auto globalLEFormName = QString("%1_LE_Form").arg(nominalName);
    auto globalTEFormName = QString("%1_TE_Form").arg(nominalName);
    return {
        { FormName::GlobalForm, globalFormName },
        { FormName::CVForm, globalCVFormName },
        { FormName::CCForm, globalCCFormName },
        { FormName::LEForm, globalLEFormName },
        { FormName::TEForm, globalTEFormName }
    };
}

QMap<ReportGenerator::AdditionalName, QString> ReportGenerator::getTemplateAdditionalNames(const QString &nominalName, const QString &measuredName) {
    auto nomMCLName = QString("%1_MCL").arg(nominalName);
    auto measMCLName = QString("%1_MCL").arg(measuredName);
    auto nomMaxDiaName = QString("%1_CMaxDia").arg(nominalName);
    auto measMaxDiaName = QString("%1_CMaxDia").arg(measuredName);
    auto nomContactLineName = QString("%1_CntctLine").arg(nominalName);
    auto measContactLineName = QString("%1_CntctLine").arg(measuredName);
    return {
        { AdditionalName::NominalMCL, nomMCLName },
        { AdditionalName::MeasuredMCL, measMCLName },
        { AdditionalName::NominalMaxDia, nomMaxDiaName },
        { AdditionalName::MeasuredMaxDia, measMaxDiaName },
        { AdditionalName::NominalCntctLine, nomContactLineName },
        { AdditionalName::MeasuredCntctLine, measContactLineName },
    };
}

QMap<ReportGenerator::InterimName, QString> ReportGenerator::getTemplateInterimNames(const QString & nominalName, const QString & measuredName) {
    auto dummyNomName = QString("_%1").arg(nominalName);
    auto dummyMeasName = QString("_%1").arg(measuredName);
    auto dummyNomCVName = QString("dummy_CV_%1").arg(nominalName);
    auto dummyMeasCVName = QString("dummy_CV_%1").arg(measuredName);
    auto dummyNomCCName = QString("dummy_CC_%1").arg(nominalName);
    auto dummyMeasCCName = QString("dummy_CC_%1").arg(measuredName);
    return {
        { InterimName::NominalCurve, dummyNomName },
        { InterimName::MeasuredCurve, dummyMeasName },
        { InterimName::NominalCV, dummyNomCVName },
        { InterimName::MeasuredCV, dummyMeasCVName },
        { InterimName::NominalCC, dummyNomCCName },
        { InterimName::MeasuredCC, dummyMeasCCName }
    };
}
