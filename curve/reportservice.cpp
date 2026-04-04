#include "curve/pch.h"

#include "deviationstatistics.h"
#include "iturbineprofileparameter.h"
#include "iturbinetransformparameter.h"
#include "reportmarkuptemplates.h"
#include "reportservice.h"

using Profile = ReportSettings::Profile;

using GlobalBestFit = ReportSettings::GlobalBestFit;

using OptionGlobalBestFit = ReportSettings::OptionGlobalBestFit;

using EdgeBestFit = ReportSettings::EdgeBestFit;

ReportService::ReportService(std::shared_ptr<ReportSettings> reportSettings)
    : _reportSettings(reportSettings)
{
    _project = &Project::instance();
}

QString ReportService::generateReport(const GlobalCurveMap& analyzedGlobalCurves)
{
    _creationTime = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss");

    QString comment = generateComment();

    QString globalView = generateGlobalView(analyzedGlobalCurves);
    QString leadingEdgeView = generateLeadingEdgeView(analyzedGlobalCurves);
    QString trailingEdgeView = generateTrailingEdgeView(analyzedGlobalCurves);

    QString parameters = generateParameterTable();

    QString partData = generatePartData();

    QString report = ReportMarkupTemplates::reportPageTemplate
                         .arg(comment)
                         .arg(globalView)
                         .arg(parameters)
                         .arg(leadingEdgeView)
                         .arg(trailingEdgeView)
                         .arg(partData);

    return report;
}

QString ReportService::generateComment()
{
    QString comment = _reportSettings->comment();
    QString preparedComment = ReportMarkupTemplates::commentTemplate
                                  .arg(comment);

    return preparedComment;
}

QString ReportService::generateGlobalView(const GlobalCurveMap& analyzedGlobalCurves)
{
    QString viewTitle = generateGlobalTitle();

    QString nominalName = _reportSettings->nominalName();
    auto [convexName, convexPoints] = analyzedGlobalCurves[FigureCreator::CurveType::GlobalCV];
    QString convexTitle = _convexTableTitleTemplate.arg(nominalName);
    QString convexStatisticsTable = generateStatisticsTable(convexPoints, convexTitle);

    auto [concaveName, cocavePoints] = analyzedGlobalCurves[FigureCreator::CurveType::GlobalCC];
    QString covcaveTitle = _concaveTableTitleTemplate.arg(nominalName);
    QString concaveStatisticsTable = generateStatisticsTable(cocavePoints, covcaveTitle);

    QString encodedImage = _reportSettings->globalBase64Image();

    QString globalView = ReportMarkupTemplates::globalViewTemplate
                             .arg(viewTitle)
                             .arg(concaveStatisticsTable)
                             .arg(convexStatisticsTable)
                             .arg(encodedImage);

    return globalView;
}

const QHash<GlobalBestFit, QString> ReportService::_globalBestFitInfo = {
    { GlobalBestFit::NoFit, "No Fit" },
    { GlobalBestFit::WholeLSQ, "Whole Profile (LSQ)" },
    { GlobalBestFit::WithoutEdgesLSQ, "Profile Without LE/TE (LSQ)" },
    { GlobalBestFit::MinForm, "Min. Form" },
};

const QHash<OptionGlobalBestFit, QString> ReportService::_limitGlobalBestFitInfo = {
    { OptionGlobalBestFit::OnlyRotation, "Rotation" },
    { OptionGlobalBestFit::OnlyTranslation, "Translation" },
    { OptionGlobalBestFit::OnlyXTranslation, "X Translation" },
    { OptionGlobalBestFit::OnlyYTranslation, "Y Translation" },
    { OptionGlobalBestFit::TranslationAndRotation, "Translation and Rotation" },
    { OptionGlobalBestFit::XTranslationAndRotation, "X Translation and Rotation" },
    { OptionGlobalBestFit::YTranslationAndRotation, "Y Translation and Rotation" },
};

