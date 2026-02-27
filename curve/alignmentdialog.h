#pragma once

#include <QDialog>
#include "ui_alignmentdialog.h"
#include "project.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AlignmentDialogClass; };
QT_END_NAMESPACE

class AlignmentDialog : public QDialog
{
    Q_OBJECT

public:
    AlignmentDialog(Project *project = nullptr, QWidget *parent = nullptr);
    ~AlignmentDialog();

    void initialization();

private slots:
    void applyAlignment();
    void changeAngle();
    void changeOffsetX();
    void changeOffsetY();

signals:
    void alignmentRequested(QString angle, QString axis, QString offsetX, QString offsetY);

private:
    Ui::AlignmentDialogClass *_ui;

    Project* _project;

    bool _angleMacrosFlag = false;
    bool _offsetXMacrosFlag = false;
    bool _offsetYMacrosFlag = false;
};
