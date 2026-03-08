#include "curve/pch.h"

#include "macrosmanager.h"
#include "filesystem.h"
#include "commandfactory.h"

#include "alignmentcommand.h"
#include "bestfitcommand.h"
#include "clearprojectcommand.h"
#include "createreportcommand.h"
#include "exporttoflrcommand.h"
#include "loadcloudcommand.h"
#include "loadprojectcommand.h"
#include "mergescanscommand.h"
#include "partdatacommand.h"
#include "printreportcommand.h"
#include "radiuscorrectioncommand.h"
#include "regeneratecurvecommand.h"
#include "removefigurecommand.h"
#include "renamefigurecommand.h"
#include "rotatefigurecommand.h"
#include "saveprojectcommand.h"
#include "setprintersettingscommand.h"
#include "shiftfigurecommand.h"

MacrosManager::MacrosManager() {
    _recordIndex = 0;
    _debugIndex = 0;
    _isRecording = false;
    _macros = new QList<std::shared_ptr<ICommand>>();
    registerAllCommands();
}

void MacrosManager::registerAllCommands() {
    auto& commandFactory = CommandFactory::instance();

    commandFactory.registerCommand<AlignmentCommand>();
    commandFactory.registerCommand<BestFitCommand>();
    commandFactory.registerCommand<ClearProjectCommand>();
    commandFactory.registerCommand<CreateReportCommand>();
    commandFactory.registerCommand<ExportToFLRCommand>();
    commandFactory.registerCommand<LoadCloudCommand>();
    commandFactory.registerCommand<LoadProjectCommand>();
    commandFactory.registerCommand<MergeScansCommand>();
    commandFactory.registerCommand<PartDataCommand>();
    commandFactory.registerCommand<PrintReportCommand>();
    commandFactory.registerCommand<RadiusCorrectionCommand>();
    commandFactory.registerCommand<RegenerateCurveCommand>();
    commandFactory.registerCommand<RemoveFigureCommand>();
    commandFactory.registerCommand<RenameFigureCommand>();
    commandFactory.registerCommand<RotateFigureCommand>();
    commandFactory.registerCommand<SaveProjectCommand>();
    commandFactory.registerCommand<SetPrinterSettingsCommand>();
    commandFactory.registerCommand<ShiftFigureCommand>();
}

MacrosManager& MacrosManager::instance() {
    static MacrosManager instance;
    return instance;
}

void MacrosManager::toggleRecording() {
    _isRecording = !_isRecording;
    emit recordingToggled();
}

void MacrosManager::setRecording(bool needRecording) {
    if(needRecording != isRecording()) {
        toggleRecording();
    }
}

void MacrosManager::swapOperations(int index1, int index2) {
    auto temp = _macros->at(index1);
    _macros->replace(index1, _macros->at(index2));
    _macros->replace(index2, temp);
}

void MacrosManager::log(std::shared_ptr<ICommand> command) {
    if(isRecording()) {
        insert(_recordIndex, command);
        emit operationLogged(command->getName(), command->getDescription());
    }
}

void MacrosManager::insert(int index, std::shared_ptr<ICommand> command) {
    _macros->insert(index, command);
    emit recordIndexChanged(++_recordIndex);
}

void MacrosManager::remove(int index) {
    if(index < _recordIndex) {
        _recordIndex--;
    }
    _macros->removeAt(index);
    emit instance().recordIndexChanged(_recordIndex);
}

bool MacrosManager::isRecording() {
    return _isRecording;
}

int MacrosManager::recordIndex() {
    return _recordIndex;
}

void MacrosManager::setRecordIndex(int newIndex) {
    _recordIndex = newIndex;
    emit recordIndexChanged(newIndex);
}

std::shared_ptr<ICommand> MacrosManager::getCommand(int index) {
    return _macros->at(index);
}

void MacrosManager::run() {
    auto startTime = QDateTime::currentSecsSinceEpoch();
    Plot::instance().setBuffering(true);
    for(auto i = 0; i < _macros->length(); i++) {
        auto status = tryExecuteOperation(i);
        if(!status) {
            break;
        }
    }
    Plot::instance().setBuffering(false);
    Plot::instance().zoomExtents();
    auto endTime = QDateTime::currentSecsSinceEpoch();
    qDebug() << "Elapsed: " << endTime - startTime;
}

void MacrosManager::executeOne(int index) {
    if(index < _macros->length()) {
        tryExecuteOperation(index);
    }
}

void MacrosManager::debugNext() {
    if(_debugIndex < _macros->length()) {
        auto status = tryExecuteOperation(_debugIndex);
        emit operationExecuted(_debugIndex, status);
        _debugIndex++;
    }
}

void MacrosManager::skipOne() {
    if(_debugIndex < _macros->length()) {
        emit instance().operationSkipped(_debugIndex);
        _debugIndex++;
    }
}

