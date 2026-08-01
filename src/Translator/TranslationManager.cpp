#include <QApplication>
#include "TranslationManager.h"

TranslationManager::TranslationManager(QObject *parent) : QObject(parent) {}

void TranslationManager::switchLanguage(const QString &langCode){
    qApp->removeTranslator(&translator); // installing new Translator (new language) require removing the old one

    if(langCode == "pl"){
        if(translator.load(":/i18n/notepad_" + langCode)){
            // installing/removing/changing QTranslator generates a languageChange event
            // which I use in changeEvent method in MainWindow to switch language
            qApp->installTranslator(&translator);
        }
    }

    // if anything else - "en" is default
    // app will show untranslated text - so the english
}
