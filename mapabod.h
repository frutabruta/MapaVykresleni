#ifndef MAPABOD_H
#define MAPABOD_H

#include <QString>
#include <QColor>

class MapaBod
{
public:
    MapaBod();
    QString hlavicka="";
    QString obsah="";
    QString kapka="";
    QColor barva=QColor::fromRgb(255,0,0);
    double lat=0.0;
    double lng=0.0;
    int radius=0;
};

#endif // MAPABOD_H
