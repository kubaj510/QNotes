#include <QTextStream>
#include <QFile>
#include "FileHandler.h"


FileResult FileHandler::saveFile(const QString &path, const QString &content){
    QFile file(path);
    FileResult result;
    
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&file);
        out << content;
        file.close();

        result.success = true;
        return result;
    }

    result.success = false;
    result.errorMessage = file.errorString();
    
    return result;
}

FileResult FileHandler::loadFile(const QString &path){
    QFile file(path);
    FileResult result;

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        
        result.content = in.readAll();
        result.success = true;
        file.close();
        
        return result;
    }

    result.errorMessage = file.errorString();
    result.success = false;

    return result;
}
