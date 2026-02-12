#include "mapabod.h"

MapaBod::MapaBod()
{

}

/*
 * Constructor used for S-JTSK coordinates
 */
MapaBod::MapaBod(QPointF coordinatesSjtsk)
{
    x=coordinatesSjtsk.x();
    y=coordinatesSjtsk.y();
}

/*
 * Constructor used for WGS84 coordinates, parameter just to distinguish
 */
MapaBod::MapaBod(QPointF coordinatesWgs84, bool wgs84)
{
    lat=coordinatesWgs84.x();
    lng=coordinatesWgs84.y();
}


QPointF MapaBod::sjtskToQPointF()
{
    return QPointF(x,y);
}

QPointF MapaBod::wgs84ToQPointF()
{
    return QPointF(lat,lng);
}



