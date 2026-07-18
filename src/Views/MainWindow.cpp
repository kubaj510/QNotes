#include "ui_MainWindowUI.h"
#include "MainWindow.h"
#include "FileHandler.h"
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      document(new Document(this)),
      ui(new Ui::MainWindowUI)
{
    ui->setupUi(this);
    
    setupConnections();
}

MainWindow::~MainWindow(){
    delete ui; 
}

void MainWindow::closeEvent(QCloseEvent *event){
    if(saveChangesPrompt()){
        event->accept();
    
    } else{
        event->ignore();
    }
}

void MainWindow::setupConnections(){
    connect(ui->contentEditor, &QTextEdit::textChanged, this, &MainWindow::onContentChanged);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpenTriggered);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onNewTriggered);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveTriggered);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::onSaveAsTriggered);
    connect(ui->actionUndo, &QAction::triggered, ui->contentEditor, &QTextEdit::undo);
    connect(ui->actionClose, &QAction::triggered, this, &QMainWindow::close);
    connect(document, &Document::contentChanged, this, &MainWindow::onDocumentContentChanged);
    connect(document, &Document::modificationChanged, this, &MainWindow::updateWindowTitle);
    connect(document, &Document::filePathChanged, this, &MainWindow::updateWindowTitle);

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
        msgBox.setText("Cannot open the given file");
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
            msgBox.setText("Cannot save file. Please try again.");
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
        msgBox.setText("Cannot save file. Please try again.");
        msgBox.setDetailedText(result.errorMessage);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    
        return false;
    }
}

bool MainWindow::saveChangesPrompt(){
    if(document->getIsModified()){
        QMessageBox msgBox(this);
        msgBox.setText("You have unsaved changes.");
        msgBox.setInformativeText("Do you want to save your changes?");
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
    QString title = "Untitled";

    if(!document->getFilePath().isEmpty()){
        title = QFileInfo(document->getFilePath()).fileName();
    }
    if(document->getIsModified()){
        title.prepend("* ");
    }

    setWindowTitle(title);
}

void MainWindow::onNewTriggered(){
    if(!saveChangesPrompt()) return;

    document->setContent("");
    document->setFilePath("");
    document->setModified(false);
}
