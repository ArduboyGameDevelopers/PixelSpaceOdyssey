#include "Settings.h"

const QString kSettingsGridVisible = "gridVisible";

const QString _NullString(QString::Null);

QSettings Settings::_settings("Space Madness");

int Settings::getInt(const QString &name, int defaultValue)
{
    return _settings.value(name, defaultValue).toInt();
}

bool Settings::getBool(const QString& name, bool defaultValue)
{
    return _settings.value(name, defaultValue).toBool();
}

QString Settings::getString(const QString& name, const QString &defaultValue)
{
    return _settings.value(name, defaultValue).toString();
}

void Settings::setInt(const QString &name, int value)
{
    _settings.setValue(name, value);
}

void Settings::setBool(const QString &name, bool value)
{
    _settings.setValue(name, value);
}

void Settings::setString(const QString &name, const QString &value)
{
    _settings.setValue(name, value);
}

