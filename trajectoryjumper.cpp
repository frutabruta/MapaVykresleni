#include "trajectoryjumper.h"

TrajectoryJumper::TrajectoryJumper():gnssWebSockerServer(12345)
{
    timerJumptoNextPoint.setInterval(500);
    connect(&timerJumptoNextPoint,&QTimer::timeout,this,&TrajectoryJumper::slotUpdatePosition);
}



void TrajectoryJumper::start()
{
    iterator=0;
    if(seznamMapaBodu.isEmpty())
    {
        emit signalError("StopList is empty");
        return;
    }
    else
    {
        timerJumptoNextPoint.start();
    }

}


void TrajectoryJumper::slotUpdatePosition()
{
    if(iterator<seznamMapaBodu.length())
    {
        currentMapaBod=seznamMapaBodu.at(iterator);
        gnssWebSockerServer.setData(currentMapaBod.lat,currentMapaBod.lng,MnozinaBodu::J_STSK);
        iterator++;
    }
    else
    {
        timerJumptoNextPoint.stop();
    }
}


