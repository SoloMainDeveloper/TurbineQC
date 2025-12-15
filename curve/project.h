#pragma once

#include "figure.h"

class Project : public QObject {
    Q_OBJECT

public:
    const QList<Figure*>& figures() const;
    const Figure* findFigure(const QString &name) const;
    const QString currentFigureName() const;
    bool containsFigure(const QString &name);

public slots:
    void insertFigure(Figure* figure);
    void renameFigure(const QString &name, const QString &newName);
    void removeFigure(const QString &name);
    void changeCurrentFigure(const QString &currentFigureName);
    void changeFigureVisibility(const QString figureName, bool visible);
    void toggleFigureVisibility(const QString figureName);
    void changeCurveParameters(const QString curveName, bool showPoints, bool connectPoints,
        bool showVectors, bool closed, bool showNumbering, int numberingInterval);
    void changeCurveTolerance(const QString curveName, double upperTolerance, double lowerTolerance);

signals:
    void figureAdded(Figure* figure);
    void figureAboutToBeRemoved(const QString figureName);
    void figureRenamed(const QString figureName, const QString newName);
    void currentFigureChanged(const QString currentFigureName);
    void figureVisibilityChanged(const QString figureName, bool visible);
    void curveParametersChanged(const QString curveName, bool showPoints, bool connectPoints,
        bool showVectors, bool closed, bool showNumbering, int numberingInterval);
    void curveToleranceChanged(const QString curveName, double upperTolerance, double lowerTolerance);

private:
    Figure* findFigureMutable(const QString &name);

    QList<Figure*> _figures;
    QString _currentFigureName = nullptr;
};