void MacrosManager::stopDebug() {
    _debugIndex = 0;
}

bool MacrosManager::tryExecuteOperation(int index) {
    auto project = &Project::instance();
    auto isCurrentlyRecording = isRecording();
    setRecording(false);
    try {
        _macros->at(index)->run();
        //switch(type) {
        //    case LoadCloud:
        //        FileSystem::loadCloud(project, data["filepath"], data["name"], data["separator"], data["skipStart"].toInt(),
        //            data["skipAfter"].toInt(), data["columnNames"], data["columnNumbers"], data["decimal"]);
        //        break;
        //    case LoadProject:
        //        FileSystem::loadProject(project, data["filepath"]);
        //        break;
        //    case SaveProject:
        //        FileSystem::saveProject(project, data["dirName"], data["ProjectName"], data["createCRV"] == "true");
        //        break;
        //    case ClearProject:
        //        project->clear();
        //        break;
        //    case MergeScans:
        //        Algorithms::tryMergePointClouds(data["firstName"], data["secondName"], data["resultName"],
        //            data["threshold"].toDouble(), data["needSorted"] == "true", project);
        //        break;
        //    case RadiusCorrection:
        //        Algorithms::makeRadiusCorrection(data["figureName"], data["newName"], new Function3Params(&data), project);
        //        break;
        //    case CalculateDeviations:
        //        Algorithms::calculateDeviations(data["nominal"], data["measured"], data["resultName"], new Function4Params(&data), project);
        //        break;
        //    case BestFit:
        //        Algorithms::calculateBestFit(data["nominal"], data["measured"], data["resultName"], data["bestFitLineName"], new Function6Params(&data), project);
        //        break;
        //    case ConstantTolerance:
        //        Algorithms::calculateConstantTolerances(data["figureName"], data["upperTolerance"].toDouble(), data["lowerTolerance"].toDouble(), project);
        //        break;
        //    case EdgesTolerance:
        //        Algorithms::calculateEdgesTolerance(data["figureName"], data["leadingEdgeDirection"].toInt(), data["leadingEdgePercent"].toDouble(),
        //            data["trailingEdgePercent"].toDouble(), data["leadingEdgeUpperTolerance"].toDouble(), data["leadingEdgeLowerTolerance"].toDouble(),
        //            data["trailingEdgeUpperTolerance"].toDouble(), data["trailingEdgeLowerTolerance"].toDouble(), data["highEdgeUpperTolerance"].toDouble(),
        //            data["highEdgeLowerTolerance"].toDouble(), data["lowEdgeUpperTolerance"].toDouble(), data["lowEdgeLowerTolerance"].toDouble(), project);
        //        break;
        //    case CreateDimension:
        //        break;
        //    case InsertBestFitPosition:
        //        Algorithms::insertBestFitDimension(data["figureName"], data["parentName"], data["x"].toDouble(), data["y"].toDouble(),
        //            data["z"].toDouble(), data["showX"] == "true", data["showY"] == "true", data["showR"] == "true", project);
        //        break;
        //    case RenameFigure:
        //        project->renameFigure(data["figureName"], data["newName"]);
        //        break;
        //    case RemoveFigure:
        //        project->removeFigure(data["figureName"]);
        //        break;
        //    case CreateWidthOfLE:
        //    {
        //        auto widthLE = WidthLE(0, 0, 0, data["distanceLE"]);
        //        auto &figureName = data["figureName"];
        //        widthLE.createMeasured(figureName, figureName, Function18Params(&data), project);
        //        break;
        //    }
        //    case CreateWidthOfTE:
        //    {
        //        auto widthTE = WidthTE(0, 0, 0, data["distanceTE"]);
        //        auto &figureName = data["figureName"];
        //        widthTE.createMeasured(figureName, figureName, Function18Params(&data), project);
        //        break;
        //    }
        //    case CreateReport:
        //        ReportGenerator::createReport(project, ReportSettings::convertToSettings(&data));
        //        break;
        //    case ChangeFigureColor:
        //        project->changeFigureColor(data["figureName"], *ColorTranslator::getColorFromInt(data["color"].toInt()));
        //        break;
        //    case ShiftFigure:
        //        project->shiftFigure(data["figureName"], data["x"], data["y"], data["z"]);
        //        break;
        //    case RotateFigure:
        //        project->rotateFigure(data["figureName"], data["angle"].toDouble(), data["x"], data["y"], data["z"]);
        //        break;
        //    case EditFigure:
        //        project->editFigure(data["figureName"], data);
        //        break;
        //    case Alignment:
        //        project->alignment(data["angle"], data["axis"], data["offsetX"], data["offsetY"]);
        //        break;
        //    case CalculateCurve:
        //        Algorithms::calculateCurve(data["figureName"], data["newFigureName"], new Function1Params(&data), project);
        //        break;
        //    case RegenerateCurve:
        //        Algorithms::regenerateCurve(data["figureName"], data["newFigureName"], new Function19Params(&data), project);
        //        break;
        //    case ChangeFigureVisibility:
        //        project->changeFigureVisibility(data["figureName"], data["visible"] == "true");
        //        break;
        //    case ChangeCurveParameters:
        //    {
        //        if(auto curve = dynamic_cast<const CurveFigure*>(project->findFigure(data["figureName"]))) {
        //            project->changeCurveParameters(data["figureName"], data["showPoints"] == "true", data["connectPoints"] == "true",
        //                data["showVectors"] == "true", data["closed"] == "true", data["showNumbering"] == "true", data["numberingInterval"].toInt(),
        //                data["amplification"].toDouble(), data["showTolerances"] == "true", data["showDeviations"] == "true",
        //                data["connectDeviations"] == "true", data["highLightOut"] == "true");
        //        } else {
        //            project->changeFigureVisibility(data["figureName"], data["isVisible"] == "true"); //isVisible
        //        }
        //        break;
        //    }
        //    case ChangeDimensionParameters:
        //        project->changeDimensionParameters(data["dimName"], data["onlyLabel"] == "true", data["showTols"] == "true",
        //            data["freePosition"] == "true");
        //        break;
        //    case CreateMaxCircle:
        //        Algorithms::createMaxCircle(data["parentName"], data["figureName"], new Function18Params(&data), project);
        //        break;
        //    case CreateMiddleCurve:
        //        Algorithms::createMiddleCurve(data["parentName"], data["figureName"], new Function18Params(&data), project);
        //        break;
        //    case CreateContactLine:
        //        Algorithms::createContactLine(data["parentName"], data["figureName"], new Function18Params(&data), project);
        //        break;
        //    case ExportToFLR:
        //    {
        //        auto curvesToTake = data["curvesToTake"].split(",");
        //        FileSystem::exportToFLR(project, data["filepath"], &curvesToTake);
        //        break;
        //    }
        //    case PartData:
        //        if(data["showPartDataWindowWhenMacroRuns"] == "Yes") {
        //            project->showPartDataDialogRequested(data["reportTitle"], data["description"], data["drawing"], data["orderNumber"], data["partNumber"], data["projectOperator"], data["note"],
        //                data["machine"], data["tool"], data["fixturing"], data["batch"], data["supplier"], data["revision"]);
        //        } else {
        //            project->changePartData(data["reportTitle"], data["description"], data["drawing"], data["orderNumber"], data["partNumber"], data["projectOperator"], data["note"],
        //                data["machine"], data["tool"], data["fixturing"], data["batch"], data["supplier"], data["revision"], false);
        //        }
        //        break;
        //    case PrintReport:
        //    {
        //        if(data.contains("pagesToTake"))
        //            Printer::print(data["pagesToTake"].split(","));
        //        else
        //            Printer::printAll();
        //        break;
        //    }
        //    case SetPrinterSettings:
        //    {
        //        auto printType = Printer::qStringToPrintType(data["printType"]);
        //        Printer::setPrinterSettings(printType);
        //        break;
        //    }
        //    case ClearReport:
        //        Printer::clear();
        //        break;
        //    case RemovePage:
        //        Printer::removePage(data["index"].toInt());
        //        break;
        //    case InsertText:
        //        project->constructText(data["name"], data["text"], data["x"].toDouble(), data["y"].toDouble(), data["textSize"].toDouble(),
        //            data["reference"], data["imageWidth"].toDouble(), data["imageHeight"].toDouble(), data["imageZoom"].toDouble());
        //        break;
        //    case ShowAll:
        //        project->showAllFigures(data["figuresType"]);
        //        break;
        //    case HideAll:
        //        project->hideAllFigures(data["figuresType"]);
        //        break;
        //    case CompareFLR:
        //        Algorithms::compareFLR(data["filepathFLR1"], data["filepathFLR2"], data["resultPath"], data["pointsStartIndex"].toInt(), data["precision"].toDouble());
        //        break;
        //    case Unknown:
        //        break;
        //}
    } catch(...) {
        return false;
    }
    setRecording(isCurrentlyRecording);
    return true;
}

QJsonArray MacrosManager::toJson() {
    QJsonArray array;
    for(auto i = 0; i < _macros->length(); i++) {
        array.append(_macros->at(i)->toJson());
    }
    return array;
}

void MacrosManager::fromJson(const QJsonArray& json) {
    auto isCurrentlyRecording = isRecording();
    setRecording(true);
    for(const auto& value : json) {
        if(value.isObject()) {
            auto command = CommandFactory::instance().createFromJson(value.toObject());
            if(command) {
                log(command);
            }
        }
    }
    setRecording(isCurrentlyRecording);

}

void MacrosManager::clear() {
    _macros->clear();
    _recordIndex = 0;
    emit recordIndexChanged(_recordIndex);
}
