#ifndef GNSSWEBSOCKETSERVER_H
#define GNSSWEBSOCKETSERVER_H


#include <QWebSocketServer>
#include <QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>
#include <QObject>
#include "mnozinabodu.h"
#include <memory>
#include <vector>

class GNSSWebSocketServer : public QObject {
    Q_OBJECT

public:
    GNSSWebSocketServer(quint16 port, QObject *parent = nullptr);


    float latitude = 50.087;
    float longitude= 14.421;
    MnozinaBodu::SouradnicovySystem souradnicovySystem=MnozinaBodu::WGS84;
    void setData(float newLatitude, float newLongitude, MnozinaBodu::SouradnicovySystem newCoordinateSystem, bool newCenterMap);
    bool centerMap=false;
    bool listen(quint16 port);
public slots:
    void onTextMessageReceived(const QString &message);
private slots:
    void onNewConnection();
    void onClientDisconnected();
    void sendGnssData();

    void onSocketDisconnected();
private:
    std::unique_ptr<QWebSocketServer> m_wsServer;
    std::vector<std::unique_ptr<QWebSocket>> m_clients;
    QTimer timer;
signals:
    void signalGnssPositionReceived(double latitude, double longitude, bool centerMap);
};



#endif // GNSSWEBSOCKETSERVER_H
