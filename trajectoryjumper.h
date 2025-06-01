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

    GNSSWebSocketServer gnssWebSockerServer;


    QTimer timerJumptoNextPoint;

public slots:
    void slotUpdatePosition();

signals:
    void signalError(QString errorString);
    void signalMapaBod(MapaBod ouput);
};

#endif // TRAJECTORYJUMPER_H
