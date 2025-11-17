#include "curve/pch.h"
#include "filesystem.h"

QString FileSystem::curveDirectory = "C:/Users/solom/source/repos/geomera projects/curve/curve-1.exe";

void FileSystem::createWorkEnvironment() {
    QDir dir;
    dir.mkpath("C:/temp/ca/project");
}

void FileSystem::fillInputWithSingleSegment(QVector<Point> points) {
    QFile input("C:/temp/ca/project/2dinp.dat");
    input.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream inputStream(&input);

    inputStream << "$ELE (NAM=input_TE,TYP=APT, FLD=(X,Y,Z))\n";
    for(auto i = 0; i < points.length(); i++) {
        inputStream << QString::number(points[i].x, 'f', 5) << "," << QString::number(points[i].y, 'f', 5) << ",";
        inputStream << QString::number(points[i].z, 'f', 5) << "," << QString::number(points[i].i, 'f', 5) << ",";
        inputStream << QString::number(points[i].j, 'f', 5) << "," << QString::number(points[i].k, 'f', 5) << ",";
        inputStream << QString::number(points[i].dev, 'f', 5) << "," << QString::number(points[i].lt, 'f', 5) << ",";
        inputStream << QString::number(points[i].ut, 'f', 5) << "\n";
    }
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

QVector<Point> FileSystem::parsePointsFromElement(QStringList element, QString separator, int startLineToSkip, int finishLineToSkip) {
    QVector<Point> result;
    for(auto i = startLineToSkip; i < element.length() - finishLineToSkip; i++) {
        auto pointStr = element[i].split(separator);
        auto point = Point();
        point.x = pointStr.length() > 0 ? pointStr[0].toDouble() : 0;
        point.y = pointStr.length() > 1 ? pointStr[1].toDouble() : 0;
        point.z = pointStr.length() > 2 ? pointStr[2].toDouble() : 0;
        point.i = pointStr.length() > 3 ? pointStr[3].toDouble() : 0;
        point.j = pointStr.length() > 4 ? pointStr[4].toDouble() : 0;
        point.k = pointStr.length() > 5 ? pointStr[5].toDouble() : 0;
        point.dev = pointStr.length() > 6 ? pointStr[6].toDouble() : 0;
        point.lt = pointStr.length() > 7 ? pointStr[7].toDouble() : 0;
        point.ut = pointStr.length() > 8 ? pointStr[8].toDouble() : 0;
        result.append(point);
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

void FileSystem::fillInputWithMultipleElements(QList<QVector<Point>> points) {
    QFile input("C:/temp/ca/project/2dinp.dat");
    input.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream inputStream(&input);

    for(auto i = 0; i < points.length(); i++) {
        inputStream << "$ELE (NAM=input" << i << ",TYP = APT, FLD = (X, Y, Z))\n";
        for(auto j = 0; j < points[i].length(); j++) {
            inputStream << QString::number(points[i][j].x, 'f', 5) << "," << QString::number(points[i][j].y, 'f', 5) << ",";
            inputStream << QString::number(points[i][j].z, 'f', 5) << "," << QString::number(points[i][j].i, 'f', 5) << ",";
            inputStream << QString::number(points[i][j].j, 'f', 5) << "," << QString::number(points[i][j].k, 'f', 5) << ",";
            inputStream << QString::number(points[i][j].dev, 'f', 5) << "," << QString::number(points[i][j].lt, 'f', 5) << ",";
            inputStream << QString::number(points[i][j].ut, 'f', 5) << "\n";
        }
        inputStream << "$END\n";
    }
    input.close();
}
