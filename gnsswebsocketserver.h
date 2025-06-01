#ifndef GNSSWEBSOCKETSERVER_H
#define GNSSWEBSOCKETSERVER_H


#include <QWebSocketServer>
#include <QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>
#include <QObject>
#include "mnozinabodu.h"

class GNSSWebSocketServer : public QObject {
    Q_OBJECT

public:
    GNSSWebSocketServer(quint16 port, QObject *parent = nullptr);


    float latitude = 50.087;
    float longitude= 14.421;
    MnozinaBodu::SouradnicovySystem souradnicovySystem=MnozinaBodu::J_STSK;
    void setData(float newLatitude, float newLongitude, MnozinaBodu::SouradnicovySystem newCoordinateSystem);
private slots:
    void onNewConnection();
    void onClientDisconnected();
    void sendGnssData();

private:
    QWebSocketServer *server;
    QList<QWebSocket *> clients;
    QTimer *timer;
};



#endif // GNSSWEBSOCKETSERVER_H
