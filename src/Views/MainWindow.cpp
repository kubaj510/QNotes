#include "ui_MainWindowUI.h"
#include "MainWindow.h"
#include "FileHandler.h"
#include "AppSettings.h"
#include "PreferencesWindow.h"
#include "SettingsManager.h"
#include "TranslationManager.h"
#include "HelpWindow.h"
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QSettings>

MainWindow::MainWindow(TranslationManager *translator, QWidget *parent)
    : QMainWindow(parent),
      document(new Document(this)),
      translator(translator),
      currentSettings(new AppSettings),
      autoSaveTimer(new QTimer(this)),
      ui(new Ui::MainWindowUI)
{
    ui->setupUi(this);
    
    // to restore previous window size after closing an app
    QSettings settings;
    this->restoreGeometry(settings.value("windowSize").toByteArray());

    autoSaveTimer->setSingleShot(true);

    setupConnections();
    applySettings(SettingsManager::loadSettings());
}

MainWindow::~MainWindow(){
    delete currentSettings;
    delete ui; 
}

void MainWindow::closeEvent(QCloseEvent *event){
    if(saveChangesPrompt()){
        // saves window size to restore it later
        QSettings settings;
        settings.setValue("windowSize", this->saveGeometry());

        event->accept();
    
    } else{
        event->ignore();
    }
}

void MainWindow::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange){
        ui->retranslateUi(this);
        updateWindowTitle(); // to translate window title
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::setupConnections(){
    connect(ui->contentEditor, &QTextEdit::textChanged, this, &MainWindow::onContentChanged);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpenTriggered);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onNewTriggered);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveTriggered);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::onSaveAsTriggered);
    connect(ui->actionPreferences, &QAction::triggered, this, &MainWindow::onPreferencesTriggered);
    connect(ui->actionUndo, &QAction::triggered, ui->contentEditor, &QTextEdit::undo);
    connect(ui->actionClose, &QAction::triggered, this, &QMainWindow::close);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::openHelpWindow);
    connect(ui->menuButton, &QPushButton::clicked,  this, [this](){
        ui->menubar->setVisible(!ui->menubar->isVisible());
        QSettings settings;
        settings.setValue("hideMenuBar", !ui->menubar->isVisible()); 
    });
    connect(document, &Document::contentChanged, this, &MainWindow::onDocumentContentChanged);
    connect(document, &Document::modificationChanged, this, &MainWindow::onModificationChanged);
    connect(document, &Document::filePathChanged, this, &MainWindow::updateWindowTitle);
    connect(autoSaveTimer, &QTimer::timeout, this, &MainWindow::onAutoSaveTimeout); 
}


