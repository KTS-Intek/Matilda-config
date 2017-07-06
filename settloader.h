#ifndef SETTLOADER_H
#define SETTLOADER_H

#include <QSettings>
#include <QObject>

#define SETT_MAIN_GEOMETRY      1
#define SETT_MAIN_APP_LOG       2
#define SETT_MAIN_CURRLANG      3
#define SETT_MAIN_LASTDIR       4
#define SETT_MAIN_OPERLISTM     5
#define SETT_MAIN_LANG_SELECTED 6
#define SETT_MAIN_OPTION_VIEW   7
#define SETT_MAIN_OPTION_TRONE  8

#define SETT_MAIN_UDP_SHOW_AVIP 9
#define SETT_MAIN_UDP_CHECK_IP  10


#define SETT_LOLO_POPO          500
#define SETT_LOLO_TOTO          501

#define SETT_PAGES_FAVORITE     1000
#define SETT_PAGES_DB           1001
#define SETT_PAGES_ML           1002
#define SETT_PAGES_DB_F         1003
#define SETT_PAGES_ML_F         1004

#define SETT_METER_MPV          2000

#define SETT_OPTIONS_DATEMASK   3000
#define SETT_OPTIONS_PDFPAPER   3001
#define SETT_OPTIONS_TEXTCODEC  3002
#define SETT_OPTIONS_PDFL       3003
#define SETT_OPTIONS_LR         3004
#define SETT_OPTIONS_MBC        3005
#define SETT_OPTIONS_DP         3006
#define SETT_OPTIONS_APPFONT    3007
#define SETT_OPTIONS_REGION     3008


#define SETT_FILTERS_METER      5000
#define SETT_FILTERS_DATABASE   5001
#define SETT_FILTERS_METERLOG   5002
#define SETT_FILTERS_STATIST    5003
#define SETT_FILTERS_FORWARD    5004
#define SETT_FILTERS_MODELDEV   5005
#define SETT_FILTERS_UPGRADE    5006
#define SETT_FILTERS_TZ         5007
#define SETT_FILTERS_LEDLAMPS   5008
#define SETT_FILTERS_LEDTASKS   5009
#define SETT_FILTERS_LED_RELAY  5010

#define SETT_FILTERS_LED_GCHART 5011


class SettLoader : public QObject
{
    Q_OBJECT
public:
    explicit SettLoader(QObject *parent = 0);

     static QVariant loadSett(const int &key, QVariant defVal = QVariant());

     static void saveSett(const int &key, const QVariant &val);

     static void saveLastDir(const QString &fileName);

     static QString getValidLastDir();

     static QString groupName4key(const int &key);

     static QString valName4key(const int &key);


     //defVals
     static QString defPath2dir();

     static QStringList simplePagesList();

     static QStringList listPath2icon();

     static QStringList realPageNameByDev(const int &devType);//en_GB

     static QHash<QString,QStringList> pageName4devTree();//en_GB


     static QStringList realPageNameDevPoll();

     static QStringList realPageNameDevStor();

     static QStringList realPageNameDevGate();

     static QStringList realPageNameDevEmul0();

     static QStringList realPageNameDevEmul1();

     static QStringList realPageNameDevEmul2();


     static QStringList realPageName();//en_GB

     static QStringList localPageName();//local lang

     static QHash<QString,QString> hashRealName2localName();

     static QHash<QString,QString> hashLocalName2realName();


     static QVariantMap getDefMpv();


     //helper
     static QStringList intList2strList(const QList<int> &l  );
     static QList<int> strList2intList(const QStringList &l);

     static QList<int> getPageCanWrite();
     static QList<int> getPageCanRead();

     static QStringList getFilterListStr(const int &startIndx, const int &count);

     static QList<int> getFilterList(const int &startIndx, const int &count);


     static QVariantHash defVal4dbFilter();

     static QString defVal4region();

     static QVariantHash defVal4pageDb();

     static QVariantHash defVal4pageMl();

     static QVariantHash defVal4pageDb_F();

     static QVariantHash defVal4pageMl_F();

     static QVariantHash varMap2varHash(const QVariantMap &map);


     //protocol v2
     static QStringList registeredKesy4pppSupervisor();


signals:

public slots:

};

#endif // SETTLOADER_H
