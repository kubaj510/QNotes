#pragma once
#include <QWidget>

namespace Ui{
    class Help;
}

class HelpWindow : public QWidget{
    Q_OBJECT
    public:
        HelpWindow(QWidget *parent = nullptr); //this constructor is fine because all helps file will be loaded here or by helper function from Utils
        ~HelpWindow();

    private:
        void setupConnections();

        Ui::Help *ui;

};
