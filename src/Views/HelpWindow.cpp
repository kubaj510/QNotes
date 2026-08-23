#include "HelpWindow.h"
#include "ui_help.h"

HelpWindow::HelpWindow(QWidget *parent) 
    : QWidget(parent),
      ui(new Ui::Help)
{
    ui->setupUi(this);

    setupConnections();
}

HelpWindow::~HelpWindow(){
    delete ui;
}

void HelpWindow::setupConnections(){
    connect(ui->okButton, &QPushButton::clicked, this, &HelpWindow::close);
}

// TODO load help files from help/ into ui->content
