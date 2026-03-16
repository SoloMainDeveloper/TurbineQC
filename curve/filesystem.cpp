#include "curve/pch.h"

#include "filesystem.h"
#include "loadcloudcommand.h"
#include "loadprojectcommand.h"
#include "saveprojectcommand.h"
#include "exporttoflrcommand.h"

Point FileSystem::_offsetPoint = Point(0, 0);
double FileSystem::_scaleFactor = 1;
QString FileSystem::curveDirectory = "C:/CurveAnalyzer/curve-1.exe";

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
    QElapsedTimer timer;
    timer.start();
    QFile output(fullFileName);
    output.open(QIODevice::ReadOnly | QIODevice::Text);
    auto result = output.readAll();
    output.close();
    return QString::fromUtf8(result).trimmed();
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

                /*if(!isColumnsCorrect(resultColumnNames, resultNumberOfColumns) || !hasDecimalSeparator(pointStr, decimal)
                    || pointStr.length() < resultNumberOfColumns.length()) {
                    break;
                } else {*/
                auto point = getPoint(pointStr, order, decimal, resultNumberOfColumns.length());
                result.append(point);
                //}
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

bool FileSystem::isColumnsCorrect(const QString& columnNames, const QString& numberOfColumns) {
    if(columnNames == "XY" && numberOfColumns == "12" || columnNames == "XYZ" && numberOfColumns == "123"
        || columnNames == "XYZIJK" && numberOfColumns == "123456" || columnNames == "XYZIJKD" && numberOfColumns == "1234567"
        || columnNames == "XYZIJKDUTLT" && numberOfColumns == "123456789") {
        return true;
    } else {
        return false;
    }
}

bool FileSystem::hasDecimalSeparator(const QStringList& pointStr, const QString& decimal) {
    for(auto i = 0; i < pointStr.length(); i++) {
        if(!pointStr[i].contains(decimal)) {
            return false;
        }
    }
    return true;
}

