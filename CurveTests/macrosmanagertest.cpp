#include "pch.h"

#include "loadcloudcommand.h"
#include "macrosmanager.h"
#include "macrosmanagertest.h"
#include "macrosreadercrm.h"
#include "mockfilesystem.h"
#include "mockplot.h"
#include "plot.h"
#include "renamefigurecommand.h"
#include "settings.h"
#include "unknowncommand.h"

void MacrosManagerTest::initTestCase()
{
    auto _mockPlot = new MockPlot();
    Plot::setTestingInstance(_mockPlot);

    _manager = &MacrosManager::instance();
    _project = &Project::instance();
    _macrosReaderCRM = new MacrosReaderCRM();
}

void MacrosManagerTest::cleanup()
{
    _project->clear();
    _manager->clear();
    _manager->setRecording(false);
}

void MacrosManagerTest::testSingleton()
{
    MacrosManager& instance1 = MacrosManager::instance();
    MacrosManager& instance2 = MacrosManager::instance();

    QVERIFY(&instance1 == &instance2);
    QVERIFY(!instance2.isRecording());

    instance1.setRecording(true);

    QVERIFY(instance2.isRecording());
}

void MacrosManagerTest::testRecording()
{
    _manager->setRecording(true);
    QVERIFY(_manager->isRecording());

    _manager->setRecording(false);
    QVERIFY(!_manager->isRecording());

    _manager->toggleRecording();
    QVERIFY(_manager->isRecording());
}

void MacrosManagerTest::testLogWhenRecording()
{
    _manager->setRecording(true);
    bool signalReceived = false;

    QObject::connect(_manager, &MacrosManager::operationLogged,
        [&signalReceived](std::shared_ptr<ICommand>) {
            signalReceived = true;
        });

    _manager->log(std::make_shared<UnknownCommand>());

    QVERIFY(signalReceived);
    QCOMPARE(_manager->commandsCount(), 1);
    QCOMPARE(_manager->recordIndex(), 1);
}

void MacrosManagerTest::testLogWhenNotRecording()
{
    _manager->setRecording(false);
    _manager->log(std::make_shared<UnknownCommand>());

    QCOMPARE(_manager->commandsCount(), 0);
    QCOMPARE(_manager->recordIndex(), 0);
}

void MacrosManagerTest::testInsert()
{
    auto command = std::make_shared<UnknownCommand>();

    _manager->insert(0, command);

    QCOMPARE(_manager->commandsCount(), 1);
    QCOMPARE(_manager->recordIndex(), 1);

    auto command2 = std::make_shared<UnknownCommand>();
    _manager->insert(0, command2);

    QCOMPARE(_manager->commandsCount(), 2);
    QCOMPARE(_manager->recordIndex(), 2);
}

void MacrosManagerTest::testRemove()
{
    _manager->insert(0, std::make_shared<UnknownCommand>());
    QCOMPARE(_manager->commandsCount(), 1);

    _manager->remove(0);

    QCOMPARE(_manager->commandsCount(), 0);
    QCOMPARE(_manager->recordIndex(), 0);
}

void MacrosManagerTest::testSwapOperations()
{
    auto command1 = std::make_shared<UnknownCommand>();
    auto command2 = std::make_shared<UnknownCommand>();

    _manager->insert(0, command1);
    _manager->insert(1, command2);

    _manager->swapOperations(0, 1);

    QVERIFY(_manager->getCommand(0) == command2);
    QVERIFY(_manager->getCommand(1) == command1);
}

void MacrosManagerTest::testCommandsCount()
{
    QCOMPARE(_manager->commandsCount(), 0);

    _manager->insert(0, std::make_shared<UnknownCommand>());
    QCOMPARE(_manager->commandsCount(), 1);

    _manager->insert(1, std::make_shared<UnknownCommand>());
    QCOMPARE(_manager->commandsCount(), 2);
}

void MacrosManagerTest::testGetCommand()
{
    auto command = std::make_shared<UnknownCommand>();
    _manager->insert(0, command);

    auto retrieved = _manager->getCommand(0);
    QVERIFY(retrieved == command);
}

void MacrosManagerTest::testClear()
{
    _manager->insert(0, std::make_shared<UnknownCommand>());
    _manager->insert(1, std::make_shared<UnknownCommand>());
    QCOMPARE(_manager->commandsCount(), 2);

    _manager->clear();

    QCOMPARE(_manager->commandsCount(), 0);
    QCOMPARE(_manager->recordIndex(), 0);
}

void MacrosManagerTest::testRun()
{
    QString filePath = MockFileSystem::getMockFilePath("A04.txt");
    auto loadCommand = std::make_shared<LoadCloudCommand>(filePath, "A04", ",", 2, 1, "X,Y,Z,I,J,K", "1,2,3,4,5,6", ".");
    _manager->setRecording(true);
    _manager->log(loadCommand);
    _manager->setRecording(false);

    _manager->run();

    QVERIFY(_project->containsFigure("A04"));
}

void MacrosManagerTest::testSkipOne()
{
    QString filePath = MockFileSystem::getMockFilePath("A04.txt");
    auto loadCommand = std::make_shared<LoadCloudCommand>(
        filePath, "A04", ",", 2, 1, "X,Y,Z,I,J,K", "1,2,3,4,5,6", ".");
    auto renameCommand1 = std::make_shared<RenameFigureCommand>("A04", "NEW_A04");
    auto renameCommand2 = std::make_shared<RenameFigureCommand>("A04", "LAST_A04");

    _manager->setRecording(true);
    _manager->log(loadCommand);
    _manager->log(renameCommand1);
    _manager->log(renameCommand2);

    _manager->debugNext();
    _manager->skipOne();

    QVERIFY(_project->containsFigure("A04"));

    _manager->debugNext();
    QVERIFY(_project->containsFigure("LAST_A04"));
}
