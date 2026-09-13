#include "mapserver.h"
#include <QTcpServer>




MapServer::MapServer(const QString &mapFilesPath, QObject *parent )
    : QObject(parent), m_mapFilesPath(mapFilesPath)
{
    m_server.route("/", [this]() {
        return serveFile("mapa.html");
    });

    m_server.route("/<arg>", [this](const QString &filename) {
        return serveFile(filename);
    });





    #if QT_VERSION < QT_VERSION_CHECK(6, 8, 0)

    m_port = m_server.listen(QHostAddress::LocalHost);
    #else
    auto tcpserver = new QTcpServer();
    if (!tcpserver->listen(QHostAddress::LocalHost) || !m_server.bind(tcpserver)) {
        delete tcpserver;
        return;
    }
    m_port = tcpserver->serverPort();
    qDebug() << "Listening on port" << m_port;
    #endif


}

void MapServer::open() {
    QDesktopServices::openUrl(QUrl("http://localhost:" + QString::number(m_port) + "/"+pageName));
}

void MapServer::setMapFilesPath(const QString &newMapFilesPath)
{
    m_mapFilesPath = newMapFilesPath;
}

quint16 MapServer::port()
{ return m_port; }


QHttpServerResponse MapServer::serveFile(const QString &filename)
{
    QFile file(m_mapFilesPath + "/" + filename);
    if (!file.open(QIODevice::ReadOnly))
        return QHttpServerResponse(QHttpServerResponse::StatusCode::NotFound);

    QString mime = m_mimeDb.mimeTypeForFile(filename).name();
    return QHttpServerResponse(mime.toUtf8(), file.readAll());
}

