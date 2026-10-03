#pragma once
#include <QWidget>

namespace Ui{
    class Help;
}

class HelpWindow : public QWidget{
    Q_OBJECT
    public:
        HelpWindow(QWidget *parent = nullptr);
        ~HelpWindow();

    protected:
        void closeEvent(QCloseEvent *event) override;

    private:
        void setupConnections();
        
        void writeSettings();
        void loadSettings();

        Ui::Help *ui;

};
