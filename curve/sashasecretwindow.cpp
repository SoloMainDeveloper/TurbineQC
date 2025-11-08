#include "curve/pch.h"
#include "sashasecretwindow.h"

SashaSecretWindow::SashaSecretWindow(QWidget *parent)
    : QMainWindow(parent), _ui(new Ui::SashaSecretWindowClass) {
    _ui->setupUi(this);

    connect(_ui->testButtonFor39Task, &QPushButton::clicked, this, [this] {
        QString pathToEndFile = "C:/temp/ca/project/2dend.dat";

        auto params1 = Function1Params(5);
        auto pathToFile = "C:/Users/Admin/Desktop/test/testsFor39Task/1stTest.txt";
        auto inputData = getPointsOfFigure(pathToFile);
        FileSystem::fillInputWithSingleSegment(inputData);
        FileSystem::fillParams(&params1);
        makeCurveCalculations();
        _ui->testResult->append(params1.toQString() + "\n");
        _ui->testResult->append(FileSystem::readFile(pathToEndFile) + "\n\n");
        
        auto params7 = Function7Params(1.7953, -7.16578, -0.3681);
        pathToFile = "C:/Users/Admin/Desktop/test/testsFor39Task/7thTest.txt";
        inputData = getPointsOfFigure(pathToFile);
        FileSystem::fillInputWithSingleSegment(inputData);
        FileSystem::fillParams(&params7);
        makeCurveCalculations();
        _ui->testResult->append(params7.toQString() + "\n");
        _ui->testResult->append(FileSystem::readFile(pathToEndFile) + "\n\n");
        
        auto params8 = Function8Params(10, 14.5516, -1.0959);
        pathToFile = "C:/Users/Admin/Desktop/test/testsFor39Task/8thTest.txt";
        inputData = getPointsOfFigure(pathToFile);
        FileSystem::fillInputWithSingleSegment(inputData);
        FileSystem::fillParams(&params8);
        makeCurveCalculations();
        _ui->testResult->append(params8.toQString() + "\n");
        _ui->testResult->append(FileSystem::readFile(pathToEndFile) + "\n\n");
        
        auto params11 = Function11Params();
        pathToFile = "C:/Users/Admin/Desktop/test/testsFor39Task/11thTest.txt";
        inputData = getPointsOfFigure(pathToFile);
        FileSystem::fillInputWithSingleSegment(inputData);
        FileSystem::fillParams(&params11);
        makeCurveCalculations();
        _ui->testResult->append(params11.toQString() + "\n");
        _ui->testResult->append(FileSystem::readFile(pathToEndFile) + "\n\n");
        
        auto params18 = Function18Params();
        pathToFile = "C:/Users/Admin/Desktop/test/testsFor39Task/18thTest.txt";
        inputData = getPointsOfFigure(pathToFile);
        FileSystem::fillInputWithSingleSegment(inputData);
        FileSystem::fillParams(&params18);
        makeCurveCalculations();
        _ui->testResult->append(params18.toQString() + "\n");
        _ui->testResult->append(FileSystem::readFile(pathToEndFile) + "\n\n");
    });
}

QVector<Point> SashaSecretWindow::getPointsOfFigure(const QString &fullPathtoFile) {
    QVector<Point> points;
    auto splittedPoints = FileSystem::readFile(fullPathtoFile).split('\n');
    for(auto i = 0; i < splittedPoints.length(); i++) {
        auto pointStr = splittedPoints[i].split(',');
        Point point(pointStr[0].toDouble(), pointStr[1].toDouble());
        points.append(point);
    }
    return points;
}

void SashaSecretWindow::makeCurveCalculations() {
    QProcess* process = new QProcess();
    QString curveDirectory = "C:/Users/Admin/Desktop/test/curve-1.exe";
    QStringList list;
    list.append("-dn");
    list.append("ca/project");
    process->start(curveDirectory, list);
    process->waitForFinished(-1);
}

SashaSecretWindow::~SashaSecretWindow() {
    delete _ui;
}
