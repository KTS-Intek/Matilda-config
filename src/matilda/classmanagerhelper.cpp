#include "classmanagerhelper.h"
#include "showmesshelper4wdgt.h"
#include "src/meter/meterstatehelper.h"
#include "src/meter/definedpollcodes.h"
#include <QJsonObject>
#include <QJsonDocument>

//-----------------------------------------------------------------------------------------------------

QStringList ClassManagerHelper::restoreList(const QStringList &inList, const int &columnListSize, const int &restoreFromLine)
{
    //відновлюю нормальний вигляд статусів, так проще)
    QStringList list2;
    for(int i = 0, iMax = inList.size(); i < iMax; i++){
        const QString s = inList.at(i);
        if(i < restoreFromLine){
            list2.append(s);
            continue;
        }

        const int jMax = s.length();

        bool okminus = (s.left(1) == "-");
        if(okminus)
            s.toDouble(&okminus);//щоб число не попало

        if(!okminus && (i < 1 || s.contains(":")))
            okminus = true;

        if((s.contains("!") || s.contains("?") || (s.contains("-") && !okminus)) && jMax > 1){
            for(int j = 0; j < jMax; j++ )
                list2.append(s.mid(j,1));
        }else{
            list2.append(s);
        }
    }
    for(int i = list2.size(); i < columnListSize; i++)
        list2.append("-");

    return list2;
}

//-----------------------------------------------------------------------------------------------------

Data2listOutSett ClassManagerHelper::addData2list(QVariantList &listOneMeter, ClassManagerSharedObjects *shrdObj, QVariantHash &hashRowCol2varData, const QStringList &list,
                                      const int &columnListSize, const Data2listInputSett &d2linput, const int &rowCounter)
{
    Data2listOutSett outsett;
    for(int i = 0; i < columnListSize; i++){
        const QString str = list.at(i);

        if(i == d2linput.meterSnIndx){
            outsett.meterSN = str;
        }else{
            if(i == d2linput.niIndx)
                outsett.meterNi = str;
        }

        if(i == 0 && d2linput.itIsGeliks){ //dateTime index

            bool okd = true;
            const QDateTime dateTM = shrdObj->lastDateIsMsec ? QDateTime::fromMSecsSinceEpoch(str.toLongLong(&okd)) :
                                                               ShowMessHelper4wdgt::tableName2DateTime(str,"yyyy-MM-dd", "hh:mm:ss");// QDateTime::fromString(str,dateTimeMask);

            QVariantList lDate;
            lDate.append((dateTM.isValid() && okd) ? dateTM.toLocalTime().toString(d2linput.dateTimeMask) : ("UTC " + str));
            lDate.append(dateTM);
            listOneMeter.append( QVariant(lDate) );

            continue;
        }

        if(i == shrdObj->lastMemoPos)
            listOneMeter.append(shrdObj->meterMemoFromCache(outsett.meterSN, outsett.meterNi));


        if(i < d2linput.colFrom){
            listOneMeter.append(str);
            continue;
        }


        const qreal coefitient = shrdObj->getTransformerCoefitient(outsett.meterSN, "", shrdObj->lastHeaderData.at(i));

        if(shrdObj->komaPos >= 0 || coefitient > 1.0){
            if(str.contains("];[")){
                const QStringList ll = str.mid(0, str.length() - 1).mid(1).split("];[");
                bool okl, okr;

                const qreal vl = ll.first().toDouble(&okl) * coefitient;
                const qreal vr = ll.last().toDouble(&okr) * coefitient;

                if(okl && okr){
                    hashRowCol2varData.insert(QString("%1;%2").arg(rowCounter).arg(i + d2linput.add4memoAndIntrvl), str);//+1 memo col
                    listOneMeter.append((shrdObj->komaPos >= 0) ? QString("[%1];[%2]")
                                                                  .arg(QString::number(vl, 'f', shrdObj->komaPos))
                                                                  .arg(QString::number(vr, 'f', shrdObj->komaPos))
                                                                :  QString("[%1];[%2]")
                                                                  .arg(QString::number(vl))
                                                                  .arg(QString::number(vr)));
                    continue;
                }
            }else{
                bool ok;
                const qreal v = str.toDouble(&ok) * coefitient;
                if(ok){
                    hashRowCol2varData.insert(QString("%1;%2").arg(rowCounter).arg(i + d2linput.add4memoAndIntrvl), v);//+1 memo col
                    listOneMeter.append((shrdObj->komaPos >= 0) ? QString::number(v, 'f', shrdObj->komaPos) : QString::number(v));
                    continue;
                }
            }
        }
        listOneMeter.append(str);
    }
    return outsett;
}

