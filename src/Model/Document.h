#pragma once
#include <QObject>

class Document : public QObject{
    Q_OBJECT
    public:
        Document(QObject *parent = nullptr);
        
        QString getContent() const;
        QString getFilePath() const;

        void setFilePath(const QString &path);
        void setContent(const QString &newContent);
        void setModified(bool modified);

        bool getIsModified();

    private:
        QString filePath;
        QString content;

        bool isModified;

};
