#include "coordinatestools.h"
#include <QDebug>
CoordinatesTools::CoordinatesTools() {}


bool CoordinatesTools::isPointInsideCircleSjtsk(double pointX, double pointY, double centerX, double centerY, float radius)
{
    //qDebug()<<Q_FUNC_INFO;
    QPointF point(pointX,pointY);
    QPointF center(centerX,centerY);

    return isPointInsideCircleSjtsk(point,center,radius);
}






// Checks if a point is inside a circle
bool CoordinatesTools::isPointInsideCircleSjtsk(const QPointF &point, const QPointF &center, double radius)
{
    //qDebug()<<Q_FUNC_INFO;
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


QVector<MapaBod> CoordinatesTools::interpolatePointsSjtsk(QPointF p1, QPointF p2, double spacing)
{
    //qDebug()<<Q_FUNC_INFO;
    QVector<MapaBod> points;

    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    double distance = distanceMetersSjtsk(p1 ,p2);

    if (spacing <= 0 || distance == 0)
        return points;

    int numPoints = static_cast<int>(distance / spacing);

    for (int i = 1; i < numPoints; ++i) {
        MapaBod newMapPoint;

        double t = (spacing * i) / distance;
        newMapPoint.x=p1.x() + t * dx;
        newMapPoint.y=p1.y() + t * dy;

        points.append(newMapPoint);
    }

    return points;
}






const double CoordinatesTools::EARTH_RADIUS_MEAN_M = 6371008.8;

double CoordinatesTools::degToRad(double deg)
{
    return deg * M_PI / 180.0;
}




double CoordinatesTools::distanceMetersSjtsk(const QPointF &p1, const QPointF &p2)
{
    //qDebug()<<Q_FUNC_INFO;
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    double distance = qSqrt(qPow(dx, 2) + qPow(dy, 2));
    return distance;
}



double CoordinatesTools::distanceMetersWgs84(QPointF pointA, QPointF pointB)
{
    //qDebug()<<Q_FUNC_INFO;
    return distanceMetersWgs84(pointA.x(),pointA.y(),pointB.x(), pointB.y());
}


double CoordinatesTools::distanceMetersWgs84(double latA, double lonA, double latB, double lonB)
{
    //qDebug()<<Q_FUNC_INFO;
    const double phi1 = CoordinatesTools::degToRad(latA);
    const double phi2 = CoordinatesTools::degToRad(latB);
    const double dphi = CoordinatesTools::degToRad(latB - latA);
    const double dlambda = CoordinatesTools::degToRad(lonB - lonA);

    const double sinHalfDphi = std::sin(dphi * 0.5);
    const double sinHalfDlambda = std::sin(dlambda * 0.5);

    const double a = (sinHalfDphi * sinHalfDphi) +
                     (std::cos(phi1) * std::cos(phi2) * sinHalfDlambda * sinHalfDlambda);

    const double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));

    const double distance = CoordinatesTools::EARTH_RADIUS_MEAN_M * c;
    return distance;
}


bool CoordinatesTools::isPointInsideCircleWgs84(const QPointF &point, const QPointF &center, double radius)
{
    //qDebug()<<Q_FUNC_INFO;
    const double distance = CoordinatesTools::distanceMetersWgs84(point.x(),point.y(), center.x(),center.y());
    return distance <= radius;
}





bool CoordinatesTools::isPointInsideCircleWgs84(double latA, double lonA, double latB, double lonB, double radiusMeters)
{
    //qDebug()<<Q_FUNC_INFO;
    const double distance = CoordinatesTools::distanceMetersWgs84(latA, lonA, latB, lonB);
    return distance <= radiusMeters;
}

/*

bool CoordinatesTools::isPointOnSegmentSimpleSjtsk(const QPointF& A, const QPointF& B, const QPointF& Ctest, double epsilon)
{
    qDebug()<<Q_FUNC_INFO;
    // 1. Check collinearity using cross product
    double cross =
        (Ctest.x() - A.x()) * (B.y() - A.y()) -
        (Ctest.y() - A.y()) * (B.x() - A.x());

    if (qAbs(cross) > epsilon)
        return false;

    // 2. Check if C is between A and B using dot product
    double dot =
        (Ctest.x() - A.x()) * (B.x() - A.x()) +
        (Ctest.y() - A.y()) * (B.y() - A.y());

    if (dot < 0)
        return false;

    double squaredLengthAB =
        (B.x() - A.x()) * (B.x() - A.x()) +
        (B.y() - A.y()) * (B.y() - A.y());

    if (dot > squaredLengthAB)
        return false;

    return true;
}*/



