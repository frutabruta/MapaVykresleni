// WebSocketRelay.h
#ifndef WEBSOCKETRELAY_H
#define WEBSOCKETRELAY_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <memory>
#include <vector>

class WebSocketRelay : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketRelay(QObject *parent = nullptr);
    ~WebSocketRelay();

    bool listen(quint16 port);
    quint16 serverPort() const;

signals:
    void signalGnssPositionReceived(double latitude, double longitude, bool centerMap);

private slots:
    void onNewConnection();
    void onTextMessageReceived(const QString &message);
    void onSocketDisconnected();

private:
    std::unique_ptr<QWebSocketServer> m_wsServer;
    std::vector<std::unique_ptr<QWebSocket>> m_clients;
};

#endif // WEBSOCKETRELAY_H