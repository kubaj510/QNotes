
#include "Document.h"

Document::Document(QObject *parent) : QObject(parent) {
    // filePath = "/home/ja/Documents/testy.txt";
    isModified = false;
}


QString Document::getContent() const    { return content; }
QString Document::getFilePath() const   { return filePath; }

bool Document::getIsModified() const    { return isModified; }


void Document::setFilePath(const QString &path){
    if(filePath == path) return;

    filePath = path;
    emit filePathChanged(filePath);
}

void Document::setContent(const QString &newContent){
    if(content == newContent) return;

    content = newContent;
    emit contentChanged();
}

void Document::setModified(bool modified){
    if(isModified == modified) return;

    isModified = modified;
    emit modificationChanged(isModified);
}
