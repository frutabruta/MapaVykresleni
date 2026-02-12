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

    MnozinaBodu::SouradnicovySystem coordinatesSystem=MnozinaBodu::S_JTSK;

    MapaBod currentMapaBod;
    MapaBod nextMapaBod;

    void start();
    int iterator=0;
    bool stopAtStops=true;
    int stopWaitTime=10000; //wait time in seconds
    int skipSegmentTime=1000;

    int skipDistanceMeters=10;

    bool isRunning=false;

    bool centerMap=true;


    // state variables
    float residualDistanceFromPreviousPoint=0.0;
    float currentDistanceFromPreviousPoint=0.0;

    QTimer timerJumptoNextPoint;
    QTimer timerStayAtStop;

    QVector<MapaBod> currentPointBuffer;

    void stop();

    void setMapaBod(QPointF currentSubPoint, MnozinaBodu::SouradnicovySystem pointCoordinateSystem);
    MnozinaBodu::SouradnicovySystem coordinateSystemFromMapaPointList(QVector<MapaBod> mapaBodList);

public slots:
    void slotUpdatePosition();
    void slotUpdatePositionNew();

signals:
    void signalError(QString errorString);
    void signalMapaBod(MapaBod ouput);
    void signalChangeWgs(QPointF output);

private slots:
    void arrivedAtStop();

private:
    void departedFromStop();
    void setMapaBod(MapaBod currentSubPoint);
};

#endif // TRAJECTORYJUMPER_H
