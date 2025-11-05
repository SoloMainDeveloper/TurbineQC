#include "curve/pch.h"

#include "curve/curvewindow.h"

#include "ui_curvewindow.h"

CurveWindow::CurveWindow(QWidget *parent) : QMainWindow(parent), _ui(new Ui::CurveWindow) {
    _ui->setupUi(this);
}

CurveWindow::~CurveWindow() {
    delete _ui;
}
