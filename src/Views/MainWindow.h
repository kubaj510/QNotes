#pragma once
#include "QMainWindow"
#include <QCloseEvent>
#include "Document.h"

namespace Ui{
    class MainWindowUI;
}

class MainWindow : public QMainWindow{
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    protected:
        void closeEvent(QCloseEvent *event) override;

    private:
        void setupConnections();

        Document *document;
        Ui::MainWindowUI *ui;

    private slots:
        void onOpenTriggered();
        bool onSaveTriggered();
        bool onSaveAsTriggered();
        void onContentChanged();
        void onDocumentContentChanged();
        void updateWindowTitle(); // when modificationChanged or filePathChanged is emited
};
