#include "curve/pch.h"
#include "filesystem.h"

Point FileSystem::_offsetPoint = Point(0, 0);
double FileSystem::_scaleFactor = 1;
QString FileSystem::curveDirectory = "C:/Users/Admin/Desktop/test/curve-1.exe";

bool FileSystem::checkCurve() {
    QFile file(curveDirectory);
    return file.exists();
}

bool FileSystem::isValidFileName(QString fileName) {
    QRegularExpression pattern("^[a-zA-Z0-9_-]+$");
    return fileName.contains(pattern);
}

void FileSystem::createWorkEnvironment() {
    QDir dir;
    dir.mkpath("C:/temp/ca/project");
}

void FileSystem::fillInputWithSingleSegment(QVector<CurvePoint> points) {
    QFile input("C:/temp/ca/project/2dinp.dat");
    input.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream inputStream(&input);
    inputStream << "$ELE (NAM=input_TE,TYP=APT, FLD=(X,Y,Z))\n";
    fillFileWithPoints(inputStream, points, 4, Order::CurveLibrary);
    inputStream << "$END\n";
    input.close();
}

void FileSystem::fillParams(FunctionParams* params) {
    QFile param("C:/temp/ca/project/2dpar.dat");
    param.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream paramStream(&param);
    paramStream << params->toQString();
    param.close();
}

QString FileSystem::readOutput() {
    return readFile("C:/temp/ca/project/2dout.dat");
}

QString FileSystem::readFile(QString fullFileName) {
    QFile output(fullFileName);
    output.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream outputStream(&output);
    auto result = outputStream.readAll().trimmed();
    output.close();
    return result;
}

void FileSystem::writeFile(QString fullFileName, QString text) {
    QFile output(fullFileName);
    output.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream outputStream(&output);
    outputStream << text;
    output.close();
}

QVector<CurvePoint> FileSystem::parsePointsFromElement(QStringList element, QString separator, int startLineToSkip, int finishLineToSkip,
    Order order, QString columnNames, QString numberOfColumns, QString decimal) {
    QVector<CurvePoint> result;

    for(auto i = startLineToSkip; i < element.length() - finishLineToSkip; i++) {
        auto pointStr = element[i].split(separator);
        if(pointStr.length() > 1) {
            if(columnNames != nullptr && numberOfColumns != nullptr) {
                auto resultColumnNames = columnNames.replace(",", "");
                auto resultNumberOfColumns = numberOfColumns.replace(",", "");

                if(!isColumnsCorrect(resultColumnNames, resultNumberOfColumns) || !hasDecimalSeparator(pointStr, decimal)
                    || pointStr.length() < resultNumberOfColumns.length()) {
                    break;
                } else {
                    auto point = getPoint(pointStr, order, decimal, resultNumberOfColumns.length());
                    result.append(point);
                }
            } else {
                auto point = getPoint(pointStr, order, decimal);
                result.append(point);
            }
        } else {
            break;
        }
    }
    return result;
}

bool FileSystem::isColumnsCorrect(const QString &columnNames, const QString &numberOfColumns) {
    if(columnNames == "XY" && numberOfColumns == "12" || columnNames == "XYZ" && numberOfColumns == "123"
        || columnNames == "XYZIJK" && numberOfColumns == "123456" || columnNames == "XYZIJKD" && numberOfColumns == "1234567"
        || columnNames == "XYZIJKDUTLT" && numberOfColumns == "123456789") {
        return true;
    } else {
        return false;
    }
}

bool FileSystem::hasDecimalSeparator(const QStringList &pointStr, const QString &decimal) {
    for(auto i = 0; i < pointStr.length(); i++) {
        if(!pointStr[i].contains(decimal)) {
            return false;
        }
    }
    return true;
}

