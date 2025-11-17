#include "curve/pch.h"
#include "curvewindow.h"

//#include "curvelibrary.h" //to delete after CURVE-44 check

void testCreateLine(Plot* plot, Project &project) {
    const auto startPoint = Point(-20, -20);
    const auto endPoint = Point(20, 20);
    const auto lineFigure = new LineFigure("line", startPoint, endPoint);

    project.insertFigure(lineFigure);
}

void testCreateCircle(Plot *plot, Project &project) {
    const auto radius = 20;
    const auto centrePoint = Point(0, 0);

    const auto circleFigure = new CircleFigure("circle", centrePoint, radius);
    const auto pointFigure = new PointFigure("centre_point", centrePoint);

    project.insertFigure(circleFigure);
    project.insertFigure(pointFigure);
}

void testUpdate(Plot *plot, Project &project) {
    auto circleFigure = dynamic_cast<const CircleFigure*>(project.findFigure("circle"));
    auto centrePointFigure = dynamic_cast<const PointFigure*>(project.findFigure("centre_point"));

    if(!circleFigure || !centrePointFigure) {
        qCritical() << "incorrect figures to update";

        return;
    }

    auto redColor = QColorConstants::Red;
    auto greenColor = QColorConstants::Green;

    // TODO: rewrite to signal-slots (_project->changeColor(...))

    // circleFigure->setColor(redColor);
    // centrePointFigure->setColor(greenColor);
    // 
    // plot->updateFigure(circleFigure);
    // plot->updateFigure(centrePointFigure);
}

void testDelete(Plot *_plot, Project &_project) {
    auto lineFigure = _project.findFigure("line");
    
    _project.removeFigure(lineFigure->name());
}

CurveWindow::CurveWindow(QWidget *parent) : QMainWindow(parent), _ui(new Ui::CurveWindow) {
    _ui->setupUi(this);
    connect(_ui->OpenFileButton, &QPushButton::clicked, this, &CurveWindow::openFile);

    _tree = _ui->tree;
    _tree->setProject(&_project);
    _plot = _ui->plot;
    _plot->setProject(&_project);

    _figureControls = new FigureControls(&_project);
    _figureControls->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui->leftBar->addWidget(_figureControls);

    testCreateLine(_plot, _project);
    testCreateCircle(_plot, _project);
    testUpdate(_plot, _project);
    // testDelete(_plot, _project);
}

void CurveWindow::enlargeCurveWithIntermediatePoints() { //todo
    auto name = "points_a5"; //take figure name from ui
    auto pointsToInsert = 5; //take from ui
    auto params = Function1Params(pointsToInsert); //take params from ui
    Algorithms::enlargeCurveWithIntermediatePoints(name, &params, &_project);
}

void CurveWindow::getMiddleCurve() { //todo
    auto name = "points_a5"; //take figure name from ui
    auto params = Function18Params(); //take params from ui
    Algorithms::getMiddleCurve(name, &params, &_project);
}

void CurveWindow::getChordLength() { //todo
    auto name = "points_a5"; //take figure name from ui
    auto params = Function18Params(); //take params from ui
    Algorithms::getChordLength(name, &params, &_project);
}

void CurveWindow::getMaxWidthCircle() { //todo
    auto name = "points_a5"; //take figure name from ui
    auto params = Function18Params(); //take params from ui
    Algorithms::getMaxWidthCircle(name, &params, &_project);
}

void CurveWindow::getWidthOfEdges() {
    auto name = "points_a5";
    auto params = Function18Params();
    Algorithms::getWidthOfEdges(name, &params, &_project);

    //TEST CURVE-8 // Set the path to the curve library in filesystem.cpp
    //QVector<Point> points;
    //auto splittedPoints = FileSystem::readFile("pathToFile.txt").split('\n');//Set the path to the test file
    //for(auto i = 0; i < splittedPoints.length(); i++) {
    //    auto pointStr = splittedPoints[i].split(' '); //Set a separator between coordinates
    //    Point point(pointStr[0].toDouble(), pointStr[1].toDouble());
    //    points.append(point);
    //}
    //auto widthOfEdges = CurveMachine::getWidthOfEdges(points, 1, 1);
    //qDebug() << widthOfEdges[0] << " " << widthOfEdges[1];
    //TEST
}

void CurveWindow::openFile() {
    auto filePath = QFileDialog::getOpenFileName(nullptr, "Open file", "", "(*.txt)");
    if(!filePath.isEmpty()) {
        auto data = FileSystem::readFile(filePath).split('\n');
        auto name = filePath.split('/').last().split('.')[0];
        auto curve = new CurveFigure(name, FileSystem::parsePointsFromElement(data, ","));
        _project.insertFigure(curve);

        //auto res = CurveLibrary::function18(curve->points(), Function18Params()); //to delete after CURVE-44 check
        //auto res2 = CurveMachine::enlargeCurveWithIntermediatePoints(curve->points(), Function1Params(5)); //to delete after CURVE-44 check
    }
}

CurveWindow::~CurveWindow() {
    delete _ui;

    for(auto item : _project.figures()) {
        delete item;
    }
}
