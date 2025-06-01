#ifndef TRAJECTORYJUMPER_H
#define TRAJECTORYJUMPER_H

#include <QObject>

#include "mapabod.h"
#include "gnsswebsocketserver.h"

class TrajectoryJumper : public QObject
{
    Q_OBJECT
public:
    TrajectoryJumper();


    QVector<MapaBod> seznamMapaBodu;
    MapaBod currentMapaBod;
    void start();
    int iterator=0;
    bool stopAtStops=true;
    int stopWaitTime=10000; //wait time in seconds
    int skipSegmentTime=1000;

    GNSSWebSocketServer gnssWebSockerServer;


    QTimer timerJumptoNextPoint;
    QTimer timerStayAtStop;

    void stop();
public slots:
    void slotUpdatePosition();

signals:
    void signalError(QString errorString);
    void signalMapaBod(MapaBod ouput);
private slots:
    void arrivedAtStop();
private:
    void departedFromStop();
};

#endif // TRAJECTORYJUMPER_H
