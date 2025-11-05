#include "curve/pch.h"

#include "curve/curvewindow.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);

    CurveWindow curveWindow;
    curveWindow.show();

    return application.exec();
}
