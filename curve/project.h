#pragma once

#include "figure.h"
#include "macrosmanager.h"
#include <QElapsedTimer>

class Project : public QObject {
    Q_OBJECT

public:
    static Project& instance();

    Project() = default;
    virtual ~Project();

    const QMap<QString, Figure*>& figures() const;
    const QList<CurveFigure*>& curveFigures() const;
    const QList<DimFigure*>& dimFigures() const;
    const QList<TextFigure*>& textFigures() const;
    const Figure* findFigure(const QString &name) const;
    const QString currentFigureName() const;
    const bool containsFigure(const QString &name) const;
    void setVisibility(const QStringList &figureNames);
    void resetVisibilityForAllFigures();
    void setCurrentFigure(const QString &currentFigureName);
    int precision() const;
    bool confirmRemoveFigure(const QString& figureName);

    double scaleFactor() const;
    Point centerPoint() const;
    void setProjectPath(const QString &path);
    const QString projectPath();

    const QString reportTitle();
    const QString description();
    const QString drawing();
    const QString orderNumber();
    const QString partNumber();
    const QString projectOperator();
    const QString note();

    const QString machine();
    const QString tool();
    const QString fixturing();
    const QString batch();
    const QString supplier();
    const QString revision();

    QString getFreeName(QString startsWith, bool firstWithNumber = true, QString separator = "");
    void addOperationtime(QString operation, quint64 time);

public slots:
    void insertFigure(Figure* figure);
    void safeInsert(QString figureName, Figure *figure, bool needToChangeCurrentFigure = true);
    void renameFigure(const QString &name, const QString &newName);
    void removeFigure(const QString &name);
    void editFigure(const QString &name, QMap<QString, QString> &paramsChanged);
    void changeCurrentFigure(const QString &currentFigureName);
    void changeFigureVisibility(const QString figureName, bool visible);
    void toggleFigureVisibility(const QString figureName);
    void changeCurveParameters(const QString curveName, bool showPoints, bool connectPoints,
        bool showVectors, bool closed, bool showNumbering, int numberingInterval,
        double amplification, bool showTolerances, bool showDeviations, bool connectDeviations, bool highLightOut);
    void changeDimensionParameters(const QString dimName, bool onlyLabel, bool showTols, bool freePosition);
    void changeCurveTolerance(const QString curveName, QVector<CurvePoint> curveWithTolerances);
    void requestFigureEditDialog(const QString figureName);
    void changeCurvePoints(const QString curveName, QVector<CurvePoint> newPoints);
    void changeFigureColor(const QString figureName, QColor color);
    void changeScale(double scaleFactor, const Point center, bool needToReplot);
    void zoomToPoint(double scaleFactor, const Point center);
    void clear();
    //void shiftFigure(QString figureName, double x, double y, double z);
    void shiftFigure(QString figureName, QString x, QString y, QString z);
    //void rotateFigure(QString figureName, double angle, double x, double y, double z);
    void rotateFigure(QString figureName, double angle, QString x, QString y, QString z);
    //void alignment(double angle, double offsetX, double offsetY);
    void alignment(QString angle, QString axis, QString offsetX, QString offsetY);
    void changeDimensionValue(const QString &dimName, const DimFigure::Value &value);
    void changePartData(QString reportTitle, QString description, QString drawing, QString orderNumber, QString partNumber, QString projectOperator, QString note,
        QString machine, QString tool, QString fixturing, QString batch, QString supplier, QString revision, bool needShowWindowWhenMacroRuns);
    void constructText(QString name, QString text, double x, double y, double textSize, QString reference,
        double imageWidth, double imageHeight, double imageZoom);
    void showAllFigures(QString figuresType);
    void hideAllFigures(QString figuresType);

signals:
    void figureAdded(Figure* figure);
    void figureAboutToBeRemoved(const QString figureName);
    void figureRenamed(const QString figureName, const QString newName);
    void currentFigureChanged(const QString currentFigureName, const QString previousFigureName);
    void figureVisibilityChanged(const QString figureName, bool visible);
    void curveParametersChanged(const QString curveName, bool showPoints, bool connectPoints,
        bool showVectors, bool closed, bool showNumbering, int numberingInterval,
        double amplification, bool showTolerances, bool showDeviations, bool connectDeviations, bool highLightOut);
    void dimensionParametersChanged(const QString dimName, bool onlyLabel, bool showTols, bool freePosition);
    void curveToleranceChanged(const QString curveName);
    void figureEditDialogRequested(const QString figureName);
    void curvePointsChanged(const QString curveName);
    void figureColorChanged(const QString figureName);
    void scaleChanged(double scaleFactor, const Point &center, bool needToReplot);
    void zoomToPointRequested(double scaleFactor, const Point &center);
    void dimensionValueChanged(const QString &dimName, const DimFigure::Value &value);
    void figureCoordsChanged(const QString figureName);
    void figureEdited(const QString figureName);
    void showPartDataDialogRequested(QString reportTitle, QString description, QString drawing, QString orderNumber, QString partNumber, QString projectOperator,
        QString note, QString machine, QString tool, QString fixturing, QString batch, QString supplier, QString revision);
    void trackMousePositionRequested();
    void raiseMainWindow();
    void mousePressed(const Point pos);
    void projectPathChanged(const QString &projectPath);

private:
    Figure* findFigureMutable(const QString &name);
    void initiateParentChildReference(Figure *figure);
    void attachChildToParent(Figure *child, QString parentName);
    void detachChildFromParent(Figure *child, QString parentName);
    void updateParent(Figure *child, QString newParent1, QString newParent2 = QString());
    QString txtFigureToText(QString txtFigure) const;

    QMap<QString, QList<quint64>> operationTimes;
    //MacrosManager* _macrosManager;

    QMap<QString, Figure*> _figures;
    QString _currentFigureName = nullptr;
    QMap<QString, QList<Figure*>> _lostParents;
    double _scaleFactor = 1.0;
    int _precision = 6;
    Point _centerPoint;
    QString _projectPath = nullptr;
    int _lastFigureIndex = -1;

    QString _reportTitle = nullptr;
    QString _description = nullptr;
    QString _drawing = nullptr;
    QString _orderNumber = nullptr;
    QString _partNumber = nullptr;
    QString _projectOperator = nullptr;
    QString _note = nullptr;

    QString _machine = nullptr;
    QString _tool = nullptr;
    QString _fixturing = nullptr;
    QString _batch = nullptr;
    QString _supplier = nullptr;
    QString _revision = nullptr;
};
