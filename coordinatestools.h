#ifndef COORDINATESTOOLS_H
#define COORDINATESTOOLS_H


#include <QPointF>
#include <QtMath>


class CoordinatesTools
{
public:
    CoordinatesTools();
    bool isPointInsideCircle(double pointX, double pointY, double centerX, double centerY, float radius);
    double abs(double input);
    bool isPointInsideCircle(const QPointF &point, const QPointF &center, double radius);
};

#endif // COORDINATESTOOLS_H
