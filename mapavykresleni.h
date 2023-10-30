#ifndef MAPAVYKRESLENI_H
#define MAPAVYKRESLENI_H

#include <QVector>
#include <QString>
#include <QDebug>
#include <QUrl>
#include <QDesktopServices>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QtGlobal>
#include "VDV301DataStructures/vehiclestate.h"
#include "mnozinabodu.h"






class MapaVykresleni
{
public:

    MapaVykresleni();
    QVector<MnozinaBodu> seznamMnozin;

    void qstringDoSouboru(QString cesta, QString obsah);
    static MapaBod zastavkaCilToMapaBod(StopPointDestination polozka, QString kapka);
    static QVector<MapaBod> seznamStopPointDestinationToSeznamMapaBod(QVector<StopPointDestination> seznamZastavek, bool poradi);
    void pridejMnozinu(QVector<MapaBod> seznamBodu, bool vykresliBody, bool vykresliSpojnici, bool vykresliTrasu, bool vykresliRadius, MnozinaBodu::SouradnicovySystem souradnicovySystem);
    void seznamMnozinDoJson(QVector<MnozinaBodu> seznamMnozin, QString popis);
    QString htmlTag(QString vstup, QString tag);
    QString spojDoTabulky(Trip vstup);

    QString uzelDoTabulky(StopPoint vstup);
    QString getCestaMapa() const;
    void setCestaMapa(const QString &newCestaMapa);

private:
    QString cestaMapa="";
    static QString escapePoznamek(QString vstup) ;
};



#endif // MAPAVYKRESLENI_H
