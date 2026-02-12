#ifndef COORDINATESTOOLS_H
#define COORDINATESTOOLS_H


#include <QPointF>
#include <QtMath>
#include <QVector>

#include "mapabod.h"

class CoordinatesTools
{
public:
    CoordinatesTools();
    bool isPointInsideCircleSjtsk(double pointX, double pointY, double centerX, double centerY, float radius);
    double abs(double input);
    bool isPointInsideCircleSjtsk(const QPointF &point, const QPointF &center, double radius);
    QVector<MapaBod> interpolatePointsSjtsk(QPointF p1, QPointF p2, double spacing);




    // Mean Earth radius in meters (IUGG recommended value)
    // https://en.wikipedia.org/wiki/Earth_radius#Mean_radius
    static const double EARTH_RADIUS_MEAN_M;

    // Compute great-circle distance (meters) between two WGS84 points (lat/lon in degrees).
    // Uses the haversine formula on a spherical Earth model.
    static double distanceMetersWgs84(double latA, double lonA, double latB, double lonB);

    // Check if point A lies within radiusMeters of point B (surface distance).
    static bool isPointInsideCircleWgs84(double latA, double lonA, double latB, double lonB, double radiusMeters);
    static bool isPointInsideCircleWgs84(const QPointF &point, const QPointF &center, double radius);

    static QPointF pointAtDistanceSjtsk(const QPointF &A, const QPointF &B, double d);
    static bool isPointOnSegmentSimpleSjtsk(const QPointF &A, const QPointF &B, const QPointF &Ctest);
    static double distanceMetersWgs84(QPointF pointA, QPointF pointB);
    static double distanceMetersSjtsk(const QPointF &p1, const QPointF &p2);
    static bool isPointOnSegmentSimpleWgs84(const QPointF &A, const QPointF &B, const QPointF &Ctest);
    static QPointF pointAtDistanceWgs84(const QPointF &A, const QPointF &B, double dMeters);
protected:

private:
    // Utility: degrees to radians
    static double degToRad(double deg);
    static double radToDeg(double rad);
};

#endif // COORDINATESTOOLS_H
