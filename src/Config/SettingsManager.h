#pragma once
#include "AppSettings.h"

// class responsible for loading and saving application settings 
class SettingsManager{
    public:
        // loads the application settings from system registry or configuration file via QSettings
        // returns the loaded settings as an AppSettings structure
        static AppSettings loadSettings();

        // saves the application settings to system registry or configuration file
        static void saveSettings(const AppSettings &newSettings);
};
