#include <QApplication>
#include <QFile>
#include "MainWindow.h"
#include "TranslationManager.h"

int main (int argc, char *argv[]){
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("kubaj510");
    QCoreApplication::setApplicationName("Quick Notes");
    QCoreApplication::setOrganizationDomain("github.com/kubaj510");

    TranslationManager translationManager;
    translationManager.switchLanguage(QLocale::system().name().left(2));

    MainWindow window(&translationManager);
    window.setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
    window.setMinimumSize(250, 190);
    // window.resize(320, 400);
    // window.setFixedSize(320, 480);

    window.show();

    return app.exec();
}
