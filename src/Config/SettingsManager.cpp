#include <QSettings>
#include "SettingsManager.h"

AppSettings SettingsManager::loadSettings(){
    AppSettings result;

    QSettings settings;
    result.isDarkMode = settings.value("isDarkMode", true).toBool();
    result.alwaysOnTop = settings.value("alwaysOnTop", true).toBool();
    result.wrapWord = settings.value("wrapWord", true).toBool();
    result.autoSave = settings.value("autoSave", false).toBool();
    result.fontSize = settings.value("fontSize", 14).toInt();
    result.language = settings.value("language", "en").toString();

    return result;
}

void SettingsManager::saveSettings(const AppSettings &newSettings){
    QSettings settings;
    
    settings.setValue("isDarkMode", newSettings.isDarkMode);
    settings.setValue("alwaysOnTop", newSettings.alwaysOnTop);
    settings.setValue("wrapWord", newSettings.wrapWord);
    settings.setValue("autoSave", newSettings.autoSave);
    settings.setValue("fontSize", newSettings.fontSize);
    settings.setValue("language", newSettings.language);
}
