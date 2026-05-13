#pragma once

class MockFileSystem {
public:
    static QDir getMockDir();
    static QString getMockFilePath(QString fileName);
};
