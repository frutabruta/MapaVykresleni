#ifndef MAPSERVER_H
#define MAPSERVER_H


#include <QObject>
#include <QHttpServer>
#include <QMimeDatabase>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>


class MapServer : public QObject
{
     Q_OBJECT
public:
    explicit MapServer(const QString &mapFilesPath, QObject *parent = nullptr);
    quint16 port();
    void open();

    void setMapFilesPath(const QString &newMapFilesPath);

private:
    QHttpServer m_server;
    QMimeDatabase m_mimeDb;
    QString m_mapFilesPath;
    quint16 m_port = 0;
    QHttpServerResponse serveFile(const QString &filename);
};

#endif // MAPSERVER_H
