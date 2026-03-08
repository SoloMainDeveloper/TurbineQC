#include "curve/pch.h"

#include "macrostranslator.h"

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

QStringList MacrosTranslator::splitCRM(const QString &text) {
    QStringList result;
    auto splitted = text.split('\n');
    QString current;

    for(auto i = 0; i < splitted.size(); i++) {
        QString line = splitted[i].trimmed(); // Удаляем пробелы в начале и конце
        current += splitted[i] + '\n'; // Сохраняем оригинальную строку с форматированием

        auto splittedLine = line.split(',');
        auto hasEnoughElements = splittedLine.length() > 1;

        if(hasEnoughElements) {
            // Удаляем возможные пробелы вокруг второго элемента
            QString secondElement = splittedLine[1].trimmed();

            // Также удаляем возможный символ возврата каретки
            secondElement.remove('\r');

            if(secondElement == "End") {
                result.append(current);
                current = QString();
            }
        }
    }
    return result;
}

MacrosTranslator::OperationCRM MacrosTranslator::operationCRMFromString(QString macrosType) {
    return static_cast<MacrosTranslator::OperationCRM>(QMetaEnum::fromType<MacrosTranslator::OperationCRM>()
        .keyToValue(macrosType.toLatin1()));
}

QList<std::shared_ptr<ICommand>>* MacrosTranslator::translateCRM(const QString &operationText) {
    auto readyOperations = new QList<std::shared_ptr<ICommand>>();
    auto operations = MacrosTranslator::splitCRM(operationText);
    for(auto i = 0; i < operations.size(); i++) {
        if(operations.at(i).isEmpty()) {
            continue;
        }

        auto operationText = operations.at(i).trimmed().split('\n');
        auto operationTypeCRM = operationCRMFromString(operationText[0].split(',').first());

        auto command = parseCRMCommand(operationTypeCRM, operationText);
        if(command) {
            readyOperations->append(command);
        }
    }
    return readyOperations;
}

