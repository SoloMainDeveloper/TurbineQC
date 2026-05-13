#include "curve/pch.h"

#include "curve/curvewindow.h"
#include "settings.h"

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);

    QString settingsFilePath = QCoreApplication::applicationDirPath() + "/settings.ini";
    Settings settings(settingsFilePath);

    QTranslator translator;
    if(Settings::instance().hasValue("app/language")) {
        QString language = Settings::instance().value<QString>("app/language");
        QString translatorFile = QString("main_%1.qm").arg(language);

        if(translator.load(translatorFile)) {
            application.installTranslator(&translator);
        }
    }

    CurveWindow curveWindow;
    curveWindow.setWindowState(Qt::WindowState::WindowMaximized);
    curveWindow.show();

    if(FileSystem::checkCurve() == false) {
        auto dialog = new QDialog();
        auto label = new QLabel(dialog);
        label->setText("Curve not found");
        dialog->show();
    }

    return application.exec();
}
