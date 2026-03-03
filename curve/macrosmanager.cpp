#include "curve/pch.h"
#include "macrosmanager.h"

int MacrosManager::_recordIndex = 0;

int MacrosManager::_debugIndex = 0;

bool MacrosManager::_isRecording = false;

QStringList* MacrosManager::_macros = new QStringList();

MacrosManager& MacrosManager::instance() {
    static MacrosManager instance;
    return instance;
}

void MacrosManager::load() {
    auto path = QFileDialog::getOpenFileName(nullptr, "Open file", "", "Macros (*.crmm *.crm)");
    if(!path.isEmpty()) {
        auto macrosType = macrosTypeFromString(path.split('.').last().toUpper());
        auto operations = translate(macrosType, FileSystem::readFile(path));
        //auto validated = validateOperations(operations);
        auto isCurrentlyRecording = isRecording();
        setRecording(true);
        for(auto i = 0; i < operations->size(); i++) {
            log(operations->at(i).first, operations->at(i).second);
        }
        setRecording(isCurrentlyRecording);
    }
}

QList<QPair<MacrosManager::Operation, QMap<QString, QString>>>* MacrosManager::translate(MacrosType type, const QString &operationText) {
    auto readyOperations = new QList<QPair<Operation, QMap<QString, QString>>>();
    switch(type) {
        case MacrosType::CRM:
        {
            auto operations = MacrosTranslator::splitCRM(operationText);
            for(auto i = 0; i < operations.size(); i++) {
                if(operations.at(i).isEmpty()) continue;
                auto operationText = operations.at(i).trimmed().split('\n');
                auto operationTypeCRM = MacrosTranslator::operationCRMFromString(operationText[0].split(',').first());
                QMap<QString, QString> params;
                switch(operationTypeCRM) {
                    case MacrosTranslator::LoadCRV:
                        params.insert("filepath", operationText[0].split(',')[2]);
                        readyOperations->push_back({ Operation::LoadProject, params });
                        break;
                    case MacrosTranslator::PartData:
                    {
                        auto firstLine = operationText[0].split(',');
                        auto secondLine = operationText[1].split(',');

                        params.insert({ { "description", firstLine.size() > 2 ? firstLine[2] : QString() },
                            { "drawing", firstLine.size() > 3 ? firstLine[3] : QString() },
                            { "orderNumber", firstLine.size() > 4 ? firstLine[4] : QString() },
                            { "partNumber", firstLine.size() > 5 ? firstLine[5] : QString() },
                            { "projectOperator", firstLine.size() > 6 ? firstLine[6] : QString() },
                            { "note", firstLine.size() > 7 ? firstLine[7] : QString() },
                            { "reportTitle", firstLine.size() > 8 ? firstLine[8] : QString() },
                            { "showPartDataWindowWhenMacroRuns", (secondLine.size() > 2 && secondLine[2] == "1") ? "Yes" : "No" },
                            { "machine", secondLine.size() > 3 ? secondLine[3] : QString() },
                            { "tool", secondLine.size() > 4 ? secondLine[4] : QString() },
                            { "fixturing", secondLine.size() > 5 ? secondLine[5] : QString() },
                            { "batch", secondLine.size() > 6 ? secondLine[6] : QString() },
                            { "supplier", secondLine.size() > 7 ? secondLine[7] : QString() },
                            { "revision", secondLine.size() > 8 ? secondLine[8] : QString() } });
                        readyOperations->push_back({ Operation::PartData, params });
                        break;
                    }
                    case MacrosTranslator::CollectCross:
                    {
                        auto firstLine = operationText[0].split(',');

                        params.insert({ { "firstName", firstLine[2] },
                            { "secondName", firstLine[3] },
                            { "resultName", firstLine[4] },
                            { "needSorted", firstLine[5] == "1" ? "true" : "false" },
                            { "threshold", firstLine[6] } });
                        readyOperations->push_back({ Operation::MergeScans, params });
                        break;
                    }
                    case MacrosTranslator::Regen2D:
                    {
                        auto firstLine = operationText[0].split(',');
                        params.insert({ { "figureName", firstLine[2] },
                            { "newFigureName", firstLine[3] },
                            { "closed", firstLine[4] == "Y" ? "Y" : "N" },
                            { "external", firstLine[5] == "Y" ? "Y" : "N" },
                            { "material", firstLine[6] == "L" ? "L" : "R" },
                            { "mode", firstLine[7] == "number" ? "number" : "density" },
                            { "value", firstLine[8] } });
                        readyOperations->push_back({ Operation::RegenerateCurve, params });
                        break;
                    }
                    case MacrosTranslator::Alignment:
                    {
                        params.insert("axis", "+X");
                        for(auto j = 1; j < operationText.size() - 1; j++) {
                            auto line = operationText[j].split(',');
                            if(line[1] == "RotateSource") {
                                params["angle"] = line[2];
                                params["axis"] = line[3];
                            } else if(line[1] == "Rotate")
                                params["angle"] = line[2];
                            else if(line[1] == "OriginX" || line[1] == "OriginSourceX")
                                params["offsetX"] = line[2];
                            else if(line[1] == "OriginY" || line[1] == "OriginSourceY")
                                params["offsetY"] = line[2];
                        }
                        readyOperations->push_back({ Operation::Alignment, params });
                        break;
                    }
                    case MacrosTranslator::ShiftCurves1:
                    {
                        auto firstLine = operationText[0].split(',');
                        params.insert({ { "figureName", firstLine[5] },
                            { "x", firstLine[2] },
                            { "y", firstLine[3] },
                            { "z", firstLine[4] } });
                        readyOperations->push_back({ Operation::ShiftFigure, params });
                        break;
                    }
                    case MacrosTranslator::RotateCurves1:
                    {
                        auto firstLine = operationText[0].split(',');
                        params.insert({ { "figureName", firstLine[7] },
                            { "angle", firstLine[3] },
                            { "x", firstLine[4].toDouble() ? firstLine[4] : firstLine[4].mid(0, firstLine[4].length() - 2) },
                            { "y", firstLine[5].toDouble() ? firstLine[5] : firstLine[5].mid(0, firstLine[5].length() - 2) },
                            { "z", firstLine[6].toDouble() ? firstLine[6] : firstLine[6].mid(0, firstLine[6].length() - 2) } });
                        readyOperations->push_back({ Operation::RotateFigure, params });
                        break;
                    }
                    case MacrosTranslator::BestFit2D:
                    {
                        auto firstLine = operationText[1].split(',');
                        auto secondLine = operationText[2].split(',');
                        auto thirdLine = operationText[3].split(',');
                        params.insert({ { "resultName", firstLine[2] },
                            { "nominal", firstLine[3] },
                            { "measured", firstLine[4] },
                            { "bestFitLineName", firstLine[2] + "_BF" },
                            { "method", secondLine[6] == "0" ? "curve" : "point" },
                            { "minimize", "Y" },
                            { "closed", secondLine[2] == "1" ? "Y" : "N" },
                            { "xshift", secondLine[3] == "1" ? "Y" : "N" },
                            { "yshift", secondLine[4] == "1" ? "Y" : "N" },
                            { "rotation", secondLine[5] == "1" ? "Y" : "N" },
                            { "needHconstraint", thirdLine.length() > 2 && thirdLine[2] == "1" ? "Y" : "N" },
                            { "xshiftfrom", thirdLine.length() > 3 && thirdLine[2] == "1" ? thirdLine[3] : 0 },
                            { "xshiftto", thirdLine.length() > 4 && thirdLine[2] == "1" ? thirdLine[4] : 0 },
                            { "needVconstraint", thirdLine.length() > 5 && thirdLine[5] == "1" ? "Y" : "N" },
                            { "yshiftfrom", thirdLine.length() > 6 && thirdLine[5] == "1" ? thirdLine[6] : 0 },
                            { "yshiftto", thirdLine.length() > 7 && thirdLine[5] == "1" ? thirdLine[7] : 0 },
                            { "needRconstraint", thirdLine.length() > 8 && thirdLine[8] == "1" ? "Y" : "N" },
                            { "rotationfrom", thirdLine.length() > 9 && thirdLine[8] == "1" ? thirdLine[9] : 0 },
                            { "rotationto", thirdLine.length() > 10 && thirdLine[8] == "1" ? thirdLine[10] : 0 }
                            });
                        readyOperations->push_back({ Operation::BestFit, params });
                        break;
                    }
                    case MacrosTranslator::Airfoil:
                    {
                        auto lines = QList<QStringList>();
                        for(int i = 0; i < operationText.size(); i++)
                            lines.push_back(operationText[i].split(','));
                        params.insert(ReportSettings::translateAirfoilSettings(lines));
                        readyOperations->push_back({ Operation::CreateReport, params });
                        break;
                    }
                    case MacrosTranslator::InsertBFPos:
                    {
                        auto firstLine = operationText[1].split(',');
                        auto secondLine = operationText[2].split(',');
                        params.insert({ { "showX", firstLine[2] == "1" ? "true" : "false" },
                            { "showY", firstLine[3] == "1" ? "true" : "false" },
                            { "showR", firstLine[4] == "1" ? "true" : "false" },
                            { "figureName", secondLine[2] },
                            { "parentName", secondLine[9] },
                            { "x", secondLine[3] },
                            { "y", secondLine[4] },
                            { "z", secondLine[5] }
                            });
                        readyOperations->push_back({ Operation::InsertBestFitPosition, params });
                        break;
                    }
                    case MacrosTranslator::ShowOptions:
                    {
                        auto firstLine = operationText[0].split(',');
                        auto secondLine = operationText[1].split(',');
                        auto thirdLine = operationText[2].split(',');
                        auto fourthLine = operationText[3].split(',');

                        params.insert({ { "figureName", firstLine[2] },
                            { "isVisible", secondLine[2] == "1" ? "true" : "false" },
                            { "showPoints", thirdLine[2] == "1" ? "true" : "false" },
                            { "connectPoints", thirdLine[3] == "1" ? "true" : "false" },
                            { "showVectors", thirdLine[4] == "1" ? "true" : "false" },
                            { "showNumbering", thirdLine[5] == "1" ? "true" : "false" },
                            { "numberingInterval", thirdLine[6] },
                            { "closed", thirdLine[7] == "1" ? "true" : "false" },
                            { "amplification", fourthLine[2] },
                            { "showTolerances", fourthLine[3] == "1" ? "true" : "false" },
                            { "showDeviations", fourthLine[7] == "1" ? "true" : "false" },
                            { "connectDeviations", fourthLine[8] == "1" ? "true" : "false" },
                            { "highLightOut", fourthLine[9] == "1" ? "true" : "false" } });
                        readyOperations->push_back({ Operation::ChangeCurveParameters, params });
                        break;
                    }
                    case MacrosTranslator::EditDim:
                    {
                        for(auto j = 5; j < operationText.size() - 1; j++) {
                            auto str = operationText[j].split(',');
                            QString updatedValue = "";
                            updatedValue += "Show:";
                            updatedValue += str[2] == "1" ? "true," : "false,";
                            updatedValue += "Nom:" + str[3] + ",";
                            updatedValue += "UT:" + str[4] + ",";
                            updatedValue += "LT:" + str[5];
                            params.insert(QString("Dim %1").arg(j - 4), updatedValue);
                        }

                        auto secondLine = operationText[1].split(',');
                        params.insert({ { "figureName", secondLine[2] },
                            { "x", secondLine[3] },
                            { "y", secondLine[4] },
                            { "z", secondLine[5] },
                            { "Ref1", secondLine[6] },
                            { "Ref2", secondLine.size() >= 8 ? secondLine[7] : QString() },
                            { "newColor", operationText[4].split(',')[2] } });
                        readyOperations->push_back({ Operation::EditFigure, params });
                        break;
                    }
                    case MacrosTranslator::SaveFLR:
                    {
                        auto firstLine = operationText[0].split(',');
                        params.insert("filepath", firstLine[2]);
                        //params.insert("mode", firstLine[3]);
                        QList<QString> curves;
                        for(auto j = 1; j < operationText.size() - 1; j++) {
                            auto line = operationText[j].split(',');
                            curves.append(line[2]);
                        }
                        params.insert("curvesToTake", curves.join(","));
                        readyOperations->push_back({ Operation::ExportToFLR, params });
                        break;
                    }
                    case MacrosTranslator::PrinterSettings:
                    {
                        params.insert("printType", operationText[0].split(',')[3]);
                        readyOperations->push_back({ Operation::SetPrinterSettings, params });
                        break;
                    }
                    case MacrosTranslator::PrintFromViewer:
                    {
                        readyOperations->push_back({ Operation::PrintReport, params });
                        break;
                    }
                    case MacrosTranslator::ImportQDS:
                    {
                        auto filepath = operationText[0].split(',')[2];
                        auto firstLine = FileSystem::readFile(filepath).split("\n")[0];
                        auto name = QRegularExpression("NAM=([^,]*)").match(firstLine).captured(1);

                        params.insert({ { "filepath", filepath },
                            { "name", name },
                            { "separator", "," },
                            { "skipStart", "1" },
                            { "skipAfter", "1" },
                            { "columnNames", "X,Y,Z,I,J,K" },
                            { "columnNumbers", "1,2,3,4,5,6" },
                            { "decimal", "." } });
                        readyOperations->push_back({ Operation::LoadCloud, params });
                        break;
                    }
                    default:
                        break;
                }
            }
            break;
        }
        case MacrosType::CRMM:
        {
            auto operations = operationText.split("$END_OPERATION");
            for(auto i = 0; i < operations.size(); i++) {
                if(operations.at(i).isEmpty()) continue;
                auto operationText = operations.at(i).trimmed().split('\n');
                auto type = operationText[0].trimmed();
                auto params = QMap<QString, QString>();
                for(auto j = 1; j < operationText.size(); j++) {
                    auto pair = operationText[j].split('=');
                    params.insert(pair[0], pair[1]);
                }
                readyOperations->push_back({ fromString(type), params });
            }
            break;
        }
        default:
            break;
    }
    return readyOperations;
}

