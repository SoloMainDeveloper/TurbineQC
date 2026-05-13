#pragma once

class IMacrosReader : public QObject {
    Q_OBJECT

public:
    virtual QList<std::shared_ptr<ICommand>>* read(const QString filepath) = 0;
    virtual QString availableExtension() = 0;
};
