#include "curve/pch.h"
#include "filesystem.h"

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

QVector<CurvePoint> FileSystem::parsePointsFromElement(QStringList element, QString separator, int startLineToSkip,
    int finishLineToSkip, Order order) {
    QVector<CurvePoint> result;
    for(auto i = startLineToSkip; i < element.length() - finishLineToSkip; i++) {
        auto pointStr = element[i].split(separator);
        if(pointStr.length() != 1) {
            auto point = CurvePoint();
            switch (order) {
                case Order::CurveLibrary: {
                    point.x = pointStr.length() > 0 ? pointStr[0].toDouble() : 0;
                    point.y = pointStr.length() > 1 ? pointStr[1].toDouble() : 0;
                    point.z = pointStr.length() > 2 ? pointStr[2].toDouble() : 0;
                    point.ut = pointStr.length() > 3 ? pointStr[3].toDouble() : 0;
                    point.i = pointStr.length() > 4 ? pointStr[4].toDouble() : 0;
                    point.j = pointStr.length() > 5 ? pointStr[5].toDouble() : 0;
                    point.k = pointStr.length() > 6 ? pointStr[6].toDouble() : 0;
                    point.dev = pointStr.length() > 7 ? pointStr[7].toDouble() : 0;
                    point.lt = pointStr.length() > 8 ? pointStr[8].toDouble() : 0;
                    break;
                }
                case Order::Default: {
                    point.x = pointStr.length() > 0 ? pointStr[0].toDouble() : 0;
                    point.y = pointStr.length() > 1 ? pointStr[1].toDouble() : 0;
                    point.z = pointStr.length() > 2 ? pointStr[2].toDouble() : 0;
                    point.i = pointStr.length() > 3 ? pointStr[3].toDouble() : 0;
                    point.j = pointStr.length() > 4 ? pointStr[4].toDouble() : 0;
                    point.k = pointStr.length() > 5 ? pointStr[5].toDouble() : 0;
                    point.dev = pointStr.length() > 6 ? pointStr[6].toDouble() : 0;
                    point.ut = pointStr.length() > 7 ? pointStr[7].toDouble() : 0;
                    point.lt = pointStr.length() > 8 ? pointStr[8].toDouble() : 0;
                    break;
                }
                default:
                    QMessageBox::critical(nullptr, "Error", "Unknown order");
                    break;
            }
            result.append(point);
        } else {
            break;
        }
    }
    return result;
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
        fillFileWithPoints(inputStream, points[i], 4, Order::CurveLibrary);
        inputStream << "$END\n";
    }
    input.close();
}

void FileSystem::saveProject(Project *project, QString dir, QString projectName, bool createCRV) {
    auto resultTXT = dir + "/" + projectName + ".txt";
    QFile input(resultTXT);
    input.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream stream(&input);
    stream << "$GENERAL\n" << "WPlane=XY\n" << "ScaleFac=0.13926210182025\n";
    stream << "DMovX=1.57995\n" << "DMovY=-0.8466\n" << "ShowAxis=1\n" << "AutoAxis=1\n";
    stream << "ShowGrid=0\n" << "AxisXOri=0\n" << "AxisYOri=0\n";
    stream << "AxisXDiv=10\n" << "AxisYDiv=10\n" << "YAmp=1\n" << "XFac=1\n";
    stream << "PartName=\n" << "Drawing=\n" << "Order=\n" << "PartNumber=\n";
    stream << "Operator=\n" << "Note=\n" << "Date=\n" << "Time=\n";
    stream << "TraceField1=\n" << "TraceField2=\n" << "TraceField3=\n";
    stream << "TraceField4=\n" << "TraceField5=\n" << "TraceField6=\n";
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
            << "copy C:/temp/ca/project/template.crv '" << dir << "';"
            << "Set-Content -Path '" << dir + "/template" + ".crv" << "' -Value $file_text;" 
            << "Rename-Item -Path '" << dir + "/template" + ".crv' '" << projectName + ".crv'" << "\"";
        system(command.toStdString().c_str());
    }
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

void FileSystem::loadProject(Project *project) {
    auto filePath = QFileDialog::getOpenFileName(nullptr, "Open file", "", "(*.txt *.crv)");
    if(!filePath.isEmpty()) {
        auto parts = readFile(filePath).split("$END GENERAL");
        auto figures = parts[1].split("$END POINTS");
        figures.removeLast();
        for(auto figure : figures) {
            QMap<QString, QString> data;
            auto parsedFigure = figure.trimmed().split('\n');
            for(auto i = 1; i < 36; i++) {
                auto pairs = parsedFigure[i].split('=');
                if(pairs.size() == 2) {
                    auto key = pairs[0].trimmed();
                    auto value = pairs[1].trimmed();
                    data[key] = value;
                }
            }
            auto points = parsePointsFromElement(parsedFigure, ",", 37, 0, Order::Default);
            auto type = data["EleType"];
            if(type == "CRV") {
                auto curve = new CurveFigure(data["Name"], points);
                curve->setVisible(data["Visible"] == "1");
                curve->setShowPoints(data["ShowPoints"] == "1");
                curve->setConnectPoints(data["ConnectPoints"] == "1");
                curve->setShowVectors(data["ShowVectors"] == "1");
                curve->setShowNumbering(data["PointNumbering"] == "1");
                curve->setNumberingInterval(data["PointNumberingShowEach"].toInt());
                curve->setClosed(data["Closed"] == "1");
                curve->computeToleranceClouds(); //needs refactoring
                project->safeInsert(curve->name(), curve);
            } else if(type == "LIN") {
                auto position = Point(data["X"].toDouble(), data["Y"].toDouble(), data["Z"].toDouble());
                auto direction = Point(data["U"].toDouble(), data["V"].toDouble(), data["W"].toDouble());
                auto line = new LineFigure(data["Name"], position, direction);
                line->setVisible(data["Visible"] == "1");
                project->safeInsert(line->name(), line);
            } else if(type == "PNT") {
                auto point = new PointFigure(data["Name"], CurvePoint(data["X"].toDouble(), data["Y"].toDouble(), data["Z"].toDouble()));
                point->setVisible(data["Visible"] == "1");
                project->safeInsert(point->name(), point);
            } else if(type == "CIR") {
                auto center = Point(data["X"].toDouble(), data["Y"].toDouble(), data["Z"].toDouble());
                auto normal = Point(data["U"].toDouble(), data["V"].toDouble(), data["W"].toDouble());
                auto circle = new CircleFigure(data["Name"], center, normal, data["A"].toDouble() / 2);
                circle->setVisible(data["Visible"] == "1");
                project->safeInsert(circle->name(), circle);
            } else if(type == "DIM") {
                auto start = const_cast<PointFigure*>(dynamic_cast<const PointFigure*>(project->findFigure(data["Rif"])));
                auto end = const_cast<PointFigure*>(dynamic_cast<const PointFigure*>(project->findFigure(data["Rif1"])));
                auto dim = CurveMachine::getLineSegment(data["Name"], start, end);
                dim->setVisible(data["Visible"] == "1");
                project->safeInsert(dim->name(), dim);
            } else {
                QMessageBox::critical(nullptr, "Error", "Unknown element type");
            }
        }
    }
}