/*
 * doesn't check colinearity, only based on distances. Expects the point C to lay on a line AB and checks if it is on segment AB.
 */

bool CoordinatesTools::isPointOnSegmentSimpleSjtsk(const QPointF& A, const QPointF& B, const QPointF& Ctest)
{
    //qDebug()<<Q_FUNC_INFO;

    double distanceAB=distanceMetersSjtsk(A,B);
    double distanceAC=distanceMetersSjtsk(A,Ctest);
    qDebug()<<"distance AB: "<<distanceAB<<" distance AC: "<<distanceAC;

    if(distanceAC>=distanceAB)
    {

        return false;
    }

    return true;
}

/*
 * doesn't check colinearity, only based on distances. Expects the point C to lay on a line AB and checks if it is on segment AB.
 */

bool CoordinatesTools::isPointOnSegmentSimpleWgs84(const QPointF& A, const QPointF& B, const QPointF& Ctest)
{
    //qDebug()<<Q_FUNC_INFO;

    double distanceAB=distanceMetersWgs84(A,B);
    double distanceAC=distanceMetersWgs84(A,Ctest);
    qDebug()<<"distance AB: "<<distanceAB<<" distance AC: "<<distanceAC;

    if(distanceAC>=distanceAB)
    {
        return false;
    }

    return true;
}


QPointF CoordinatesTools::pointAtDistanceSjtsk(const QPointF& A, const QPointF& B, double d)
{
    //qDebug()<<Q_FUNC_INFO;
    double dx = B.x() - A.x();
    double dy = B.y() - A.y();

    double length = qSqrt(dx * dx + dy * dy);

    // Avoid division by zero if A == B
    if (qFuzzyIsNull(length)) {
        return A;
    }

    double ux = dx / length;
    double uy = dy / length;

    return QPointF(
        A.x() + ux * d,
        A.y() + uy * d
        );
}


QPointF CoordinatesTools::pointAtDistanceWgs84(const QPointF &A, const QPointF &B, double dMeters)
{
    // A.x() = latitude [deg], A.y() = longitude [deg]
    // B.x() = latitude [deg], B.y() = longitude [deg]

    const double total = CoordinatesTools::distanceMetersWgs84(A, B);
    if (qFuzzyIsNull(total))
    {
        return A;
    }

    // Convert to radians
    const double phi1 = CoordinatesTools::degToRad(A.x());
    const double lambda1 = CoordinatesTools::degToRad(A.y());
    const double phi2 = CoordinatesTools::degToRad(B.x());
    const double lambda2 = CoordinatesTools::degToRad(B.y());

    // Initial bearing from A to B (radians)
    const double dLambda = lambda2 - lambda1;
    const double y = std::sin(dLambda) * std::cos(phi2);
    const double x = std::cos(phi1) * std::sin(phi2) - std::sin(phi1) * std::cos(phi2) * std::cos(dLambda);
    const double theta = std::atan2(y, x);

    // Angular distance to travel
    const double delta = dMeters / CoordinatesTools::EARTH_RADIUS_MEAN_M;

    // Destination point from A with bearing theta and arc distance delta
    const double sinPhi1 = std::sin(phi1);
    const double cosPhi1 = std::cos(phi1);
    const double sinDelta = std::sin(delta);
    const double cosDelta = std::cos(delta);
    const double sinTheta = std::sin(theta);
    const double cosTheta = std::cos(theta);

    // Latitude
    double sinPhi = sinPhi1 * cosDelta + cosPhi1 * sinDelta * cosTheta;

    // Clamp for numeric safety (domain of asin)
    if (sinPhi > 1.0)
    {
        sinPhi = 1.0;
    }
    else if (sinPhi < -1.0)
    {
        sinPhi = -1.0;
    }

    const double phi = std::asin(sinPhi);

    // Longitude
    const double y2 = sinTheta * sinDelta * cosPhi1;
    const double x2 = cosDelta - sinPhi1 * std::sin(phi);
    double lambda = lambda1 + std::atan2(y2, x2);

    // Normalize longitude to [-pi, pi]
    const double twoPi = 2.0 * M_PI;
    lambda = std::fmod(lambda + M_PI, twoPi);
    if (lambda < 0.0)
    {
        lambda += twoPi;
    }
    lambda -= M_PI;

    return QPointF(
        CoordinatesTools::radToDeg(phi),
        CoordinatesTools::radToDeg(lambda)
        );
}



double CoordinatesTools::radToDeg(double rad)
{
    return rad * 180.0 / M_PI;
}

