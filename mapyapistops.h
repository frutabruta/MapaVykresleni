#ifndef MAPYAPISTOPS_H
#define MAPYAPISTOPS_H

#include "mapavykresleni.h"
#include "VDV301DataStructures/stoppointdestination.h"
#include "VDV301DataStructures/trip.h"

class MapyApiStops : public MapaVykresleni
{
public:
    MapyApiStops();

    static MapaBod zastavkaCilToMapaBod(StopPointDestination polozka, QString kapka);
    static QVector<MapaBod> seznamStopPointDestinationToSeznamMapaBod(QVector<StopPointDestination> seznamZastavek, bool poradi);
    QString spojDoTabulky(Trip vstup);
    QString uzelDoTabulky(StopPoint vstup);

    static QVector<MapaBod> polygonToSeznamMapaBod(QPolygonF polygon);
private:
     static QString escapePoznamek(QString vstup) ;

};

#endif // MAPYAPISTOPS_H
