#include "curve/pch.h"

#include "createreportcommand.h"
#include "reportgenerator.h"
#include "reportservice.h"
#include "screenshotcreator.h"

void ReportGenerator::createReport(std::shared_ptr<ReportSettings> reportSettings)
{
    auto& project = Project::instance();

    auto nominalCurve = dynamic_cast<const CurveFigure*>(project.findFigure(reportSettings->nominalName()));
    auto measuredCurve = dynamic_cast<const CurveFigure*>(project.findFigure(reportSettings->measuredName()));

    ARGUMENT_ASSERT(nominalCurve && measuredCurve, "Report generator: curve's not found");

    MacrosManager::instance().executeWithoutLogging([&]() {
        auto curveAnalyzer = CurveAnalyzer(reportSettings);
        auto figureCreator = FigureCreator(reportSettings);
        auto screenshotCreator = ScreenshotCreator(reportSettings);

        auto analyzedGlobalCurves = curveAnalyzer.run();
        figureCreator.run(analyzedGlobalCurves);
        screenshotCreator.run(analyzedGlobalCurves);

        if(reportSettings->needPrintWithTemplate()) {
            auto reportService = ReportService(reportSettings);

            QString report = reportService.generateReport(analyzedGlobalCurves);
            QString time = reportService.creationTime();
            QMap<QString, QString> information = {
                { "markup", report },
                { "nominalCurve", nominalCurve->name() },
                { "measuredCurve", measuredCurve->name() },
                { "time", time },
            };

            Printer::instance().addPage(report, information);
        }
    });

    MacrosManager::instance().log(std::make_shared<CreateReportCommand>(reportSettings));
}

QMap<ReportGenerator::GlobalName, QString> ReportGenerator::getTemplateGlobalNames(const QString& nominalName, const QString& measuredName)
{
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

QMap<ReportGenerator::FormName, QString> ReportGenerator::getTemplateFormNames(const QString& nominalName)
{
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

QMap<ReportGenerator::AdditionalName, QString> ReportGenerator::getTemplateAdditionalNames(const QString& nominalName, const QString& measuredName)
{
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

QMap<ReportGenerator::InterimName, QString> ReportGenerator::getTemplateInterimNames(
    const QString& nominalName, const QString& measuredName)
{
    auto dummyNominalName = QString("_%1").arg(nominalName);
    auto dummyMeasuredName = QString("_%1").arg(measuredName);

    auto dummyNominalCVName = QString("dummy_CV_%1").arg(nominalName);
    auto dummyMeasuredCVName = QString("dummy_CV_%1").arg(measuredName);
    auto dummyNominalCCName = QString("dummy_CC_%1").arg(nominalName);
    auto dummyMeasuredCCName = QString("dummy_CC_%1").arg(measuredName);

    auto dummyNominalLEName = QString("dummy_LE_%1").arg(nominalName);
    auto dummyMeasuredLEName = QString("dummy_LE_%1").arg(measuredName);
    auto dummyNominalTEName = QString("dummy_TE_%1").arg(nominalName);
    auto dummyMeasuredTEName = QString("dummy_TE_%1").arg(measuredName);

    return {
        { InterimName::NominalCurve, dummyNominalName },
        { InterimName::MeasuredCurve, dummyMeasuredName },
        { InterimName::NominalCV, dummyNominalCVName },
        { InterimName::MeasuredCV, dummyMeasuredCVName },
        { InterimName::NominalCC, dummyNominalCCName },
        { InterimName::MeasuredCC, dummyMeasuredCCName },
        { InterimName::NominalLE, dummyNominalLEName },
        { InterimName::MeasuredLE, dummyMeasuredLEName },
        { InterimName::NominalTE, dummyNominalTEName },
        { InterimName::MeasuredTE, dummyMeasuredTEName },
    };
}
