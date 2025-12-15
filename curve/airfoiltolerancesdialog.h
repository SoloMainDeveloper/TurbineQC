#pragma once

#include <QDialog>
#include "project.h"
#include "ui_airfoiltolerancesdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AirfoilTolerancesDialogClass; };
QT_END_NAMESPACE

class AirfoilTolerancesDialog : public QDialog
{
    Q_OBJECT

public:
    AirfoilTolerancesDialog(Project* mainProject, QWidget *parent = nullptr);
    ~AirfoilTolerancesDialog();

public slots:
    void dialogInitialization();
    void changeCurrentMode();
    void assignTolerances();

signals:
    void curveChangeToleranceRequested(const QString curveName, double upperTolerance, double lowerTolerance);

private:
    Ui::AirfoilTolerancesDialogClass *ui;

    Project* _project;
    QDialogButtonBox* _buttonBox;
    QLineEdit* upperToleranceLE;
    QLineEdit* lowerToleranceLE;
};
