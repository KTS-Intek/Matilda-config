#ifndef SHOWMESSHELPER4WDGT_H
#define SHOWMESSHELPER4WDGT_H

#include <QObject>
#include <QStringList>
#include <QDateTime>

#define MESS_OPERATION_IN_PROGRESS      -1
#define MESS_NO_MESS                    0
#define MESS_NO_TABLE                   1
#define MESS_CORRUPTED_DATA             2
#define MESS_CORRUPTED_PACKET           3
#define MESS_NO_ANSWER_FROM_DEVICE      4
#define MESS_NO_DATA                    5
#define MESS_OPERATION_ABORTED          6
#define MESS_CANT_CONNECT2DEV           7

class ShowMessHelper4wdgt : public QObject
{
    Q_OBJECT
public:
    explicit ShowMessHelper4wdgt(QObject *parent = 0);

    static QString addWithFontColorRed(const QString m);
    static QString emailUtcOffset(const int offst);
    static QString addRez2endOfLine(const QString m, const bool &rezGood);
    static QStringList recalcColumns(const QStringList &list, const QString spliter = "\t", const bool skipEmptyParts = true);

    static QString name4command(const quint16 &command);

    static QHash<QString,QString> columnKey2humanLang();

    static QHash<QString,QString> status2human();

    static QHash<QString,QString> region2human();

    static QHash<QString,QString> city2human();


    static QHash<QString,QString> devCommand2human();

    static QHash<QString,QString> manDevCommand2human();

    static QHash<QString,QString> devState2human();



    static QStringList devCode2txt(const int &devCode);


    static QDateTime tableName2DateTime(const QString &tableName, const qint32 addDays = 0, const bool useLocal = false) ;
    static QDateTime tableName2DateTime(const QString &tableName, const QString &dateFormat, const QString &timeFormat, const qint32 addDays = 0) ;
    static QDateTime tableVal2DateTime(const QString &text, const QString &mask, const bool useLocal = false) ;

    static QString messFromCode(const int &messCode);


    static QString byte2humanRead(const qint64 &bytes);

    static QString addSpaceEvry(const QString &s, const int &evrySymb);

    static QString str2hexStr(const QString &s);

    static QString prettyFileName4selFromDb(const QString &pName, const QDateTime &dtFrom, const QDateTime &dtTo, const bool &useLocalTime);

    static QString prettyFileName4selFromDb(const QVariantHash &h, const bool &useLocalTime);

    static QStringList prettyTableShapka(const QVariantHash &h, const QString &dateMask, const bool &useLocalTime);

    static QString matildaFileName(const QString &fileName);

    static QString prepaidFileName(const QString &fileName);

    static QString timeZoneShortAbbreviation(const QDateTime &dt);

    static QString eventCode2human(const int &event);

    static QHash<QString,QString> meterEv2human();

    static QHash<QString,QString> matildaEv2human();

    static QString translateString(QString inStr, const QHash<QString,QString> &hDev2human);


    static QString meterEvCode2humanV2(const int &ucmEventCode);


    static QStringList dataKeys2human(const QStringList &datalist);

    static int calcPercent4thisInterval(const QVariantHash &hashMemo, const QVariantList &lastLine );

    static int calcPercent4thisInterval(const QVariantHash &hashMemo, const QString &tableName );

    static int calcPercent4thisInterval(const QVariantHash &hashMemo, QDateTime dateX );



signals:

public slots:
};

#endif // SHOWMESSHELPER4WDGT_H
