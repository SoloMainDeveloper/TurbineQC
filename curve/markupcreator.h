#pragma once

#include "figurecreator.h"

using CurveType = FigureCreator::CurveType;
using GlobalCurveMap = QMap<CurveType, QPair<QString, QVector<CurvePoint>>>;

class MarkupCreator {
public:
    static const QString index;

    explicit MarkupCreator(Project *project, std::shared_ptr<ReportSettings> reportSettings);
    virtual ~MarkupCreator() = default;

    QString run(const GlobalCurveMap &analyzedGlobalCurves);
    QString reportCreationTime();

private:
    Project *_project;
    std::shared_ptr<ReportSettings> _reportSettings;

    QString _reportTemplate;
    QString _tableRowTemplate;
    QString _creationTime;

    QString getCommentMarkup();
    QString getGlobalViewMarkup(const GlobalCurveMap &analyzedGlobalCurves);
    QString getParametersMarkup();
    QString getMaxWidthMarkup();
    QString getXMaxWidthMarkup();
    QString getYMaxWidthMarkup();
    QString getContactLineLengthMarkup();
    QString getWidthLEMarkup();
    QString getWidthTEMarkup();
    QString getRadiusLEMarkup();
    QString getRadiusTEMarkup();
    QString getMinXMarkup();
    QString createParameterMarkup(ReportSettings::TurbineParameter param, QString title, QString type);
    QString getOOTMarkup(double upperTolerance, double downTolerance, double deviation);
    QString getViewLEMarkup(const GlobalCurveMap &analyzedGlobalCurves);
    QString getViewTEMarkup(const GlobalCurveMap &analyzedGlobalCurves);
    QString getTableMarkup(const QVector<CurvePoint> &points, const QString &caption, const QString &style = "");
    QString getEncodedScreenshot(const QImage &screenshot);
    QString getPartDataMarkup();
};
