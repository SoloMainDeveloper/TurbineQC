#pragma once

#include "figurecreator.h"
#include "reportsettings.h"

using CurveType = FigureCreator::CurveType;
using GlobalCurveMap = QMap<CurveType, QPair<QString, QVector<CurvePoint>>>;

class Project;
class ITurbineParameter;

class ReportService {
public:
    explicit ReportService(std::shared_ptr<ReportSettings> reportSettings);
    ~ReportService() = default;

    QString generateReport(const GlobalCurveMap& analyzedGlobalCurves);
    QString creationTime();

private:
    static const QHash<ReportSettings::GlobalBestFit, QString> _globalBestFitInfo;
    static const QHash<ReportSettings::OptionGlobalBestFit, QString> _limitGlobalBestFitInfo;
    static const QHash<ReportSettings::EdgeBestFit, QString> _edgeBestFitInfo;

    Project* _project;
    std::shared_ptr<ReportSettings> _reportSettings;

    QString _creationTime;

    const QString _convexTableTitleTemplate = "Convex_%1";
    const QString _concaveTableTitleTemplate = "Concave_%1";
    const QString _leadingEdgeTableTitleTemplate = "LE_%1";
    const QString _trailingEdgeTableTitleTemplate = "TE_%1";

    QString generateComment();

    QString generateGlobalView(const GlobalCurveMap& analyzedGlobalCurves);
    QString generateGlobalTitle();

    QString generateLeadingEdgeView(const GlobalCurveMap& analyzedGlobalCurves);
    QString generateTrailingEdgeView(const GlobalCurveMap& analyzedGlobalCurves);
    QString generateEdgeTitle(ReportSettings::EdgeBestFit edgeBestFit, double edgeAmplification, const QString& edgeTitleTemplate);

    QString generateStatisticsTable(const QVector<CurvePoint>& points, const QString& tableTitle);

    QString generateParameterTable();
    QString generateParameterTableRow(const ITurbineParameter* parameter);
    QString generateOutOfTolerance(double deviation, double upperTolerance, double lowerTolerance);

    QString generatePartData();
};