void MainWindow::onOpenTriggered(){
    if(!saveChangesPrompt()) return;

    QString path = QFileDialog::getOpenFileName(this, tr("Open new file"));
    if(path.isEmpty()) return;

    FileResult result = FileHandler::loadFile(path);

    if(result.success){
        document->setFilePath(path);
        document->setContent(result.content);
        document->setModified(false);

    } else{
        QMessageBox msgBox(this);
        msgBox.setText(tr("Cannot open the given file"));
        msgBox.setDetailedText(result.errorMessage);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

bool MainWindow::onSaveTriggered(){
    QString currentPath = document->getFilePath();

    if(currentPath.isEmpty()){
        return onSaveAsTriggered();

    } else{
        FileResult result = FileHandler::saveFile(currentPath, document->getContent());
        if(result.success){
            document->setModified(false);
            return true;

        } else{
            QMessageBox msgBox(this);
            msgBox.setText(tr("Cannot save file. Please try again."));
            msgBox.setDetailedText(result.errorMessage);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
        
            return false;
        }
    }
}

bool MainWindow::onSaveAsTriggered(){
    QString path = QFileDialog::getSaveFileName(this, tr("Save your file"));

    if(path.isEmpty()) return false;

    FileResult result = FileHandler::saveFile(path, document->getContent());
    if(result.success){
        document->setFilePath(path);
        document->setModified(false);

        return true;

    } else{
        QMessageBox msgBox(this);
        msgBox.setText(tr("Cannot save file. Please try again."));
        msgBox.setDetailedText(result.errorMessage);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    
        return false;
    }
}

void MainWindow::onPreferencesTriggered(){
    AppSettings settings = SettingsManager::loadSettings();
    Preferences *preferencesWindow = new Preferences(settings, this);
    preferencesWindow->setAttribute(Qt::WA_DeleteOnClose); // to delete the window when closed before the main window is closed

    connect(preferencesWindow, &QDialog::finished, this, [this, preferencesWindow](int result){
        if(result == QDialog::Accepted){
            SettingsManager::saveSettings(preferencesWindow->getSettings());
            applySettings(SettingsManager::loadSettings());
        }
    });

    preferencesWindow->open();
}

bool MainWindow::saveChangesPrompt(){
    if(document->getIsModified()){
        QMessageBox msgBox(this);
        msgBox.setText(tr("You have unsaved changes."));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        msgBox.setIcon(QMessageBox::Question);
        int res = msgBox.exec();

        switch(res){
            case QMessageBox::Save:
                return onSaveTriggered();

            case QMessageBox::Discard:
                return true;

            case QMessageBox::Cancel:
            default:
                return false;
        }
    }
    return true; // if current document is NOT modified
}

void MainWindow::onContentChanged(){
    if(document->getContent() == ui->contentEditor->toPlainText()) return;
    
    document->setContent(ui->contentEditor->toPlainText());
    document->setModified(true);

}

void MainWindow::onDocumentContentChanged(){
    if(document->getContent() != ui->contentEditor->toPlainText()){
        ui->contentEditor->setPlainText(document->getContent());
    }
}

void MainWindow::updateWindowTitle(){
    QString title = tr("Untitled");

    if(!document->getFilePath().isEmpty()){
        title = QFileInfo(document->getFilePath()).fileName();
    }
    if(document->getIsModified()){
        title.prepend("* ");
    }

    setWindowTitle(title);
}

void MainWindow::onModificationChanged(bool modified){
    updateWindowTitle();

    if(modified && currentSettings->autoSave && !document->getFilePath().isEmpty()){
        if(!autoSaveTimer->isActive()){
            autoSaveTimer->start(300000); // 5min
        }
    } else{
        if(autoSaveTimer->isActive()){
            autoSaveTimer->stop();
        }
    }
}

void MainWindow::onNewTriggered(){
    if(!saveChangesPrompt()) return;

    document->setContent("");
    document->setFilePath("");
    document->setModified(false);
}

void MainWindow::openHelpWindow(){
    if(!helpWindow){
        helpWindow = new HelpWindow(this);
        helpWindow->setWindowFlags(Qt::Window); // to ensure that helpWindow is actually a seperate window, not part of MainWindow
    }

    helpWindow->show();
    helpWindow->raise();
    helpWindow->activateWindow();
}

void MainWindow::applySettings(const AppSettings &settings){
    *currentSettings = settings;

    // changing theme of app between dark and light
    QString stylePath = settings.isDarkMode ? ":/src/UI/Style/style.qss" : ":/src/UI/Style/light-style.qss";
    QFile styleFile(stylePath);
    if(styleFile.open(QFile::ReadOnly | QFile::Text)){
        QTextStream in(&styleFile);
        QString style = in.readAll();
        qApp->setStyleSheet(style);
    }

    // change font size
    QFont font = ui->contentEditor->font();
    font.setPointSize(settings.fontSize);
    ui->contentEditor->setFont(font);

    // wrap word
    ui->contentEditor->setWordWrapMode(settings.wrapWord ? QTextOption::WordWrap : QTextOption::NoWrap);

    // alwaysOnTop
    Qt::WindowFlags flags = windowFlags();
    if(settings.alwaysOnTop){
        flags |= Qt::WindowStaysOnTopHint;
    } else{
        flags &= ~Qt::WindowStaysOnTopHint;
    }
    setWindowFlags(flags);
    show();

    // change language
    translator->switchLanguage(settings.language);

    // hide menu bar
    if(settings.hideMenuBar){
        ui->menubar->setVisible(false);

    } else{
        ui->menubar->setVisible(true);
    }

    // auto save
    // if the user enables autoSave while having a modified (unsaved) document
    // then onModificationChanged will never be invoked unless
    // the user saves the file  to avoid this problem
    // I invoke it here to start the autoSave timer if needed
    onModificationChanged(document->getIsModified());
}


void MainWindow::onAutoSaveTimeout(){
    onSaveTriggered();
    qDebug() << "autoSave triggered";
}

