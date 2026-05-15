#include "pch.h"

#include "algorithms.h"
#include "clearprojectcommand.h"
#include "loadprojectcommand.h"
#include "macrosmanager.h"
#include "macrosreadercrm.h"
#include "mockfilesystem.h"
#include "mockplot.h"
#include "plot.h"
#include "projecttest.h"
#include "qcustomplot.h"
#include "tests.h"

void TestMyClass::initTestCase()
{
    auto _mockPlot = new MockPlot();
    Plot::setTestingInstance(_mockPlot);

    _manager = &MacrosManager::instance();
    _project = &Project::instance();
    _macrosReaderCRM = new MacrosReaderCRM();
}

void TestMyClass::cleanup()
{
    _manager->clear();
    _project->clear();
}

void TestMyClass::macros1324Test()
{
    QString projectFilePath = MockFileSystem::getMockFilePath("Macros1324/pcdmis-data/points_pcdmis_final.crv");
    QString macrosFilePath = MockFileSystem::getMockFilePath("Macros1324/pcdmis-data/macros.crm");

    FileSystem::loadProject(projectFilePath);
    loadMacros(macrosFilePath);

    QCOMPARE(_manager->commandsCount(), 209);

    _manager->run();

    QString resultFLR = MockFileSystem::getMockFilePath("Macros1324/pcdmis-data/1.flr");
    QString curveAnalyzerResult = MockFileSystem::getMockFilePath("Macros1324/pcdmis-data/curveanalyzer.flr");

    auto compareResults = BladeGeometryService::compareOnlyDimensionsFLR(resultFLR, curveAnalyzerResult, 304);

    double tolerancePercent = 20;
    for(auto compareParams : compareResults) {
        QVERIFY(!compareParams->isDiffBiggerThanTolerance(tolerancePercent));
    }
}

void TestMyClass::loadMacros(QString filePath)
{
    _manager->setRecording(true);
    auto macros = _macrosReaderCRM->read(filePath);
    for(auto command : *macros) {
        _manager->log(command);
    }
    _manager->setRecording(false);
}
