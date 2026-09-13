#include "gnsswebsocketserver.h"


GNSSWebSocketServer::GNSSWebSocketServer(quint16 port, QObject *parent)
    : QObject(parent)

{

    /*
    m_wsServer(new QWebSocketServer(QStringLiteral("GNSS Server"),
                                QWebSocketServer::NonSecureMode, this))
*/


    m_wsServer = std::make_unique<QWebSocketServer>(
        QStringLiteral("GNSS Relay"), QWebSocketServer::NonSecureMode, this);

    listen(port);

    connect(m_wsServer.get(), &QWebSocketServer::newConnection,
            this, &GNSSWebSocketServer::onNewConnection);
    //connect(&timer,&QTimer::timeout, this, GNSSWebSocketServer::sendGnssData);
    //timer.start(1000); // Send data every second
}


bool GNSSWebSocketServer::listen(quint16 port)
{
    if (!m_wsServer->listen(QHostAddress::AnyIPv4, port))
    {
        qWarning() << "GNSSWebSocketServer: failed to listen on port" << port
                   << m_wsServer->errorString();
        return false;
    }
    qDebug() << "GNSSWebSocketServer listening on port" << m_wsServer->serverPort();
    return true;
}


void GNSSWebSocketServer::onNewConnection()
{
    std::unique_ptr<QWebSocket> client(m_wsServer->nextPendingConnection());

    connect(client.get(), &QWebSocket::textMessageReceived,
            this, &GNSSWebSocketServer::onTextMessageReceived);
    connect(client.get(), &QWebSocket::disconnected,
            this, &GNSSWebSocketServer::onSocketDisconnected);

    m_clients.push_back(std::move(client));
}

void GNSSWebSocketServer::onClientDisconnected()
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

void GNSSWebSocketServer::onSocketDisconnected()
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

void GNSSWebSocketServer::sendGnssData()
{
    QJsonObject gnssData;
    gnssData["latitude"]= latitude;
    gnssData["longitude"] = longitude;
    gnssData["center_map"] = centerMap;

    if(souradnicovySystem==MnozinaBodu::S_JTSK)
    {
        gnssData["coordinate_system"] = "S_JTSK";
    }
    else
    {
        gnssData["coordinate_system"] = "WGS84";
    }

    QJsonDocument doc(gnssData);
    QString jsonString = doc.toJson(QJsonDocument::Compact);


   // for (QWebSocket *client : std::as_const(clients)) {  // didnt work for qt5
    for (const std::unique_ptr<QWebSocket>& client : m_clients)
    {
        if (client->isValid())
        {
            client->sendTextMessage(jsonString);
        }
    }
}

void GNSSWebSocketServer::setData(float newLatitude, float newLongitude,MnozinaBodu::SouradnicovySystem newCoordinateSystem, bool newCenterMap)
{
    latitude=newLatitude;
    longitude=newLongitude;
    souradnicovySystem=newCoordinateSystem;
    centerMap=newCenterMap;
    sendGnssData();
}

void GNSSWebSocketServer::onTextMessageReceived(const QString &message)
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
