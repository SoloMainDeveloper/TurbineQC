#pragma once

#include <QDialog>
#include "ui_partdatadialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PartDataDialogClass; };
QT_END_NAMESPACE

class PartDataDialog : public QDialog
{
    Q_OBJECT

public:
    PartDataDialog(Project *project, QWidget *parent = nullptr);
    ~PartDataDialog();

public slots:
    void initialization();
    void initializationByMacros(QString reportTitle = nullptr, QString description = nullptr, QString drawing = nullptr, QString orderNumber = nullptr,
        QString partNumber = nullptr, QString projectOperator = nullptr, QString note = nullptr, QString machine = nullptr, QString tool = nullptr,
        QString fixturing = nullptr, QString batch = nullptr, QString supplier = nullptr, QString revision = nullptr);

    void applyPartDataChanges();

signals:
    void partDataChangeRequested(QString reportTitle, QString description, QString drawing, QString orderNumber, QString partNumber, QString projectOperator, QString note,
        QString machine, QString tool, QString fixturing, QString batch, QString supplier, QString revision, bool needShowWindowWhenMacroRuns);

private:
    Ui::PartDataDialogClass *_ui;
    Project *_project;
};
