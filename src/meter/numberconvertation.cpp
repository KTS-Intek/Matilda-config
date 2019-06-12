#include "numberconvertation.h"

#ifdef STANDARD_METER_PLUGIN
#include "moji_defy.h"
#else
#include "src/matilda/matildalimits.h"
#endif

//-------------------------------------------------------------------------------------------------------------------------------------
QString NumberConvertation::prettyNumber(const qreal &val, int mPrec)
{
    return prettyNumber(val, mPrec, MAX_PRECISION);

}
//-------------------------------------------------------------------------------------------------------------------------------------
QString NumberConvertation::validateCoordinates(const QString &instr, bool &ok)
{
    ok = false;
    QString s = instr.simplified().trimmed();

    if(s.isEmpty()){
        ok = true;
        return s;
    }
    const QStringList l = s.split(",", QString::SkipEmptyParts);
    s.clear();
    if(l.size() == 2){
        bool oklat ,oklon;
        qreal lat = l.first().toFloat(&oklat);
        qreal lon = l.last().toFloat(&oklon);

        if(oklat && oklon && lat <= 90.0 && lat >= (-90.0)){
            quint32 cntr = 0, cntrMax = 0xFFFFFF;
            if(lon > 180.0){
                while(lon > 180.0 && cntr < cntrMax){
                    lon -= 360;
                    cntr++;
                }
            }else if(lon < -180.0){
                while(lon < -180.0 && cntr < cntrMax){
                    lon += 360;
                    cntr++;
                }
            }
            if(cntr < cntrMax){
                s = QString::number(lat, 'f', 6).replace(",", ".") + "," + QString::number(lon, 'f', 6).replace(",", ".");
                ok = true;
            }
        }
    }
    return s;
}
//-------------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------------
QString NumberConvertation::prettyNumber(const qreal &val, int mPrec, const int &maxPlgPrec)
{
    mPrec = qMin(qMax(mPrec, 1), maxPlgPrec);

    QString s = QString::number(val, 'f', mPrec);
    s = s.replace(",", ".");
    if(s.isEmpty())
        return s;
    while(s.right(1) == "0")
        s.chop(1);

    if(s.right(1) == ".")
        s.chop(1);

    if(s == "0." || s.isEmpty())
        return "0";

    return s;
}
//-------------------------------------------------------------------------------------------------------------------------------------
QString NumberConvertation::prettyLongNumber(const QString &s)
{
    bool ok;
    int val = s.toInt(&ok);

    return ok ? prettyLongNumber(val) : s;
}
//-------------------------------------------------------------------------------------------------------------------------------------
QString NumberConvertation::prettyLongNumber(const int &v)
{
    QString ss = QString::number(v);
    QStringList l;
    for(int i = 0, iMax = ss.length(); i < iMax; i += 3){
        l.prepend(ss.right(3));
        ss.chop(3);
    }
    if(!ss.isEmpty())
        l.prepend(ss);
    return l.join(" ");
}
//-------------------------------------------------------------------------------------------------------------------------------------
QString NumberConvertation::prettyBabloNumber(const QString &s)
{
    bool ok;
    s.toDouble(&ok);
    QString r = s;
    if(ok){
        r.clear();
        QString ss = s;
        int chopSize = 3;
        if(ss.contains("."))
            chopSize += ss.length() - ss.indexOf(".") ;
        else if(ss.contains(","))
            chopSize += ss.length() - ss.indexOf(",") ;

        QStringList l;
        for(int i = 0, iMax = ss.length(); i < iMax; i += chopSize, chopSize = 3){
            l.prepend(ss.right(chopSize));
            ss.chop(chopSize);
        }
        if(!ss.isEmpty())
            l.prepend(ss);
        r = l.join(" ");
    }
    return r;
}

//-------------------------------------------------------------------------------------------------------------------------------------

QList<quint8> NumberConvertation::convert2uint8(const QStringList &l)
{
    QList<quint8> r;
    for(int i = 0, imax = l.size(); i < imax; i++){
        bool ok;
        const quint8 v = l.at(i).toUInt(&ok);
        if(ok)
            r.append(v);
    }
    return r;
}
//-------------------------------------------------------------------------------------------------------------------------------------
QStringList NumberConvertation::convertFromUint8(const QList<quint8> &l)
{
    QStringList r;
    for(int i = 0, imax = l.size(); i < imax; i++)
        r.append(QString::number(l.at(i)));
    return r;
}
//-------------------------------------------------------------------------------------------------------------------------------------
int NumberConvertation::calcTMsecCheck(int tMSec, bool &tMSecCheck)
{
    tMSecCheck = true;
    if(tMSec < 1){
        tMSec = 0xFFFFFFF;//268 435 455 ms   4 473,92425 sec
        tMSecCheck = false;
    }else{
        if(tMSec < 700)
            tMSec = 700;
    }
    return tMSec;
}
//-------------------------------------------------------------------------------------------------------------------------------------
