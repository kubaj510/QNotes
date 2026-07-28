#include "PreferencesWindow.h"
#include "ui_preferences.h"

Preferences::Preferences(const AppSettings &currentSettings, QWidget *parent)
 : QDialog(parent),
   ui(new Ui::PreferencesWindow)
{
    ui->setupUi(this);

    // necessary to set the language options in the combo box
    // because the UI file does not provide a way to set the data (like "en") for each item
    // also it is impossible to get the name of the language from so had to manually set the data
    ui->selectLanguage->addItem("English", "en");
    ui->selectLanguage->addItem("Polish", "pl");

    applyCurrentSettings(currentSettings);
}

Preferences::~Preferences(){
    delete ui;
}

AppSettings Preferences::getSettings() const{
    AppSettings settings;

    settings.isDarkMode = ui->darkButton->isChecked();
    settings.alwaysOnTop = ui->alwaysTopCheck->isChecked();
    settings.wrapWord = ui->wrapCheck->isChecked();
    settings.autoSave = ui->autoSaveCheck->isChecked();
    settings.fontSize = ui->fontSize->value();
    settings.language = ui->selectLanguage->currentData().toString();

    return settings;
}

void Preferences::applyCurrentSettings(const AppSettings &settings){
    ui->alwaysTopCheck->setChecked(settings.alwaysOnTop);
    ui->wrapCheck->setChecked(settings.wrapWord);
    ui->autoSaveCheck->setChecked(settings.autoSave);
    ui->fontSize->setValue(settings.fontSize);
    ui->selectLanguage->setCurrentIndex(ui->selectLanguage->findData(settings.language));

    if(settings.isDarkMode){
        ui->darkButton->setChecked(true);
    } else{
        ui->lightButton->setChecked(true);
    }
}
