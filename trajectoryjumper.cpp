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
        isRunning=true;
    }

}


void TrajectoryJumper::stop()
{
    iterator=0;
    timerJumptoNextPoint.stop();
    timerStayAtStop.stop();
    isRunning=false;
}




void TrajectoryJumper::slotUpdatePosition()
{

    MnozinaBodu::SouradnicovySystem pointCoordinateSystem=MnozinaBodu::WGS84;
    if(!currentPointBuffer.isEmpty())
    {

        MapaBod currentSubPoint;
        if(!currentPointBuffer.isEmpty())
        {
            currentSubPoint=currentPointBuffer.takeFirst();
            if(currentSubPoint.x==0.0)
            {
                if(currentSubPoint.lat==0.0)
                {
                    if(pointCoordinateSystem==MnozinaBodu::WGS84)
                    {
                        qDebug()<<"invalid coordinates, keeping WGS84";
                    }
                    else
                    {
                        qDebug()<<"invalid coordinates, keeping S_JTSK";
                    }
                }
                else
                {
                    qDebug()<<"coordinates override to WGS84";
                    pointCoordinateSystem=MnozinaBodu::WGS84;
                }
            }
            else
            {
                qDebug()<<"coordinates override to S_JTSK";
                pointCoordinateSystem=MnozinaBodu::S_JTSK;
            }

        }
        else
        {
            qDebug()<<"empty point list";
        }


        gnssWebSockerServer.setData(currentSubPoint.lat,currentSubPoint.lng,pointCoordinateSystem, centerMap);
        emit signalMapaBod(currentSubPoint);

        if(stopAtStops)
        {
            if(currentSubPoint.isStop )
            {
                arrivedAtStop();
            }
        }
        else
        {
           // departedFromStop();
        }



    }
    else
    {
        if(iterator<seznamMapaBodu.length())
        {

            currentMapaBod=seznamMapaBodu.at(iterator);

            if((iterator+1)<seznamMapaBodu.count())
            {
                nextMapaBod=seznamMapaBodu.at(iterator+1);
                currentPointBuffer=coordinatesTools.interpolatePoints(currentMapaBod,nextMapaBod,10);
                currentPointBuffer.append(nextMapaBod);
                iterator++;

            }
            else
            {
                qDebug()<<"next point out of range";
                stop();
            }


        }
        else
        {
            timerJumptoNextPoint.stop();
            qDebug()<<"this point out of range";
            stop();
        }


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




