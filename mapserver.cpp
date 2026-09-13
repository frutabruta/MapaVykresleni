#include "mapserver.h"
#include <QTcpServer>



MapServer::MapServer(const QString &mapFilesPath, QObject *parent )
    : QObject(parent), m_mapFilesPath(mapFilesPath)
{
    qDebug()<<Q_FUNC_INFO<<" "<<mapFilesPath;
    m_server.route("/", [this]() {
        return serveFile("mapa.html");
    });

    m_server.route("/ping", [this]()
                   {
                       qDebug() << Q_FUNC_INFO << "ping route hit";
                       return QHttpServerResponse("text/plain", QByteArray("pong"));
                   });

    m_server.setMissingHandler(this, [this](const QHttpServerRequest &request, QHttpServerResponder &responder)
                               {
                                   QString path = request.url().path();
                                   qDebug() << Q_FUNC_INFO << "unmatched request for" << path;

                                   QString relativePath = path.startsWith("/") ? path.mid(1) : path;
                                   QHttpServerResponse response = serveFile(relativePath);
                                   responder.sendResponse(std::move(response));
                               });

#if QT_VERSION < QT_VERSION_CHECK(6, 8, 0)

    m_port = m_server.listen(QHostAddress::LocalHost);
#else
    auto tcpserver = new QTcpServer();
    if (!tcpserver->listen(QHostAddress::Any) || !m_server.bind(tcpserver)) {
        delete tcpserver;
        return;
    }
    m_port = tcpserver->serverPort();
    qDebug() << "Listening on port" << m_port;
#endif
}

void MapServer::open() {
    QDesktopServices::openUrl(QUrl("http://127.0.0.1:" + QString::number(m_port) + "/"+pageName));
}

void MapServer::setMapFilesPath(const QString &newMapFilesPath)
{
    m_mapFilesPath = newMapFilesPath;
    qDebug()<<"new maps path:"<<newMapFilesPath;
}

quint16 MapServer::port()
{ return m_port; }


QHttpServerResponse MapServer::serveFile(const QString &filename)
{
    QFile file(m_mapFilesPath + "/" + filename);
    qDebug()<<Q_FUNC_INFO<<" "<<m_mapFilesPath<<"/"<<filename;
    if (!file.open(QIODevice::ReadOnly))
        return QHttpServerResponse(QHttpServerResponse::StatusCode::NotFound);

    QString mime = m_mimeDb.mimeTypeForFile(filename).name();
    return QHttpServerResponse(mime.toUtf8(), file.readAll());
}

