#include "meteroperations.h"

#include "moji_defy.h"



QStringList MeterOperations::getKeys4metersByProtocol(const int &protocolVersion)
{
    QString s;

    if(protocolVersion > MATILDA_PROTOCOL_VERSION_V2)
    s.prepend("pwrn,tc,dta,");

    if(protocolVersion > MATILDA_PROTOCOL_VERSION_V1)
        s.prepend("crdnts,");

    s.prepend("model,NI,SN,memo,passwd,on,politic,trff,vrsn,");
    s.chop(1);

    return s.split(",");
}

QStringList MeterOperations::getKeys4metersByProtocolWoVrsn(const int &protocolVersion)
{
    QStringList l = getKeys4metersByProtocol(protocolVersion);
    l.removeOne("vrsn");
    return l;
}

MeterTransformer MeterOperations::transformerSettFromLine(const QString &line)
{
    const QStringList l = line.split(" ", QString::SkipEmptyParts);

    int ucDividend = 1, ucDivisor = 1, icDividend = 1, icDivisor = 1;
    bool only4disp = true, enTransformer = false;

    if(l.size() >= 2){
        enTransformer = true;
        only4disp = (l.last() == "*");

        ucDividend = l.at(0).split(":").first().toInt();
        ucDivisor = l.at(0).split(":").last().toInt();

        icDividend = l.at(1).split(":").first().toInt();
        icDivisor = l.at(1).split(":").last().toInt();
    }
    return MeterTransformer(enTransformer, only4disp, ucDividend, ucDivisor, icDividend, icDivisor);
}

QString MeterOperations::transformerSett2line(const MeterTransformer &trans)
{

    return trans.hasTransformer ? QString("%1:%2 %3:%4%5")
                                                                  .arg(trans.ucDividend)
                                                                  .arg(trans.ucDivisor)
                                                                  .arg(trans.icDividend)
                                                                  .arg(trans.icDivisor)
                                                                  .arg(trans.only4display ? " *" : "") : "";
}

qreal MeterOperations::getCoefitient4enrg(const MeterTransformer &transformer, const QString &enrgKey)
{
    qreal t = 1.0;
    if(enrgKey.startsWith("U")){
        t = transformer.endUcoefitient;

    }else{
        if(enrgKey.startsWith("I")){
            t = transformer.endIcoefitient;

        }else{
            if(enrgKey.startsWith("A") || enrgKey.startsWith("R") || enrgKey.startsWith("T") || enrgKey.startsWith("P") || enrgKey.startsWith("Q")){
                t = transformer.endCoefitient;
            }
        }
    }
    return (t > 0.0) ? t : 1.0;
}

qreal MeterOperations::getCoefitient4enrgPlg(const MeterTransformer &transformer, const QString &enrgKey)
{
    return ((transformer.hasTransformer && !transformer.only4display) ? getCoefitient4enrg(transformer, enrgKey) : 1.0);
}
