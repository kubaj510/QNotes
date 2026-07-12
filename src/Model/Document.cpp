
#include "Document.h"

Document::Document(QObject *parent) : QObject(parent) {
    // filePath = "/home/ja/Documents/testy.txt";
    isModified = false;
}


QString Document::getContent() const                  { return content; }
QString Document::getFilePath() const                 { return filePath; }


void Document::setFilePath(const QString &path)       { filePath = path; }
void Document::setContent(const QString &newContent)  { content = newContent; }
void Document::setModified(bool modified)             { isModified = modified; }


bool Document::getIsModified()                        { return isModified; }
