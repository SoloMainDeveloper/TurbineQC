#pragma once

#include "imacrosreader.h"

class MacrosReaderCRM : public IMacrosReader {
public:
    QList<std::shared_ptr<ICommand>>* read(const QString filepath) override;
    QString availableExtension() override;
};
