#include "curve/pch.h"

#include "curve/curvewindow.h"
#include "curve/sashasecretwindow.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);

    CurveWindow curveWindow;
    curveWindow.show();

    /*SashaSecretWindow sashaSecretWindow;
    sashaSecretWindow.show();*/

    return application.exec();
}
