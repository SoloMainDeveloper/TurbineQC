#pragma once

class Project;
class Plot;
class ReportSettings;

class ReportGenerator {
public:
    enum class GlobalName {
        GlobalCurve,
        GlobalCV,
        GlobalCC,
        GlobalLE,
        GlobalTE,
    };

    enum class FormName {
        GlobalForm,
        CVForm,
        CCForm,
        LEForm,
        TEForm,
    };

    enum class AdditionalName {
        NominalMCL,
        MeasuredMCL,
        NominalMaxDia,
        MeasuredMaxDia,
        NominalCntctLine,
        MeasuredCntctLine
    };

    enum class InterimName {
        NominalCurve,
        MeasuredCurve,
        NominalCV,
        MeasuredCV,
        NominalCC,
        MeasuredCC,
        NominalLE,
        MeasuredLE,
        NominalTE,
        MeasuredTE,
    };

    static void createReport(std::shared_ptr<ReportSettings> reportSettings);
    static QMap<GlobalName, QString> getTemplateGlobalNames(const QString &nominalName, const QString &measuredName);
    static QMap<FormName, QString> getTemplateFormNames(const QString &nominalName);
    static QMap<AdditionalName, QString> getTemplateAdditionalNames(const QString &nominalName, const QString &measuredName);
    static QMap<InterimName, QString> getTemplateInterimNames(const QString &nominalName, const QString &measuredName);
};
