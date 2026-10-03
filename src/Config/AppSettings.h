#pragma once
#include <QString>

// data structure for storing application settings from the preferences window
// this is used to pass settings data between the preferences window,
// the settings manager, and the main application logic
struct AppSettings{
    bool isDarkMode = true;
    bool alwaysOnTop = true;
    bool wrapWord = true;
    bool autoSave = false;
    bool hideMenuBar = false;

    int fontSize = 14; // font size for the text editor

    QString language = "en"; // "en" or "pl"
};
