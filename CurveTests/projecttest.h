#pragma once

#include <QtTest/QtTest>

class Project;

class ProjectTest : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanup();

    // Тестовые методы
    void testAddFigure();
    void testRemoveFigure();
    void testClearProject();
    void testFigureRenamed();

    void testToggleFigureVisibility();
    void testChangeFigureColor();
    void testShowAllFigures();

private:
    Project* _project;
};
