#ifndef TRAJECTORYJUMPER_H
#define TRAJECTORYJUMPER_H

#include <QObject>

#include "mapabod.h"
#include "gnsswebsocketserver.h"
#include "coordinatestools.h"

class TrajectoryJumper : public QObject
{
    Q_OBJECT
public:
    TrajectoryJumper();



    GNSSWebSocketServer gnssWebSockerServer;
    CoordinatesTools coordinatesTools;

    QVector<MapaBod> seznamMapaBodu;

    MapaBod currentMapaBod;
    MapaBod nextMapaBod;

    void start();
    int iterator=0;
    bool stopAtStops=true;
    int stopWaitTime=10000; //wait time in seconds
    int skipSegmentTime=1000;

    bool isRunning=false;

    bool centerMap=true;





    QTimer timerJumptoNextPoint;
    QTimer timerStayAtStop;

    QVector<MapaBod> currentPointBuffer;



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
