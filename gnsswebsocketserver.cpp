#include "gnsswebsocketserver.h"


GNSSWebSocketServer::GNSSWebSocketServer(quint16 port, QObject *parent)
    : QObject(parent),
    server(new QWebSocketServer(QStringLiteral("GNSS Server"),
                                QWebSocketServer::NonSecureMode, this)),
    timer(new QTimer(this))
{
    if (server->listen(QHostAddress::Any, port)) {
        connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    }

    connect(timer, SIGNAL(timeout()), this, SLOT(sendGnssData()));
    timer->start(1000); // Send data every second
}

void GNSSWebSocketServer::onNewConnection() {
    QWebSocket *clientSocket = server->nextPendingConnection();
    clients << clientSocket;

    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
}

void GNSSWebSocketServer::onClientDisconnected() {
    QWebSocket *clientSocket = qobject_cast<QWebSocket *>(sender());
    if (clientSocket) {
        clients.removeAll(clientSocket);
        clientSocket->deleteLater();
    }
}

void GNSSWebSocketServer::sendGnssData() {
    QJsonObject gnssData;
    gnssData["latitude"]= latitude;
    gnssData["longitude"] = longitude;

    if(souradnicovySystem==MnozinaBodu::J_STSK)
    {
        gnssData["coordinate_system"] = "J_STSK";
    }
    else
    {
        gnssData["coordinate_system"] = "WGS84";
    }

    QJsonDocument doc(gnssData);
    QString jsonString = doc.toJson(QJsonDocument::Compact);

    for (QWebSocket *client : std::as_const(clients)) {
        if (client->isValid()) {
            client->sendTextMessage(jsonString);
        }
    }
}

void GNSSWebSocketServer::setData(float newLatitude, float newLongitude,MnozinaBodu::SouradnicovySystem newCoordinateSystem)
{
    latitude=newLatitude;
    longitude=newLongitude;
    souradnicovySystem=newCoordinateSystem;
    sendGnssData();
}



