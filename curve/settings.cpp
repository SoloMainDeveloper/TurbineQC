#include "curve/pch.h"

#include "settings.h"

Settings& Settings::instance()
{
    static Settings instance;
    return instance;
}

Settings::Settings()
    : Settings(QCoreApplication::applicationDirPath() + "/settings.ini")
{
}

Settings::Settings(const QString& filePath)
{
    if(!QFile::exists(filePath)) {
        createDefault(filePath);
    }
    else {
        _data.reset(new QSettings(filePath, QSettings::IniFormat));
    }
}

void Settings::remove(const QString& key)
{
    if(_data) {
        _data->remove(key);
    }
}

bool Settings::hasValue(const QString& key)
{
    if(_data) {
        return _data->contains(key);
    }

    return false;
}

void Settings::createDefault(QString filePath)
{
    QFileInfo fileInfo(filePath);
    QDir().mkpath(fileInfo.absolutePath());

    QSettings defaultSettings(filePath, QSettings::IniFormat);
    defaultSettings.setValue("app/language", "ru");
    defaultSettings.sync();

    qDebug() << "Created settings file with defaults:" << filePath;
}
