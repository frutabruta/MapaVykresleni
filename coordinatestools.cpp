#include "coordinatestools.h"

CoordinatesTools::CoordinatesTools() {}


bool CoordinatesTools::isPointInsideCircle(double pointX, double pointY, double centerX, double centerY, float radius)
{
    QPointF point(pointX,pointY);
    QPointF center(centerX,centerY);

    return isPointInsideCircle(point,center,radius);
}






// Checks if a point is inside a circle
bool CoordinatesTools::isPointInsideCircle(const QPointF &point, const QPointF &center, double radius)
{
    double distanceSquared = qPow(point.x() - center.x(), 2) + qPow(point.y() - center.y(), 2);

     return distanceSquared <= qPow(radius, 2);
}


double CoordinatesTools::abs(double input)
{
    if(input>0)
        return input;
    else
        return -input;
}
