#ifndef CLASSMANAGERHELPER_H
#define CLASSMANAGERHELPER_H

#include <QStringList>
#include <QVariantList>
#include "src/matilda-conf/classmanagersharedobjects.h"
#include "src/matilda/classmanagertypes.h"

struct Data2listInputSett
{
    int meterSnIndx;
    int niIndx;
    bool itIsGeliks;
    QString dateTimeMask;
    int colFrom;
    int add4memoAndIntrvl;

    Data2listInputSett() {}

    Data2listInputSett(const int &meterSnIndx, const int &niIndx, const bool &itIsGeliks, const QString &dateTimeMask, const int &colFrom, const int &add4memoAndIntrvl)
        : meterSnIndx(meterSnIndx), niIndx(niIndx), itIsGeliks(itIsGeliks), dateTimeMask(dateTimeMask), colFrom(colFrom), add4memoAndIntrvl(add4memoAndIntrvl) {}

};


struct Data2listOutSett
{
    QString meterSN;
    QString meterNi;
    Data2listOutSett() {}

    Data2listOutSett(const QString &meterSN, const QString &meterNi) : meterSN(meterSN), meterNi(meterNi) {}

};

struct FramedHashAnswr
{
    MyListStringList listRows;
    QVariantMap col2data;
    int totalCount;
    int lastIndex;
    QStringList header;
    bool hasError;
    QString mess;

    FramedHashAnswr() : hasError(true) {}
    FramedHashAnswr(const MyListStringList &listRows, const QVariantMap &col2data, const int &totalCount, const int &lastIndex, const QStringList &header, const bool &hasError, const QString &mess) :
        listRows(listRows), col2data(col2data), totalCount(totalCount), lastIndex(lastIndex), header(header), hasError(hasError), mess(mess) {}
};

class ClassManagerHelper
{
public:
    static QStringList restoreList(const QStringList &inList, const int &columnListSize, const int &restoreFromLine);

    static Data2listOutSett addData2list(QVariantList &listOneMeter, ClassManagerSharedObjects *shrdObj, QVariantHash &hashRowCol2varData, const QStringList &list,
                             const int &columnListSize, const Data2listInputSett &d2linput, const int &rowCounter);


    static Data2listOutSett addData2listState(QVariantList &listOneMeter, ClassManagerSharedObjects *shrdObj, QVariantHash &hashRowCol2varData, const QStringList &list,
                             const int &columnListSize, const Data2listInputSett &d2linput, const int &rowCounter, const quint8 &pollCode, const bool &hasModelLine);

    static void addVls2listPretty(QVariantList &listOneMeter, const QString &str, const QString &datemask);


    static void addWaterVls2listPretty(QVariantList &listOneMeter, const QString &str);

    static void addWaterPrm2listPretty(QVariantList &listOneMeter, const QString &str);

    static FramedHashAnswr readFromHashFramed(const QVariantHash &hash);

    static QVariantHash onReadFramedHash(const int &lastIndx);

};

#endif // CLASSMANAGERHELPER_H
