#include <QApplication>

#include "pch.h"

#include "executorcli.h"

static bool _quiet = false;

void quietMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    // ¬ тихом режиме ничего не выводим
    if(_quiet)
        return;

    // »наче выводим стандартно (можно оставить как есть)
    fprintf(stderr, "%s\n", msg.toLocal8Bit().constData());
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    ExecutorCLI executor = ExecutorCLI();

    QCommandLineParser parser;
    parser.setApplicationDescription("Curve CLI Macro Runner");
    parser.addHelpOption();
    parser.addPositionalArgument("macro", "Macro file (.crm)");
    parser.addOption(QCommandLineOption("project", "Load project file first", "project"));
    parser.addOption(QCommandLineOption("quiet", "Suppress output"));

    parser.process(app);

    const QStringList args = parser.positionalArguments();
    if(args.isEmpty()) {
        qCritical() << "Usage: CurveCLI <macro.crm> [--project project.crv]";
        return 1;
    }

    if(parser.isSet("quiet")) {
        _quiet = true;
        qInstallMessageHandler(quietMessageHandler);
    }

    QString macroFile = args.first();
    QString projectFile = parser.value("project");

    if(QFile::exists(macroFile)) {
        executor.loadMacros(macroFile);
    }
    else {
        qCritical() << "Macro file not found:" << macroFile;
        return 1;
    }

    if(!projectFile.isEmpty()) {
        if(!QFile::exists(projectFile)) {
            qCritical() << "Project file not found:" << projectFile;
            return 1;
        }
        executor.loadProject(projectFile);
    }

    return executor.run();
}
