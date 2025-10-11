#ifndef MAPABOD_H
#define MAPABOD_H

#include <QString>
#include <QColor>
#include <QPointF>

class MapaBod
{
public:
    MapaBod();
    QString hlavicka="";
    QString obsah="";
    QString kapka="";
    QColor barva=QColor::fromRgb(255,0,0);
    double x=0.0; //S-JTSK coordinate
    double y=0.0; //S-JTSK coordinate
    double lat=0.0;
    double lng=0.0;
    int radius=0;
    bool isStop=false;
    QPointF toQPointF();
};

#endif // MAPABOD_H