CurvePoint FileSystem::getPoint(QStringList pointStr, Order order, const QString &decimal, int numberOfColumns) {
    auto point = CurvePoint();
    switch(order) {
        case Order::CurveLibrary:
        {
            point.x = pointStr.length() > 0 && 1 <= numberOfColumns ? pointStr[0].replace(decimal, ".").toDouble() : 0;
            point.y = pointStr.length() > 1 && 2 <= numberOfColumns ? pointStr[1].replace(decimal, ".").toDouble() : 0;
            point.z = pointStr.length() > 2 && 3 <= numberOfColumns ? pointStr[2].replace(decimal, ".").toDouble() : 0;
            point.ut = pointStr.length() > 3 && 4 <= numberOfColumns ? pointStr[3].replace(decimal, ".").toDouble() : 0;
            point.i = pointStr.length() > 4 && 5 <= numberOfColumns ? pointStr[4].replace(decimal, ".").toDouble() : 0;
            point.j = pointStr.length() > 5 && 6 <= numberOfColumns ? pointStr[5].replace(decimal, ".").toDouble() : 0;
            point.k = pointStr.length() > 6 && 7 <= numberOfColumns ? pointStr[6].replace(decimal, ".").toDouble() : 0;
            point.dev = pointStr.length() > 7 && 8 <= numberOfColumns ? pointStr[7].replace(decimal, ".").toDouble() : 0;
            point.lt = pointStr.length() > 8 && 9 <= numberOfColumns ? pointStr[8].replace(decimal, ".").toDouble() : 0;
            break;
        }
        case Order::Default:
        {
            point.x = pointStr.length() > 0 && 1 <= numberOfColumns ? pointStr[0].replace(decimal, ".").toDouble() : 0;
            point.y = pointStr.length() > 1 && 2 <= numberOfColumns ? pointStr[1].replace(decimal, ".").toDouble() : 0;
            point.z = pointStr.length() > 2 && 3 <= numberOfColumns ? pointStr[2].replace(decimal, ".").toDouble() : 0;
            point.i = pointStr.length() > 3 && 4 <= numberOfColumns ? pointStr[3].replace(decimal, ".").toDouble() : 0;
            point.j = pointStr.length() > 4 && 5 <= numberOfColumns ? pointStr[4].replace(decimal, ".").toDouble() : 0;
            point.k = pointStr.length() > 5 && 6 <= numberOfColumns ? pointStr[5].replace(decimal, ".").toDouble() : 0;
            point.dev = pointStr.length() > 6 && 7 <= numberOfColumns ? pointStr[6].replace(decimal, ".").toDouble() : 0;
            point.ut = pointStr.length() > 7 && 8 <= numberOfColumns ? pointStr[7].replace(decimal, ".").toDouble() : 0;
            point.lt = pointStr.length() > 8 && 9 <= numberOfColumns ? pointStr[8].replace(decimal, ".").toDouble() : 0;
            break;
        }
        default:
        {
            QMessageBox::critical(nullptr, "Error", "Unknown order");
            break;
        }
    }
    return point;
}

QMap<QString, QStringList> FileSystem::parseOutputToElements(QStringList elements) {
    QMap<QString, QStringList> result;
    for(auto i = 0; i < elements.length(); i++) {
        auto strList = elements[i].split('\n');
        auto index1 = strList[0].indexOf('=') + 1;
        auto index2 = strList[0].indexOf(',');
        auto header = strList[0].mid(index1, index2 - index1);
        result.insert(header, strList);
    }
    return result;
}

void FileSystem::fillInputWithMultipleElements(QList<QVector<CurvePoint>> points) {
    QFile input("C:/temp/ca/project/2dinp.dat");
    input.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream inputStream(&input);
    for(auto i = 0; i < points.length(); i++) {
        inputStream << "$ELE (NAM=input" << i << ",TYP = APT, FLD = (X, Y, Z))\n";
        fillFileWithPoints(inputStream, points[i], 9, Order::CurveLibrary);
        inputStream << "$END\n";
    }
    input.close();
}

