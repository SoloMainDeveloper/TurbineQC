#pragma once

#include "figurecreator.h"

using CurveType = FigureCreator::CurveType;
using GlobalCurveMap = QMap<CurveType, QPair<QString, QVector<CurvePoint>>>;

class MarkupCreator {
public:
    static const QString index;
    static const QString tableRowTemplate;
    static QString getOOTMarkup(double upperTolerance, double downTolerance, double deviation);

    explicit MarkupCreator(Project *project, std::shared_ptr<ReportSettings> reportSettings);
    virtual ~MarkupCreator() = default;

    QString run(const GlobalCurveMap &analyzedGlobalCurves);
    QString reportCreationTime();

private:
    Project *_project;
    std::shared_ptr<ReportSettings> _reportSettings;

    QString _reportTemplate;
    QString _creationTime;

    QString getCommentMarkup();
    QString getGlobalViewMarkup(const GlobalCurveMap &analyzedGlobalCurves);
    QString getParametersMarkup();
    QString getViewLEMarkup(const GlobalCurveMap &analyzedGlobalCurves);
    QString getViewTEMarkup(const GlobalCurveMap &analyzedGlobalCurves);
    QString getTableMarkup(const QVector<CurvePoint> &points, const QString &caption, const QString &style = "");
    QString getEncodedScreenshot(const QImage &screenshot);
    QString getPartDataMarkup();
};
