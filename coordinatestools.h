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
    bool isPointInsideCircle(double pointX, double pointY, double centerX, double centerY, float radius);
    double abs(double input);
    bool isPointInsideCircle(const QPointF &point, const QPointF &center, double radius);
    QVector<MapaBod> interpolatePoints(const MapaBod &p1, const MapaBod &p2, double spacing);
};

#endif // COORDINATESTOOLS_H
