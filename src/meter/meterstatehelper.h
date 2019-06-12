#ifndef METERSTATEHELPER_H
#define METERSTATEHELPER_H

#include <QStringList>
#include <QVariantHash>
#include <QMap>


typedef QMap<QString,QString> MapString;

class MeterStateHelper
{

public:
    static QStringList getMeterStateHeaderKeys(const bool &addMemo);

    static QStringList getWaterMeterStateHeaderKeys(const bool &addMemo);


    static QStringList getAllKeys4table();

    static QStringList getKeys4table();

    static QStringList getEngrKeys4table();


    static QStringList convert2humanReadable(const MapString &map, const QStringList &lk);

    static QStringList saldoValueKeys();

    static QStringList saldoParamKeys();


    static QStringList waterBatSttsKeys();

    static QStringList waterSleepParamKeys();


    static QString saldoValue2line(const QVariantHash &saldoValue);

    static QString saldoParam2line(const QVariantHash &saldoParam);


    static QString waterBatStts2line(const QVariantHash &batstts);

    static QString waterSleepParam2line(const QVariantHash &sleepprofile);



    static QVariantHash line2hash(const QString &line, const QStringList &lk);

    static QVariantHash saldoValueLine2hash(const QString &line);

    static QVariantHash saldoParamLine2hash(const QString &line);

    static QVariantHash waterVlsLine2hash(const QString &line);

    static QVariantHash waterPrmsLine2hash(const QString &line);

};

#endif // METERSTATEHELPER_H
