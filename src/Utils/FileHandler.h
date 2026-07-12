#pragma once
#include <QString>


struct FileResult{
    QString content;
    QString errorMessage;

    bool success;
};


class FileHandler{
    public:
        static FileResult saveFile(const QString &path, const QString &content);
        static FileResult loadFile(const QString &path);      
};
