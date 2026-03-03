#pragma once

#include <QDialog>
#include "ui_compareflrdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class CompareFLRDialogClass;
};
QT_END_NAMESPACE

class CompareFLRDialog : public QDialog
{
    Q_OBJECT

public:
    CompareFLRDialog(QWidget *parent = nullptr);
    ~CompareFLRDialog();
    void initialization();
    void compare();

private:
    Ui::CompareFLRDialogClass *_ui;
};