void FileSystem::loadCloud(Project *project, QString filePath, QString name, QString sep, int skipStart, int skipAfter,
    QString columnNames, QString columnNumbers, QString decimal, Order order) {
    ARGUMENT_ASSERT(QFile(filePath).exists(), "Load Cloud: could not find such file");
    auto data = readFile(filePath).split("\n");
    auto points = parsePointsFromElement(data, sep, skipStart, skipAfter, order, columnNames, columnNumbers, decimal);
    ARGUMENT_ASSERT(points.length() > 0, "Load Cloud: no data to interprete");
    auto curve = new CurveFigure(name, points, 1);
    project->safeInsert(name, curve);
    MacrosManager::log(MacrosManager::LoadCloud, {
        { "filepath", filePath },
        { "name", name },
        { "separator", sep },
        { "skipStart", QString::number(skipStart) },
        { "skipAfter", QString::number(skipAfter) },
        { "columnNames", columnNames },
        { "columnNumbers", columnNumbers },
        { "decimal", decimal },
        });
}

void FileSystem::saveProject(Project *project, QString dir, QString projectName, bool createCRV) {
    QFileInfo dirInfo(dir);
    ARGUMENT_ASSERT(dirInfo.exists() && dirInfo.isDir(), "Save Project: could not find such directory");
    auto resultTXT = dir + "/" + projectName + ".txt";
    QFile input(resultTXT);
    input.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream stream(&input);
    stream << "$GENERAL\n" << "WPlane=XY\n";
    stream << "ScaleFac=" << project->scaleFactor() << "\n";
    stream << "DMovX=" << project->centerPoint()->x << "\n";
    stream << "DMovY=" << project->centerPoint()->y << "\n";
    stream << "ShowAxis=1\n" << "AutoAxis=1\n";
    stream << "ShowGrid=0\n" << "AxisXOri=0\n" << "AxisYOri=0\n";
    stream << "AxisXDiv=10\n" << "AxisYDiv=10\n" << "YAmp=1\n" << "XFac=1\n";
    stream << "PartName=" << project->description() << "\n";
    stream << "Drawing=" << project->drawing() << "\n";
    stream << "Order=" << project->orderNumber() << "\n";
    stream << "PartNumber=" << project->partNumber() << "\n";
    stream << "Operator=" << project->projectOperator() << "\n";
    stream << "Note=" << project->note() << "\n";
    stream << "Date=\n" << "Time=\n";
    stream << "TraceField1=" << project->machine() << "\n";
    stream << "TraceField2=" << project->tool() << "\n";
    stream << "TraceField3=" << project->fixturing() << "\n";
    stream << "TraceField4=" << project->batch() << "\n";
    stream << "TraceField5=" << project->supplier() << "\n";
    stream << "TraceField6=" << project->revision() << "\n";
    stream << "TraceField7=\n" << "TraceField8=\n" << "TraceField9=\n";
    stream << "$END GENERAL\n";
    for(auto figure : project->figures()) {
        inputFigure(stream, figure->settings());
    }
    input.close();
    if(createCRV) {
        QString command;
        QTextStream commandStream(&command);
        commandStream << "powershell.exe -Command " << "\"$file_text = Get-Content '" << resultTXT << "';"
            << "Set-Content -Path '" << dir + "/" + projectName + ".crv" << "' -Value $file_text;\"";
        system(command.toStdString().c_str());
    }
    MacrosManager::log(MacrosManager::SaveProject, {
        { "dirName", dir },
        { "ProjectName", projectName },
        { "createCRV", createCRV ? "true" : "false" }
    });
}

