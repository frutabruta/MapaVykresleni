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

#include "mnozinabodu.h"






class MapaVykresleni
{
public:

    MapaVykresleni();
    QVector<MnozinaBodu> seznamMnozin;

    void qstringDoSouboru(QString cesta, QString obsah);

    void pridejMnozinu(QVector<MapaBod> seznamBodu, bool vykresliBody, bool vykresliSpojnici, bool vykresliTrasu, bool vykresliRadius, bool vykresliPolygon, MnozinaBodu::SouradnicovySystem souradnicovySystem);
    void seznamMnozinDoJson(QVector<MnozinaBodu> seznamMnozin, QString popis);
    QString htmlTag(QString vstup, QString tag);



    QString getHtmlResultPath() const;
    void setHtmlResultPath(const QString &htmlResultPath);

private:
    QString mHtmlResultPath="";

};



#endif // MAPAVYKRESLENI_H
