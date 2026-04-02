#include "mapserver.h"




MapServer::MapServer(const QString &mapFilesPath, QObject *parent )
    : QObject(parent), m_mapFilesPath(mapFilesPath)
{
    m_server.route("/", [this]() {
        return serveFile("mapa.html");
    });

    m_server.route("/<arg>", [this](const QString &filename) {
        return serveFile(filename);
    });

    m_port = m_server.listen(QHostAddress::LocalHost);
}

void MapServer::open() {
    QDesktopServices::openUrl(QUrl("http://localhost:" + QString::number(m_port) + "/mapa.html"));
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