//-----------------------------------------------------------------------------------------------------

Data2listOutSett ClassManagerHelper::addData2listState(QVariantList &listOneMeter, ClassManagerSharedObjects *shrdObj, QVariantHash &hashRowCol2varData, const QStringList &list, const int &columnListSize, const Data2listInputSett &d2linput, const int &rowCounter, const quint8 &pollCode, const bool &hasModelLine)
{
    // msec meter_sn meter_ni relay deg vls prm
    // must be columnList = QString("date_time meter_sn meter_ni memo relay deg balance last_charge balance_charge tarif_values params").split(" ", QString::SkipEmptyParts);
    //date_time meter_sn meter_ni relay deg balance last_charge balance_charge balance_counter tarif_values params
    Data2listOutSett outsett;
    const int maxMinus = (pollCode == POLL_CODE_READ_METER_STATE ) ? 4 : ((hasModelLine) ? 3 : 1);
    const int indxPlus = hasModelLine ? 1 : 0;
    for(int i = 0, iMax = columnListSize - maxMinus; i < columnListSize && i < iMax; i++){
        const QString str = list.at(i);

        if(i == d2linput.meterSnIndx){
            outsett.meterSN = str;
        }else{
            if(i == d2linput.niIndx)
                outsett.meterNi = str;
        }

        if(i == 0 && d2linput.itIsGeliks){ //dateTime index

            bool okd = true;
            const QDateTime dateTM = QDateTime::fromMSecsSinceEpoch(str.toLongLong(&okd)) ;

            QVariantList lDate;
            lDate.append((dateTM.isValid() && okd) ? dateTM.toLocalTime().toString(d2linput.dateTimeMask) : ("UTC " + str));
            lDate.append(dateTM);
            listOneMeter.append( QVariant(lDate) );

            continue;
        }

        if(i == shrdObj->lastMemoPos)
            listOneMeter.append(shrdObj->meterMemoFromCache(outsett.meterSN, outsett.meterNi));


        if(i < d2linput.colFrom || i < 4){
            listOneMeter.append(str);
            continue;
        }

        switch(pollCode){
        case POLL_CODE_READ_METER_STATE:{
            switch(i){
            case 4: listOneMeter.append( (str.toInt()) ? QObject::tr("Load On") : QObject::tr("Load Off")); hashRowCol2varData.insert(QString("%1;%2").arg(rowCounter).arg(i + d2linput.add4memoAndIntrvl), str); break; //relay
            case 5: listOneMeter.append(str); break; //deg
            case 6: addVls2listPretty(listOneMeter, str, d2linput.dateTimeMask)         ; break;//values

            default:{
                bool ok;
                const qreal v = str.toDouble(&ok) ;
                if(ok){
                    hashRowCol2varData.insert(QString("%1;%2").arg(rowCounter).arg(i + d2linput.add4memoAndIntrvl), v);//+1 memo col
                    listOneMeter.append((shrdObj->komaPos >= 0) ? QString::number(v, 'f', shrdObj->komaPos) : QString::number(v));
                    continue;
                }

                listOneMeter.append(str); break;}
            }

            break;}

        case POLL_CODE_WTR_METER_STATE:{
            switch(i - indxPlus){
            case 4: listOneMeter.append(str); break; //deg
            case 5: addWaterVls2listPretty(listOneMeter, str)         ; break;//values
            case 6: addWaterPrm2listPretty(listOneMeter, str)         ; break;//values
            case 7: break;
            default:{
                bool ok;
                const qreal v = str.toDouble(&ok) ;
                if(ok){
                    hashRowCol2varData.insert(QString("%1;%2").arg(rowCounter).arg(i + d2linput.add4memoAndIntrvl), v);//+1 memo col
                    listOneMeter.append((shrdObj->komaPos >= 0) ? QString::number(v, 'f', shrdObj->komaPos) : QString::number(v));
                    continue;
                }

                listOneMeter.append(str); break;}
            }

            break;}

        }

    }
    return outsett;
}

