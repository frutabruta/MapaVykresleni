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


QVector<MapaBod> CoordinatesTools::interpolatePoints(const MapaBod& p1, const MapaBod& p2, double spacing) {
    QVector<MapaBod> points;

    double dx = p2.lat - p1.lat;
    double dy = p2.lng - p1.lng;
    double distance = qSqrt(qPow(dx, 2) + qPow(dy, 2));

    if (spacing <= 0 || distance == 0)
        return points;

    int numPoints = static_cast<int>(distance / spacing);

    for (int i = 1; i < numPoints; ++i) {
        MapaBod newMapPoint;

        double t = (spacing * i) / distance;
        newMapPoint.lat=p1.lat + t * dx;
        newMapPoint.lng=p1.lng + t * dy;


        points.append(newMapPoint);
    }

    return points;
}

