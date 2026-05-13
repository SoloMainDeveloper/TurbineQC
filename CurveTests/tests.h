#pragma once
#include <QtTest/QtTest>

class MacrosManager;
class MacrosReaderCRM;
class Project;

class TestMyClass : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanup();

    void macros1324Test();
    // void testAnotherMethod_data();
private:
    MacrosManager* _manager;
    Project* _project;
    MacrosReaderCRM* _macrosReaderCRM;

    void loadMacros(QString filePath);
};
