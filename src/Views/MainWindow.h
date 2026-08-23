#pragma once
#include "QMainWindow"
#include <QCloseEvent>
#include "Document.h"


struct AppSettings;
class TranslationManager;
class HelpWindow;

namespace Ui{
    class MainWindowUI;
}

class MainWindow : public QMainWindow{
    Q_OBJECT
    public:
        MainWindow(TranslationManager *translator, QWidget *parent = nullptr);
        ~MainWindow();

    protected:
        void closeEvent(QCloseEvent *event) override;
        void changeEvent(QEvent *event) override;

    private:
        void setupConnections();
        void applySettings(const AppSettings &settings);
        bool saveChangesPrompt();

        Document *document;
        TranslationManager *translator;
        AppSettings *currentSettings;
        HelpWindow *helpWindow = nullptr;

        QTimer *autoSaveTimer;

        Ui::MainWindowUI *ui;

    private slots:
        void onOpenTriggered();
        void onNewTriggered(); 
        void onPreferencesTriggered();
        void onContentChanged();
        void onDocumentContentChanged();
        void onModificationChanged(bool modified);
        void onAutoSaveTimeout();
        void updateWindowTitle(); // when modificationChanged or filePathChanged is emited
        void openHelpWindow(); // opens modeless help window

        bool onSaveTriggered();
        bool onSaveAsTriggered();
};