void FileSystem::inputFigure(QTextStream &stream, FigureSettings *set) {
    stream << "$POINT SETTINGS\nName=" << set->name;
    stream << "\nEleType=" << set->type << "\n";
    stream << "Rif=" << set->rif << "\nRif1=" << set->rif1 << "\n";
    stream << "Colour=" << set->colour << "\n";
    stream << "X=" << set->x << "\n" << "Y=" << set->y << "\n" << "Z=" << set->z << "\n";
    stream << "U=" << set->u << "\n" << "V=" << set->v << "\n" << "W=" << set->w << "\n";
    stream << "L=" << set->l << "\n" << "M=" << set->m << "\n" << "N=" << set->n << "\n";
    stream << "A=" << set->a << "\n" << "B=" << set->b << "\n" << "C=" << set->c << "\n";
    stream << "D=" << set->d << "\n" << "E=" << set->e << "\n" << "F=" << set->f << "\n";

    stream << "Visible=" << set->isVisible << "\n";
    stream << "ShowPoints=" << set->showPoints << "\n";
    stream << "ConnectPoints=" << set->connectPoints << "\n";
    stream << "ShowVectors=" << set->showVectors << "\n";
    stream << "PointNumbering=" << set->showNumbering << "\n";
    stream << "PointNumberingShowEach=" << set->numberEach << "\n";
    stream << "Closed=" << set->isClosed << "\n";
    stream << "Amp=" << set->amplitude << "\n";
    stream << "ShowTols=" << set->showTols << "\n";
    stream << "UseGenTols=" << set->useGenTols << "\n";
    stream << "UT=" << set->UT << "\n";
    stream << "LT=" << set->LT << "\n";
    stream << "ShowDev=" << set->showDev << "\n";
    stream << "ConnectDev=" << set->connectDev << "\n";
    stream << "HighlightDev=" << set->highlightDev << "\n";
    stream << "$START POINTS\n";
    fillFileWithPoints(stream, set->points, 12, Order::Default);
    stream << "$END POINTS\n";
}

void FileSystem::fillFileWithPoints(QTextStream &stream, const QVector<CurvePoint> points, int precision, Order order) {
    for(auto i = 0; i < points.length(); i++) {
        switch(order) {
            case Order::Default:
                stream << QString::number(points.at(i).x, 'f', precision) << "," << QString::number(points.at(i).y, 'f', precision) << "," <<
                    QString::number(points.at(i).z, 'f', precision) << "," << QString::number(points.at(i).i, 'f', precision) << "," <<
                    QString::number(points.at(i).j, 'f', precision) << "," << QString::number(points.at(i).k, 'f', precision) << "," <<
                    QString::number(points.at(i).dev, 'f', precision) << "," << QString::number(points.at(i).ut, 'f', precision) << "," <<
                    QString::number(points.at(i).lt, 'f', precision) << "\n";
                break;
            case Order::CurveLibrary:
                stream << QString::number(points.at(i).x, 'f', precision) << "," << QString::number(points.at(i).y, 'f', precision) << "," <<
                    QString::number(points.at(i).z, 'f', precision) << "," << QString::number(points.at(i).ut, 'f', precision) << "," <<
                    QString::number(points.at(i).i, 'f', precision) << "," << QString::number(points.at(i).j, 'f', precision) << "," <<
                    QString::number(points.at(i).k, 'f', precision) << "," << QString::number(points.at(i).dev, 'f', precision) << "," <<
                    QString::number(points.at(i).lt, 'f', precision) << "\n";
                break;
            default:
                QMessageBox::critical(nullptr, "Error", "Unknown order");
                return;
        }
    }
}