QString ReportService::generateGlobalTitle()
{
    QString nominalName = _reportSettings->nominalName();

    GlobalBestFit globalBestFit = _reportSettings->globalBestFit();
    QString defaultBestFitInfo = "Add new best-fit";
    QString globalBestFitInfo = _globalBestFitInfo.value(globalBestFit, defaultBestFitInfo);

    OptionGlobalBestFit optionGlobalBestFit = _reportSettings->optionGlobalBestFit();
    QString defaultOptionBestFitInfo = "Add new option best-fit";
    QString optionGlobalBestFitInfo = _limitGlobalBestFitInfo.value(optionGlobalBestFit, defaultOptionBestFitInfo);

    Profile profileType = _reportSettings->profileType();
    QString globalBestFitValuesInfo;

    if(profileType == Profile::Whole || profileType == Profile::WithoutTE || profileType == Profile::WithoutEdges) {
        double xShift = _reportSettings->xShift();
        double yShift = _reportSettings->yShift();
        double rotation = _reportSettings->rotation();

        globalBestFitValuesInfo = ReportMarkupTemplates::bestFitValuesTemplate
                                      .arg(QString::number(xShift, 'f', 3))
                                      .arg(QString::number(yShift, 'f', 3))
                                      .arg(QString::number(rotation, 'f', 3));
    }
    else {
        double xShiftOfConvex = _reportSettings->xShiftCV();
        double yShiftOfConvex = _reportSettings->yShiftCV();
        double rotationOfConvex = _reportSettings->rotationCV();
        double xShiftOfConcave = _reportSettings->xShiftCC();
        double yShiftOfConcave = _reportSettings->yShiftCC();
        double rotationOfConcave = _reportSettings->rotationCC();

        globalBestFitValuesInfo = ReportMarkupTemplates::bestFitDualValuesTemplate
                                      .arg(QString::number(xShiftOfConvex, 'f', 3))
                                      .arg(QString::number(yShiftOfConvex, 'f', 3))
                                      .arg(QString::number(rotationOfConvex, 'f', 3))
                                      .arg(QString::number(xShiftOfConcave, 'f', 3))
                                      .arg(QString::number(yShiftOfConcave, 'f', 3))
                                      .arg(QString::number(rotationOfConcave, 'f', 3));
    }

    QString globalAmplification = QString::number(_reportSettings->globalAmplification());

    QString globalBestFitTitle = ReportMarkupTemplates::globalTitleTemplate
                                     .arg(nominalName)
                                     .arg(globalBestFitInfo)
                                     .arg(optionGlobalBestFitInfo)
                                     .arg(globalAmplification)
                                     .arg(globalBestFitValuesInfo);

    return globalBestFitTitle;
}

QString ReportService::generateLeadingEdgeView(const GlobalCurveMap& analyzedGlobalCurves)
{
    QString viewTitle;
    QString statisticsTable;
    QString imageHTMLCode;

    if(_reportSettings->isLeadingEdgeVisible()) {
        EdgeBestFit bestFit = _reportSettings->leadingEdgeBestFit();
        double amplification = _reportSettings->leadingEdgeAmplification();
        QString titleTemplate = ReportMarkupTemplates::leadingEdgeTitleTemplate;
        viewTitle = generateEdgeTitle(bestFit, amplification, titleTemplate);

        auto [name, points] = analyzedGlobalCurves[FigureCreator::CurveType::GlobalLE];
        QString tableTitle = _leadingEdgeTableTitleTemplate.arg(_reportSettings->nominalName());
        statisticsTable = generateStatisticsTable(points, tableTitle);

        QString encodedImage = _reportSettings->leadingEdgeBase64Image();
        imageHTMLCode = ReportMarkupTemplates::leadingEdgeImageTemplate.arg(encodedImage);
    }

    QString leadingEdgeView = ReportMarkupTemplates::leadingEdgeViewTemplate
                                  .arg(viewTitle)
                                  .arg(statisticsTable)
                                  .arg(imageHTMLCode);

    return leadingEdgeView;
}

