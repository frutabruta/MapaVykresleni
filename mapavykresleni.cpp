#include "mapavykresleni.h"



MapaVykresleni::MapaVykresleni()
{

}



void MapaVykresleni::pridejMnozinu(QVector<MapaBod> seznamBodu , bool vykresliBody, bool vykresliSpojnici, bool vykresliTrasu, bool vykresliRadius, MnozinaBodu::SouradnicovySystem souradnicovySystem)
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
        if(polozkaMnozina.souradnicovySystem==MnozinaBodu::J_STSK)
        {
            jPolozka.insert("souradnicovySystem", QJsonValue::fromVariant("J_STSK"));
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
            recordObject.insert("lat", QJsonValue::fromVariant(polozkaZastavka.lat));
            recordObject.insert("lng", QJsonValue::fromVariant(polozkaZastavka.lng));
            recordObject.insert("radius", QJsonValue::fromVariant(polozkaZastavka.radius));
            recordObject.insert("kapka", QJsonValue::fromVariant(polozkaZastavka.kapka ));
            radkyJSonArray.append(recordObject);

        }
        jPolozka.insert("data", radkyJSonArray);
        jSeznamPolozek.push_back(jPolozka);
    }

    jRoot.insert("polozky",jSeznamPolozek);
    jRoot.insert("popis",popis);


    QJsonDocument doc(jRoot);

    //   qDebug().noquote() << doc.toJson();

    qstringDoSouboru(cestaMapa+"/data.js","var dataj='"+doc.toJson(QJsonDocument::Compact)+"';");

    QDesktopServices::openUrl(QUrl::fromLocalFile(cestaMapa+"/mapa.html"));
}


QString MapaVykresleni::htmlTag(QString vstup, QString tag)
{
    return "<"+tag+">"+vstup+"</"+tag+">";
}




QString MapaVykresleni::getCestaMapa() const
{
    return cestaMapa;
}




void MapaVykresleni::setCestaMapa(const QString &newCestaMapa)
{
    cestaMapa = newCestaMapa;
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
