// WebSocketRelay.cpp
#include "WebSocketRelay.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

WebSocketRelay::WebSocketRelay(QObject *parent)
    : QObject(parent)
{
    m_wsServer = std::make_unique<QWebSocketServer>(
        QStringLiteral("GNSS Relay"), QWebSocketServer::NonSecureMode, this);

    connect(m_wsServer.get(), &QWebSocketServer::newConnection,
            this, &WebSocketRelay::onNewConnection);
}

WebSocketRelay::~WebSocketRelay() = default;

bool WebSocketRelay::listen(quint16 port)
{
    if (!m_wsServer->listen(QHostAddress::AnyIPv4, port))
    {
        qWarning() << "WebSocketRelay: failed to listen on port" << port
                   << m_wsServer->errorString();
        return false;
    }
    qDebug() << "WebSocketRelay listening on port" << m_wsServer->serverPort();
    return true;
}

quint16 WebSocketRelay::serverPort() const
{
    return m_wsServer->serverPort();
}

void WebSocketRelay::onNewConnection()
{
    std::unique_ptr<QWebSocket> client(m_wsServer->nextPendingConnection());

    connect(client.get(), &QWebSocket::textMessageReceived,
            this, &WebSocketRelay::onTextMessageReceived);
    connect(client.get(), &QWebSocket::disconnected,
            this, &WebSocketRelay::onSocketDisconnected);

    m_clients.push_back(std::move(client));
}

/*
void WebSocketRelay::onSocketDisconnected()
{
    QWebSocket *sender = qobject_cast<QWebSocket *>(QObject::sender());
    if (!sender)
    {
        return;
    }

    for (std::vector<std::unique_ptr<QWebSocket>>::iterator it = m_clients.begin();
         it != m_clients.end(); ++it)
    {
        if (it->get() == sender)
        {
            m_clients.erase(it);
            break;
        }
    }
}
*/

void WebSocketRelay::onSocketDisconnected()
{
    QWebSocket *sender = qobject_cast<QWebSocket *>(QObject::sender());
    if (!sender)
    {
        return;
    }

    for (std::vector<std::unique_ptr<QWebSocket>>::iterator it = m_clients.begin();
         it != m_clients.end(); ++it)
    {
        if (it->get() == sender)
        {
            it->release();       // give up ownership before erase, so the vector doesn't delete it
            sender->deleteLater(); // safe deferred deletion
            m_clients.erase(it);
            break;
        }
    }
}

void WebSocketRelay::onTextMessageReceived(const QString &message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (!doc.isObject())
    {
        qWarning() << "WebSocketRelay: received non-object JSON";
        return;
    }

    QJsonObject obj = doc.object();
    QString coordinateSystem = obj.value("coordinate_system").toString();

    if (coordinateSystem != QStringLiteral("WGS84"))
    {
        qWarning() << "WebSocketRelay: unexpected coordinate system" << coordinateSystem;
        return;
    }

    double latitude = obj.value("latitude").toDouble();
    double longitude = obj.value("longitude").toDouble();
    bool centerMap = obj.value("center_map").toBool();

    emit signalGnssPositionReceived(latitude, longitude, centerMap);
}