QString ReportService::generateTrailingEdgeView(const GlobalCurveMap& analyzedGlobalCurves)
{
    QString viewTitle;
    QString statisticsTable;
    QString imageHTMLCode;

    if(_reportSettings->isTrailingEdgeVisible()) {
        EdgeBestFit bestFit = _reportSettings->trailingEdgeBestFit();
        double amplification = _reportSettings->trailingEdgeAmplification();
        QString titleTemplate = ReportMarkupTemplates::trailingEdgeTitleTemplate;
        viewTitle = generateEdgeTitle(bestFit, amplification, titleTemplate);

        auto [name, points] = analyzedGlobalCurves[FigureCreator::CurveType::GlobalTE];
        QString tableTitle = _trailingEdgeTableTitleTemplate.arg(_reportSettings->nominalName());
        statisticsTable = generateStatisticsTable(points, tableTitle);

        QString encodedImage = _reportSettings->trailingEdgeBase64Image();
        imageHTMLCode = ReportMarkupTemplates::trailingEdgeImageTemplate.arg(encodedImage);
    }

    QString trailingEdgeView = ReportMarkupTemplates::trailingEdgeViewTemplate
                                   .arg(viewTitle)
                                   .arg(statisticsTable)
                                   .arg(imageHTMLCode);

    return trailingEdgeView;
}

const QHash<EdgeBestFit, QString> ReportService::_edgeBestFitInfo = {
    { EdgeBestFit::GlobalFit, "Use global fit" },
    { EdgeBestFit::FreeFitForm, "Free Fit (Form)" },
    { EdgeBestFit::NoFit, "No fit" },
};

QString ReportService::generateEdgeTitle(EdgeBestFit edgeBestFit, double edgeAmplification, const QString& edgeTitleTemplate)
{
    QString defaultBestFitInfo = "Add new best-fit";
    QString edgeBestFitInfo = _edgeBestFitInfo.value(edgeBestFit, defaultBestFitInfo);

    auto convertedAmplification = QString::number(edgeAmplification);

    QString edgeTitle = edgeTitleTemplate
                            .arg(edgeBestFitInfo)
                            .arg(convertedAmplification);

    return edgeTitle;
}

QString ReportService::generateStatisticsTable(const QVector<CurvePoint>& points, const QString& tableTitle)
{
    auto deviationStatistics = DeviationStatistics(points);

    QMap<int, QPair<QString, std::function<double()>>> statisticsMap = {
        { 6, { "MinMax", [&]() {
                  return deviationStatistics.minMax();
              } } },
        { 5, { "Form", [&]() {
                  return deviationStatistics.form();
              } } },
        { 4, { "Min", [&]() {
                  return deviationStatistics.min();
              } } },
        { 3, { "Max", [&]() {
                  return deviationStatistics.max();
              } } },
        { 2, { "MaxAbs", [&]() {
                  return deviationStatistics.maxAbs();
              } } },
        { 1, { "SupUT", [&]() {
                  return deviationStatistics.superiorUpperTolerance();
              } } },
        { 0, { "InfLT", [&]() {
                  return deviationStatistics.inferiorLowerTolerance();
              } } },
    };

    int outputFormMode = _reportSettings->outputFormMode();
    int formValuesNumber = 7;
    QString binCode = QString("%1").arg(outputFormMode, formValuesNumber, 2, QChar('0'));

    QString statisticsValues;
    QString statisticsRowTemplate = ReportMarkupTemplates::statisticsTableRowTemplate;

    for(auto [key, statistics] : statisticsMap.asKeyValueRange()) {
        if(QString(binCode[key]).toInt()) {
            auto [name, value] = statistics;
            QString convertedValue = QString::number(value(), 'f', 3);
            statisticsValues += statisticsRowTemplate.arg(name).arg(convertedValue);
        }
    }

    QString statisticsTable = ReportMarkupTemplates::statisticsTableTemplate
                                  .arg(tableTitle)
                                  .arg(statisticsValues);

    return statisticsTable;
}

