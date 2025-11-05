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
        inputStream << QString::number(points[i].x, 'f', 4) << "," << QString::number(points[i].y, 'f', 4) << "\n";
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
        Point point(pointStr[0].toDouble(), pointStr[1].toDouble()); //todo for all 9 doubles
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
