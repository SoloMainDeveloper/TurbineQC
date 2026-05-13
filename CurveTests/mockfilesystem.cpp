#include "pch.h"

#include "mockfilesystem.h"

QDir MockFileSystem::getMockDir()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString librariesDir = env.value("LIBRARIES_DIR", ".");

    auto dir = QDir(librariesDir);
    dir.cd("mock");

    return dir;
}

QString MockFileSystem::getMockFilePath(QString fileName)
{
    auto dir = getMockDir();

    return dir.filePath(fileName);
}