void MacrosManager::toggleRecording() {
    _isRecording = !_isRecording;
    emit instance().recordingToggled();
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

void MacrosManager::log(const Operation operation, const QMap<QString, QString> &params) {
    if(isRecording()) {
        auto operationText = createOperationText(operation, params);
        insert(_recordIndex, operationText);
        emit instance().operationLogged(toString(operation), operationText);
    }
}

QString MacrosManager::createOperationText(const Operation operation, const QMap<QString, QString> &params) {
    auto type = toString(operation);
    auto operationStr = type + '\n';
    for(auto pair = params.constBegin(); pair != params.constEnd(); pair++) {
        operationStr += pair.key() + "=" + pair.value() + "\n";
    }
    operationStr += "$END_OPERATION\n";
    return operationStr;
}

void MacrosManager::insert(int index, QString operationText) {
    _macros->insert(index, operationText);
    emit instance().recordIndexChanged(++_recordIndex);
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

void MacrosManager::editOperation(int index, QString newOperationText) {
    _macros->replace(index, newOperationText);
}

int MacrosManager::recordIndex() {
    return _recordIndex;
}

void MacrosManager::setRecordIndex(int newIndex) {
    _recordIndex = newIndex;
    emit instance().recordIndexChanged(newIndex);
}

void MacrosManager::run(Project *project, Plot *plot) {
    auto startTime = QDateTime::currentSecsSinceEpoch();
    plot->setBuffering(true);
    for(auto i = 0; i < _macros->length(); i++) {
        auto status = tryExecuteOperation(project, plot, i);
        if(!status) {
            break;
        }
    }
    plot->setBuffering(false);
    plot->zoomExtents();
    auto endTime = QDateTime::currentSecsSinceEpoch();
    qDebug() << "Elapsed: " << endTime - startTime;
}

void MacrosManager::executeOne(Project *project, Plot *plot, int index) {
    if(index < _macros->length()) {
        tryExecuteOperation(project, plot, index);
    }
}

void MacrosManager::debugNext(Project *project, Plot *plot) {
    if(_debugIndex < _macros->length()) {
        auto status = tryExecuteOperation(project, plot, _debugIndex);
        emit instance().operationExecuted(_debugIndex, status);
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

bool MacrosManager::tryExecuteOperation(Project *project, Plot *plot, int index) {
    auto isCurrentlyRecording = isRecording();
    setRecording(false);
    auto operation = _macros->at(index).trimmed().split('\n');
    auto type = fromString(operation[0]);
    QMap<QString, QString> data;
    for(auto i = 1; i < operation.length(); i++) {
        auto pairs = operation[i].split('=');
        if(pairs.size() == 2) {
            auto key = pairs[0];
            auto value = pairs[1];
            data[key] = value;
        }
    }
    try {
        switch(type) {
            case LoadCloud:
                FileSystem::loadCloud(project, data["filepath"], data["name"], data["separator"], data["skipStart"].toInt(),
                    data["skipAfter"].toInt(), data["columnNames"], data["columnNumbers"], data["decimal"]);
                break;
            case LoadProject:
                FileSystem::loadProject(project, data["filepath"]);
                break;
            case SaveProject:
                FileSystem::saveProject(project, data["dirName"], data["ProjectName"], data["createCRV"] == "true");
                break;
            case ClearProject:
                project->clear();
                break;
            case MergeScans:
                Algorithms::tryMergePointClouds(data["firstName"], data["secondName"], data["resultName"],
                    data["threshold"].toDouble(), data["needSorted"] == "true", project);
                break;
            case RadiusCorrection:
                Algorithms::makeRadiusCorrection(data["figureName"], data["newName"], new Function3Params(&data), project);
                break;
            case CalculateDeviations:
                Algorithms::calculateDeviations(data["nominal"], data["measured"], data["resultName"], new Function4Params(&data), project);
                break;
            case BestFit:
                Algorithms::calculateBestFit(data["nominal"], data["measured"], data["resultName"], data["bestFitLineName"], new Function6Params(&data), project);
                break;
            case ConstantTolerance:
                Algorithms::calculateConstantTolerances(data["figureName"], data["upperTolerance"].toDouble(), data["lowerTolerance"].toDouble(), project);
                break;
            case EdgesTolerance:
                Algorithms::calculateEdgesTolerance(data["figureName"], data["leadingEdgeDirection"].toInt(), data["leadingEdgePercent"].toDouble(),
                    data["trailingEdgePercent"].toDouble(), data["leadingEdgeUpperTolerance"].toDouble(), data["leadingEdgeLowerTolerance"].toDouble(),
                    data["trailingEdgeUpperTolerance"].toDouble(), data["trailingEdgeLowerTolerance"].toDouble(), data["highEdgeUpperTolerance"].toDouble(),
                    data["highEdgeLowerTolerance"].toDouble(), data["lowEdgeUpperTolerance"].toDouble(), data["lowEdgeLowerTolerance"].toDouble(), project);
                break;
            case CreateDimension:
                break;
            case InsertBestFitPosition:
                Algorithms::insertBestFitDimension(data["figureName"], data["parentName"], data["x"].toDouble(), data["y"].toDouble(),
                    data["z"].toDouble(), data["showX"] == "true", data["showY"] == "true", data["showR"] == "true", project);
                break;
            case RenameFigure:
                project->renameFigure(data["figureName"], data["newName"]);
                break;
            case RemoveFigure:
                project->removeFigure(data["figureName"]);
                break;
            case CreateWidthOfLE:
            {
                auto widthLE = WidthLE(0, 0, 0, data["distanceLE"]);
                auto &figureName = data["figureName"];
                widthLE.createMeasured(figureName, figureName, Function18Params(&data), project);
                break;
            }
            case CreateWidthOfTE:
            {
                auto widthTE = WidthTE(0, 0, 0, data["distanceTE"]);
                auto &figureName = data["figureName"];
                widthTE.createMeasured(figureName, figureName, Function18Params(&data), project);
                break;
            }
            case CreateReport:
                ReportGenerator::createReport(project, plot, ReportSettings::convertToSettings(&data));
                break;
            case ChangeFigureColor:
                project->changeFigureColor(data["figureName"], *ColorTranslator::getColorFromInt(data["color"].toInt()));
                break;
            case ShiftFigure:
                project->shiftFigure(data["figureName"], data["x"], data["y"], data["z"]);
                break;
            case RotateFigure:
                project->rotateFigure(data["figureName"], data["angle"].toDouble(), data["x"], data["y"], data["z"]);
                break;
            case EditFigure:
                project->editFigure(data["figureName"], data);
                break;
            case Alignment:
                project->alignment(data["angle"], data["axis"], data["offsetX"], data["offsetY"]);
                break;
            case CalculateCurve:
                Algorithms::calculateCurve(data["figureName"], data["newFigureName"], new Function1Params(&data), project);
                break;
            case RegenerateCurve:
                Algorithms::regenerateCurve(data["figureName"], data["newFigureName"], new Function19Params(&data), project);
                break;
            case ChangeFigureVisibility:
                project->changeFigureVisibility(data["figureName"], data["visible"] == "true");
                break;
            case ChangeCurveParameters:
            {
                if(auto curve = dynamic_cast<const CurveFigure*>(project->findFigure(data["figureName"]))) {
                    project->changeCurveParameters(data["figureName"], data["showPoints"] == "true", data["connectPoints"] == "true",
                        data["showVectors"] == "true", data["closed"] == "true", data["showNumbering"] == "true", data["numberingInterval"].toInt(),
                        data["amplification"].toDouble(), data["showTolerances"] == "true", data["showDeviations"] == "true",
                        data["connectDeviations"] == "true", data["highLightOut"] == "true");
                } else {
                    project->changeFigureVisibility(data["figureName"], data["isVisible"] == "true"); //isVisible
                }
                break;
            }
            case ChangeDimensionParameters:
                project->changeDimensionParameters(data["dimName"], data["onlyLabel"] == "true", data["showTols"] == "true",
                    data["freePosition"] == "true");
                break;
            case CreateMaxCircle:
                Algorithms::createMaxCircle(data["parentName"], data["figureName"], new Function18Params(&data), project);
                break;
            case CreateMiddleCurve:
                Algorithms::createMiddleCurve(data["parentName"], data["figureName"], new Function18Params(&data), project);
                break;
            case CreateContactLine:
                Algorithms::createContactLine(data["parentName"], data["figureName"], new Function18Params(&data), project);
                break;
            case ExportToFLR:
            {
                auto curvesToTake = data["curvesToTake"].split(",");
                FileSystem::exportToFLR(project, data["filepath"], &curvesToTake);
                break;
            }
            case PartData:
                if(data["showPartDataWindowWhenMacroRuns"] == "Yes") {
                    project->showPartDataDialogRequested(data["reportTitle"], data["description"], data["drawing"], data["orderNumber"], data["partNumber"], data["projectOperator"], data["note"],
                        data["machine"], data["tool"], data["fixturing"], data["batch"], data["supplier"], data["revision"]);
                } else {
                    project->changePartData(data["reportTitle"], data["description"], data["drawing"], data["orderNumber"], data["partNumber"], data["projectOperator"], data["note"],
                        data["machine"], data["tool"], data["fixturing"], data["batch"], data["supplier"], data["revision"], false);
                }
                break;
            case PrintReport:
            {
                if(data.contains("pagesToTake"))
                    Printer::print(data["pagesToTake"].split(","));
                else
                    Printer::printAll();
                break;
            }
            case SetPrinterSettings:
            {
                auto printType = Printer::qStringToPrintType(data["printType"]);
                Printer::setPrinterSettings(printType);
                break;
            }
            case ClearReport:
                Printer::clear();
                break;
            case RemovePage:
                Printer::removePage(data["index"].toInt());
                break;
            case InsertText:
                project->constructText(data["name"], data["text"], data["x"].toDouble(), data["y"].toDouble(), data["textSize"].toDouble(),
                    data["reference"], data["imageWidth"].toDouble(), data["imageHeight"].toDouble(), data["imageZoom"].toDouble());
                break;
            case ShowAll:
                project->showAllFigures(data["figuresType"]);
                break;
            case HideAll:
                project->hideAllFigures(data["figuresType"]);
                break;
            case CompareFLR:
                Algorithms::compareFLR(data["filepathFLR1"], data["filepathFLR2"], data["resultPath"], data["pointsStartIndex"].toInt(), data["precision"].toDouble());
                break;
            case Unknown:
                break;
        }
    } catch(...) {
        return false;
    }
    setRecording(isCurrentlyRecording);
    return true;
}

void MacrosManager::save(const QString fileName) {
    FileSystem::writeFile(fileName, _macros->join(""));
}

void MacrosManager::clear() {
    _macros->clear();
    _recordIndex = 0;
    emit instance().recordIndexChanged(_recordIndex);
}

MacrosManager::Operation MacrosManager::fromString(QString operation) {
    return static_cast<Operation>(QMetaEnum::fromType<Operation>().keyToValue(operation.toLatin1()));
}

QString MacrosManager::toString(Operation operation) {
    return QMetaEnum::fromType<Operation>().valueToKey(operation);
}

MacrosManager::MacrosType MacrosManager::macrosTypeFromString(QString macrosType) {
    return static_cast<MacrosType>(QMetaEnum::fromType<MacrosType>().keyToValue(macrosType.toLatin1()));
}

QStringList MacrosTranslator::splitCRM(const QString &text) {
    QStringList result;
    auto splitted = text.split('\n');
    QString current;
    for(auto i = 0; i < splitted.size(); i++) {
        current += splitted[i] + '\n';
        if(splitted[i].split(',')[1] == "End") {
            result.append(current);
            current = QString();
        }
    }
    return result;
}

MacrosTranslator::OperationCRM MacrosTranslator::operationCRMFromString(QString macrosType) {
    return static_cast<MacrosTranslator::OperationCRM>(QMetaEnum::fromType<MacrosTranslator::OperationCRM>()
        .keyToValue(macrosType.toLatin1()));
}
