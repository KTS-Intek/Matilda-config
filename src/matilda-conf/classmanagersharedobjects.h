#ifndef CLASSMANAGERSHAREDOBJECTS_H
#define CLASSMANAGERSHAREDOBJECTS_H

#include <QVariantHash>
#include <QStringList>

#include "src/meter/meteroperations.h"

class ClassManagerSharedObjects : public QObject
{

public:
    explicit ClassManagerSharedObjects(QObject *parent = nullptr) : QObject(parent){

    }




    struct MeterInfo
    {
        QString memo;
        QString crdnts;
        quint8 trff;
        QString model;
        QString vrsn;

        MeterInfo() {}

        MeterInfo(const QString &memo, const QString &crdnts, const quint8 &trff, const QString &model, const QString &vrsn) : memo(memo), crdnts(crdnts), trff(trff), model(model), vrsn(vrsn) {}
    };

    struct MeterSn2info{
        QString ni;

        MeterInfo minfo;
        MeterTransformer transformer;

        MeterSn2info() {}

        MeterSn2info(const QString &ni, const MeterInfo &minfo, const MeterTransformer &transformer) : ni(ni), minfo(minfo), transformer(transformer) {}

        MeterSn2info(const QString &ni, const QString &memo, const QString &crdnts, const quint8 &trff, const MeterTransformer &transformer, const QString &model, const QString &vrsn) : ni(ni), minfo(MeterInfo(memo, crdnts, trff, model, vrsn)), transformer(transformer) {}

        MeterSn2info(const QString &ni, const QString &memo, const QString &crdnts, const quint8 &trff, const bool &hasTransformer, const bool &only4display, const quint32 &ucDividend, const quint32 &ucDivisor, const quint32 &icDividend, const quint32 &icDivisor, const QString &model, const QString &vrsn) :
            ni(ni), minfo(MeterInfo(memo, crdnts, trff, model, vrsn)), transformer(MeterTransformer(hasTransformer, only4display, ucDividend, ucDivisor, icDividend, icDivisor)) {}

    };

    struct MeterNi2info{

        QString sn;
        MeterInfo minfo;
        MeterTransformer transformer;

        MeterNi2info() {}

        MeterNi2info(const QString &sn, const MeterInfo &minfo, const MeterTransformer &transformer) : sn(sn), minfo(minfo), transformer(transformer) {}

        MeterNi2info(const QString &sn, const QString &memo, const QString &crdnts, const quint8 &trff, const MeterTransformer &transformer, const QString &model, const QString &vrsn) : sn(sn), minfo(MeterInfo(memo, crdnts, trff, model, vrsn)), transformer(transformer) {}

        MeterNi2info(const QString &sn, const QString &memo, const QString &crdnts, const quint8 &trff, const bool &hasTransformer, const bool &only4display, const quint32 &ucDividend, const quint32 &ucDivisor, const quint32 &icDividend, const quint32 &icDivisor, const QString &model, const QString &vrsn) :
            sn(sn), minfo(MeterInfo(memo, crdnts, trff, model, vrsn)), transformer(MeterTransformer(hasTransformer, only4display, ucDividend, ucDivisor, icDividend, icDivisor)) {}

    };


    QString meterMemoFromCache(const QString &meterSn, const QString &meterNi){
        if(!meterSn.isEmpty() && hashMeterSn2info.contains(meterSn))
            return hashMeterSn2info.value(meterSn).minfo.memo;
        if(!meterNi.isEmpty() && hashMeterNi2info.contains(meterNi))
            return hashMeterNi2info.value(meterNi).minfo.memo;
        return "";
    }

    QString meterCoordinatesFromCache(const QString &meterSn, const QString &meterNi){
        if(!meterSn.isEmpty() && hashMeterSn2info.contains(meterSn))
            return hashMeterSn2info.value(meterSn).minfo.crdnts;
        if(!meterNi.isEmpty() && hashMeterNi2info.contains(meterNi))
            return hashMeterNi2info.value(meterNi).minfo.crdnts;
        return "";
    }

    quint8 getMeterTariff(const QString &meterSn, const QString &meterNi, const quint8 tariff = 2 ){
        quint8 t = tariff;
        if(!meterSn.isEmpty() && hashMeterSn2info.contains(meterSn))
            t = hashMeterSn2info.value(meterSn).minfo.trff;
        else if(!meterNi.isEmpty() && hashMeterNi2info.contains(meterNi))
            t = hashMeterNi2info.value(meterNi).minfo.trff;

        if(t < 1 || t > 4)
            t = tariff;


        if(t < 1 || t > 4)
            t = 2;

        return t;
    }

    qreal getTransformerCoefitient(const QString &meterSn, const QString &meterNi, const QString &enrgKey){

        bool foundVal = false;
        MeterTransformer transformer;

        if(!meterSn.isEmpty() && hashMeterSn2info.contains(meterSn)){
            transformer = hashMeterSn2info.value(meterSn).transformer;
            foundVal = true;
        }else if(!meterNi.isEmpty() && hashMeterNi2info.contains(meterNi)){
            transformer = hashMeterNi2info.value(meterNi).transformer;
            foundVal = true;
        }
        return (foundVal && transformer.hasTransformer) ? MeterOperations::getCoefitient4enrg(transformer, enrgKey) : 1.0;


    }




