#include <QApplication>
#include <QtTest/QtTest>

#include "macrosmanagertest.h"
#include "projecttest.h"
#include "tests.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QLoggingCategory::setFilterRules("*.debug=false\n*.warning=false");

    int status = 0;

    // ProjectTest projectTest;
    // status |= QTest::qExec(&projectTest, argc, argv);

    // MacrosManagerTest macrosManagerTest;
    // status |= QTest::qExec(&macrosManagerTest, argc, argv);

    TestMyClass testMacros;
    status |= QTest::qExec(&testMacros, argc, argv);

    return status;
}
