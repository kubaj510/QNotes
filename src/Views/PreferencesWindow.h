#pragma once
#include <QDialog>
#include "AppSettings.h"

namespace Ui{
    class PreferencesWindow;
}

// this class provides a UI for modifying settings in Preferences window
// it is initialized with current application settings
class Preferences : public QDialog{
    Q_OBJECT
    public:
        Preferences(const AppSettings &currentSettings, QWidget *parent = nullptr);
        ~Preferences();

        // returns AppSettings structure containing user's preferences
        AppSettings getSettings() const; 
        
    private:
        Ui::PreferencesWindow *ui;

        // sets the UI fields with the given settings
        void applyCurrentSettings(const AppSettings &settings);
};
