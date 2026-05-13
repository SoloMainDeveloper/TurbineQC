#pragma once

#include <QtTest/QtTest>

class Project;
class MacrosManager;
class MacrosReaderCRM;

class MacrosManagerTest : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanup();

    void testSingleton();
    void testRecording();
    void testLogWhenRecording();
    void testLogWhenNotRecording();

    void testInsert();
    void testRemove();
    void testSwapOperations();
    void testCommandsCount();
    void testGetCommand();
    void testClear();

    void testRun();
    void testSkipOne();

private:
    MacrosManager* _manager;
    Project* _project;
    MacrosReaderCRM* _macrosReaderCRM;
};
