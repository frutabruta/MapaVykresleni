#include "trajectoryjumper.h"

TrajectoryJumper::TrajectoryJumper():gnssWebSockerServer(12345)
{
    qDebug()<<Q_FUNC_INFO;
    timerJumptoNextPoint.setInterval(skipSegmentTime);
    timerStayAtStop.setInterval(stopWaitTime);
    timerStayAtStop.setSingleShot(true);
    connect(&timerJumptoNextPoint,&QTimer::timeout,this,&TrajectoryJumper::slotUpdatePositionNew);
    connect(&timerStayAtStop,&QTimer::timeout,this,&TrajectoryJumper::departedFromStop);
}



void TrajectoryJumper::start()
{
    qDebug()<<Q_FUNC_INFO;
    iterator=0;
    if(seznamMapaBodu.isEmpty())
    {
        emit signalError("StopList is empty");
        return;
    }
    else
    {
        timerJumptoNextPoint.start();
        slotUpdatePositionNew();
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
    qDebug()<<Q_FUNC_INFO;

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
        emit signalChangeWgs(currentSubPoint.wgs84ToQPointF());

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
                nextMapaBod=seznamMapaBodu.at(iterator+1);
                currentPointBuffer=coordinatesTools.interpolatePointsSjtsk(currentMapaBod.sjtskToQPointF(),nextMapaBod.sjtskToQPointF(),skipDistanceMeters);
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

//WIP
void TrajectoryJumper::slotUpdatePositionNew()
{
    qDebug()<<Q_FUNC_INFO;
    if(seznamMapaBodu.isEmpty())
    {
        qDebug()<<"empty point list";
        stop();
        return;
    }

    if(coordinatesSystem==MnozinaBodu::coordinatesUnknown)
    {
        qDebug()<<"unknown coordinate system";
        return;
    }


    if(iterator<seznamMapaBodu.count())
    {
        bool stopIsSet=false;
        bool override=false;
        while((!stopIsSet)||override)
        {
            MapaBod currentPoint=seznamMapaBodu.value(iterator);
            MapaBod nextPoint;
            if((iterator+1)<seznamMapaBodu.count())
            {
                // setMapaBod(testedPoint,MnozinaBodu::S_JTSK);
                qDebug()<<"distance: "<<currentDistanceFromPreviousPoint;
                nextPoint=seznamMapaBodu.value(iterator+1);
                QPointF testedPoint;
                bool isOnSegment=false;

                if(coordinatesSystem==MnozinaBodu::S_JTSK)
                {
                    testedPoint=coordinatesTools.pointAtDistanceSjtsk( currentPoint.sjtskToQPointF(),nextPoint.sjtskToQPointF(),currentDistanceFromPreviousPoint);
                    isOnSegment=coordinatesTools.isPointOnSegmentSimpleSjtsk( currentPoint.sjtskToQPointF(),nextPoint.sjtskToQPointF(),testedPoint);
                }
                else if(coordinatesSystem==MnozinaBodu::WGS84)
                {
                    testedPoint=coordinatesTools.pointAtDistanceWgs84(currentPoint.wgs84ToQPointF(),nextPoint.wgs84ToQPointF(),currentDistanceFromPreviousPoint);
                    isOnSegment=coordinatesTools.isPointOnSegmentSimpleWgs84(currentPoint.wgs84ToQPointF(),nextPoint.wgs84ToQPointF(),testedPoint);
                }
                else
                {
                    qDebug()<<"unknown coordinate system";
                    return;
                }

                if(isOnSegment)
                {
                    setMapaBod(testedPoint,coordinatesSystem);

                    if(coordinatesSystem==MnozinaBodu::WGS84)
                    {
                        emit signalChangeWgs(testedPoint);
                        emit signalMapaBod(MapaBod(testedPoint,true));
                    }
                    else if(coordinatesSystem==MnozinaBodu::S_JTSK)
                    {
                        emit signalMapaBod(MapaBod(testedPoint));
                    }
                    // emit signalMapaBod(MapaBod(testedPoint));
                    residualDistanceFromPreviousPoint=0.0;
                    currentDistanceFromPreviousPoint+=skipDistanceMeters;
                    stopIsSet=true;
                }
                else
                {
                    //setMapaBod(testedPoint,MnozinaBodu::S_JTSK); //xxx
                    if(coordinatesSystem==MnozinaBodu::S_JTSK)
                    {
                        residualDistanceFromPreviousPoint=coordinatesTools.distanceMetersSjtsk(nextPoint.sjtskToQPointF(),testedPoint);
                    }
                    else if(coordinatesSystem==MnozinaBodu::WGS84)
                    {
                        residualDistanceFromPreviousPoint=coordinatesTools.distanceMetersWgs84(nextPoint.wgs84ToQPointF(),testedPoint);
                    }

                    currentDistanceFromPreviousPoint=residualDistanceFromPreviousPoint;
                    qDebug()<<"skipping to next segment, residual distance: "<<residualDistanceFromPreviousPoint;
                    iterator++;
                    //nextPoint();
                }
                //currentDistanceFromPreviousPoint=residualDistanceFromPreviousPoint;
                //residualDistanceFromPreviousPoint=0.0;
            }

            else
            {
                qDebug()<<"last stop";
                override=true;
                if(coordinatesSystem==MnozinaBodu::S_JTSK)
                {
                    setMapaBod(currentPoint.sjtskToQPointF(),coordinatesSystem);
                }
                else if(coordinatesSystem==MnozinaBodu::WGS84)
                {
                    setMapaBod(currentPoint.wgs84ToQPointF(),coordinatesSystem);
                }


                stop();
            }
        }


    }
    else
    {
        qDebug()<<"iterator is out of map point range";
        stop();
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
    qDebug()<<Q_FUNC_INFO;
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

void TrajectoryJumper::setMapaBod(QPointF currentSubPoint, MnozinaBodu::SouradnicovySystem pointCoordinateSystem)
{
    qDebug()<<Q_FUNC_INFO;


    if((currentSubPoint.x()==0.0)&&currentSubPoint.y()==0.0)
    {
        qDebug()<<"null coordinates";
        return;
    }

    if(pointCoordinateSystem==MnozinaBodu::WGS84)
    {
        qDebug()<<" x: "<<QString::number(currentSubPoint.x(),'f',6)<<" y: "<<QString::number(currentSubPoint.y(),'f',6);
        gnssWebSockerServer.setData(currentSubPoint.x(),currentSubPoint.y(),pointCoordinateSystem, centerMap);
    }
    else
    {
        qDebug()<<" x: "<<QString::number(currentSubPoint.x(),'f',2)<<" y: "<<QString::number(currentSubPoint.y(),'f',2);
        gnssWebSockerServer.setData(currentSubPoint.x(),currentSubPoint.y(),pointCoordinateSystem, centerMap);
    }

}

MnozinaBodu::SouradnicovySystem TrajectoryJumper::coordinateSystemFromMapaPointList(QVector<MapaBod> mapaBodList)
{
    qDebug()<<Q_FUNC_INFO;

    int wgsCount=0;
    int sjtskCount=0;
    int unknownCount=0;

    foreach(MapaBod mapaBod, mapaBodList)
    {
        if((mapaBod.lat==0.0)&&(mapaBod.lng==0.0))
        {
            if((mapaBod.x==0.0)&&(mapaBod.y==0.0))
            {
                unknownCount++;
            }
            else
            {
                sjtskCount++;
            }
        }
        else
        {
            wgsCount++;
        }
    }

    qDebug()<<"wgs: "<<wgsCount<<" sjts: "<<sjtskCount<<" unknown: "<<unknownCount;


    if(wgsCount==0)
    {
        if(sjtskCount==0)
        {
            return MnozinaBodu::coordinatesUnknown;
        }
        else
        {
            return MnozinaBodu::S_JTSK;
        }
    }
    else
    {
        return MnozinaBodu::WGS84;
    }

    return MnozinaBodu::coordinatesUnknown;
}





