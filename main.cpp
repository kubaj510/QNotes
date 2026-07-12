#include <QApplication>
#include <QFile>
#include "MainWindow.h"

int main (int argc, char *argv[]){
    QApplication app(argc, argv);

    MainWindow window;
    window.setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
    window.setFixedSize(320, 480);

    QFile styleFile(":src/UI/Style/style.qss");
    styleFile.open(QFile::ReadOnly);
    QString style = QString::fromLatin1(styleFile.readAll());
    window.setStyleSheet(style);
    window.show();

    return app.exec();
}
