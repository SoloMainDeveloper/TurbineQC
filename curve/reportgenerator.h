#pragma once

#include "plot.h"
#include "project.h"
#include "reportsettings.h"
#include "curveanalyzer.h"
#include "figurecreator.h"
#include "screenshotcreator.h"
#include "markupcreator.h"
#include "printer.h"

class ReportSettings;

class ReportGenerator {
public:
    enum class GlobalName {
        GlobalCurve,
        GlobalCV, GlobalCC,
        GlobalLE, GlobalTE,
    };

    enum class FormName {
        GlobalForm,
        CVForm, CCForm,
        LEForm, TEForm,
    };

    enum class AdditionalName {
        NominalMCL, MeasuredMCL,
        NominalMaxDia, MeasuredMaxDia,
        NominalCntctLine, MeasuredCntctLine
    };

    static void createReport(Project *project, Plot *plot, std::shared_ptr<ReportSettings> reportSettings);
    static QMap<GlobalName, QString> getTemplateGlobalNames(const QString &nominalName);
    static QMap<FormName, QString> getTemplateFormNames(const QString &nominalName);
    static QMap<AdditionalName, QString> getTemplateAdditionalNames(const QString &nominalName, const QString &measuredName);
};
