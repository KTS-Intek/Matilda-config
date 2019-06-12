#ifndef NUMBERCONVERTATION_H
#define NUMBERCONVERTATION_H

#include <QStringList>
#include <QVariantHash>


class NumberConvertation
{

public:

    static QString prettyNumber(const qreal &val, int mPrec) ;

     static QString validateCoordinates(const QString &instr, bool &ok);


     static QString prettyNumber(const qreal &val, int mPrec, const int &maxPlgPrec) ;

     static QString prettyLongNumber(const QString &s) ;

     static QString prettyLongNumber(const int &v) ;

     static QString prettyBabloNumber(const QString &s) ;

     static QList<quint8> convert2uint8(const QStringList &l);

     static QStringList convertFromUint8(const QList<quint8> &l);

     static int calcTMsecCheck(int tMSec, bool &tMSecCheck);

};

#endif // NUMBERCONVERTATION_H