void FileSystem::loadProject(Project *project, const QString &filePath) {
    ARGUMENT_ASSERT(!filePath.isEmpty(), "Load Project: File path is empty");
    ARGUMENT_ASSERT(QFile::exists(filePath), "Load Project: File not found");
    QMap<QString, QString> generalData;
    auto parts = readFile(filePath).split("$END GENERAL");
    ARGUMENT_ASSERT(parts.length() == 2, "Load Project: Wrong file format");

    auto generalPart = parts[0].split('\n');

    for(const auto &part : generalPart) {
        auto pairs = part.split('=');
        if(pairs.size() == 2) {
            auto key = pairs[0].trimmed();
            auto value = pairs[1].trimmed();
            generalData[key] = value;
        }
    }

    auto scaleFactor = generalData["ScaleFac"].toDouble();
    auto offsetPoint = Point(generalData["DMovX"].toDouble(), generalData["DMovY"].toDouble());
    auto description = QString(generalData["PartName"]);
    auto drawing = QString(generalData["Drawing"]);
    auto orderNumber = QString(generalData["Order"]);
    auto partNumber = QString(generalData["PartNumber"]);
    auto projectOperator = QString(generalData["Operator"]);
    auto note = QString(generalData["Note"]);
    auto machine = QString(generalData["TraceField1"]);
    auto tool = QString(generalData["TraceField2"]);
    auto fixturing = QString(generalData["TraceField3"]);
    auto batch = QString(generalData["TraceField4"]);
    auto supplier = QString(generalData["TraceField5"]);
    auto revision = QString(generalData["TraceField6"]);

    auto figures = parts[1].split("$END POINTS");
    figures.removeLast();
    auto postponedCases = 0;
    for(auto i = 0; i < figures.length(); i++) {
        QMap<QString, QString> data;
        auto parsedFigure = figures[i].trimmed().split('\n');
        for(auto i = 1; i < 36; i++) {
            auto pairs = parsedFigure[i].split('=');
            if(pairs.size() == 2) {
                auto key = pairs[0].trimmed();
                auto value = pairs[1].trimmed();
                data[key] = value;
            }
        }
        auto type = data["EleType"];
        if(type == "DIM" && (data["Rif"] != "" && !project->containsFigure(data["Rif"]) ||
            data["Rif1"] != "" && !project->containsFigure(data["Rif1"]))) {
            if(postponedCases + i > figures.length()) {
                QMessageBox::critical(nullptr, "Error", "Postpone queue error. Check DIM`s references");
            } else {
                figures.push_back(figures.takeAt(i));
                postponedCases++;
                i--;
                continue;
            }
        }
        auto points = parsePointsFromElement(parsedFigure, ",", 37, 0, Order::Default);
        if(type == "CRV") {
            auto curve = new CurveFigure(data["Name"], points);
            curve->setVisible(data["Visible"] == "1");
            curve->setShowPoints(data["ShowPoints"] == "1");
            curve->setConnectPoints(data["ConnectPoints"] == "1");
            curve->setShowVectors(data["ShowVectors"] == "1");
            curve->setShowNumbering(data["PointNumbering"] == "1");
            curve->setNumberingInterval(data["PointNumberingShowEach"].toInt());
            curve->setClosed(data["Closed"] == "1");
            //curve->computeToleranceClouds(); //needs refactoring
            curve->setColor(*ColorTranslator::getColorFromInt(data["Colour"].toInt()));
            project->safeInsert(curve->name(), curve);
        } else if(type == "LIN") {
            auto position = Point(data["X"].toDouble(), data["Y"].toDouble(), data["Z"].toDouble());
            auto direction = Point(data["U"].toDouble(), data["V"].toDouble(), data["W"].toDouble());
            auto line = new LineFigure(data["Name"], position, direction);
            line->setColor(*ColorTranslator::getColorFromInt(data["Colour"].toInt()));
            line->setVisible(data["Visible"] == "1");
            project->safeInsert(line->name(), line);
        } else if(type == "PNT") {
            auto point = new PointFigure(data["Name"], CurvePoint(data["X"].toDouble(), data["Y"].toDouble(), data["Z"].toDouble()));
            point->setColor(*ColorTranslator::getColorFromInt(data["Colour"].toInt()));
            point->setVisible(data["Visible"] == "1");
            project->safeInsert(point->name(), point);
        } else if(type == "CIR") {
            auto center = Point(data["X"].toDouble(), data["Y"].toDouble(), data["Z"].toDouble());
            auto normal = Point(data["U"].toDouble(), data["V"].toDouble(), data["W"].toDouble());
            auto circle = new CircleFigure(data["Name"], center, normal, data["A"].toDouble() / 2);
            circle->setColor(*ColorTranslator::getColorFromInt(data["Colour"].toInt()));
            circle->setVisible(data["Visible"] == "1");
            project->safeInsert(circle->name(), circle);
        } else if(type == "DIM") {
            auto labelPoint = Point(data["X"].toDouble(), data["Y"].toDouble(), data["Z"].toDouble());
            auto dim = createDim(data["Name"], data["A"].toInt(), labelPoint, project->findFigure(data["Rif"]),
                project->findFigure(data["Rif1"]), points);
            dim->setColor(*ColorTranslator::getColorFromInt(data["Colour"].toInt()));
            dim->setVisible(data["Visible"] == "1");
            project->safeInsert(dim->name(), dim);
        } else if(type == "TXT") {
            project->constructText(data["Name"], data["Rif"], data["X"].toDouble(), data["Y"].toDouble(), data["A"].toDouble(), data["Rif1"],
                data["B"].toDouble(), data["C"].toDouble(), data["D"].toDouble());
        } else {
            QMessageBox::critical(nullptr, "Error", "Unknown element type");
        }
    }
    project->changeScale(scaleFactor, offsetPoint);
    project->changePartData(QString(), description,drawing,orderNumber,partNumber,projectOperator,note, machine, tool, fixturing, batch, supplier, revision, false);
    MacrosManager::log(MacrosManager::LoadProject, { { "filepath", filePath } });
}

