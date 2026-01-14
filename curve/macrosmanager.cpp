#include "curve/pch.h"
#include "macrosmanager.h"

int MacrosManager::_operationsCount = 0;

bool MacrosManager::_isRecording = false;

QStringList* MacrosManager::_macros = new QStringList();

MacrosManager& MacrosManager::instance() {
    static MacrosManager instance;  
    return instance;
}

void MacrosManager::toggleRecording() {
    _isRecording = !_isRecording;
    emit instance().recordingToggled();
}

void MacrosManager::log(const Operation operation, const QMap<QString, QString> &params, QString comment) {
    if(_isRecording) {
        auto type = toString(operation);
        _operationsCount++;
        auto operationStr = type + '\n';
        for(auto pair = params.constBegin(); pair != params.constEnd(); pair++) {
            operationStr += pair.key() + "=" + pair.value() + "\n";
        }
        operationStr += comment.length() > 0 ? comment + "\n" : "";
        operationStr += "$END_OPERATION\n";
        _macros->append(operationStr);
        emit instance().operationLogged(type, operationStr);
    }
}

void MacrosManager::remove(int index) {
    _operationsCount--;
    _macros->removeAt(index);
}

bool MacrosManager::isRecording() {
    return _isRecording;
}

void MacrosManager::runMacros(Project *project) {
    for(auto i = 0; i < _macros->length(); i++) {
        auto operation = _macros->at(i).trimmed().split('\n');
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
                Algorithms::makeRadiusCorrection(data["figureName"], data["newName"],new Function3Params(data["radius_corr"].toDouble(),
                    data["closed"] == "Y", data["external"] == "Y", data["material"] == "L" ? FunctionParams::Direction::Left :
                    FunctionParams::Direction::Right, data["sort"] == "Y"), project);
                break;
            case CalculateDeviations:
                Algorithms::calculateDeviations(data["nominal"], data["measured"], data["resultName"], new Function4Params(
                    data["nomtol"].toDouble(), data["evaluationplace"].toInt(), data["evaluationdirection"].toInt(), data["closed"] == "Y",
                    data["external"] == "Y", data["material"] == "L" ? FunctionParams::Direction::Left : FunctionParams::Direction::Right,
                    data["sort"] == "Y"), project);
                break;
            case BestFit:
                Algorithms::calculateBestFit(data["nominal"], data["measured"], data["resultName"], new Function6Params(data["minimize"] == "Y",
                    data["method"] == "curve" ? Function6Params::Algorithm::Curve : Function6Params::Algorithm::Point, data["closed"] == "Y",
                    data["xshift"] == "Y", data["yshift"] == "Y", data["rotation"] == "Y", false, data["xshiftfrom"].toDouble(), data["xshiftto"].toDouble(),
                    false, data["yshiftfrom"].toDouble(), data["yshiftto"].toDouble(), false, data["rotationfrom"].toDouble(), data["rotationto"].toDouble()
                ), project);
                break;
            case AssignTolerance:
                break;
            case CreateDimension:
                break;
            case RenameFigure:
                project->renameFigure(data["figureName"], data["newName"]);
                break;
            case RemoveFigure:
                project->removeFigure(data["figureName"]);
                break;
            case GetWidthOfEdges:
                Algorithms::getWidthOfEdges(data["figureName"], data["distanceLE"].toDouble(), data["distanceTE"].toDouble(),
                    project, data["createSegmentLE"] == "true", data["createSegmentTE"] == "true");
                break;
            case FigureVisibilityChanged:
                project->changeFigureVisibility(data["figureName"], data["isVisible"] == "true");
                break;
            case Unknown:
                break;
        }
    }
}

void MacrosManager::saveRecord(const QString fileName) {
    FileSystem::writeFile(fileName, _macros->join(""));
    _macros->clear();
    _operationsCount = 0;
    _isRecording = false;
}

void MacrosManager::clear() {
    _macros->clear();
    _operationsCount = 0;
    _isRecording = false;
}

MacrosManager::Operation MacrosManager::fromString(QString operation) {
    return static_cast<Operation>(QMetaEnum::fromType<Operation>().keyToValue(operation.toLatin1()));// stringToEnumMap.value(operation, Unknown);
}

QString MacrosManager::toString(Operation operation) {
    return QMetaEnum::fromType<Operation>().valueToKey(operation); // stringToEnumMap.key(operation, "Unknown");
}
