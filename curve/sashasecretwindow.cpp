#include "curve/pch.h"
#include "sashasecretwindow.h"

SashaSecretWindow::SashaSecretWindow(QWidget *parent)
    : QMainWindow(parent), _ui(new Ui::SashaSecretWindowClass) {
    _ui->setupUi(this);

    _pathToInputFile = "C:/Users/Admin/Desktop/test/testsFor42Task/%1";
    _pathToTestDataFile = "C:/Users/Admin/Desktop/test/functions/%1";

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
        
    connect(_ui->testButtonFor42Task, &QPushButton::clicked, this, [this] {
        QString pathToRealDataFile = "C:/temp/ca/project/2dout.dat";

        auto params1 = Function1Params(5);
        auto inputData = getPointsOfFigure(_pathToInputFile.arg("1stTest.txt"));
        CurveLibrary::function1(inputData, params1);
        printText("FUNCTION_1:", _pathToTestDataFile.arg("func1/out.txt"), pathToRealDataFile);

        auto params3 = Function3Params(1);
        inputData = getPointsOfFigure(_pathToInputFile.arg("3rdTest.txt"));
        CurveLibrary::function3(inputData, params3);
        printText("FUNCTION_3:", _pathToTestDataFile.arg("func3/out.txt"), pathToRealDataFile);

        auto params4 = Function4Params(4.861037848);
        auto firstInputData = getPointsOfFigure(_pathToInputFile.arg("4th(1)Test.txt"));
        auto secondInputData = getPointsOfFigure(_pathToInputFile.arg("4th(2)Test.txt"));
        CurveLibrary::function4(firstInputData, secondInputData, params4);
        printText("FUNCTION_4:", _pathToTestDataFile.arg("func4/out.txt"), pathToRealDataFile);

        auto params6 = Function6Params();
        firstInputData = getPointsOfFigure(_pathToInputFile.arg("6th(1)Test.txt"));
        secondInputData = getPointsOfFigure(_pathToInputFile.arg("6th(1)Test.txt"));
        printText("FUNCTION_6:", _pathToTestDataFile.arg("func6/out.txt"), pathToRealDataFile);

        auto params7 = Function7Params(1.795299532, -7.16579905, -0.368086059);
        inputData = getPointsOfFigure(_pathToInputFile.arg("7thTest.txt"));
        CurveLibrary::function7(inputData, params7);
        printText("FUNCTION_7:", _pathToTestDataFile.arg("func7/out.txt"), pathToRealDataFile);

        auto params8 = Function8Params(10, 14.551649366, -1.095929329);
        inputData = getPointsOfFigure(_pathToInputFile.arg("8thTest.txt"));
        CurveLibrary::function8(inputData, params8);
        printText("FUNCTION_8:", _pathToTestDataFile.arg("func8/out.txt"), pathToRealDataFile);

        auto params11 = Function11Params();
        inputData = getPointsOfFigure(_pathToInputFile.arg("11thTest.txt"));
        CurveLibrary::function11(inputData, params11);
        printText("FUNCTION_11:", _pathToTestDataFile.arg("func11/out.txt"), pathToRealDataFile);

        auto params12 = Function12Params();
        inputData = getPointsOfFigure(_pathToInputFile.arg("12thTest.txt"));
        CurveLibrary::function12(inputData, params12);
        printText("FUNCTION_12:", _pathToTestDataFile.arg("func12/out.txt"), pathToRealDataFile);

        auto params13 = Function13Params(Function13Params::Filter::Polar, Function13Params::FilterType::LowPass, 50, true);
        inputData = getPointsOfFigure(_pathToInputFile.arg("13thTest.txt"));
        CurveLibrary::function13(inputData, params13);
        printText("FUNCTION_13:", _pathToTestDataFile.arg("func13/out.txt"), pathToRealDataFile);

        auto params14 = Function14Params();
        firstInputData = getPointsOfFigure(_pathToInputFile.arg("14th(1)Test.txt"));
        secondInputData = getPointsOfFigure(_pathToInputFile.arg("14th(2)Test.txt"));
        CurveLibrary::function14(firstInputData, secondInputData, params14);
        printText("FUNCTION_14:", _pathToTestDataFile.arg("func14/out.txt"), pathToRealDataFile);

        auto params15 = Function15Params(true);
        inputData = getPointsOfFigure(_pathToInputFile.arg("15thTest.txt"));
        CurveLibrary::function15(inputData, params15);
        printText("FUNCTION_15:", _pathToTestDataFile.arg("func15/out.txt"), pathToRealDataFile);

        auto params18 = Function18Params();
        inputData = getPointsOfFigure(_pathToInputFile.arg("18thTest.txt"));
        CurveLibrary::function18(inputData, params18);
        _ui->testResult->setTextColor(QColor(0, 255, 0));
        printText("FUNCTION_18:", _pathToTestDataFile.arg("func18/out.txt"), pathToRealDataFile);
    });
}

QVector<Point> SashaSecretWindow::getPointsOfFigure(const QString &fullPathToFile) {
    QVector<Point> points;
    auto splittedPoints = FileSystem::readFile(fullPathToFile).split('\n');
    for(auto i = 0; i < splittedPoints.length(); i++) {
        auto pointStr = splittedPoints[i].split(',');
        Point point(pointStr[0].toDouble(), pointStr[1].toDouble(), pointStr[2].toDouble(), pointStr[3].toDouble(), pointStr[4].toDouble(),
            pointStr[5].toDouble(), pointStr[6].toDouble(), pointStr[7].toDouble(), pointStr[8].toDouble());
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

void SashaSecretWindow::printText(const QString &header, const QString &pathToTestDataFile, const QString &pathToRealDataFile) {
    _ui->testResult->setTextColor(QColor(0, 255, 0));
    _ui->testResult->append(header);
    _ui->testResult->append(getResultOfComparingTwoFiles(pathToTestDataFile, pathToRealDataFile));
    _ui->testResult->setTextColor(QColor(0, 0, 0));
    _ui->testResult->append(FileSystem::readFile(pathToRealDataFile) + "\n\n");
}

QString SashaSecretWindow::getResultOfComparingTwoFiles(const QString &pathToTestDataFile, const QString &pathToRealDataFile) {
    auto testData = FileSystem::readFile(pathToTestDataFile).split("\n");
    auto realData = FileSystem::readFile(pathToRealDataFile).split("\n");

    for(auto i = 0; i < testData.length(); i++) {
        if(testData[i].replace(" ", "") == realData[i].replace(" ", "")) {
            return "THE TEST IS PASSED";
        }
        else {
            qDebug() << "Error\n" << testData[i] << "\n" << realData[i] << "\n";
        }
    }
    return "THE TEST FAILED WITH AN ERROR";
}

SashaSecretWindow::~SashaSecretWindow() {
    delete _ui;
}
