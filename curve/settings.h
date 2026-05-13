#pragma once

class Settings {
public:
    static Settings& instance();

    Settings();
    Settings(const QString& filePath);
    virtual ~Settings() = default;

    template<typename T>
    T value(const QString& key, const T& defaultValue = T());

    template<typename T>
    void setValue(const QString& key, const T& value);

    void remove(const QString& key);

    bool hasValue(const QString& key);

private:
    std::shared_ptr<QSettings> _data;

    void createDefault(QString filePath);
};

// TODO: not working with int and maybe not working all types exclude string-like
template<typename T>
T Settings::value(const QString& key, const T& defaultValue)
{
    auto settings = instance();

    auto variant = settings._data->value(key);

    return variant.isValid() && variant.canConvert<T>() ? qvariant_cast<T>(variant) : defaultValue;
}

template<typename T>
void Settings::setValue(const QString& key, const T& value)
{
    auto settings = instance();

    settings._data->setValue(key, QVariant::fromValue(value));
}
