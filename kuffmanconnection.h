#ifndef KUFFMANCONNECTION_H
#define KUFFMANCONNECTION_H

#include <QObject>
#include <QDebug>
#include <QGuiApplication>
#include <kuffmanprocess.h>

class KuffmanConnection : public QObject
{
    Q_OBJECT
public:
    explicit KuffmanConnection(QObject *parent = nullptr);
    Q_INVOKABLE void log(QString log);
    Q_INVOKABLE void loadKuffmanList(QString path);
    Q_INVOKABLE void compressKuffman(QString path,QString target);
    Q_INVOKABLE void decompressKuffman(QString path, QString target);

signals:
    void clearKuffmanList();
    void addKuffmanList(QString path, QString name, int type);

    void openKuffmanProgress();
    void progressKuffmanProgress(int progress);
    void closeKuffmanProgress();

public slots:
    void onOpenProgress();
    void onProgressProgress(int progress);
    void onCloseProgress();
private:
};

#endif // KUFFMANCONNECTION_H
