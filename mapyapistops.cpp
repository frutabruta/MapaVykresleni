#include "mapyapistops.h"
#include <QColor>

MapyApiStops::MapyApiStops() {}


QVector<MapaBod> MapyApiStops::seznamStopPointDestinationToSeznamMapaBod(QVector<StopPointDestination> seznamZastavek, bool poradi)
{
    qDebug()<<Q_FUNC_INFO;
    QVector<MapaBod> vystup;
    int counter=1;

    foreach(StopPointDestination polozka, seznamZastavek)
    {
        QString kapka="";
        if(poradi)
        {
            kapka=QString::number(counter);
        }
        else
        {
            kapka=QString::number(polozka.stopPoint.idZ);
        }
        MapaBod nacteno=zastavkaCilToMapaBod(polozka,kapka);
        vystup.push_back(nacteno);
        counter++;
    }
    return vystup;
}


MapaBod MapyApiStops::zastavkaCilToMapaBod(StopPointDestination polozka, QString kapka)
{
    qDebug()<<Q_FUNC_INFO;
    MapaBod vystup;

    QString priznaky="";
    if(polozka.stopPoint.onRequest)
    {
        priznaky+="🔔 ";
    }
    if(polozka.stopPoint.transferTrain)
    {
        priznaky+="🚆 ";
    }
    if(polozka.stopPoint.transferMetroA)
    {
        priznaky+="🟩A ";
    }
    if(polozka.stopPoint.transferMetroB)
    {
        priznaky+="🟨B ";
    }
    if(polozka.stopPoint.transferMetroC)
    {
        priznaky+="🟥C ";
    }
    if(polozka.stopPoint.transferMetroD)
    {
        priznaky+="🟦D ";
    }

    if(polozka.stopPoint.transferFerry)
    {
        priznaky+="⚓";
    }

    if(polozka.stopPoint.transferAirplane)
    {
        priznaky+="✈";
    }

    QString pasma= FareZone::fareZoneListToString(polozka.stopPoint.fareZoneList,",");

    vystup.obsah+="<table>";
    vystup.obsah+="<tr>";
    vystup.obsah+="<td>popis</td><td> "+polozka.stopPoint.NameLcd+"</td>";
    vystup.obsah+="</tr>";
    vystup.obsah+="<tr>";
    vystup.obsah+="<td>cis</td><td> "+QString::number(polozka.stopPoint.idCis)+"</td>";
    vystup.obsah+="</tr>";
    vystup.obsah+="<tr>";
    vystup.obsah+="<td>ASW zast. </td><td> "+QString::number(polozka.stopPoint.idU)+"</td>";
    vystup.obsah+="</tr>";
    vystup.obsah+="<tr>";
    vystup.obsah+="<td>ASW slp.</td><td> "+QString::number(polozka.stopPoint.idZ)+"</td>";
    vystup.obsah+="</tr>";
    vystup.obsah+="<tr>";
    vystup.obsah+="<td>radius</td><td> "+QString::number(polozka.stopPoint.radius)+"</td>";
    vystup.obsah+="</tr>";
    vystup.obsah+="<tr>";
    vystup.obsah+="<td>nástupiště</td><td> "+polozka.stopPoint.platformName+"</td>";
    vystup.obsah+="</tr>";
    vystup.obsah+="<tr>";
    vystup.obsah+="<td>odjezd</td><td> "+polozka.stopPoint.departureToQTime().toString("h:mm:ss")+"</td>";
    vystup.obsah+="</tr>";
    vystup.obsah+="<tr>";
    vystup.obsah+="<td>pásma</td><td> "+pasma+"</td>";
    vystup.obsah+="</tr>";

    qDebug()<<"pocet poznamek:"<<QString::number(polozka.stopPoint.notesList.count());
    foreach(QString poznamka, polozka.stopPoint.notesList)
    {
        vystup.obsah+="<tr>";
        vystup.obsah+="<td>poznámka</td><td> "+escapePoznamek(poznamka)+"</td>";
        vystup.obsah+="</tr>";
    }

    vystup.obsah+="</table>";

    /*
    vystup.obsah+="ASW zast.: "+QString::number(polozka.stopPoint.cisloU)+"<br>";
    vystup.obsah+="ASW slp.: "+QString::number(polozka.stopPoint.cisloZ)+"<br>";
    vystup.obsah+="radius: "+QString::number(polozka.stopPoint.radius)+"<br>";
    vystup.obsah+="nástupiště: "+polozka.stopPoint.nastupiste+"<br>";
    vystup.obsah+="odjezd: "+polozka.stopPoint.odjezdQTime().toString("h:mm:ss")+"<br>";
*/
    vystup.hlavicka="<b>"+polozka.stopPoint.StopName+" "+priznaky+"</b>";
    vystup.lat=polozka.stopPoint.lat;
    vystup.lng=polozka.stopPoint.lng;
    vystup.radius=polozka.stopPoint.radius;
    vystup.kapka=kapka;
    vystup.isStop=true;
    if(polozka.stopPoint.isViapoint)
    {
        vystup.barva=QColor::fromRgb(0,0,255);
    }

    return vystup;
}


QString MapyApiStops::spojDoTabulky(Trip vstup)
{
    qDebug()<<Q_FUNC_INFO;
    QString vystup="<h1>vykreslení spoje</h1>";
    vystup+="<table>";
    vystup+="<tr><td>číslo linky ASW</td><td>"+QString::number(vstup.line.c)+"</td></tr>";
    vystup+="<tr><td>alias linky</td><td>"+vstup.line.lineName+"</td></tr>";
    vystup+="<tr><td>licenční číslo</td><td>"+vstup.line.lineNumber+"</td></tr>";
    vystup+="<tr><td>spoj ROPID</td><td>"+QString::number(vstup.idRopid)+"</td></tr>";
    vystup+="<tr><td>spoj ID</td><td>"+QString::number(vstup.id)+"</td></tr>";
    if(!vstup.globalStopPointDestinationList.isEmpty())
    {
        qDebug()<<"pocetZastavek ve vektoru:"<<vstup.globalStopPointDestinationList.length();
        vystup+="<tr><td>ze zastávky</td><td>"+vstup.globalStopPointDestinationList.first().stopPoint.NameLcd+"</td></tr>";
        vystup+="<tr><td>do zastávky</td><td>"+vstup.globalStopPointDestinationList.last().stopPoint.NameLcd+"</td></tr>";

    }


    vystup+="</table>";
    return vystup;
}

QString MapyApiStops::uzelDoTabulky(StopPoint vstup)
{
    QString vystup="<h1>vykreslení uzlu</h1>";
    vystup+="<table>";
    vystup+="<tr><td>uzel</td><td>"+QString::number(vstup.idU)+"</td></tr>";
    vystup+="<tr><td>sloupek</td><td>"+QString::number(vstup.idZ)+"</td></tr>";
    vystup+="<tr><td>název</td><td>"+vstup.NameLcd+"</td></tr>";
    vystup+="</table>";
    return vystup;
}

QString MapyApiStops::escapePoznamek(QString vstup)
{
    vstup=vstup.replace("\\","");
    vstup=vstup.replace("\"","");

    return vstup ;
}