//-----------------------------------------------------------------------------------------------------

void ClassManagerHelper::addVls2listPretty(QVariantList &listOneMeter, const QString &str, const QString &datemask)
{
    //    return QString("lastSaldoDate currentSaldo lastSaldoWrite saldoCounter saldoEnergyT1 saldoEnergyT2 saldoEnergyT3 saldoEnergyT4").split(" ", QString::SkipEmptyParts);

    const QVariantHash h = MeterStateHelper::saldoValueLine2hash(str);
    listOneMeter.append(h.value("currentSaldo").toString());
    const QDateTime dt = h.value("lastSaldoDate").toDateTime().toLocalTime();
    listOneMeter.append(dt.isValid() ? dt.toString(datemask) : "?");
    listOneMeter.append(h.value("lastSaldoWrite").toString());
    listOneMeter.append(h.value("saldoCounter").toString());

    QStringList l;
    for(int i = 1; i < 5; i++)
        l.append(h.value(QString("saldoEnergyT%1").arg(i)).toString());

    listOneMeter.append(l.join(" "));


}

//-----------------------------------------------------------------------------------------------------

void ClassManagerHelper::addWaterVls2listPretty(QVariantList &listOneMeter, const QString &str)
{
    const QVariantHash h = MeterStateHelper::waterVlsLine2hash(str);
//return QString("mmb emb mvb evb").split(" ", QString::SkipEmptyParts);
    listOneMeter.append(h.value("mmb").toString());
    listOneMeter.append(h.value("emb").toString());
    listOneMeter.append(h.value("mvb").toString());
    listOneMeter.append(h.value("evb").toString());

}

//-----------------------------------------------------------------------------------------------------

void ClassManagerHelper::addWaterPrm2listPretty(QVariantList &listOneMeter, const QString &str)
{
    //return QString("dow dom hour minute actvt").split(" ", QString::SkipEmptyParts);
    const QVariantHash h = MeterStateHelper::waterPrmsLine2hash(str);
    listOneMeter.append(QString(QJsonDocument(QJsonObject::fromVariantHash(h)).toJson(QJsonDocument::Compact)));
}

//-----------------------------------------------------------------------------------------------------

FramedHashAnswr ClassManagerHelper::readFromHashFramed(const QVariantHash &hash)
{
    //i - indx, -1 endOfList or not exists
    //t - total
    //s - stringlist split by "|"
    //c - header (columns)

    MyListStringList list;

    const QStringList l = hash.value("s").toStringList();

    bool need2correct = false;
    int maxCol = 1;
    for(int i = 0, imax = l.size(); i < imax; i++){
        const QStringList ll = l.at(i).split("|");
        if(ll.isEmpty())
            continue;
        if(!need2correct){
            const int c =  ll.size();
            maxCol = qMax(c, maxCol);
            if(maxCol != c)
                need2correct = true;
        }
        list.append(ll);
    }

    QString mess;
    if(need2correct){
        mess.append(QObject::tr("Bad columns("));
        for(int i = 0, imax = list.size(); i < imax; i++){
            QStringList l = list.at(i);
            for(int c = l.size(); c < maxCol; c++)
                l.append("?");
            list.replace(i, l);
        }
    }

    const QStringList header = hash.contains("c") ? hash.value("c").toString().split("|", QString::SkipEmptyParts) : QStringList();
    return FramedHashAnswr(list, QVariantMap(), hash.value("t", 0).toInt(), hash.value("i", -1).toInt(), header, need2correct, mess);


}

QVariantHash ClassManagerHelper::onReadFramedHash(const int &lastIndx)
{
    QVariantHash h;
    h.insert("i", lastIndx);
    return h;
}
//-----------------------------------------------------------------------------------------------------
