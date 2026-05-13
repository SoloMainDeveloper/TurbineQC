#pragma once

#include <QObject>

class MacrosManager;
class MacrosReaderCRM;
class Project;

class ExecutorCLI : public QObject {
    Q_OBJECT

public:
    ExecutorCLI();

    int run();

    void loadMacros(QString filePath);
    void loadProject(QString filePath);

private:
    MacrosManager* _manager;
    Project* _project;
    MacrosReaderCRM* _macrosReaderCRM;
};
