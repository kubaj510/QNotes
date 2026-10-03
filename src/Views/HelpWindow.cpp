#include "HelpWindow.h"
#include "ui_help.h"
#include <QSettings>
#include <QCloseEvent>

HelpWindow::HelpWindow(QWidget *parent) 
    : QWidget(parent),
      ui(new Ui::Help)
{
    ui->setupUi(this);
    loadSettings(); // restore window geometry 

    setupConnections();
    
    ui->content->setSource(QUrl("qrc:/help/example.html"));
}

HelpWindow::~HelpWindow(){
    delete ui;
}

void HelpWindow::closeEvent(QCloseEvent *event){
    writeSettings();
    event->accept();
}

void HelpWindow::setupConnections(){
    connect(ui->okButton, &QPushButton::clicked, this, &HelpWindow::close);
}

void HelpWindow::writeSettings(){
    QSettings settings;
    settings.beginGroup("HelpWindow");
    settings.setValue("geometry", saveGeometry());
    settings.endGroup();
}

void HelpWindow::loadSettings(){
    QSettings settings;
    settings.beginGroup("HelpWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    settings.endGroup();
}

