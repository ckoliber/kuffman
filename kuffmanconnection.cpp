#include "kuffmanconnection.h"

KuffmanConnection::KuffmanConnection(QObject *parent) : QObject(parent){}

void KuffmanConnection::log(QString log){
    qDebug() << log;
}
void KuffmanConnection::loadKuffmanList(QString path){
    emit clearKuffmanList();
    QDir dirs(path);
    dirs.setFilter(QDir::AllDirs);
    dirs.setSorting(QDir::Name);
    QStringList dirs_list = dirs.entryList();
    for(int cursor = 0 ; cursor < dirs_list.size() ; cursor++){
        QString dirs_path = dirs_list.at(cursor);
        dirs_path = path + dirs_path;
        QStringList dirs_path_parts = dirs_path.split("/");
        emit addKuffmanList(dirs_path,dirs_path_parts.at(dirs_path_parts.size()-1),1);
    }
    QDir files(path);
    files.setFilter(QDir::Files);
    files.setSorting(QDir::Name);
    QStringList files_list = files.entryList();
    for(int cursor = 0 ; cursor < files_list.size() ; cursor++){
        QString files_path = files_list.at(cursor);
        files_path = path + files_path;
        QStringList files_path_parts = files_path.split("/");
        emit addKuffmanList(files_path,files_path_parts.at(files_path_parts.size()-1),0);
    }
}
void KuffmanConnection::compressKuffman(QString path,QString target){
    KuffmanProcess *kuffman_process = new KuffmanProcess;
    connect(kuffman_process,SIGNAL(openProgress()),this,SLOT(onOpenProgress()));
    connect(kuffman_process,SIGNAL(progressProgress(int)),this,SLOT(onProgressProgress(int)));
    connect(kuffman_process,SIGNAL(closeProgress()),this,SLOT(onCloseProgress()));
    kuffman_process->compress(path,target);
}
void KuffmanConnection::decompressKuffman(QString path, QString target){
    KuffmanProcess *kuffman_process = new KuffmanProcess;
    connect(kuffman_process,SIGNAL(openProgress()),this,SLOT(onOpenProgress()));
    connect(kuffman_process,SIGNAL(progressProgress(int)),this,SLOT(onProgressProgress(int)));
    connect(kuffman_process,SIGNAL(closeProgress()),this,SLOT(onCloseProgress()));
    kuffman_process->decompress(path,target);
}

void KuffmanConnection::onOpenProgress(){
    emit openKuffmanProgress();
}
void KuffmanConnection::onProgressProgress(int progress){
    emit progressKuffmanProgress(progress);
}
void KuffmanConnection::onCloseProgress(){
    emit closeKuffmanProgress();
}