std::shared_ptr<ICommand> MacrosTranslator::parseCRMCommand(OperationCRM type, QStringList operationText) {
    switch(type) {
        case MacrosTranslator::LoadCRV:
        {
            return std::make_shared<LoadProjectCommand>(operationText[0].split(',')[2]);
        }
        case MacrosTranslator::PartData:
        {
            auto firstLine = operationText[0].split(',');
            auto secondLine = operationText[1].split(',');

            auto description = firstLine.size() > 2 ? firstLine[2] : QString();
            auto drawing = firstLine.size() > 3 ? firstLine[3] : QString();
            auto orderNumber = firstLine.size() > 4 ? firstLine[4] : QString();
            auto partNumber = firstLine.size() > 5 ? firstLine[5] : QString();
            auto projectOperator = firstLine.size() > 6 ? firstLine[6] : QString();
            auto note = firstLine.size() > 7 ? firstLine[7] : QString();
            auto reportTitle = firstLine.size() > 8 ? firstLine[8] : QString();
            auto showPartDataWindowWhenMacroRuns = (secondLine.size() > 2 && secondLine[2] == "1") ? "Yes" : "No";
            auto machine = secondLine.size() > 3 ? secondLine[3] : QString();
            auto tool = secondLine.size() > 4 ? secondLine[4] : QString();
            auto fixturing = secondLine.size() > 5 ? secondLine[5] : QString();
            auto batch = secondLine.size() > 6 ? secondLine[6] : QString();
            auto supplier = secondLine.size() > 7 ? secondLine[7] : QString();
            auto revision = secondLine.size() > 8 ? secondLine[8] : QString();

            return std::make_shared<PartDataCommand>(
                reportTitle,
                description,
                drawing,
                orderNumber,
                partNumber,
                projectOperator,
                note,
                machine,
                tool,
                fixturing,
                batch,
                supplier,
                revision,
                showPartDataWindowWhenMacroRuns
            );
        }
        case MacrosTranslator::CollectCross:
        {
            auto firstLine = operationText[0].split(',');

            auto firstName = firstLine[2];
            auto secondName = firstLine[3];
            auto resultName = firstLine[4];
            auto needSorted = firstLine[5] == "1";
            auto threshold = firstLine[6].toDouble();

            return std::make_shared<MergeScansCommand>(firstName, secondName, resultName, threshold, needSorted);
        }
        case MacrosTranslator::Regen2D:
        {
            auto firstLine = operationText[0].split(',');
            auto figureName = firstLine[2];
            auto newFigureName = firstLine[3];
            auto closed = firstLine[4] == "Y";
            auto external = firstLine[5] == "Y";
            auto material = firstLine[6] == "L" ? FunctionParams::Direction::Left : FunctionParams::Direction::Right;
            QString mode = firstLine[7] == "number" ? "number" : "density";
            auto value = firstLine[8].toInt();

            return std::make_shared<RegenerateCurveCommand>(
                figureName, newFigureName, new Function19Params(closed, external, material, mode, value)
            );
        }
        case MacrosTranslator::Alignment:
        {
            QString angle;
            QString axis = "+X";
            QString offsetX;
            QString offsetY;

            for(auto j = 1; j < operationText.size() - 1; j++) {
                auto line = operationText[j].split(',');
                if(line[1] == "RotateSource") {
                    angle = line[2];
                    axis = line[3];
                } else if(line[1] == "Rotate")
                    angle = line[2];
                else if(line[1] == "OriginX" || line[1] == "OriginSourceX")
                    offsetX = line[2];
                else if(line[1] == "OriginY" || line[1] == "OriginSourceY")
                    offsetY = line[2];
            }
            return std::make_shared<AlignmentCommand>(angle, axis, offsetX, offsetY);
        }
        case MacrosTranslator::ShiftCurves1:
        {
            auto firstLine = operationText[0].split(',');

            auto figureName = firstLine[5];
            auto x = firstLine[2];
            auto y = firstLine[3];
            auto z = firstLine[4];

            return std::make_shared<ShiftFigureCommand>(figureName, x, y, z);
        }
        case MacrosTranslator::RotateCurves1:
        {
            auto firstLine = operationText[0].split(',');

            auto figureName = firstLine[7];
            auto angle = firstLine[3].toDouble();
            auto x = firstLine[4].toDouble() ? firstLine[4] : firstLine[4].mid(0, firstLine[4].length() - 2);
            auto y = firstLine[5].toDouble() ? firstLine[5] : firstLine[5].mid(0, firstLine[5].length() - 2);
            auto z = firstLine[6].toDouble() ? firstLine[6] : firstLine[6].mid(0, firstLine[6].length() - 2);

            return std::make_shared<RotateFigureCommand>(figureName, angle, x, y, z);
        }
        case MacrosTranslator::BestFit2D:
        {
            auto firstLine = operationText[1].split(',');
            auto secondLine = operationText[2].split(',');
            auto thirdLine = operationText[3].split(',');

            auto resultName = firstLine[2];
            auto nominal = firstLine[3];
            auto measured = firstLine[4];
            auto bestFitLineName = firstLine[2] + "_BF";

            auto method = secondLine[6] == "0" ? Function6Params::Algorithm::Curve : Function6Params::Algorithm::Point;
            auto minimize = true;
            auto closed = secondLine[2] == "1";
            auto xshift = secondLine[3] == "1";
            auto yshift = secondLine[4] == "1";
            auto rotation = secondLine[5] == "1";

            auto needHconstraint = thirdLine.length() > 2 && thirdLine[2] == "1";
            auto xshiftfrom = thirdLine.length() > 3 && thirdLine[2] == "1" ? thirdLine[3].toDouble() : 0;
            auto xshiftto = thirdLine.length() > 4 && thirdLine[2] == "1" ? thirdLine[4].toDouble() : 0;

            auto needVconstraint = thirdLine.length() > 5 && thirdLine[5] == "1";
            auto yshiftfrom = thirdLine.length() > 6 && thirdLine[5] == "1" ? thirdLine[6].toDouble() : 0;
            auto yshiftto = thirdLine.length() > 7 && thirdLine[5] == "1" ? thirdLine[7].toDouble() : 0;

            auto needRconstraint = thirdLine.length() > 8 && thirdLine[8] == "1";
            auto rotationfrom = thirdLine.length() > 9 && thirdLine[8] == "1" ? thirdLine[9].toDouble() : 0;
            auto rotationto = thirdLine.length() > 10 && thirdLine[8] == "1" ? thirdLine[10].toDouble() : 0;

            return std::make_shared<BestFitCommand>(
                nominal, measured, resultName, bestFitLineName,
                new Function6Params(
                minimize, method, closed, xshift, yshift, rotation,
                needHconstraint, xshiftfrom, xshiftto,
                needVconstraint, yshiftfrom, yshiftto,
                needRconstraint, rotationfrom, rotationto
            ));
        }
        case MacrosTranslator::Airfoil:
        {
            auto lines = QList<QStringList>();
            for(int i = 0; i < operationText.size(); i++) {
                lines.push_back(operationText[i].split(','));
            }

            auto data = ReportSettings::translateAirfoilSettings(lines);
            auto settings = ReportSettings::convertToSettings(&data);

            return std::make_shared<CreateReportCommand>(settings);
        }
        //case MacrosTranslator::InsertBFPos:
        //{
        //    auto firstLine = operationText[1].split(',');
        //    auto secondLine = operationText[2].split(',');
        //    params.insert({ { "showX", firstLine[2] == "1" ? "true" : "false" },
        //        { "showY", firstLine[3] == "1" ? "true" : "false" },
        //        { "showR", firstLine[4] == "1" ? "true" : "false" },
        //        { "figureName", secondLine[2] },
        //        { "parentName", secondLine[9] },
        //        { "x", secondLine[3] },
        //        { "y", secondLine[4] },
        //        { "z", secondLine[5] }
        //        });
        //    readyOperations->push_back({ Operation::InsertBestFitPosition, params });
        //}
        //case MacrosTranslator::ShowOptions:
        //{
        //    auto firstLine = operationText[0].split(',');
        //    auto secondLine = operationText[1].split(',');
        //    auto thirdLine = operationText[2].split(',');
        //    auto fourthLine = operationText[3].split(',');

        //    params.insert({ { "figureName", firstLine[2] },
        //        { "isVisible", secondLine[2] == "1" ? "true" : "false" },
        //        { "showPoints", thirdLine[2] == "1" ? "true" : "false" },
        //        { "connectPoints", thirdLine[3] == "1" ? "true" : "false" },
        //        { "showVectors", thirdLine[4] == "1" ? "true" : "false" },
        //        { "showNumbering", thirdLine[5] == "1" ? "true" : "false" },
        //        { "numberingInterval", thirdLine[6] },
        //        { "closed", thirdLine[7] == "1" ? "true" : "false" },
        //        { "amplification", fourthLine[2] },
        //        { "showTolerances", fourthLine[3] == "1" ? "true" : "false" },
        //        { "showDeviations", fourthLine[7] == "1" ? "true" : "false" },
        //        { "connectDeviations", fourthLine[8] == "1" ? "true" : "false" },
        //        { "highLightOut", fourthLine[9] == "1" ? "true" : "false" } });
        //    readyOperations->push_back({ Operation::ChangeCurveParameters, params });
        //}
        //case MacrosTranslator::EditDim:
        //{
        //    for(auto j = 5; j < operationText.size() - 1; j++) {
        //        auto str = operationText[j].split(',');
        //        QString updatedValue = "";
        //        updatedValue += "Show:";
        //        updatedValue += str[2] == "1" ? "true," : "false,";
        //        updatedValue += "Nom:" + str[3] + ",";
        //        updatedValue += "UT:" + str[4] + ",";
        //        updatedValue += "LT:" + str[5];
        //        params.insert(QString("Dim %1").arg(j - 4), updatedValue);
        //    }

        //    auto secondLine = operationText[1].split(',');
        //    params.insert({ { "figureName", secondLine[2] },
        //        { "x", secondLine[3] },
        //        { "y", secondLine[4] },
        //        { "z", secondLine[5] },
        //        { "Ref1", secondLine[6] },
        //        { "Ref2", secondLine.size() >= 8 ? secondLine[7] : QString() },
        //        { "newColor", operationText[4].split(',')[2] } });
        //    readyOperations->push_back({ Operation::EditFigure, params });
        //}
        case MacrosTranslator::SaveFLR:
        {
            auto firstLine = operationText[0].split(',');

            QString filepath = firstLine[2];
            QList<QString> curves;
            for(auto j = 1; j < operationText.size() - 1; j++) {
                auto line = operationText[j].split(',');
                curves.append(line[2]);
            }

            return std::make_shared<ExportToFLRCommand>(filepath, curves);
        }
        case MacrosTranslator::PrinterSettings:
        {
            Printer::PrintType type = Printer::qStringToPrintType(operationText[0].split(',')[3]);

            return std::make_shared<SetPrinterSettingsCommand>(type);
        }
        case MacrosTranslator::PrintFromViewer:
        {
            return std::make_shared<PrintReportCommand>();
        }
        case MacrosTranslator::ImportQDS:
        {
            // TODO
            auto filepath = operationText[0].split(',')[2];
            auto firstLine = FileSystem::readFile(filepath).split("\n")[0];
            auto name = QRegularExpression("NAM=([^,]*)").match(firstLine).captured(1);

            auto separator = ",";
            auto skipStart = 1;
            auto skipAfter = 1;
            auto columnNames = "X,Y,Z,I,J,K";
            auto columnNumbers = "1,2,3,4,5,6";
            auto decimal = ".";

            return std::make_shared<LoadCloudCommand>(
                filepath, name, separator, skipStart,
                skipAfter, columnNames, columnNumbers, decimal);
        }
        default:
            return nullptr;
            throw new std::runtime_error("Команда не имеет аналога");
    }
}
