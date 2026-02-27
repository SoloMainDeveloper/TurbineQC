#pragma once

#include <QDialog>
#include "ui_inserttextdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class InsertTextDialogClass; };
QT_END_NAMESPACE

class InsertTextDialog : public QDialog
{
    Q_OBJECT

public:
    InsertTextDialog(Project *project, QWidget *parent = nullptr);
    ~InsertTextDialog();

    void initialization();

private:
    int findFreeIndex();
    void insertText();
    void openImage();
    void selectCoords();

    Ui::InsertTextDialogClass *_ui;
    Project *_project;
};
