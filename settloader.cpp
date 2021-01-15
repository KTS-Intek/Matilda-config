#include "settloader.h"

#include <QStandardPaths>
#include <QtCore>

#include "moji_defy.h"

//------------------------------------------------------------------------------------------------------------------------------------------------
SettLoader::SettLoader(QObject *parent) : QObject(parent)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------
QVariant SettLoader::loadSett(const int &key, QVariant defVal)
{
    if(defVal.isNull()){
        switch(key){
        case SETT_MAIN_GEOMETRY:        defVal = QRect();                               break;
        case SETT_MAIN_APP_LOG:         defVal = true;                                  break;
        case SETT_MAIN_CURRLANG:        defVal = "uk";                                  break;
        case SETT_MAIN_LASTDIR:         defVal = defPath2dir();                         break;
        case SETT_MAIN_OPERLISTM:       defVal = 1;                                     break;
        case SETT_MAIN_LANG_SELECTED:   defVal = false;                                 break;
        case SETT_MAIN_OPTION_VIEW:     defVal = false;                                 break;
        case SETT_MAIN_OPTION_TRONE:    defVal = false;                                 break;
        case SETT_MAIN_UDP_SHOW_AVIP:   defVal = false;                                 break;
        case SETT_MAIN_UDP_CHECK_IP:    defVal = false;                                 break;


        case SETT_LOLO_POPO:            defVal = "";                                    break;
        case SETT_LOLO_TOTO:            defVal = QVariantHash();                        break;

        case SETT_PAGES_FAVORITE:       defVal = simplePagesList();                     break;
        case SETT_PAGES_DB:             defVal = defVal4pageDb();                       break;
        case SETT_PAGES_ML:             defVal = defVal4pageMl();                       break;
        case SETT_PAGES_DB_F:           defVal = defVal4pageDb_F();                     break;
        case SETT_PAGES_ML_F:           defVal = defVal4pageMl_F();                     break;

        case SETT_METER_MPV:            defVal = getDefMpv();                           break;

        case SETT_OPTIONS_DATEMASK:     defVal = 6;                                     break;
        case SETT_OPTIONS_PDFPAPER:     defVal = 0;                                     break;
        case SETT_OPTIONS_TEXTCODEC:    defVal = "";                                    break;
        case SETT_OPTIONS_PDFL:         defVal = true;                                  break;
        case SETT_OPTIONS_LR:           defVal = 200000;                                break;
        case SETT_OPTIONS_MBC:          defVal = 100000;                                break;
        case SETT_OPTIONS_DP:           defVal = 3;                                     break;
        case SETT_OPTIONS_APPFONT:      defVal = 12;                                    break;
        case SETT_OPTIONS_REGION:       defVal = defVal4region();                       break;


        case SETT_FILTERS_METER:        defVal = getFilterListStr(0,6);                    break;
        case SETT_FILTERS_DATABASE:     defVal = defVal4dbFilter();                     break;
        case SETT_FILTERS_METERLOG:     defVal = getFilterListStr(1,3);                    break;
        case SETT_FILTERS_STATIST:      defVal = getFilterListStr(0,2);                    break;
        case SETT_FILTERS_FORWARD:      defVal = getFilterListStr(0,2);                    break;
        case SETT_FILTERS_MODELDEV:     defVal = getFilterListStr(0,3);                    break;
        case SETT_FILTERS_UPGRADE:      defVal = getFilterListStr(0,3);                    break;
        case SETT_FILTERS_TZ:           defVal = getFilterListStr(0,4);                    break;
        case SETT_FILTERS_LEDLAMPS      : defVal = getFilterListStr(0,3)                   ; break;
        case SETT_FILTERS_LEDTASKS      : defVal = getFilterListStr(0,3)                   ; break;

        }
    }

    QString name = valName4key(key);
    QString gNmame = groupName4key(key);
    if(name .isEmpty() || gNmame.isEmpty())
        return defVal;

    QSettings settings("kts-intek", "matilda-dev");

//    qDebug() << "sett file name=" << settings.fileName();

    settings.beginGroup(gNmame );
    QVariant val = settings.value(name, defVal);
    settings.endGroup();
    return val;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void SettLoader::saveSett(const int &key, const QVariant &val)
{
    QString name = valName4key(key);
    QString gNmame = groupName4key(key);
    if(name .isEmpty() || gNmame.isEmpty())
        return ;

    QSettings settings("kts-intek", "matilda-dev");

    settings.beginGroup(gNmame );
    settings.setValue(name, val);
    settings.endGroup();
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void SettLoader::saveLastDir(const QString &fileName)
{
    QFileInfo fi(fileName);
    SettLoader::saveSett(SETT_MAIN_LASTDIR, fi.absoluteDir().absolutePath());
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QString SettLoader::getValidLastDir()
{
    QString slahDoPapky = loadSett(SETT_MAIN_LASTDIR).toString();
    if(slahDoPapky.right(1) == "/")
        slahDoPapky.clear();
    return slahDoPapky;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QString SettLoader::groupName4key(const int &key)
{
    if(key >= SETT_MAIN_GEOMETRY        && key < SETT_LOLO_POPO)
        return "main";

    if(key >= SETT_LOLO_POPO            && key < SETT_PAGES_FAVORITE)
        return "lolo";

    if(key >= SETT_PAGES_FAVORITE       && key < SETT_METER_MPV)
        return "pages";

    if(key >= SETT_METER_MPV            && key < SETT_OPTIONS_DATEMASK)
        return "meter";

    if(key >= SETT_OPTIONS_DATEMASK     && key < 4000)
        return "options";


    if(key >= SETT_FILTERS_METER     && key < 6000)
        return "filter";

    return "";
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QString SettLoader::valName4key(const int &key)
{
    QString name;
    switch(key){
    case SETT_MAIN_GEOMETRY:        name = "geometry";          break;
    case SETT_MAIN_APP_LOG:         name = "showAppLog";        break;
    case SETT_MAIN_CURRLANG:        name = "currLang";          break;
    case SETT_MAIN_LASTDIR:         name = "lastOpenSaveDir";   break;
    case SETT_MAIN_OPERLISTM:       name = "mode";              break;
    case SETT_MAIN_LANG_SELECTED:   name = "langSelected";      break;
    case SETT_MAIN_OPTION_VIEW:     name = "tbViewMode";        break;
    case SETT_MAIN_OPTION_TRONE:    name = "actTreeOneExp";     break;
    case SETT_MAIN_UDP_SHOW_AVIP:   name = "udpShowOnlyAvIP";   break;
    case SETT_MAIN_UDP_CHECK_IP:    name = "checkTcpIp";        break;


    case SETT_LOLO_POPO:            name = "popo";              break;
    case SETT_LOLO_TOTO:            name = "toto";              break;

    case SETT_PAGES_FAVORITE:       name = "favorite";          break;
    case SETT_PAGES_DB:             name = "db";                break;
    case SETT_PAGES_ML:             name = "ml";                break;
    case SETT_PAGES_DB_F:           name = "dbf";               break;
    case SETT_PAGES_ML_F:           name = "mlf";               break;

    case SETT_METER_MPV:            name = "mpv";               break;

    case SETT_OPTIONS_DATEMASK:     name = "dateMask";          break;
    case SETT_OPTIONS_PDFPAPER:     name = "pdfPaper";          break;
    case SETT_OPTIONS_TEXTCODEC:    name = "textCodeName";      break;
    case SETT_OPTIONS_PDFL:         name = "pdfL";              break;
    case SETT_OPTIONS_LR:           name = "lr";                break;
    case SETT_OPTIONS_MBC:          name = "mbc";               break;
    case SETT_OPTIONS_DP:           name = "dp";                break;
    case SETT_OPTIONS_APPFONT:      name = "appFontS";          break;
    case SETT_OPTIONS_REGION:       name = "tzReg";             break;

    case SETT_FILTERS_METER:        name = "meter";             break;
    case SETT_FILTERS_DATABASE:     name = "db";                break;
    case SETT_FILTERS_METERLOG:     name = "ml";                break;
    case SETT_FILTERS_STATIST:      name = "stat";              break;
    case SETT_FILTERS_FORWARD:      name = "fw";                break;
    case SETT_FILTERS_MODELDEV:     name = "md";                break;
    case SETT_FILTERS_UPGRADE:      name = "upg";               break;
    case SETT_FILTERS_TZ:           name = "tz";                break;
    case SETT_FILTERS_LEDLAMPS      : name = "ledlamp"          ; break;
    case SETT_FILTERS_LEDTASKS      : name = "ledtasks"         ; break;

    }
    return name;
}
//------------------------------------------------------------------------------------------------------------------------------------------------

QString SettLoader::defPath2dir(){ return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation); }

//------------------------------------------------------------------------------------------------------------------------------------------------
QStringList SettLoader::simplePagesList()
{
    QStringList l;
    l.append( QString("About object") );
    l.append( QString("Date and time") );
    l.append( QString("State") );
    l.append( QString("Statistic of exchange") );
    l.append( QString("System") );

    l.append( QString("Serial log") );

    l.append( QString("Direct access") );
    l.append( QString("Direct access client") );
    l.append( QString("Matilda client") );

    l.append( QString("Poll") );
    l.append( QString("Forward"));
    l.append( QString("Poll schedule") );
    l.append( QString("Meter list") );
    l.append( QString("Database") );
    l.append( QString("Meter logs") );

    l.append( QString("LCUs"));
    l.append( QString("Schedule for groups"));
    l.append( QString("Temporary schedule"));
    l.append( QString("Rescue schedules"));
    l.append( QString("Schedule state"));

    return l;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QStringList SettLoader::listPath2icon()
{
    QStringList l ;
    l.append( ":/katynko/svg2/exinfo.svg");
    l.append( ":/katynko/svg2/kt-bandwidth-scheduler.svg");
    l.append( ":/katynko/svg2/view-readermode-active.svg" );
    l.append( ":/katynko/svg/view-statistics.svg");
    l.append( ":/katynko/svg2/lc_extendedhelp.svg");
    l.append( ":/katynko/svg2/network-disconnect.svg" );

    l.append( ":/katynko/svg/address-book-new.svg" );
    l.append( ":/katynko/svg2/view-readermode-gr.svg" );
    l.append( ":/katynko/svg2/view-readermode-gr2.svg" );
    l.append( ":/katynko/svg2/call-start.svg");
    l.append( ":/katynko/svg2/network-wired.svg");
    l.append( ":/katynko/svg2/network-wireless.svg" );


    l.append( ":/katynko/svg/view-list-text.svg");
    l.append( ":/katynko/svg2/view-readermode-red2.svg" );
    l.append( ":/katynko/svg2/view-readermode-yel2.svg" );

    l.append( ":/katynko/svg2/zoom-in-2.svg");
    l.append( ":/katynko/svg2/zoom-in.svg" );
    l.append( ":/katynko/svg2/view-web-browser-dom-tree.svg" );

    l.append( ":/katynko/svg2/homerun.svg" );
//    l.append( ":/katynko/svg/document-save.svg");
    l.append( ":/katynko/svg2/kt-queue-manager.svg");
    l.append( ":/katynko/svg/table.svg");
    l.append( ":/katynko/svg2/dashboard-show.svg");
    l.append( ":/katynko/svg/format-justify-fill.svg" );
    l.append( ":/katynko/svg2/db.svg" );
    l.append( ":/katynko/svg2/lc_dbqueryedit.svg");
    l.append( ":/katynko/svg2/lc_dbtableedit.svg" );


    l.append( ":/katynko/svg/format-justify-fill.svg" );
    l.append( ":/katynko/svg2/dashboard-show.svg" );
    l.append( ":/katynko/svg4/dashboard-red.svg" );
    l.append( ":/katynko/svg2/exwarning.svg");
    l.append( ":/katynko/svg4/kt-queue-manager-red.svg");


    for(int i = 0, iMax = l.size(); i < iMax; i++){
        if(l.at(i).left(2) != ":/")
            l.replace(i, ":/katynko/svg2/empty.svg");
    }

    return l;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QStringList SettLoader::realPageNameByDev(const int &devType)
{
   QStringList l;
   switch(devType){
   case DEV_POLL: l = realPageNameDevPoll(); break;
   case DEV_STOR: l = realPageNameDevStor(); break;
   case DEV_GATE: l = realPageNameDevGate(); break;

   case DEV_POLL_EMULATOR_L0: l = realPageNameDevEmul0(); break;
   case DEV_POLL_EMULATOR_L1: l = realPageNameDevEmul1(); break;
   case DEV_POLL_EMULATOR_L2: l = realPageNameDevEmul2(); break;
   default: l = realPageName(); break;
   }
   return l;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QHash<QString, QStringList> SettLoader::pageName4devTree()
{
    //items must be with real name? root item: tr
    QHash<QString, QStringList> h;
    QStringList lKeys;

    lKeys.append(QString("About object"));
    h.insert(lKeys.last(), QStringList());

    lKeys.append(QString("Date and time"));
    h.insert(lKeys.last(), QStringList());

    lKeys.append(QString("State"));
    h.insert(lKeys.last(), QStringList());


    QStringList l;
//    l.append( QString("State")                   );
    l.append( QString("System") );
    l.append( QString("Network interfaces") );

    l.append( QString("Application events") );
    l.append( QString("Statistic of exchange")   );
    l.append( QString("Meter plugin") );
    l.append( QString("Poll events") );
    l.append( QString("Poll error events") );
    l.append( QString("Poll warning events") );
    l.append( QString("Serial log") );

    lKeys.append(tr("Info"));
    h.insert(lKeys.last(), l);

    l.clear();
    l.append( QString("Commands") );
    lKeys.append(tr("Commands"));
    h.insert(lKeys.last(), l);

    l.clear();
    l.append( QString("Database") );
    l.append( QString("Meter logs") );
//    l.append( QString("Hash summ" ) );
    lKeys.append(tr("Poll data"));
    h.insert(lKeys.last(), l);

    l.clear();
    l.append( QString("GSM/PPP") );
    l.append( QString("TCP interface") );
    l.append( QString("ZigBee") );
    l.append( QString("Direct access") );
    l.append( QString("Direct access client") );
    l.append( QString("Matilda client") );



    lKeys.append(tr("Network settings"));
    h.insert(lKeys.last(), l);


    l.clear();

    l.append( QString("Meter Poll") );
    l.append( QString("Forward"));
    l.append( QString("Poll schedule") );
    l.append( QString("Meter list") );


    lKeys.append(tr("General settings"));
    h.insert(lKeys.last(), l);

    l.clear();
    l.append( QString("LCUs"));
    l.append( QString("Schedule for groups"));
    l.append( QString("Temporary schedule"));
    l.append( QString("Rescue schedules"));
    l.append( QString("Schedule state"));

    lKeys.append(tr("Lamp controllers"));
    h.insert(lKeys.last(), l);

    l.clear();

    h.insert("\r\nlKeys\r\n", lKeys);
    return h;
}

//------------------------------------------------------------------------------------------------------------------------------------------------
QStringList SettLoader::realPageNameDevPoll()
{
    return realPageName();
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QStringList SettLoader::realPageNameDevStor()
{
    QStringList l ;
    l.append( QString("About object")            );
    l.append( QString("Date and time")           );
    l.append( QString("State")                   );

    l.append( QString("System") );
    l.append( QString("Network interfaces") );

    l.append( QString("Application events") );
    l.append( QString("GSM/PPP") );
    l.append( QString("TCP interface") );

    l.append( QString("Matilda client") );

    l.append( QString("Commands") );


    l.append( QString("Database") );
    l.append( QString("Meter logs") );
//    l.append( QString("Hash summ" ) );


    return l;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QStringList SettLoader::realPageNameDevGate()
{
    QStringList l ;
    l.append( QString("About object")            );
    l.append( QString("Date and time")           );
    l.append( QString("State")                   );

    l.append( QString("System") );
    l.append( QString("Network interfaces") );
    l.append( QString("Meter plugin") );
    l.append( QString("Application events") );

    l.append( QString("GSM/PPP") );
    l.append( QString("TCP interface") );
    l.append( QString("ZigBee") );
    l.append( QString("Serial log") );


    l.append( QString("Direct access") );
    l.append( QString("Direct access client") );
    l.append( QString("Matilda client") );

    l.append( QString("Commands") );


    l.append( QString("Forward"));
//    l.append( QString("Hash summ" ) );


    return l;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QStringList SettLoader::realPageNameDevEmul0()
{
    return realPageNameDevEmul2();
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QStringList SettLoader::realPageNameDevEmul1()
{
    return realPageNameDevEmul2();
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QStringList SettLoader::realPageNameDevEmul2()
{
    QStringList l ;
    l.append( QString("About object")            );
    l.append( QString("Date and time")           );
    l.append( QString("State")                   );
    l.append( QString("Statistic of exchange")   );
    l.append( QString("System") );
    l.append( QString("Network interfaces") );

    l.append( QString("Meter plugin") );
    l.append( QString("Application events") );
    l.append( QString("Poll events") );

    l.append( QString("TCP interface") );
    l.append( QString("ZigBee") );

    l.append( QString("Serial log") );
    l.append( QString("Poll error events") );
    l.append( QString("Poll warning events") );


    l.append( QString("Direct access") );
    l.append( QString("Direct access client") );
    l.append( QString("Matilda client") );

    l.append( QString("Commands") );

    l.append( QString("Poll") );
    l.append( QString("Forward"));

    l.append( QString("Meter list") );
    l.append( QString("Database") );
    l.append( QString("Meter logs") );



//    l.append( QString("Hash summ" ) );


    return l;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QStringList SettLoader::realPageName()
{
    QStringList l ;
    l.append( QString("About object")            );
    l.append( QString("Date and time")           );
    l.append( QString("State")                   );
    l.append( QString("Statistic of exchange")   );
    l.append( QString("System") );
    l.append( QString("Network interfaces") );
    l.append( QString("Meter plugin") );
    l.append( QString("Application events") );
    l.append( QString("Poll events") );
    l.append( QString("GSM/PPP") );
    l.append( QString("TCP interface") );
    l.append( QString("ZigBee") );
    l.append( QString("Serial log") );
    l.append( QString("Poll error events") );
    l.append( QString("Poll warning events") );


    l.append( QString("Direct access") );
    l.append( QString("Direct access client") );
    l.append( QString("Matilda client") );

    l.append( QString("Commands") );

    l.append( QString("Poll") );
    l.append( QString("Forward"));
    l.append( QString("Poll schedule") );
    l.append( QString("Meter list") );
    l.append( QString("Database") );
    l.append( QString("Meter logs") );
//    l.append( QString("Hash summ" ) );
//size 33
    l.append( QString("LCUs"));
    l.append( QString("Schedule for groups"));
    l.append( QString("Temporary schedule"));
    l.append( QString("Rescue schedules"));
    l.append( QString("Schedule state"));


    return l;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QStringList SettLoader::localPageName()
{
    QStringList l ;
    l.append( tr("About object") );
    l.append( tr("Date and time") );
    l.append( tr("State") );
    l.append( tr("Statistic of exchange") );
    l.append( tr("System") );
    l.append( tr("Network interfaces") );
    l.append( tr("Meter plugin") );
    l.append( tr("Application events") );
    l.append( tr("Poll events") );
    l.append( tr("GSM/PPP") );
    l.append( tr("TCP interface") );
    l.append( tr("ZigBee") );
    l.append( tr("Serial log") );
    l.append( tr("Poll error events") );
    l.append( tr("Poll warning events") );
    l.append( tr("Direct access") );
    l.append( tr("Direct access client") );
    l.append( tr("Matilda client") );
    l.append( tr("Commands") );
    l.append( tr("Poll") );
    l.append( tr("Forward"));
    l.append( tr("Poll schedule") );
    l.append( tr("Meter list") );
    l.append( tr("Data base") );
    l.append( tr("Meter logs") );
//    l.append( tr("Hash summ" ) );

    l.append( tr("LCUs"));
    l.append( tr("Schedule for groups"));
    l.append( tr("Temporary schedule"));
    l.append( tr("Rescue schedules"));
    l.append( tr("Schedule state"));


    return l;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QHash<QString, QString> SettLoader::hashRealName2localName()
{
    QStringList lr = realPageName();
    QStringList ll = localPageName();
    QHash<QString,QString> h;
    for(int i = 0, iMax = lr.size(), iMax2 = ll.size(); i < iMax && i < iMax2; i++)
        h.insert(lr.at(i), ll.at(i));
    return h;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QHash<QString, QString> SettLoader::hashLocalName2realName()
{
    QStringList lr = realPageName();
    QStringList ll = localPageName();
    QHash<QString,QString> h;
    for(int i = 0, iMax = lr.size(), iMax2 = ll.size(); i < iMax && i < iMax2; i++)
        h.insert(ll.at(i), lr.at(i));
    return h;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QVariantMap SettLoader::getDefMpv()
{
    QVariantMap mpv  ;
    QStringList list = QString("A+;A+,R+;A+,R+,R-;A+,A-,R+,R-;A+,A-").split(";");
    while(!list.isEmpty()){
        QString s = list.takeFirst();
        mpv.insert(s, s);
    }
    return mpv;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QStringList SettLoader::intList2strList(const QList<int> &l)
{
    QStringList sl;
    for(int i = 0, iMax = l.size(); i < iMax; i++)
        sl.append(QString::number(l.at(i)));
    return sl;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QList<int> SettLoader::strList2intList(const QStringList &l)
{
    QList<int> li;
    for(int i = 0, iMax = l.size(); i < iMax; i++){
        bool ok;
        li.append(l.at(i).toInt(&ok));
        if(!ok)
            li.removeLast();
    }
    return li;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QList<int> SettLoader::getPageCanWrite()
{
    QList<int> listInt;
    listInt.append( COMMAND_WRITE_ABOUT_OBJECT );
    listInt.append( 0 );
    listInt.append( 0 );
    listInt.append( 0 );
    listInt.append( 0 );
    listInt.append( 0 );
    listInt.append( 0 );
    listInt.append( 0 );
    listInt.append( 0 );
    listInt.append( COMMAND_WRITE_GPRS_SETT );
    listInt.append( COMMAND_WRITE_TCP_SETT   );
    listInt.append( COMMAND_WRITE_ZIGBEE_SETT );
    listInt.append( 0 );
    listInt.append( 0 );
    listInt.append( 0 );
    listInt.append( COMMAND_WRITE_DA_SERVICE_SETT );
    listInt.append( COMMAND_WRITE_PEREDAVATOR_AC_SETT );
    listInt.append( COMMAND_WRITE_MATILDA_AC_SETT );
    listInt.append( 0 );
    listInt.append( COMMAND_WRITE_POLL_SETT );
    listInt.append( COMMAND_WRITE_FRWRD_SETT);
    listInt.append( COMMAND_WRITE_POLL_SCHEDULE );
    listInt.append( COMMAND_WRITE_METER_LIST_FRAMED );
    listInt.append( 0 );
    listInt.append( 0 );
//    listInt.append( 0 ); COMMAND_READ_TABLE_HASH_SUMM
    listInt.append( COMMAND_WRITE_LEDLAMPLIST_FRAMED );
    listInt.append( COMMAND_WRITE_GROUP_SCHEDULE );
    listInt.append( COMMAND_WRITE_TEMP_LED_SHCEDULE );
    listInt.append( COMMAND_WRITE_GROUP_RESCUE_SCHEDULE );
    listInt.append( 0 );


    return listInt;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QList<int> SettLoader::getPageCanRead()
{
    QList<int> listInt;
    listInt.append( COMMAND_READ_ABOUT_OBJECT );
    listInt.append( COMMAND_READ_DATE_SETT );
    listInt.append( COMMAND_READ_STATE );
    listInt.append( COMMAND_READ_POLL_STATISTIC );
    listInt.append( COMMAND_READ_SYSTEM_SETTINGS );
    listInt.append( COMMAND_READ_IFCONFIG );
    listInt.append( COMMAND_READ_ABOUT_PLG );
    listInt.append( COMMAND_READ_APP_LOG );
    listInt.append( COMMAND_READ_ZBR_LOG );
    listInt.append( COMMAND_READ_GPRS_SETT );
    listInt.append( COMMAND_READ_TCP_SETT );
    listInt.append( COMMAND_READ_ZIGBEE_SETT   );
    listInt.append( COMMAND_READ_SERIAL_LOG );
    listInt.append( COMMAND_READ_PLUGIN_LOG_ERROR );
    listInt.append( COMMAND_READ_PLUGIN_LOG_WARN );
    listInt.append( COMMAND_READ_DA_SERVICE_SETT );
    listInt.append( COMMAND_READ_PEREDAVATOR_AC_SETT );
    listInt.append( COMMAND_READ_MATILDA_AC_SETT );
    listInt.append( 0  );
    listInt.append( COMMAND_READ_POLL_SETT );
    listInt.append( COMMAND_READ_FRWRD_SETT);
    listInt.append( COMMAND_READ_POLL_SCHEDULE );
    listInt.append( COMMAND_READ_METER_LIST_FRAMED );
    listInt.append( COMMAND_READ_DATABASE );
    listInt.append( COMMAND_READ_METER_LOGS_GET_TABLES );
//    listInt.append( COMMAND_READ_TABLE_HASH_SUMM );

    listInt.append( COMMAND_READ_LEDLAMPLIST_FRAMED );
    listInt.append( COMMAND_READ_GROUP_SCHEDULE );
    listInt.append( COMMAND_READ_TEMP_LED_SHCEDULE );
    listInt.append( COMMAND_READ_GROUP_RESCUE_SCHEDULE );
    listInt.append( COMMAND_READ_FIREFLY_SCHEDULESTATE );

    return listInt;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QStringList SettLoader::getFilterListStr(const int &startIndx, const int &count)
{
    QStringList l;
    for(int i = startIndx, iMax = startIndx + count; i < iMax; i++)
        l.append(QString::number(i));
    return l;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QList<int> SettLoader::getFilterList(const int &startIndx, const int &count)
{
    QList<int> l;
    for(int i = startIndx, iMax = startIndx + count; i < iMax; i++)
        l.append(i);
    return l;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QVariantHash SettLoader::defVal4dbFilter()
{
    QVariantHash h;
    QStringList l = getFilterListStr(0,3);
    for(int i = 1; i < 4; i++)
        h.insert(QString::number(i), l);
    return h;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QString SettLoader::defVal4region()
{
    QString s = QTimeZone::systemTimeZoneId();

    s = s.contains("/") ? s.split("/").first() : s;
    if(s.isEmpty())
        s = tr("All");
    return s;

}
//------------------------------------------------------------------------------------------------------------------------------------------------
QVariantHash SettLoader::defVal4pageDb()
{
    QVariantHash h;
    h.insert("cbMemo", true);
    h.insert("cbSn", true);
    h.insert("cbNi", true);
    return h;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QVariantHash SettLoader::defVal4pageMl()
{
    return defVal4pageDb();
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QVariantHash SettLoader::defVal4pageDb_F()
{
    QVariantHash h;
    h.insert("1", QString("A+").split(" "));
    h.insert("2", QString("A+").split(" "));
    h.insert("3", QString("UA UB UC").split(" "));
    h.insert("gbMeterDataFromTo", false);
    h.insert("dteMeterDataFrom", QDateTime::currentDateTime().addDays(-5));
    h.insert("dteMeterDataTo", QDateTime::currentDateTime());


    h.insert("gbMeterDataYangerThan", true);
    h.insert("cbMeterDataKftnt", 2);
    h.insert("sbMeterDataInterval", 3);

    h.insert("lvMeterDataTariff", QString("0 1 2").split(" "));

    return h;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QVariantHash SettLoader::defVal4pageMl_F()
{
    QVariantHash h;
    h.insert("gbMeterDataFromTo", false);
    h.insert("dteMeterDataFrom", QDateTime::currentDateTime().addDays(-5));
    h.insert("dteMeterDataTo", QDateTime::currentDateTime());


    h.insert("gbMeterDataYangerThan", true);
    h.insert("cbMeterDataKftnt", 2);
    h.insert("sbMeterDataInterval", 3);

    return h;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QVariantHash SettLoader::varMap2varHash(const QVariantMap &map)
{
    QVariantHash h;
    QList<QString> lk = map.keys();
    for(int i = 0, iMax = lk.size(); i < iMax; i++){
        h.insert(lk.at(i), map.value(lk.at(i)));
    }
    return h;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
QStringList SettLoader::registeredKesy4pppSupervisor()
{
    return QString("$dns $ntp $mtd $da").split(" ");
}
//------------------------------------------------------------------------------------------------------------------------------------------------
