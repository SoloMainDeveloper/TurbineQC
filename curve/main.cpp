#include "curve/pch.h"

#include "curve/curvewindow.h"
#include "curve/sashasecretwindow.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);

    CurveWindow curveWindow;
    curveWindow.setWindowState(Qt::WindowState::WindowMaximized);
    curveWindow.show();

    if(FileSystem::checkCurve() == false) {
        auto dialog = new QDialog();
        auto label = new QLabel(dialog);
        label->setText("Curve not found");
        dialog->show();
    }

    /*SashaSecretWindow sashaSecretWindow;
    sashaSecretWindow.show();*/

    return application.exec();
}