    void addMeterInfo(const QVariantHash &oneMeter){


        const MeterInfo minfo = MeterInfo(oneMeter.value("memo").toString(), oneMeter.value("crdnts").toString(), oneMeter.value("trff").toUInt(), oneMeter.value("model").toString(), oneMeter.value("vrsn").toString());
        const MeterTransformer transformer = MeterOperations::transformerSettFromLine(oneMeter.value("tc").toString());

        const QString ni = oneMeter.value("NI").toString();
        const QString sn = oneMeter.value("SN").toString();


        if(!sn.isEmpty())
            addMeter2meterSn2info(sn, ni, minfo, transformer);

        addMeter2meterNi2info(ni, sn, minfo, transformer);
    }

    void addMeter2meterSn2info(const QString &sn, const QString &memo, const QString &ni, const QString &crdnts, const quint8 &trff, const bool &hasTransformer, const bool &only4display, const quint32 &ucDividend, const quint32 &ucDivisor, const quint32 &icDividend, const quint32 &icDivisor, const QString &model, const QString &vrsn){
        addMeter2meterSn2info(sn, ni, MeterInfo(memo, crdnts, trff, model, vrsn), MeterTransformer(hasTransformer, only4display, ucDividend, ucDivisor, icDividend, icDivisor));
    }

    void addMeter2meterSn2info(const QString &sn, const QString &memo, const QString &ni, const QString &crdnts, const quint8 &trff, const MeterTransformer &transformer, const QString &model, const QString &vrsn){
        addMeter2meterSn2info(sn, ni, MeterInfo(memo, crdnts, trff, model, vrsn), transformer);
    }

    void addMeter2meterSn2info(const QString &sn, const QString &ni, const MeterInfo &minfo, const MeterTransformer &transformer){
        if(sn.isEmpty())
            return;

        hashMeterSn2info.insert(sn, MeterSn2info(ni, minfo, transformer));
        if(!minfo.memo.isEmpty())
            hashMeterSn2memo.insert(sn, minfo.memo);
        if(!ni.isEmpty())
            hashMeterSn2ni.insert(sn,ni);
    }

    void addMeter2meterNi2info(const QString &ni, const QString &memo, const QString &sn, const QString &crdnts, const quint8 &trff, const bool &hasTransformer, const bool &only4display, const quint32 &ucDividend, const quint32 &ucDivisor, const quint32 &icDividend, const quint32 &icDivisor, const QString &model, const QString &vrsn){
        addMeter2meterNi2info(ni, sn, MeterInfo(memo, crdnts, trff, model, vrsn), MeterTransformer(hasTransformer, only4display, ucDividend, ucDivisor, icDividend, icDivisor));
    }

    void addMeter2meterNi2info(const QString &ni, const QString &memo, const QString &sn, const QString &crdnts, const quint8 &trff, const MeterTransformer &transformer, const QString &model, const QString &vrsn){
        addMeter2meterNi2info(ni, sn, MeterInfo(memo, crdnts, trff, model, vrsn), transformer);
    }

    void addMeter2meterNi2info(const QString &ni, const QString &sn, const MeterInfo &minfo, const MeterTransformer &transformer){
        if(ni.isEmpty())
            return;
        if(!listnis.contains(ni))
            listnis.append(ni);
        hashMeterNi2info.insert(ni, MeterNi2info(sn, minfo, transformer));

        if(!minfo.memo.isEmpty())
            hashMeterNi2memo.insert(ni,minfo.memo);
    }

    void clear(){
        hashMeterSn2info.clear();
        hashMeterNi2info.clear();
        listnis.clear();
    }

    void clearWrite(){
        hashMemoWrite.clear();
        lastTableList.clear();
        totalTables = 0;
        doneTables = 0;
        blockDone = 0;
        lastSnMemoHash.clear();
        commandCounter = 0;

    }


    QHash<QString, MeterSn2info> hashMeterSn2info;
    QHash<QString, MeterNi2info> hashMeterNi2info;


    QHash<QString,QString> hashMeterSn2memo, hashMeterNi2memo, hashMeterSn2ni, hashLampNi2memo/*, hashMeterNi2vrsn*/;
    QStringList listnis;

    QHash<QString,QStringList> hashIp2Ids;

    QVariantHash lastPairSn2meterInfo;
    int komaPos, lastMemoPos, lastSnIndx, lastNiIndx, lampOfflineAfterMinutes;
    bool lastDateIsMsec;
    int timeOutMsec, lastColumnListSize;
    QString dateMask;
    QStringList lastTableList;
    int totalTables, doneTables, blockDone;
    QVariantHash lastSnMemoHash;
    QHash<int, QVariantHash> hashMemoWrite;

    quint32 commandCounter;

    quint16 commandSequenceAfter;
    QVariant dataSequenceAfter;

    QStringList lastHeaderData;
    bool isWaiting4header;

    quint8 lastPollCode;
};

#endif // CLASSMANAGERSHAREDOBJECTS_H
