#include "trajectoryjumper.h"

TrajectoryJumper::TrajectoryJumper():gnssWebSockerServer(12345)
{
    timerJumptoNextPoint.setInterval(skipSegmentTime);
    timerStayAtStop.setInterval(stopWaitTime);
    timerStayAtStop.setSingleShot(true);
    connect(&timerJumptoNextPoint,&QTimer::timeout,this,&TrajectoryJumper::slotUpdatePosition);
    connect(&timerStayAtStop,&QTimer::timeout,this,&TrajectoryJumper::departedFromStop);
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


void TrajectoryJumper::stop()
{
    iterator=0;
    timerJumptoNextPoint.stop();
    timerStayAtStop.stop();
}




void TrajectoryJumper::slotUpdatePosition()
{
    if(iterator<seznamMapaBodu.length())
    {
        currentMapaBod=seznamMapaBodu.at(iterator);
        gnssWebSockerServer.setData(currentMapaBod.lat,currentMapaBod.lng,MnozinaBodu::J_STSK);
        emit signalMapaBod(currentMapaBod);
         iterator++;
        if(stopAtStops)
        {
            if(currentMapaBod.isStop )
            {
                arrivedAtStop();
            }
        }





    }
    else
    {
        timerJumptoNextPoint.stop();
    }
}

void TrajectoryJumper::arrivedAtStop()
{
    qDebug()<<Q_FUNC_INFO;
    timerJumptoNextPoint.stop();
    timerStayAtStop.start();
}

void TrajectoryJumper::departedFromStop()
{
    qDebug()<<Q_FUNC_INFO;
    timerJumptoNextPoint.start();
}




