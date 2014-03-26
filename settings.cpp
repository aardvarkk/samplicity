#include "settings.h"

QSettings* Settings::settings = nullptr;

QSettings* Settings::getSettings()
{
    if (!settings) {
        settings = new QSettings("The SOFAR Channel", "Samplicity");
    }
    return settings;
}
