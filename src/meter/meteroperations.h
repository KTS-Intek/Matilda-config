#ifndef METEROPERATIONS_H
#define METEROPERATIONS_H

#include <QStringList>


struct MeterTransformer{
    bool hasTransformer;
    bool only4display;
    quint32 ucDividend;
    quint32 ucDivisor;
    quint32 icDividend;
    quint32 icDivisor;

    qreal endUcoefitient;
    qreal endIcoefitient;
    qreal endCoefitient;

    MeterTransformer() : hasTransformer(false) {}

    MeterTransformer(const bool &hasTransformer) :
        hasTransformer(hasTransformer), only4display(true), ucDividend(1), ucDivisor(1), icDividend(1), icDivisor(1), endCoefitient(1) {}

    MeterTransformer(const bool &hasTransformer, const bool &only4display, const quint32 &ucDividend, const quint32 &ucDivisor, const quint32 &icDividend, const quint32 &icDivisor) :
    hasTransformer(hasTransformer), only4display(only4display), ucDividend(ucDividend), ucDivisor(ucDivisor), icDividend(icDividend), icDivisor(icDivisor),
      endUcoefitient(hasTransformer ? (qreal)((qreal)ucDividend/(qreal)ucDivisor) : 1.0 ), endIcoefitient(hasTransformer ? (qreal)((qreal)icDividend/(qreal)icDivisor) : 1.0 ),
      endCoefitient( hasTransformer ? (endUcoefitient * endIcoefitient) : 1.0 ) {}
};

class MeterOperations
{

public:


    static QStringList getKeys4metersByProtocol(const int &protocolVersion);

    static QStringList getKeys4metersByProtocolWoVrsn(const int &protocolVersion);

    static MeterTransformer transformerSettFromLine(const QString &line);

    static QString transformerSett2line(const MeterTransformer &trans);

    static qreal getCoefitient4enrg(const MeterTransformer &transformer, const QString &enrgKey);

    static qreal getCoefitient4enrgPlg(const MeterTransformer &transformer, const QString &enrgKey);

};


#endif // METEROPERATIONS_H