CurvePoint FileSystem::getPoint(QStringList pointStr, Order order, const QString& decimal, int numberOfColumns) {
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

void FileSystem::loadCloud(QString filePath, QString name, QString sep, int skipStart, int skipAfter,
    QString columnNames, QString columnNumbers, QString decimal, Order order) {

    ARGUMENT_ASSERT(QFile(filePath).exists(), "Load Cloud: could not find such file");
    auto data = readFile(filePath).split("\n");
    auto points = parsePointsFromElement(data, sep, skipStart, skipAfter, order, columnNames, columnNumbers, decimal);
    ARGUMENT_ASSERT(points.length() > 0, "Load Cloud: no data to interprete");
    auto curve = new CurveFigure(name, points, 1);
    Project::instance().safeInsert(name, curve);
    MacrosManager::instance().log(std::make_shared<LoadCloudCommand>(
        filePath, name, sep, skipStart, skipAfter, columnNames, columnNumbers, decimal));
}

void FileSystem::saveProject(QString fileNameWithoutExtension) {
    //QFileInfo fileInfo(fileName);
    //ARGUMENT_ASSERT(fileInfo.exists(), "Save Project: could not create a file with such name");
    auto project = &Project::instance();
    auto fileNameTXT = fileNameWithoutExtension + ".txt";
    auto fileNameCRV = fileNameWithoutExtension + ".crv";

    QFile input(fileNameTXT);
    input.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream stream(&input);

    stream << "$GENERAL\n" << "WPlane=XY\n";
    stream << "ScaleFac=" << project->scaleFactor() << "\n";
    stream << "DMovX=" << project->centerPoint().x << "\n";
    stream << "DMovY=" << project->centerPoint().y << "\n";
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

    auto command = QString("Get-Content '%1' | Set-Content -Path '%2'").arg(fileNameTXT, fileNameCRV);
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.start("powershell.exe", QStringList() << "-Command" << command);
    process.waitForFinished();

    QFile file(fileNameTXT);
    if(file.exists()) {
        file.remove();
    }

    project->setProjectPath(fileNameCRV);
    MacrosManager::instance().log(std::make_shared<SaveProjectCommand>(fileNameWithoutExtension));
}

void FileSystem::inputFigure(QTextStream& stream, FigureSettings* set) {
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

void FileSystem::fillFileWithPoints(QTextStream& stream, const QVector<CurvePoint> points, int precision, Order order) {
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

void FileSystem::loadProject(const QString& filePath) {
    auto project = &Project::instance();

    QElapsedTimer timer;
    timer.start();
    ARGUMENT_ASSERT(!filePath.isEmpty(), "Load Project: File path is empty");
    ARGUMENT_ASSERT(QFile::exists(filePath), "Load Project: File not found");
    auto parts = readFile(filePath).split("$END GENERAL");
    ARGUMENT_ASSERT(parts.length() == 2, "Load Project: Wrong file format");

    auto generalPart = parts[0].split('\n');
    QMap<QString, QString> generalData;

    for(auto i = 0; i < 32; i++) {
        auto pair = generalPart[i].split('=');
        if(pair.size() == 2) {
            auto key = pair[0].trimmed();
            auto value = pair[1].trimmed();
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

    /*QVector<QFuture<Figure*>> futures;
    for (const auto& figure : figures) {
        futures.append(QtConcurrent::run([=]() {
            return parseFigureFromCRV(project, figure);
            }));
    }
    QVector<Figure*> results;
    for (const auto& future : futures) {
        results.append(future.result());
    }
    for (auto figure : results)
        project->safeInsert(figure->name(), figure);*/

    for(auto& figure : figures) {
        auto parsedFigure = parseFigureFromCRV(project, figure);
        project->safeInsert(parsedFigure->name(), parsedFigure);
    }

    project->zoomToPoint(scaleFactor, offsetPoint);
    MacrosManager::instance().executeWithoutLogging([&]() {
        project->changePartData(QString(), description, drawing, orderNumber, partNumber, projectOperator, note,
            machine, tool, fixturing, batch, supplier, revision, false);
    });
    project->setProjectPath(filePath);
    MacrosManager::instance().log(std::make_shared<LoadProjectCommand>(filePath));
}

Figure* FileSystem::parseFigureFromCRV(Project* project, QString figureText) {
    QElapsedTimer timer;
    timer.start();
    QMap<QString, QString> data;
    auto parsedFigure = figureText.trimmed().split('\n');
    for(auto i = 1; i < 36; i++) {
        auto pairs = parsedFigure[i].split('=');
        if(pairs.size() == 2) {
            auto key = pairs[0].trimmed();
            auto value = pairs[1].trimmed();
            data[key] = value;
        }
    }
    qDebug() << "Figure parsed: " << data["Name"] << timer.elapsed();
    auto type = data["EleType"];
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
        curve->setColor(*ColorTranslator::getColorFromInt(data["Colour"].toInt()));
        return curve;
    } else if(type == "LIN") {
        auto position = Point(data["X"].toDouble(), data["Y"].toDouble(), data["Z"].toDouble());
        auto direction = Point(data["U"].toDouble(), data["V"].toDouble(), data["W"].toDouble());
        auto line = new LineFigure(data["Name"], position, direction, qInf());
        line->setColor(*ColorTranslator::getColorFromInt(data["Colour"].toInt()));
        line->setVisible(data["Visible"] == "1");
        return line;
    } else if(type == "PNT") {
        auto point = new PointFigure(data["Name"], CurvePoint(data["X"].toDouble(), data["Y"].toDouble(), data["Z"].toDouble()));
        point->setColor(*ColorTranslator::getColorFromInt(data["Colour"].toInt()));
        point->setVisible(data["Visible"] == "1");
        return point;
    } else if(type == "CIR") {
        auto center = Point(data["X"].toDouble(), data["Y"].toDouble(), data["Z"].toDouble());
        auto normal = Point(data["U"].toDouble(), data["V"].toDouble(), data["W"].toDouble());
        auto circle = new CircleFigure(data["Name"], center, normal, data["A"].toDouble() / 2);
        circle->setColor(*ColorTranslator::getColorFromInt(data["Colour"].toInt()));
        circle->setVisible(data["Visible"] == "1");
        return circle;
    } else if(type == "DIM") {
        auto labelPoint = Point(data["X"].toDouble(), data["Y"].toDouble(), data["Z"].toDouble());
        auto dim = new DimFigure(data["Name"], labelPoint, data["Rif"], data["Rif1"]);
        dim->setA(data["A"].toInt());
        dim->setValues(points);
        dim->setColor(*ColorTranslator::getColorFromInt(data["Colour"].toInt()));
        dim->setVisible(data["Visible"] == "1");
        return dim;
    } else if(type == "TXT") {
        auto txt = new TextFigure(data["Name"], data["Rif"], Point(data["X"].toDouble(), data["Y"].toDouble(), 0), data["A"].toDouble(), data["Rif1"],
            data["B"].toDouble(), data["C"].toDouble(), data["D"].toDouble());
        return txt;
    } else {
        QMessageBox::critical(nullptr, "Error", "Unknown element type");
    }
    qDebug() << "Figure FULL parsed: " << data["Name"] << timer.elapsed();
}

void FileSystem::exportToFLR(QString filepath, QStringList* curvesToTake) {
    auto& project = Project::instance();

    auto slash1 = filepath.lastIndexOf("/");
    auto slash2 = filepath.lastIndexOf("\\");
    auto slashIndex = slash1 > slash2 ? slash1 : slash2;
    auto dirName = filepath.mid(0, slashIndex);
    QFileInfo dirInfo(dirName);
    ARGUMENT_ASSERT(dirInfo.exists() && dirInfo.isDir(), "Export to FLR: could not find such directory");

    QFile input(filepath);
    input.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream stream(&input);

    stream << "C1=" << project.description() << "\n" << "C2=" << project.drawing() << "\n" << "C3=" << project.orderNumber() << "\n";
    stream << "C4=" << project.partNumber() << "\n" << "C5=" << project.projectOperator() << "\n" << "C6=" << project.note() << "\n";
    stream << "C7=" << project.machine() << "\n" << "C8=" << project.tool() << "\n" << "C9=" << project.fixturing() << "\n";
    stream << "C10=" << project.batch() << "\n" << "C11=" << project.supplier() << "\n" << "C12=" << project.revision();
    stream << "\n" << "C13=" << "\n" << "C14= \n" << "C15= \n";

    auto time = QDateTime::currentDateTime();
    stream << "C21=" << time.toString("yyyy.MM.dd") << "\n" << "C22=" << time.toString("HH:mm:ss") << "\n";
    stream << "C23=" << time.toString("yyyy.MM.dd HH:mm:ss") << "\n";
    stream << "$END$\n";

    auto dims = project.dimFigures();
    std::sort(dims.begin(), dims.end(), [](const auto& a, const auto& b) { return a->index() < b->index(); });
    for(auto dim : dims) {
        stream << dim->exportToFLR(4);
    }
    for(auto name : *curvesToTake) {
        auto curve = dynamic_cast<const CurveFigure*>(project.findFigure(name));
        ARGUMENT_ASSERT(curve, "Export to FLR: haven`t found the curve from curvesToTake list");
        stream << curve->exportToFLR(4);
    }
    stream << "$END$\n";
    input.close();

    MacrosManager::instance().log(std::make_shared<ExportToFLRCommand>(filepath, *curvesToTake));
}

void FileSystem::writeCompareFLR(QString fullFileName, QList<ResultCompare2Params*> result, double precision) {
    QFile file(fullFileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning("Could not open file to write");
        return;
    }

    QTextStream out(&file);
    out << QString("%1 %2 %3 %4\n")
        .arg("Name 1", -25)
        .arg("Name 2", -25)
        .arg("Difference", -15)
        .arg(QString("Status (%1)").arg(precision), -20);
    out << QString("%1 %2 %3 %4\n")
        .arg(QString("-").repeated(25), -25)
        .arg(QString("-").repeated(25), -25)
        .arg(QString("-").repeated(15), -15)
        .arg(QString("-").repeated(20), -20);

    for(auto i = 0; i < result.count(); i++) {
        out << QString("%1 %2 %3 %4\n")
            .arg(result[i]->name1(), -25)
            .arg(result[i]->name2(), -25)
            .arg(result[i]->diff(), -15)
            .arg(result[i]->diff() > precision ? "FAIL" : "OK", -5);
    }
    file.close();
}
