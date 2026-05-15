#include "pch.h"

#include "executorcli.h"
#include "macrosmanager.h"
#include "macrosreadercrm.h"
#include "mockplot.h"
#include "plot.h"
#include "qcustomplot.h"

ExecutorCLI::ExecutorCLI()
{
	auto _mockPlot = new MockPlot();
	Plot::setTestingInstance(_mockPlot);

	_manager = &MacrosManager::instance();
	_project = &Project::instance();
	_macrosReaderCRM = new MacrosReaderCRM();
}

int ExecutorCLI::run()
{
	qDebug() << "Executing macros. Total commands:" << _manager->commandsCount();

	_manager->run();

	qDebug() << "Macros execution finished.";

	return 0;
}

void ExecutorCLI::loadMacros(QString filePath)
{
	qDebug() << "Reading macros from:" << filePath;

	_manager->setRecording(true);
	auto macros = _macrosReaderCRM->read(filePath);
	for (auto command : *macros) {
		_manager->log(command);
	}
	_manager->setRecording(false);
}

void ExecutorCLI::loadProject(QString filePath)
{
	qDebug() << "Loading project:" << filePath;

	FileSystem::loadProject(filePath);
}
