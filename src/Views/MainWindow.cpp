#include "ui_MainWindowUI.h"
#include "MainWindow.h"
#include "FileHandler.h"
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QStandardPaths>
#include <QDir>
#include <QFileDialog>
#include <QTemporaryFile>
#include <QFileSystemWatcher>


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
    if(document->getIsModified()){
        onSaveTriggered();
        event->ignore();

        return;
    }
    event->accept();
}

void MainWindow::setupConnections(){
    // connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::onSaveTriggered);
    connect(ui->contentEditor, &QTextEdit::textChanged, this, &MainWindow::onContentChanged);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpenTriggered);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveTriggered);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::onSaveAsTriggered);
    connect(ui->actionUndo, &QAction::triggered, ui->contentEditor, &QTextEdit::undo);
    connect(ui->actionClose, &QAction::triggered, this, &QMainWindow::close);

}


void MainWindow::onOpenTriggered(){
    QString path = QFileDialog::getOpenFileName();
    QFile file(path);
    
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream in(&file);
    
    document->setFilePath(path);
    document->setContent(in.readAll());
    ui->contentEditor->setPlainText(document->getContent());
}

void MainWindow::onCloseTriggered(){
    if(document->getIsModified()) onSaveTriggered();
}

void MainWindow::onSaveTriggered(){
    QString currentPath = document->getFilePath();

    if(currentPath.isEmpty()){
        onSaveAsTriggered();

    } else{
        FileResult result = FileHandler::saveFile(currentPath, ui->contentEditor->toPlainText());
    }
}

void MainWindow::onSaveAsTriggered(){
    QString path = QFileDialog::getSaveFileName();

    if(path.isEmpty()) return;

    FileResult result = FileHandler::saveFile(path, ui->contentEditor->toPlainText());
}

void MainWindow::onContentChanged(){
    document->setModified(true);
}

// void MainWindow::exercise(){
//     QString documentsFolder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
//     QDir app(documentsFolder);
//
//     QString fullPath = app.filePath("MojaAplikacja");
//     if(!app.exists("MojaAplikacja")){
//         app.mkpath("MojaAplikacja");
//     }
//     qDebug() << "Folder aplikacji: " << fullPath;
//
//     QString pathToProfile = app.filePath("MojaAplikacja/profil.dat");
//
//     QFile file(pathToProfile);
//     if(!file.open(QIODevice::WriteOnly)){
//         qDebug() << file.errorString();
//
//         return;
//     }
//     QString nazwa = "EkspertQt";
//     int poziom = 99;
//     bool trybCiemny = true;
//     QDataStream in(&file);
//     in.setVersion(QDataStream::Qt_6_0);
//     in << nazwa << poziom << trybCiemny;
//     file.close();
//
//
//     QString pathToFile = QFileDialog::getOpenFileName();
//     if(pathToFile == "") return;
//
//     qDebug() << pathToFile;
//
//     QString destination = app.filePath("MojaAplikacja/zaimportowane_logi.txt");
//     QFileInfo f(pathToFile);
//     if(!QFile::exists(destination)){
//         QFile::copy(pathToFile, destination);
//     }
//     QFile copy(destination);
//     if(!copy.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){
//         return;
//     }
//     QTextStream out(&copy);
//     out << "Import zakonczony sukcesem";
//
//     copy.close();
//
//
//     QTemporaryFile temp;
//     if(temp.open()){
//         QString pathToTemp = temp.fileName();
//         QTextStream out2(&temp);
//         out2 << "kopia robocza wykonan";
//         qDebug() << "path to temporary file: " << pathToTemp;
//     }
//
//
//     QFileSystemWatcher *watcher = new QFileSystemWatcher(this);
//     if(!watcher->addPath(pathToProfile)){
//         qDebug() << "watcher cannot be created";
//         return;
//     }
//
//     connect(watcher, &QFileSystemWatcher::fileChanged, this, [this](){ qDebug() << "modyfikacja pliku"; });
//
//
// }

