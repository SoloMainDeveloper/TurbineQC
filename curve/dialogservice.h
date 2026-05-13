#pragma once

#include "basedialog.h"
#include <QMap>
#include <memory>
#include <utility>

class DialogService {
public:
    static DialogService& instance();

    explicit DialogService();
    virtual ~DialogService() = default;

    void show(DialogType type);
    void showInfo(QString title, QString info);
    void showWarning(QString title, QString message);

private:
    QMap<DialogType, BaseDialog*> _dialogs;
};
