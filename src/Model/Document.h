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

        bool getIsModified() const;

    private:
        QString filePath;
        QString content;

        bool isModified;

    signals:
        void contentChanged();
        void filePathChanged(const QString &newPath);
        void modificationChanged(const bool isModified);
};
