#pragma once

#include <QObject>
#include "reportsettings.h"

struct CurveParts {
public:
    CurveParts();
    CurveParts(QVector<CurvePoint> pointsOfLE, QVector<CurvePoint> pointsOfTE, QVector<CurvePoint> pointsOfHigh, QVector<CurvePoint> pointsOfLow);

    QVector<CurvePoint> pointsOfLE() const;
    QVector<CurvePoint> pointsOfTE() const;
    QVector<CurvePoint> pointsOfHigh() const;
    QVector<CurvePoint> pointsOfLow() const;

private:
    QVector<CurvePoint> _pointsOfLE;
    QVector<CurvePoint> _pointsOfTE;
    QVector<CurvePoint> _pointsOfHigh;
    QVector<CurvePoint> _pointsOfLow;
};

class CreatingMarkup : public QObject {
    Q_OBJECT

public:
    explicit CreatingMarkup(std::shared_ptr<ReportSettings> reportSettings, CurveParts &curveParts);
    virtual ~CreatingMarkup();

    void run(Project *project);
private:
    std::shared_ptr<ReportSettings> _reportSettings;
    CurveParts _curveParts;

    QString _index;
    QString _tableRowTemplate;

    QString getComment();
    QString getGlobalView();
    QString getParameters();
    QString getMaxWidth();
    QString getXMaxWidth();
    QString getYMaxWidth();
    QString getContactLineLength();
    QString getLEWidth();
    QString getTEWidth();
    QString getLERadius();
    QString getTERadius();
    QString getOOT(double upperTolerance, double downTolerance, double deviation);
    QString getLEView();
    QString getTEView();
    QString getTable(const QVector<CurvePoint> &points, const QString &caption, const QString &style = "");
    QString getEncodedScreenshot(const QImage &screenshot);
    QString getPartData(Project *project);
};
