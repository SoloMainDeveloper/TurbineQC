#include "curve/pch.h"
#include "curvewindow.h"

CurveWindow::CurveWindow(QWidget *parent) : QMainWindow(parent), _ui(new Ui::CurveWindow) {
    _ui->setupUi(this);
    connect(_ui->OpenFileButton, &QPushButton::clicked, this, &CurveWindow::openFile);

    tree = _ui->tree;

    connect(tree, &Tree::objectRenamed, this, &CurveWindow::renameFigure);
    connect(tree, &Tree::objectRemoved, this, &CurveWindow::removeFigure);
    connect(tree, &Tree::toggleVisibility, this, &CurveWindow::toggleVisibility);
}

void CurveWindow::enlargeCurveWithIntermediatePoints() { //todo
    auto name = "points_a5"; //take figure name from ui
    auto pointsToInsert = 5; //take from ui
    auto params = Function1Params(pointsToInsert); //take params from ui
    Algorithms::enlargeCurveWithIntermediatePoints(name, &params, &project);
}

void CurveWindow::getMiddleCurve() { //todo
    auto name = "points_a5"; //take figure name from ui
    auto params = Function18Params(); //take params from ui
    Algorithms::getMiddleCurve(name, &params, &project);
}

void CurveWindow::getChordLength() { //todo
    auto name = "points_a5"; //take figure name from ui
    auto params = Function18Params(); //take params from ui
    Algorithms::getChordLength(name, &params, &project);
}

void CurveWindow::getMaxWidthCircle() { //todo
    auto name = "points_a5"; //take figure name from ui
    auto params = Function18Params(); //take params from ui
    Algorithms::getMaxWidthCircle(name, &params, &project);
}

void CurveWindow::openFile() {
    auto filePath = QFileDialog::getOpenFileName(nullptr, "Open file", "", "(*.txt)");
    if(!filePath.isEmpty()) {
        auto data = FileSystem::readFile(filePath).split('\n');
        auto name = filePath.split('/').last().split('.')[0];
        auto *curve = new CurveFigure(name, FileSystem::parsePointsFromElement(data, " "), 1);
        project.insertFigure(curve);
        tree->updateTree(project.figures());
    }
}

void CurveWindow::renameFigure(const QString &name, const QString &newName) {
    project.renameFigure(name, newName);
}
void CurveWindow::removeFigure(const QString &name) {
    project.removeFigure(name);
}
void CurveWindow::toggleVisibility(const QString &name) {
    project.findFigure(name)->toggleVisible();
}

CurveWindow::~CurveWindow() {
    delete _ui;

    for(auto item : project.figures()) {
        delete item;
    }
}
