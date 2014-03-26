#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings
{
public:
    static QSettings* getSettings();
private:
    static QSettings* settings;
};

#endif // SETTINGS_H
