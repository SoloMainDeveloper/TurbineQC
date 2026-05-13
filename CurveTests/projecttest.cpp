#include "pch.h"

#include "filesystem.h"
#include "mockfilesystem.h"
#include "project.h"
#include "projecttest.h"

void ProjectTest::initTestCase()
{
    _project = &Project::instance();
}

void ProjectTest::cleanup()
{
    _project->clear();
}

void ProjectTest::testAddFigure()
{
    QString filePath = MockFileSystem::getMockFilePath("A04.txt");
    FileSystem::loadCloud(filePath, "A04", ",", 2, 1, "X,Y,Z,I,J,K", "1,2,3,4,5,6", ".");

    const QMap<QString, Figure*> figures = _project->figures();

    QCOMPARE(figures.values().count(), 1);
    QVERIFY(figures.contains("A04"));

    CurveFigure* curve = dynamic_cast<CurveFigure*>(figures.value("A04"));
    QVERIFY(curve);
    QCOMPARE(curve->points().count(), 804);
}

void ProjectTest::testRemoveFigure()
{
    QString name = "Circle";
    CircleFigure* circle = new CircleFigure(name, Point(0, 0), Point(1, 0, 0), 5);
    _project->insertFigure(circle);

    const QMap<QString, Figure*> figures = _project->figures();

    QCOMPARE(figures.values().count(), 1);
    QVERIFY(figures.contains(name));

    _project->removeFigure(name);
    QVERIFY(!_project->containsFigure(name));
    QCOMPARE(_project->figures().values().count(), 0);
}

void ProjectTest::testClearProject()
{
    _project->insertFigure(new CircleFigure("Circle", Point(0, 0), Point(1, 0, 0), 5));
    _project->insertFigure(new PointFigure("Point", CurvePoint(1, 0, 0)));

    QCOMPARE(_project->figures().values().count(), 2);

    _project->clear();

    QCOMPARE(_project->figures().values().count(), 0);
}

void ProjectTest::testFigureRenamed()
{
    QString firstName = "Circle";
    _project->insertFigure(new CircleFigure(firstName, Point(0, 0), Point(1, 0, 0), 5));

    QCOMPARE(_project->figures().values().count(), 1);
    QVERIFY(_project->containsFigure(firstName));

    QString newName = "New-Circle-name";
    _project->renameFigure(firstName, newName);

    QCOMPARE(_project->figures().values().count(), 1);
    QVERIFY(!_project->containsFigure(firstName));
    QVERIFY(_project->containsFigure(newName));
}

void ProjectTest::testToggleFigureVisibility()
{
    CircleFigure* circle = new CircleFigure("Circle", Point(0, 0), Point(1, 0, 0), 5);
    _project->insertFigure(circle);

    _project->toggleFigureVisibility("Circle");
    QVERIFY(!circle->isVisible());

    _project->toggleFigureVisibility("Circle");
    QVERIFY(circle->isVisible());
}

void ProjectTest::testChangeFigureColor()
{
    CircleFigure* circle = new CircleFigure("Circle", Point(0, 0), Point(1, 0, 0), 5);
    _project->insertFigure(circle);

    QColor newColor = Qt::red;
    bool signalReceived = false;

    QObject::connect(_project, &Project::figureColorChanged,
        [&signalReceived](const QString& name) {
            signalReceived = true;
            QCOMPARE(name, QString("Circle"));
        });

    _project->changeFigureColor("Circle", newColor);

    QCOMPARE(circle->color(), newColor);
    QVERIFY(signalReceived);
}

void ProjectTest::testShowAllFigures()
{
    _project->insertFigure(new CircleFigure("Circle1", Point(0, 0), Point(1, 0, 0), 5));
    _project->insertFigure(new PointFigure("Point1", CurvePoint(1, 0, 0)));
    _project->insertFigure(new TextFigure("Text1", "Hello", Point(2, 2)));

    _project->hideAllFigures("ANY");

    for(auto* figure : _project->figures()) {
        QVERIFY(!figure->isVisible());
    }

    _project->showAllFigures("ANY");

    for(auto* figure : _project->figures()) {
        QVERIFY(figure->isVisible());
    }
}