void FileSystem::exportToFLR(Project *project, QString filepath, QStringList *curvesToTake) {
    auto slash1 = filepath.lastIndexOf("/");
    auto slash2 = filepath.lastIndexOf("\\");
    auto slashIndex = slash1 > slash2 ? slash1 : slash2;
    auto dirName = filepath.mid(0, slashIndex);
    QFileInfo dirInfo(dirName);
    ARGUMENT_ASSERT(dirInfo.exists() && dirInfo.isDir(), "Export to FLR: could not find such directory");
    QFile input(filepath);
    input.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream stream(&input);

    stream << "C1= \n" << "C2= \n" << "C3= \n" << "C4= \n" << "C5= \n" << "C6= \n" << "C7= \n" << "C8= \n";
    stream << "C9= \n" << "C10= \n" << "C11= \n" << "C12= \n" << "C13= \n" << "C14= \n" << "C15= \n";
    auto time = QDateTime::currentDateTime();
    stream << "C21=" << time.toString("yyyy.MM.dd") << "\n" << "C22=" << time.toString("HH:mm:ss") << "\n";
    stream << "C23=" << time.toString("yyyy.MM.dd HH:mm:ss") << "\n";
    stream << "$END$\n";

    for(auto dim : project->dimFigures()) {
        stream << dim->exportToFLR(4);
    }
    for(auto name : *curvesToTake) {
        auto curve = dynamic_cast<const CurveFigure*>(project->findFigure(name));
        ARGUMENT_ASSERT(curve, "Export to FLR: haven`t found the curve from curvesToTake list");
        stream << curve->exportToFLR(4);
    }
    stream << "$END$\n";
    input.close();

    MacrosManager::log(MacrosManager::ExportToFLR, {
        { "filepath", filepath },
        { "curvesToTake", curvesToTake->join(",") }
    });
}

