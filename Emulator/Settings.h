#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>

extern const QString kSettingsGridVisible;
extern const QString kSettingsLastLevel;

class Settings
{
private:
    Settings();

public:
    static int getInt(const QString& name, int defaultValue = 0);
    static bool getBool(const QString& name, bool defaultValue = false);
    static QString getString(const QString& name, const QString& defaultValue = Settings::_NullString);

    static void setInt(const QString& name, int value);
    static void setBool(const QString& name, bool value);
    static void setString(const QString& name, const QString& value);

private:
    static QSettings _settings;
    static const QString _NullString;
};

#endif // SETTINGS_H
