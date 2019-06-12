#include "meterstatehelper.h"

#include "src/meter/numberconvertation.h"
#include <QDateTime>
#include <QDebug>

//----------------------------------------------------------------------------

QStringList MeterStateHelper::getMeterStateHeaderKeys(const bool &addMemo)
{
    return QString("date_time meter_model meter_sn meter_ni %1relay deg balance last_charge balance_charge balance_counter tarif_values params").arg( addMemo ? "memo " : "").split(" ", QString::SkipEmptyParts);
}

QStringList MeterStateHelper::getWaterMeterStateHeaderKeys(const bool &addMemo)
{
    return QString("date_time meter_model meter_sn meter_ni %1relay deg meter_bat modem_bat meter_bat_v modem_bat_v profile_params").arg( addMemo ? "memo " : "").split(" ", QString::SkipEmptyParts);

}

//----------------------------------------------------------------------------

QStringList MeterStateHelper::getAllKeys4table()
{
  return QString("msec meter_sn meter_ni " + getKeys4table().join(" ")).split(" ", QString::SkipEmptyParts);
}

//----------------------------------------------------------------------------

QStringList MeterStateHelper::getKeys4table()
{
    return QString("model " + getEngrKeys4table().join(" ")).split(" ", QString::SkipEmptyParts);
}
//----------------------------------------------------------------------------
QStringList MeterStateHelper::getEngrKeys4table()
{
    return QString("relay deg vls prm etc").split(" ", QString::SkipEmptyParts);

}

//----------------------------------------------------------------------------

QStringList MeterStateHelper::convert2humanReadable(const MapString &map, const QStringList &lk)
{
    QStringList l;
    l.append(QDateTime::fromMSecsSinceEpoch(map.value("msec").toLongLong()).toString("yyyy-MM-dd hh:mm:ss"));

    for(int i = 1, iMax = lk.size(); i < iMax; i++)
        l.append(map.value(lk.at(i)));
//    qDebug() << "human data " << l;
    return l;
}

//----------------------------------------------------------------------------


QStringList MeterStateHelper::saldoValueKeys()
{
    return QString("lastSaldoDate currentSaldo lastSaldoWrite saldoCounter saldoEnergyT1 saldoEnergyT2 saldoEnergyT3 saldoEnergyT4").split(" ", QString::SkipEmptyParts);
}

//----------------------------------------------------------------------------


QStringList MeterStateHelper::saldoParamKeys()
{
    return QString("creditAfter doNotOffRelayAfter doNotOffRelayBefore doNotOffRelayWeekend dotIndication hourOffRelayAfter hourOffRelayBefore offRelayLoad priceT1 priceT2 priceT3 priceT4 saldoEnabled saldoPowerLimit showSaldoAfter").split(" ", QString::SkipEmptyParts);
}
//----------------------------------------------------------------------------
QStringList MeterStateHelper::waterBatSttsKeys()
{
    return QString("mmb emb mvb evb").split(" ", QString::SkipEmptyParts);
}
//----------------------------------------------------------------------------
QStringList MeterStateHelper::waterSleepParamKeys()
{
    return QString("dow dom hour minute actvt").split(" ", QString::SkipEmptyParts);
}

//----------------------------------------------------------------------------


QString MeterStateHelper::saldoValue2line(const QVariantHash &saldoValue)
{
    const QDateTime dt = saldoValue.value("lastSaldoDate").toDateTime().toUTC();
    if(!dt.isValid())
        return "";

    QStringList l;
    l.append(QString::number(dt.toMSecsSinceEpoch()/1000L)); //toSecsSinceEpoch
    const QStringList lk = saldoValueKeys();

    for(int i = 1, iMax = lk.size(); i < iMax; i++){
        const QString s = NumberConvertation::prettyNumber(saldoValue.value(lk.at(i)).toReal(), 3);
        if(s.isEmpty())
            return "";
        l.append(s);
    }
    return l.join(" ");

}

//----------------------------------------------------------------------------


QString MeterStateHelper::saldoParam2line(const QVariantHash &saldoParam)
{
    QStringList l;
    const QStringList lk = saldoParamKeys();

    for(int i = 0, iMax = lk.size(); i < iMax; i++){
        const QString s = NumberConvertation::prettyNumber(saldoParam.value(lk.at(i)).toReal(), 3);
        if(s.isEmpty())
            return "";
        l.append(s);
    }
    return l.join(" ");
}

//----------------------------------------------------------------------------

QString MeterStateHelper::waterBatStts2line(const QVariantHash &batstts)
{
    QStringList l;
    const QStringList lk = waterBatSttsKeys();//    return QString("mmb emb mvb evb").split(" ", QString::SkipEmptyParts);

    for(int i = 0, iMax = lk.size(); i < iMax; i++){
        const QString s = NumberConvertation::prettyNumber(batstts.value(lk.at(i)).toReal(), 3);
        if(s.isEmpty())
            break;
        l.append(s);
    }
    return l.join(" ");
}

//----------------------------------------------------------------------------

QString MeterStateHelper::waterSleepParam2line(const QVariantHash &sleepprofile)
{
    //sleepprofile
    QStringList l;
    const QStringList lk = waterSleepParamKeys();
    for(int i = 0, iMax = lk.size(); i < iMax; i++){
        const QString s = sleepprofile.value(lk.at(i)).toString();
        if(s.isEmpty())
            return "";
        l.append(s);
    }
    return l.join(" ");
}

//----------------------------------------------------------------------------

QVariantHash MeterStateHelper::line2hash(const QString &line, const QStringList &lk)
{
    QVariantHash h;
    const QStringList l = line.split(" ", QString::SkipEmptyParts);

    const int iMax = lk.size();
    const int iMax2 = l.size();

    for(int i = 0; i < iMax && i < iMax2; i++)
        h.insert(lk.at(i), l.at(i));
    return h;
}

//----------------------------------------------------------------------------

QVariantHash MeterStateHelper::saldoValueLine2hash(const QString &line)
{
    QVariantHash h = line2hash(line, saldoValueKeys());
    h.insert("lastSaldoDate", QDateTime::fromMSecsSinceEpoch(h.value("lastSaldoDate").toLongLong() * 1000L));
    return h;
}

//----------------------------------------------------------------------------

QVariantHash MeterStateHelper::saldoParamLine2hash(const QString &line)
{
    return line2hash(line, saldoParamKeys());
}
//----------------------------------------------------------------------------
QVariantHash MeterStateHelper::waterVlsLine2hash(const QString &line)
{
    return line2hash(line, waterBatSttsKeys());

}

//----------------------------------------------------------------------------

QVariantHash MeterStateHelper::waterPrmsLine2hash(const QString &line)
{
    return line2hash(line, waterSleepParamKeys());
}

//----------------------------------------------------------------------------