DimFigure* FileSystem::createDim(QString name, int A, Point labelPoint, const Figure *rif,
    const Figure *rif1, QList<CurvePoint> points) {
        QList<DimFigure::Value> values;
        DimFigure::DimType type;
        switch(A) {
            case 1:
                type = DimFigure::DimType::Position;
                values.append({
                    DimFigure::Value(DimFigure::ValueType::X),
                    DimFigure::Value(DimFigure::ValueType::Y),
                    DimFigure::Value(DimFigure::ValueType::Z),
                    DimFigure::Value(DimFigure::ValueType::PolarRad),
                    DimFigure::Value(DimFigure::ValueType::PolarAngle),
                    DimFigure::Value(DimFigure::ValueType::Diameter),
                    DimFigure::Value(DimFigure::ValueType::Radius)
                    });
                break;
            case 2:
                type = DimFigure::DimType::Radius;
                values.append(DimFigure::Value(DimFigure::ValueType::Radius));
                break;
            case 3:
                type = DimFigure::DimType::Diameter;
                values.append(DimFigure::Value(DimFigure::ValueType::Diameter));
                break;
            case 6:
            case 10:
                type = DimFigure::DimType::Distance;
                values.append(DimFigure::Value(DimFigure::ValueType::Length));
                break;
            case 8:
                type = DimFigure::DimType::AngleTo;
                values.append({
                    DimFigure::Value(DimFigure::ValueType::Angle0),
                    DimFigure::Value(DimFigure::ValueType::Angle90),
                    DimFigure::Value(DimFigure::ValueType::Angle180),
                    DimFigure::Value(DimFigure::ValueType::Angle270)
                    });
                break;
            case 11:
                type = DimFigure::DimType::AngleBetween;
                values.append({
                    DimFigure::Value(DimFigure::ValueType::AngleTo),
                    DimFigure::Value(DimFigure::ValueType::AngleFrom)
                    });
                break;
            case 12:
                type = DimFigure::DimType::MinMax;
                values.append(DimFigure::Value(DimFigure::ValueType::MinMax));
                break;
            case 13:
                type = DimFigure::DimType::Min;
                values.append(DimFigure::Value(DimFigure::ValueType::Min));
                break;
            case 16:
                type = DimFigure::DimType::Perimeter;
                values.append(DimFigure::Value(DimFigure::ValueType::Length));
                break;
            case 17:
                type = DimFigure::DimType::Mean;
                values.append(DimFigure::Value(DimFigure::ValueType::Mean));
                break;
            case 18:
                type = DimFigure::DimType::Sigma;
                values.append(DimFigure::Value(DimFigure::ValueType::Sigma));
                break;
            case 19:
                type = DimFigure::DimType::BestFitData;
                values.append({
                    DimFigure::Value(DimFigure::ValueType::dX),
                    DimFigure::Value(DimFigure::ValueType::dY),
                    DimFigure::Value(DimFigure::ValueType::Rotation)
                    });
                break;
            case 20:
                type = DimFigure::DimType::Form;
                values.append({
                    DimFigure::Value(DimFigure::ValueType::MinMax),
                    DimFigure::Value(DimFigure::ValueType::Form),
                    DimFigure::Value(DimFigure::ValueType::Min),
                    DimFigure::Value(DimFigure::ValueType::Max),
                    DimFigure::Value(DimFigure::ValueType::MaxAbs),
                    DimFigure::Value(DimFigure::ValueType::SupUT),
                    DimFigure::Value(DimFigure::ValueType::InfLT)
                    });
                break;
            case 21:
                type = DimFigure::DimType::TruePosition;
                values.append({
                    DimFigure::Value(DimFigure::ValueType::X),
                    DimFigure::Value(DimFigure::ValueType::Y),
                    DimFigure::Value(DimFigure::ValueType::TruePosition)
                    });
                break;
            case 22:
                type = DimFigure::DimType::RMS;
                values.append(DimFigure::Value(DimFigure::ValueType::RMS));
                break;
            default:
                QMessageBox::critical(nullptr, "Error", "Unknown DIM type");
                break;
        }
        auto *dim = new DimFigure(name, type, labelPoint, rif, rif1);
        for(auto i = 0; i < points.length(); i++) {
            values[i].isShow = std::fabs(points[i].x - 1) < 1e-6;
            values[i].nominal = points[i].y;
            values[i].measurement = points[i].k;
            values[i].lowerTolerance = points[i].lt;
            values[i].upperTolerance = points[i].ut;
            dim->addValue(values.at(i));
        }
        return dim;
}