QString ReportService::generateParameterTable()
{
    auto& turbineParameters = _reportSettings->turbineParameters();

    QString parameterRows;
    QList<ITurbineTransformParameter*> transforms;

    for(auto [type, list] : turbineParameters.asKeyValueRange()) {
        for(auto parameter : list) {
            if(auto transformParameter = qobject_cast<ITurbineTransformParameter*>(parameter)) {
                transforms.append(transformParameter);
            }
            if(auto profileParameter = qobject_cast<ITurbineProfileParameter*>(parameter)) {
                parameterRows += ReportMarkupTemplates::parameterTableRowTitleTemplate
                                     .arg(profileParameter->dimensionName());
                parameterRows += generateParameterTableRow(parameter);
            }
        }
    }

    bool isFirstTransformParameter = true;

    for(auto transformParameter : transforms) {
        if(isFirstTransformParameter) {
            QString commonName = ITurbineTransformParameter::commonDimensionName();
            parameterRows += ReportMarkupTemplates::parameterTableRowTitleTemplate.arg(commonName);
            parameterRows += generateParameterTableRow(transformParameter);

            isFirstTransformParameter = false;
        }
        else {
            parameterRows += generateParameterTableRow(transformParameter);
        }
    }

    QString parameterTable = ReportMarkupTemplates::parameterTableTemplate
                                 .arg(parameterRows);

    return parameterTable;
}

QString ReportService::generateParameterTableRow(const ITurbineParameter* parameter)
{
    QString parameterTableRow;

    QString type = parameter->reportType();
    QString outOfTolerance = ReportMarkupTemplates::defaultOutOfToleranceTemplate;

    double deviation = parameter->measured() - parameter->nominal();

    if(parameter->nominal() == 0 && parameter->upperTolerance() == 0 && parameter->lowerTolerance() == 0) {
        parameterTableRow = ReportMarkupTemplates::parameterTableRowTemplate
                                .arg(type)
                                .arg("")
                                .arg("")
                                .arg("")
                                .arg(QString::number(parameter->measured(), 'f', 3))
                                .arg("")
                                .arg(outOfTolerance);
    }
    else if(parameter->upperTolerance() == 0 && parameter->lowerTolerance() == 0) {
        parameterTableRow = ReportMarkupTemplates::parameterTableRowTemplate
                                .arg(type)
                                .arg(QString::number(parameter->nominal(), 'f', 3))
                                .arg("")
                                .arg("")
                                .arg(QString::number(parameter->measured(), 'f', 3))
                                .arg(QString::number(deviation, 'f', 3))
                                .arg(outOfTolerance);
    }
    else {
        outOfTolerance = generateOutOfTolerance(deviation, parameter->upperTolerance(), parameter->lowerTolerance());
        parameterTableRow = ReportMarkupTemplates::parameterTableRowTemplate
                                .arg(type)
                                .arg(QString::number(parameter->nominal(), 'f', 3))
                                .arg(QString::number(parameter->upperTolerance(), 'f', 3))
                                .arg(QString::number(parameter->lowerTolerance(), 'f', 3))
                                .arg(QString::number(parameter->measured(), 'f', 3))
                                .arg(QString::number(deviation, 'f', 3))
                                .arg(outOfTolerance);
    }

    return parameterTableRow;
}

QString ReportService::generateOutOfTolerance(double deviation, double upperTolerance, double lowerTolerance)
{
    QString outOfTolerance;

    if(upperTolerance > deviation && deviation > lowerTolerance) {
        double step = (upperTolerance - lowerTolerance) / 10;
        outOfTolerance = ReportMarkupTemplates::valueInToleranceTemplate
                             .arg(lowerTolerance)
                             .arg(upperTolerance)
                             .arg(step)
                             .arg(deviation);
    }
    else {
        double difference = deviation < lowerTolerance
            ? deviation - lowerTolerance
            : deviation - upperTolerance;

        outOfTolerance = ReportMarkupTemplates::valueOutOfToleranceTemplate
                             .arg(QString::number(difference, 'f', 3));
    }

    return outOfTolerance;
}

QString ReportService::generatePartData()
{
    QString partData = ReportMarkupTemplates::partDataTemplate
                           .arg(_project->description())
                           .arg(_project->partNumber())
                           .arg(_project->drawing())
                           .arg(_project->projectOperator())
                           .arg(_project->orderNumber())
                           .arg(_project->note())
                           .arg(_creationTime);

    return partData;
}

QString ReportService::creationTime()
{
    return _creationTime;
}
