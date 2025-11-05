#include "mapavykresleni.h"



MapaVykresleni::MapaVykresleni()
{

}



void MapaVykresleni::pridejMnozinu(QVector<MapaBod> seznamBodu , bool vykresliBody, bool vykresliSpojnici, bool vykresliTrasu, bool vykresliRadius,bool vykresliPolygon,bool vykresliPolygonOut, MnozinaBodu::SouradnicovySystem souradnicovySystem)
{
    qDebug()<<Q_FUNC_INFO;
    //QVector<StopPointDestination> seznam
    MnozinaBodu mnozinaBodu;
    mnozinaBodu.seznamMapaBodu=seznamBodu;
    mnozinaBodu.vykresliBody=vykresliBody;
    mnozinaBodu.vykresliSpojnici=vykresliSpojnici;
    mnozinaBodu.vykresliTrasu=vykresliTrasu;
    mnozinaBodu.vykresliRadius=vykresliRadius;
    mnozinaBodu.souradnicovySystem=souradnicovySystem;
    mnozinaBodu.vykresliPolygon=vykresliPolygon;
    mnozinaBodu.vykresliPolygonOut=vykresliPolygonOut;
    seznamMnozin.push_back(mnozinaBodu);
}

void MapaVykresleni::seznamMnozinDoJson(QVector<MnozinaBodu> seznamMnozin,  QString popis)
{
    qDebug()<<Q_FUNC_INFO;
    //QJsonObject jRoot;
    QJsonArray jSeznamPolozek;
    QJsonObject jRoot;




    foreach(MnozinaBodu polozkaMnozina, seznamMnozin)
    {
        QJsonObject jPolozka;
        jPolozka.insert("vykresliBody", QJsonValue::fromVariant(polozkaMnozina.vykresliBody));
        jPolozka.insert("vykresliSpojnici", QJsonValue::fromVariant(polozkaMnozina.vykresliSpojnici));
        jPolozka.insert("vykresliTrasu", QJsonValue::fromVariant(polozkaMnozina.vykresliTrasu));
        jPolozka.insert("vykresliRadius", QJsonValue::fromVariant(polozkaMnozina.vykresliRadius));
        jPolozka.insert("vykresliPolygon", QJsonValue::fromVariant(polozkaMnozina.vykresliPolygon));
        jPolozka.insert("vykresliPolygonOut", QJsonValue::fromVariant(polozkaMnozina.vykresliPolygonOut));

        if(!polozkaMnozina.seznamMapaBodu.isEmpty())
        {
            if(polozkaMnozina.seznamMapaBodu.first().x==0.0)
            {
                if(polozkaMnozina.seznamMapaBodu.first().lat==0.0)
                {
                    if(polozkaMnozina.souradnicovySystem==MnozinaBodu::WGS84)
                    {
                        qDebug()<<"invalid coordinates, keeping WGS84";
                    }
                    else
                    {
                        qDebug()<<"invalid coordinates, keeping S_JTSK";
                    }
                }
                else
                {
                    qDebug()<<"coordinates override to WGS84";
                    polozkaMnozina.souradnicovySystem=MnozinaBodu::WGS84;
                }
            }
            else
            {
                qDebug()<<"coordinates override to S_JTSK";
                polozkaMnozina.souradnicovySystem=MnozinaBodu::S_JTSK;
            }

        }
        else
        {
            qDebug()<<"empty point list";
        }


        if(polozkaMnozina.souradnicovySystem==MnozinaBodu::S_JTSK)
        {
            jPolozka.insert("souradnicovySystem", QJsonValue::fromVariant("S_JTSK"));
        }
        else
        {
            jPolozka.insert("souradnicovySystem", QJsonValue::fromVariant("WGS84"));
        }

        QJsonArray radkyJSonArray;

        foreach(MapaBod polozkaZastavka, polozkaMnozina.seznamMapaBodu)
        {
            QJsonObject recordObject;
            recordObject.insert("title", QJsonValue::fromVariant(polozkaZastavka.hlavicka));
            recordObject.insert("cont", QJsonValue::fromVariant(polozkaZastavka.obsah));

            if(polozkaMnozina.souradnicovySystem==MnozinaBodu::WGS84)
            {
                recordObject.insert("lat", QJsonValue::fromVariant(polozkaZastavka.lat));
                recordObject.insert("lng", QJsonValue::fromVariant(polozkaZastavka.lng));

            }
            else
            {
                recordObject.insert("lat", QJsonValue::fromVariant(polozkaZastavka.x));
                recordObject.insert("lng", QJsonValue::fromVariant(polozkaZastavka.y));
            }


            recordObject.insert("radius", QJsonValue::fromVariant(polozkaZastavka.radius));
            recordObject.insert("kapka", QJsonValue::fromVariant(polozkaZastavka.kapka ));
            recordObject.insert("color", QJsonValue::fromVariant(polozkaZastavka.barva ));
            radkyJSonArray.append(recordObject);

        }
        jPolozka.insert("data", radkyJSonArray);
        jSeznamPolozek.push_back(jPolozka);
    }

    jRoot.insert("polozky",jSeznamPolozek);
    jRoot.insert("popis",popis);


    QJsonDocument doc(jRoot);

    //   qDebug().noquote() << doc.toJson();

    qstringDoSouboru(mHtmlResultPath+"/data.js","var dataj='"+doc.toJson(QJsonDocument::Compact)+"';");

    QDesktopServices::openUrl(QUrl::fromLocalFile(mHtmlResultPath+"/mapa.html"));
}


QString MapaVykresleni::htmlTag(QString vstup, QString tag)
{
    return "<"+tag+">"+vstup+"</"+tag+">";
}




QString MapaVykresleni::getHtmlResultPath() const
{
    return mHtmlResultPath;
}




void MapaVykresleni::setHtmlResultPath(const QString &htmlResultPath)
{
    mHtmlResultPath = htmlResultPath;
}





void MapaVykresleni::qstringDoSouboru(QString cesta, QString obsah)
{
    qDebug()<<Q_FUNC_INFO;

    QFile data(cesta);
    if (data.open(QFile::WriteOnly | QFile::Truncate))
    {
        qDebug()<<"soubor se povedlo otevrit";
        QTextStream out(&data);



#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        //Qt5
        out.setCodec("UTF-8");
#else
        //Qt6
        out.setAutoDetectUnicode(true);
        out.setEncoding(QStringConverter::Utf8);
#endif

        out<<obsah;
    }
    else
    {
        qDebug()<<"soubor se nepovedlo otevrit";
    }
}
