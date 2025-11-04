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
        slotUpdatePosition();
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


    if(!currentPointBuffer.isEmpty())
    {

        MapaBod currentSubPoint;
        if(!currentPointBuffer.isEmpty())
        {
            currentSubPoint=currentPointBuffer.takeFirst();
            setMapaBod(currentSubPoint);

        }
        else
        {
            qDebug()<<"empty point list";
        }


       emit signalMapaBod(currentSubPoint);

        if(stopAtStops)
        {
            if(currentSubPoint.isStop )
            {
                arrivedAtStop();
                 qDebug()<<"at top";
            }
            else
            {
                qDebug()<<"not at stop";
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
                if(iterator==0)
                {

                }
                nextMapaBod=seznamMapaBodu.at(iterator+1);
                currentPointBuffer=coordinatesTools.interpolatePoints(currentMapaBod,nextMapaBod,10);
                currentPointBuffer.append(nextMapaBod);
                if(iterator==0)
                {
                    currentPointBuffer.push_front(currentMapaBod);
                }
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

void TrajectoryJumper::setMapaBod(MapaBod currentSubPoint)
{
    MnozinaBodu::SouradnicovySystem pointCoordinateSystem=MnozinaBodu::WGS84;
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
            gnssWebSockerServer.setData(currentSubPoint.lat,currentSubPoint.lng,pointCoordinateSystem, centerMap);

        }
    }
    else
    {
        qDebug()<<"coordinates override to S_JTSK";
        pointCoordinateSystem=MnozinaBodu::S_JTSK;
        gnssWebSockerServer.setData(currentSubPoint.x,currentSubPoint.y,pointCoordinateSystem, centerMap);
    }
}




