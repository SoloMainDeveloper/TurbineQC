#pragma once

#include "basedialog.h"
#include <memory>
#include <utility>
#include <QMap>

class DialogService {
public:
    static DialogService& instance();

    explicit DialogService();
    virtual ~DialogService() = default;

    void show(DialogType type);

private:
    QMap<DialogType, BaseDialog*> _dialogs;
};
