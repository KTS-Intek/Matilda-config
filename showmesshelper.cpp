#include "showmesshelper.h"
#include <QtCore>

#include "moji_defy.h"
#include "ucmetereventcodes.h"
//-------------------------------------------------------------------------------------------------------------------------------------
ShowMessHelper::ShowMessHelper(QObject *parent) : QObject(parent)
{

}
//-------------------------------------------------------------------------------------------------------------------------------------
QString ShowMessHelper::addWithFontColorRed(const QString m)
{
    return QString("<font color=red>%1</font>").arg(m);
}
//-------------------------------------------------------------------------------------------------------------------------------------
QString ShowMessHelper::emailUtcOffset(const int offst)
{
    QString h = QString::number(qAbs(offst)/3600).rightJustified(  2 , '0');
    QString m = QString::number( ( qAbs(offst)%3600)/60).rightJustified(2, '0');

    QString alfj = (offst > 0) ? "+" : "-";
    if(offst == 0)
        alfj = "±";
    return alfj + h + ":" + m;
}
//-------------------------------------------------------------------------------------------------------------------------------------
QString ShowMessHelper::addRez2endOfLine(const QString m, const bool &rezGood)
{
    return tr("%1. Operation: %2").arg(m).arg( rezGood ? tr("Done)") : addWithFontColorRed(tr("Failed(")) );
}
//-------------------------------------------------------------------------------------------------------------------------------------
QStringList ShowMessHelper::recalcColumns(const QStringList &list, const QString spliter, const bool skipEmptyParts)
{
    QHash<int,int> hashColMaxLen;
    int maxCol = 0;
    int iMax = list.size();
    for(int i = 0; i < iMax; i++){
        QStringList l = list.at(i).split(spliter, skipEmptyParts ? QString::SkipEmptyParts : QString::KeepEmptyParts );
        int jMax = l.size();
        if(jMax > maxCol)
            maxCol = jMax;
        for(int j = 0; j < jMax; j++){
            if(l.at(j).length() > hashColMaxLen.value(j))
                hashColMaxLen.insert(j, l.at(j).length());
        }
    }
    QStringList retList;
    for(int i = 0; i < iMax; i++){
        QStringList l = list.at(i).split(spliter, skipEmptyParts ? QString::SkipEmptyParts : QString::KeepEmptyParts );
        int jMax = l.size();

        int j = 0;
        QStringList l2;

        for( ; j < jMax; j++)
            l2.append(l.at(j).leftJustified(hashColMaxLen.value(j), ' '));

        for( ; j < maxCol; j++)
            l2.append(" ");

        retList.append(l2.join(spliter));
    }
    return retList;
}
//-------------------------------------------------------------------------------------------------------------------------------------
QString ShowMessHelper::name4command(const quint16 &command)
{
    QString s;
    switch(command){
    case COMMAND_ZULU:                          break;
    case COMMAND_YOUR_ID_AND_MAC:               break;

    case COMMAND_AUTHORIZE                      : s = tr("Authorization"); break;

    case COMMAND_ACCESS_DENIED:                 break;
    case COMMAND_CONNECT_2_THIS_SERVICE:        break;
        //case COMMAND_LAST_OPER_RESULT                5
    case COMMAND_I_AM_A_ZOMBIE:                 break;
    case COMMAND_ERROR_CODE:                    break;

    case COMMAND_COMPRESSED_PACKET:             break;
    case COMMAND_COMPRESSED_STREAM:             break;
    case COMMAND_I_NEED_MORE_TIME:              break;
    case COMMAND_CONNECT_ME_2_THIS_ID_OR_MAC:   break;
    case COMMAND_ERROR_CODE_EXT:                break;



        //READ ROOT AND GUEST
    case COMMAND_READ_SYSTEM_SETTINGS           : s = tr("system info")         ; break;
    case COMMAND_READ_TASK_INFO                 : s = tr("running process")     ; break;
    case COMMAND_READ_ABOUT_PLG                 : s = tr("meter plugin")        ; break;
    case COMMAND_READ_DATE_SETT                 : s = tr("date and time")       ; break;
    case COMMAND_READ_GPRS_SETT                 : s = tr("GSM settings")        ; break;
    case COMMAND_READ_STATE                     : s = tr("state")               ; break;
    case COMMAND_READ_IFCONFIG                  : s = tr("network interfaces")  ; break;
    case COMMAND_READ_APP_LOG                   : s = tr("application events")  ; break;
    case COMMAND_READ_POLL_SCHEDULE             : s = tr("poll schedule")       ; break;
    case COMMAND_READ_METER_LIST                : s = tr("meter list")          ; break;

    case COMMAND_READ_DATABASE                  : s = tr("data base")           ; break;

    case COMMAND_READ_DATABASE_GET_TABLES       : s = tr("data base")           ; break;
    case COMMAND_READ_DATABASE_GET_VAL          : s = tr("data base")           ; break;
    case COMMAND_READ_METER_LOGS                : s = tr("meter logs")          ; break;
    case COMMAND_READ_METER_LOGS_GET_TABLES     : s = tr("meter logs")          ; break;
    case COMMAND_READ_METER_LOGS_GET_VAL        : s = tr("meter logs")          ; break;

    case COMMAND_READ_METER_LIST_FRAMED         : s = tr("meter list")          ; break;

    case COMMAND_READ_DATABASE_TABLES_PARSING   : s = tr("data base")           ; break;
    case COMMAND_READ_ZBR_LOG                   : s = tr("zbyrator events")     ; break;
    case COMMAND_READ_ABOUT_OBJECT              : s = tr("about object")        ; break;
    case COMMAND_READ_POLL_SETT                 : s = tr("poll settings")       ; break;
    case COMMAND_READ_POLL_STATISTIC            : s = tr("statistic of exchange"); break;

    case COMMAND_READ_TABLE_HASH_SUMM           : s = tr("hash summ")           ; break;


    case COMMAND_READ_IP_FILTER_SETT            : s = tr("IP filter")           ; break;

    case COMMAND_READ_METER_LIST_HASH_SUMM      : s = tr("meter list hash summ"); break;
    case COMMAND_READ_SERIAL_LOG                : s = tr("serial log")          ; break;
    case COMMAND_READ_COMMANDS                  : s = tr("device commands")     ; break;


    case COMMAND_READ_DA_DATA_FROM_COORDINATOR  :                                 break;
    case COMMAND_READ_DA_SERVICE_SETT           : s = tr("direct access")       ; break;
    case COMMAND_READ_PLUGIN_LOG_WARN           : s = tr("warning events")      ; break;
    case COMMAND_READ_PLUGIN_LOG_ERROR          : s = tr("error events")        ; break;

    case COMMAND_READ_PEREDAVATOR_AC_SETT       : s = tr("direct access active socket"); break;
    case COMMAND_READ_MATILDA_AC_SETT           : s = tr("matilda active socket"); break;
    case COMMAND_READ_BACKUP_LIST               : s = tr("backup")              ; break;
    case COMMAND_READ_UDP_BEACON                : s = tr("UDP beacon")          ; break;

    case COMMAND_READ_METER_LIST_HASH_SUMM_EXT  : s = tr("meter list hash summ"); break;
    case COMMAND_READ_METER_LIST_FRAMED_EXT     : s = tr("caching info about meters..."); break;

    case COMMAND_READ_ZIGBEE_SETT               : s = tr("ZigBee settings")     ; break;
    case COMMAND_READ_TCP_SETT                  : s = tr("TCP settings")        ; break;
    case COMMAND_READ_FRWRD_SETT                : s = tr("forward settings")    ; break;

   //protocol v2
//    case COMMAND_READ_IPTABLE                   : s = tr("IP Table")            ; break;
//    case COMMAND_READ_PPP_SUPERVISOR            : s = tr("PPP Supervisor")      ; break;

    case COMMAND_READ_DEVICE_SERIAL_NUMBER      : s = tr("Serial Number")       ; break;

        ///LED LAMP
//    case COMMAND_READ_LEDLAMPLIST_FRAMED        : s = tr("LED Lamp List")       ; break;
//    case COMMAND_READ_GROUP_SCHEDULE            : s = tr("Group Schedule")      ; break;
//    case COMMAND_READ_FF_TASK_TABLE_FRAMED             : s = tr("Firefly: Tasks")      ; break;
//    case COMMAND_READ_POWER_RELAY_SETT          : s = tr("Firefly: Relay")      ; break;
//    case COMMAND_READ_FIREFLY_MODE_SETT         : s = tr("Firefly: Modes")      ; break;
        //case COMMAND_READ_MEMO_SN                36


        //ROOT || OPERATOR WRITE
    case COMMAND_WRITE_FIRST_4_OPERATOR         : break;
    case COMMAND_WRITE_POLL_SCHEDULE            : s = tr("poll schedule")       ; break;
    case COMMAND_WRITE_METER_LIST               : s = tr("meter list")          ; break;
    case COMMAND_WRITE_METER_LIST_FRAMED        : s = tr("meter list")          ; break;
    case COMMAND_WRITE_DATE_SETT                : s = tr("date and time")       ; break;
    case COMMAND_WRITE_RESET_MODEM              : s = tr("reset modem")         ; break;
    case COMMAND_WRITE_POLL_SETT                : s = tr("poll settings")       ; break;
    case COMMAND_WRITE_METER_LIST_ONE_PART      : s = tr("meter list")          ; break;
    case COMMAND_WRITE_METER_LIST_POLL_ON       : s = tr("poll on")             ; break;
    case COMMAND_WRITE_METER_LIST_POLL_OFF      : s = tr("poll off")            ; break;
    case COMMAND_WRITE_METER_LIST_DEL_NI        : s = tr("delete meters")       ; break;
    case COMMAND_WRITE_COMMANDS                 : s = tr("command")             ; break;
    case COMMAND_WRITE_DA_SERVICE_SETT          : s = tr("direct access")       ; break;
    case COMMAND_WRITE_PEREDAVATOR_AC_SETT      : s = tr("direct access active client") ; break;
    case COMMAND_WRITE_DA_OPEN_CLOSE            : s = tr("open/close direct access over matilda protocol"); break;
    case COMMAND_WRITE_DA_DATA_2_COORDINATOR    : break;
    case COMMAND_WRITE_FRWRD_SETT               : s = tr("forward settings")    ; break;

        //protocol v2
        ///LED LAMP
//    case COMMAND_WRITE_LEDLAMPLIST_FRAMED       : s = tr("LED Lamp List")       ; break;
//    case COMMAND_WRITE_GROUP_SCHEDULE           : s = tr("Group Schedule")      ; break;
//    case COMMAND_WRITE_POWER_RELAY_SETT         : s = tr("Firefly: Relay")      ; break;
//    case COMMAND_WRITE_FIREFLY_MODE_SETT        : s = tr("Firefly: Modes")      ; break;


        //ROOT: ONLY WRITE
    case COMMAND_WRITE_FIRST                    : break;
    case COMMAND_WRITE_UPGRADE                  : s = tr("upgrade")             ; break;

    case COMMAND_WRITE_GPRS_SETT                : s = tr("GSM settings")        ; break;
    case COMMAND_WRITE_REBOOT                   : s = tr("reboot command")      ; break;
    case COMMAND_WRITE_DAEMON_RESTART           : s = tr("daemon restart command"); break;
    case COMMAND_WRITE_FULL_UPGRADE             : s = tr("full upgrade")        ; break;
    case COMMAND_WRITE_ROOT_LOGIN_PSWD          : s = tr("administrator login and password"); break;
    case COMMAND_WRITE_GUEST_LOGIN_PSWD         : s = tr("guest login and password"); break;
    case COMMAND_WRITE_OPEARTOR_LOGIN_PSWD      : s = tr("operator login and password"); break;
    case COMMAND_WRITE_DROP_TABLE_GET_COUNT     : s = tr("drop table, get count"); break;
    case COMMAND_WRITE_DROP_TABLE               : s = tr("drop table")          ; break;
    case COMMAND_WRITE_DROP_TABLE_ALL           : s = tr("drop table all")      ; break;
    case COMMAND_WRITE_TIMEZONE                 : s = tr("timezone")            ; break;
    case COMMAND_WRITE_NTP_SETTINGS             : s = tr("NTP settings")        ; break;
    case COMMAND_WRITE_ABOUT_OBJECT             : s = tr("about object")        ; break;
    case COMMAND_WRITE_IP_FILTER_SETT           : s = tr("IP filter")           ; break;
    case COMMAND_WRITE_ERASE_ALL_DATA           : s = tr("erase all data")      ; break;
    case COMMAND_WRITE_MATILDA_AC_SETT          : s = tr("matilda active socket"); break;
    case COMMAND_WRITE_DELETE_BACKUP_FILE       : s = tr("delete backup file"); break;
    case COMMAND_WRITE_BACK_IN_TIME             : s = tr("back in time")        ; break;
    case COMMAND_WRITE_CREATE_BACK_IN_TIME      : s = tr("create backup file"); break;
    case COMMAND_WRITE_UDP_BEACON               : s = tr("UDP beacon")          ; break;

    case COMMAND_WRITE_ZIGBEE_SETT              : s = tr("ZigBee settings")     ; break;
    case COMMAND_WRITE_TCP_SETT                 : s = tr("TCP settings")        ; break;
    case COMMAND_WRITE_COMMAND2BASH             : s = tr("Simple BASH")         ; break;


    case COMMAND_GET_BACKUP_FILE                : s = tr("get backup file")     ; break;
    case COMMAND_PUSH_BACKUP_FILE_AND_APPLY     : s = tr("push backup file")    ; break;
    }

    if(command >= COMMAND_READ_SYSTEM_SETTINGS)
        return (command > COMMAND_WRITE_FIRST_4_OPERATOR) ? tr("Write: %1").arg(s) : tr("Read: %1").arg(s);

    return s;
}
//-------------------------------------------------------------------------------------------------------------------------------------
QHash<QString, QString> ShowMessHelper::columnKey2humanLang()
{
    QStringList lKey = QString("A+ A- R+ R-").split(" ");
//    QStringList lHumanKeyMetric = tr("kW kW kvar kvar").split(" ");
    QStringList lHumanKeyMetric2 = tr("kWh kWh kvarh kvarh").split(" ");

    QStringList lHumanKey = tr("A+;A-;R+;R-").split(";");
    QHash<QString,QString> hKeyHuman;

    for(int i = 0; i < 4; i++){
        hKeyHuman.insert(lKey.at(i), QString("%1 [%2]").arg(lHumanKey.at(i)).arg(lHumanKeyMetric2.at(i)));

        hKeyHuman.insert(QString("T0_%1").arg(lKey.at(i)), tr("∑T %1 [%2]").arg(lHumanKey.at(i)).arg(lHumanKeyMetric2.at(i)));
        for(int t = 1; t < MAX_TARIFF_COUNT; t++){
            hKeyHuman.insert(QString("T%1_%2").arg(t).arg(lKey.at(i)), tr("T%1 %2 [%3]").arg(t).arg(lHumanKey.at(i)).arg(lHumanKeyMetric2.at(i)));
        }
    }
    lKey = QString("F evnt_code comment date_time meter_sn meter_ni meter_model intrvl memo").split(" ");
    lHumanKey = tr("Frequency [Hz];Event code;Comment;Date time;Serial Number;Meter Addres;Meter model;Interval;Memo").split(";");

    lKey.append(QString("crdnts"));
    lHumanKey.append(tr("Coordinates"));

    lKey.append(QString("Modem NI;Model;Poll Code;Time;Mess. Count;Write/Read byte;Elapsed (s);Last retry").split(";"));
    lHumanKey.append(tr("Modem NI;Model;Poll Code;Time;Mess. Count;Write/Read byte;Elapsed (s);Last retry").split(";"));


    QStringList listEnrg = QString("U I P Q cos_f").split(" ");
    QStringList listPhs = QString("A B C").split(" ");

    QStringList listEnrgHuman = tr("U;I;P;Q;Cos φ").split(";");
    QStringList listPhsHuman = tr("Phs.A;Phs. B;Phs. C").split(";");
    QStringList listMetricHuman = tr("V;A;kW;kvar;").split(";");


    for(int i = 0, iMax = listEnrg.size(), jMax = listPhs.size(); i < iMax; i++){
        for(int j = 0; j < jMax; j++){
            lKey.append(listEnrg.at(i) + listPhs.at(j));
            if(listMetricHuman.at(i).isEmpty())
                lHumanKey.append(QString("%1 %2").arg(listEnrgHuman.at(i)).arg(listPhsHuman.at(j)) );
            else
                lHumanKey.append(QString("%1 %2 [%3]").arg(listEnrgHuman.at(i)).arg(listPhsHuman.at(j)).arg(listMetricHuman.at(i)) );


        }
    }

    for(int i = 0, iMax = lKey.size(), iMax2 = lHumanKey.size(); i < iMax && i < iMax2; i++)
        hKeyHuman.insert(lKey.at(i), lHumanKey.at(i));




    return hKeyHuman;
}
//-------------------------------------------------------------------------------------------------------------------------------------
QHash<QString, QString> ShowMessHelper::status2human()
{
    QStringList lMachine = QString("ready;closing;closed;connecting").split(";");
    QStringList lHuman = tr("Ready;Closing;Closed;Connecting").split(";");
    QHash<QString, QString> h;
    for(int i = 0, iMax = lMachine.size(), iMax2 = lHuman.size(); i < iMax && i < iMax2; i++)
        h.insert(lMachine.at(i), lHuman.at(i));
    return h;
}
//-------------------------------------------------------------------------------------------------------------------------------------
QHash<QString, QString> ShowMessHelper::region2human()
{
    return machine2human(QString("Africa;America;Antarctica;Arctic;Asia;Atlantic;Australia;Europe;Indian;Pacific;UTC").split(";"),
                         tr("Africa;America;Antarctica;Arctic;Asia;Atlantic;Australia;Europe;Indian;Pacific;UTC").split(";"));
}
//-------------------------------------------------------------------------------------------------------------------------------------
QHash<QString, QString> ShowMessHelper::city2human()
{
    return machine2human(QString("Abidjan;Accra;Adak;Addis_Ababa;Adelaide;Aden;Algiers;Almaty;Amman;Amsterdam;Anadyr;Anchorage;Andorra;Anguilla;Antananarivo;Antigua;Apia;Aqtau;Aqtobe;Araguaina;Aruba;Ashgabat;Asmara;Astrakhan;Asuncion;Athens;Atikokan;Auckland;Azores;"
                                 "Baghdad;Bahia;Bahia_Banderas;Bahrain;Baku;Bamako;Bangkok;Bangui;Banjul;Barbados;Barnaul;Beirut;Belem;Belgrade;Belize;Berlin;Bermuda;Beulah;Bishkek;Bissau;Blanc-Sablon;Blantyre;Boa_Vista;Bogota;Boise;Bougainville;Bratislava;Brazzaville;Brisbane;Broken_Hill;Brunei;Brussels;Bucharest;Budapest;Buenos_Aires;Bujumbura;Busingen;"
                                 "Cairo;Cambridge_Bay;Campo_Grande;Canary;Cancun;Cape_Verde;Caracas;Casablanca;Casey;Catamarca;Cayenne;Cayman;Center;Ceuta;Chagos;Chatham;Chicago;Chihuahua;Chisinau;Chita;Choibalsan;Christmas;Chuuk;Cocos;Colombo;Comoro;Conakry;Copenhagen;Cordoba;Costa_Rica;Creston;Cuiaba;Curacao;Currie;"
                                 "Dakar;Damascus;Danmarkshavn;Dar_es_Salaam;Darwin;Davis;Dawson;Dawson_Creek;Denver;Detroit;Dhaka;Dili;Djibouti;Dominica;Douala;Dubai;Dublin;DumontDUrville;Dushanbe;"
                                 "Easter;Edmonton;Efate;Eirunepe;El_Aaiun;El_Salvador;Enderbury;Eucla;"
                                 "Fakaofo;Faroe;Fiji;Fort_Nelson;Fortaleza;Freetown;Funafuti;"
                                 "Gaborone;Galapagos;Gambier;Gaza;Gibraltar;Glace_Bay;Godthab;Goose_Bay;Grand_Turk;Grenada;Guadalcanal;Guadeloupe;Guam;Guatemala;Guayaquil;Guernsey;Guyana;"
                                 "Halifax;Harare;Havana;Hebron;Helsinki;Hermosillo;Ho_Chi_Minh;Hobart;Hong_Kong;Honolulu;Hovd;"
                                 "Indianapolis;Inuvik;Iqaluit;Irkutsk;Isle_of_Man;Istanbul;Jakarta;"
                                 "Jamaica;Jayapura;Jersey;Jerusalem;Johannesburg;Johnston;Juba;Jujuy;Juneau;"
                                 "Kabul;Kaliningrad;Kamchatka;Kampala;Karachi;Kathmandu;Kerguelen;Khandyga;Khartoum;Kiev;Kigali;Kinshasa;Kiritimati;Kirov;Knox;Kolkata;Kosrae;Kralendijk;Krasnoyarsk;Kuala_Lumpur;Kuching;Kuwait;Kwajalein;"
                                 "La_Paz;La_Rioja;Lagos;Libreville;Lima;Lindeman;Lisbon;Ljubljana;Lome;London;Longyearbyen;Lord_Howe;Los_Angeles;Louisville;Lower_Princes;Luanda;Lubumbashi;Lusaka;Luxembourg;"
                                 "Macau;Maceio;Macquarie;Madeira;Madrid;Magadan;Mahe;Majuro;Makassar;Malabo;Maldives;Malta;Managua;Manaus;Manila;Maputo;Marengo;Mariehamn;Marigot;Marquesas;Martinique;Maseru;Matamoros;Mauritius;Mawson;Mayotte;Mazatlan;Mbabane;McMurdo;Melbourne;Mendoza;Menominee;Merida;Metlakatla;Mexico_City;Midway;Minsk;Miquelon;Mogadishu;Monaco;Moncton;Monrovia;Monterrey;Montevideo;Monticello;Montserrat;Moscow;Muscat;"
                                 "Nairobi;Nassau;Nauru;Ndjamena;New_Salem;New_York;Niamey;Nicosia;Nipigon;Niue;Nome;Norfolk;Noronha;Nouakchott;Noumea;Novokuznetsk;Novosibirsk;"
                                 "Ojinaga;Omsk;Oral;Oslo;Ouagadougou;"
                                 "Pago_Pago;Palau;Palmer;Panama;Pangnirtung;Paramaribo;Paris;Perth;Petersburg;Phnom_Penh;Phoenix;Pitcairn;Podgorica;Pohnpei;Pontianak;Port_Moresby;Port_of_Spain;Port-au-Prince;Porto_Velho;Porto-Novo;Prague;Puerto_Rico;Pyongyang;"
                                 "Qatar;Qyzylorda;"
                                 "Rainy_River;Rangoon;Rankin_Inlet;Rarotonga;Recife;Regina;Resolute;Reunion;Reykjavik;Riga;Rio_Branco;Rio_Gallegos;Riyadh;Rome;Rothera;"
                                 "Saipan;Sakhalin;Salta;Samara;Samarkand;San_Juan;San_Luis;San_Marino;Santarem;Santiago;Santo_Domingo;Sao_Paulo;Sao_Tome;Sarajevo;Scoresbysund;Seoul;Shanghai;Simferopol;Singapore;Sitka;Skopje;Sofia;South_Georgia;Srednekolymsk;St_Barthelemy;St_Helena;St_Johns;St_Kitts;St_Lucia;St_Thomas;St_Vincent;Stanley;Stockholm;Swift_Current;Sydney;Syowa;"
                                 "Tahiti;Taipei;Tallinn;Tarawa;Tashkent;Tbilisi;Tegucigalpa;Tehran;Tell_City;Thimphu;Thule;Thunder_Bay;Tijuana;Tirane;Tokyo;Tomsk;Tongatapu;Toronto;Tortola;Tripoli;Troll;Tucuman;Tunis;"
                                 "Ulaanbaatar;Ulyanovsk;Urumqi;Ushuaia;Ust-Nera;Uzhgorod;"
                                 "Vaduz;Vancouver;Vatican;Vevay;Vienna;Vientiane;Vilnius;Vincennes;Vladivostok;Volgograd;Vostok;"
                                 "Wake;Wallis;Warsaw;Whitehorse;Winamac;Windhoek;Winnipeg;"
                                 "Yakutat;Yakutsk;Yekaterinburg;Yellowknife;Yerevan;"
                                 "Zagreb;Zaporozhye;Zurich").split(";"),

                         tr("Abidjan;Accra;Adak;Addis_Ababa;Adelaide;Aden;Algiers;Almaty;Amman;Amsterdam;Anadyr;Anchorage;Andorra;Anguilla;Antananarivo;Antigua;Apia;Aqtau;Aqtobe;Araguaina;Aruba;Ashgabat;Asmara;Astrakhan;Asuncion;Athens;Atikokan;Auckland;Azores;"
                            "Baghdad;Bahia;Bahia_Banderas;Bahrain;Baku;Bamako;Bangkok;Bangui;Banjul;Barbados;Barnaul;Beirut;Belem;Belgrade;Belize;Berlin;Bermuda;Beulah;Bishkek;Bissau;Blanc-Sablon;Blantyre;Boa_Vista;Bogota;Boise;Bougainville;Bratislava;Brazzaville;Brisbane;Broken_Hill;Brunei;Brussels;Bucharest;Budapest;Buenos_Aires;Bujumbura;Busingen;"
                            "Cairo;Cambridge_Bay;Campo_Grande;Canary;Cancun;Cape_Verde;Caracas;Casablanca;Casey;Catamarca;Cayenne;Cayman;Center;Ceuta;Chagos;Chatham;Chicago;Chihuahua;Chisinau;Chita;Choibalsan;Christmas;Chuuk;Cocos;Colombo;Comoro;Conakry;Copenhagen;Cordoba;Costa_Rica;Creston;Cuiaba;Curacao;Currie;"
                            "Dakar;Damascus;Danmarkshavn;Dar_es_Salaam;Darwin;Davis;Dawson;Dawson_Creek;Denver;Detroit;Dhaka;Dili;Djibouti;Dominica;Douala;Dubai;Dublin;DumontDUrville;Dushanbe;"
                            "Easter;Edmonton;Efate;Eirunepe;El_Aaiun;El_Salvador;Enderbury;Eucla;"
                            "Fakaofo;Faroe;Fiji;Fort_Nelson;Fortaleza;Freetown;Funafuti;"
                            "Gaborone;Galapagos;Gambier;Gaza;Gibraltar;Glace_Bay;Godthab;Goose_Bay;Grand_Turk;Grenada;Guadalcanal;Guadeloupe;Guam;Guatemala;Guayaquil;Guernsey;Guyana;"
                            "Halifax;Harare;Havana;Hebron;Helsinki;Hermosillo;Ho_Chi_Minh;Hobart;Hong_Kong;Honolulu;Hovd;"
                            "Indianapolis;Inuvik;Iqaluit;Irkutsk;Isle_of_Man;Istanbul;Jakarta;"
                            "Jamaica;Jayapura;Jersey;Jerusalem;Johannesburg;Johnston;Juba;Jujuy;Juneau;"
                            "Kabul;Kaliningrad;Kamchatka;Kampala;Karachi;Kathmandu;Kerguelen;Khandyga;Khartoum;Kiev;Kigali;Kinshasa;Kiritimati;Kirov;Knox;Kolkata;Kosrae;Kralendijk;Krasnoyarsk;Kuala_Lumpur;Kuching;Kuwait;Kwajalein;"
                            "La_Paz;La_Rioja;Lagos;Libreville;Lima;Lindeman;Lisbon;Ljubljana;Lome;London;Longyearbyen;Lord_Howe;Los_Angeles;Louisville;Lower_Princes;Luanda;Lubumbashi;Lusaka;Luxembourg;"
                            "Macau;Maceio;Macquarie;Madeira;Madrid;Magadan;Mahe;Majuro;Makassar;Malabo;Maldives;Malta;Managua;Manaus;Manila;Maputo;Marengo;Mariehamn;Marigot;Marquesas;Martinique;Maseru;Matamoros;Mauritius;Mawson;Mayotte;Mazatlan;Mbabane;McMurdo;Melbourne;Mendoza;Menominee;Merida;Metlakatla;Mexico_City;Midway;Minsk;Miquelon;Mogadishu;Monaco;Moncton;Monrovia;Monterrey;Montevideo;Monticello;Montserrat;Moscow;Muscat;"
                            "Nairobi;Nassau;Nauru;Ndjamena;New_Salem;New_York;Niamey;Nicosia;Nipigon;Niue;Nome;Norfolk;Noronha;Nouakchott;Noumea;Novokuznetsk;Novosibirsk;"
                            "Ojinaga;Omsk;Oral;Oslo;Ouagadougou;"
                            "Pago_Pago;Palau;Palmer;Panama;Pangnirtung;Paramaribo;Paris;Perth;Petersburg;Phnom_Penh;Phoenix;Pitcairn;Podgorica;Pohnpei;Pontianak;Port_Moresby;Port_of_Spain;Port-au-Prince;Porto_Velho;Porto-Novo;Prague;Puerto_Rico;Pyongyang;"
                            "Qatar;Qyzylorda;"
                            "Rainy_River;Rangoon;Rankin_Inlet;Rarotonga;Recife;Regina;Resolute;Reunion;Reykjavik;Riga;Rio_Branco;Rio_Gallegos;Riyadh;Rome;Rothera;"
                            "Saipan;Sakhalin;Salta;Samara;Samarkand;San_Juan;San_Luis;San_Marino;Santarem;Santiago;Santo_Domingo;Sao_Paulo;Sao_Tome;Sarajevo;Scoresbysund;Seoul;Shanghai;Simferopol;Singapore;Sitka;Skopje;Sofia;South_Georgia;Srednekolymsk;St_Barthelemy;St_Helena;St_Johns;St_Kitts;St_Lucia;St_Thomas;St_Vincent;Stanley;Stockholm;Swift_Current;Sydney;Syowa;"
                            "Tahiti;Taipei;Tallinn;Tarawa;Tashkent;Tbilisi;Tegucigalpa;Tehran;Tell_City;Thimphu;Thule;Thunder_Bay;Tijuana;Tirane;Tokyo;Tomsk;Tongatapu;Toronto;Tortola;Tripoli;Troll;Tucuman;Tunis;"
                            "Ulaanbaatar;Ulyanovsk;Urumqi;Ushuaia;Ust-Nera;Uzhgorod;"
                            "Vaduz;Vancouver;Vatican;Vevay;Vienna;Vientiane;Vilnius;Vincennes;Vladivostok;Volgograd;Vostok;"
                            "Wake;Wallis;Warsaw;Whitehorse;Winamac;Windhoek;Winnipeg;"
                            "Yakutat;Yakutsk;Yekaterinburg;Yellowknife;Yerevan;"
                            "Zagreb;Zaporozhye;Zurich").split(";"));
}
//-------------------------------------------------------------------------------------------------------------------------------------
QHash<QString, QString> ShowMessHelper::lang2human()
{
    return machine2human(QString("uk;en;ru;pl").split(";"),
                         QString("Українська;English;Русский;Polskie").split(";"));
}
//-------------------------------------------------------------------------------------------------------------------------------------
QHash<QString, QString> ShowMessHelper::devCommand2human()
{
    return machine2human(QString("Stop All;Network Reset;Meter Journal;Voltage;Power;Now;End of day;End of month").split(";"),
                         tr("Stop All;Network Reset;Meter Journal;Voltage;Power;Now;End of day;End of month").split(";"));
}
//-------------------------------------------------------------------------------------------------------------------------------------
QHash<QString, QString> ShowMessHelper::manDevCommand2human()
{
    QStringList listDevCommand = QString("Custom date time, mask yyyy-MM-dd hh:mm|Depth interval,|"
                                         "space separeted NI list. Add model: <model>&<NI>. Add password: <NI>@<password>. Add model and password <model>&<NI>@<password>. If meter NI exists in device memory model and password can be omitted.|"
                                         "Max. tariff. By default|"
                                         "Space separeted energy keys. Default|"
                                         "Ignore existing poll data. Default: not ignored.").split("|");

    QStringList listDevCommandH = tr("Custom date time, mask yyyy-MM-dd hh:mm|Depth interval,|"
                                     "space separeted NI list. Add model: <model>&<NI>. Add password: <NI>@<password>. Add model and password <model>&<NI>@<password>. If meter NI exists in device memory model and password can be omitted.|"
                                     "Max. tariff. By default|"
                                     "Space separeted energy keys. Default|"
                                     "Ignore existing poll data. Default: not ignored.").split("|");

    listDevCommand.append(QString("TCP destination host and port, format <IPv4>:<Port> or [<IPv6>]:<Port>, <Port> value can be omitted, default value|"
                                  "Serial port: must be valid port name|"
                                  "Enable async mode. Default: disabled.").split("|"));


    listDevCommandH.append(tr("TCP destination host and port, format <IPv4>:<Port> or [<IPv6>]:<Port>, <Port> value can be omitted, default value|"
                                  "Serial port: must be valid port name|"
                                  "Enable async mode. Default: disabled.").split("|"));
    return machine2human(listDevCommand, listDevCommandH );
}
//-------------------------------------------------------------------------------------------------------------------------------------
QHash<QString, QString> ShowMessHelper::devState2human()
{
    QStringList listDevCommand = QString("Device|Direct Access|Interface|server:|client:|"
                                         "disabled|enabled|false|true|Running|"
                                         "Poll history|Poll state|Total/Ok/Error/Elapsed|(hh:mm:ss)|RamInfo|"
                                         "Uptime|ZigBee bytes w/r|Not started"
                                         "").split("|");


    QStringList listDevCommandH = tr("Device|Direct Access|Interface|server:|client:|"
                                   "disabled|enabled|false|true|Running|"
                                   "Poll history|Poll state|Total/Ok/Error/Elapsed|(hh:mm:ss)|RamInfo|"
                                   "Uptime|ZigBee bytes w/r|Not started"
                                   "").split("|");
    listDevCommand.append(QString("Direct Access Interface|Peredavator|Zbyrator|Matilda|(d hh:mm:ss)").split("|"));
    listDevCommandH.append(tr("Direct Access Interface|Peredavator|Zbyrator|Matilda|(d hh:mm:ss)").split("|"));

    listDevCommand.append(QString("Firefly").split("|"));
    listDevCommandH.append(tr("Firefly").split("|"));

    listDevCommand.append(QString("Suspended.|Resume.|Last code: |Wait for the signal ready. | [s]|"
                                  "Data from: |. model: |Date not valid. Current date: |Poll code |Checking the coordinator cofiguration.|"
                                  "Can't enter command mode.|Check API+ mode.|Can't configure the modem.|Enabling API+ mode.|Mode |"
                                  "Network reset").split("|"));
    listDevCommandH.append(tr("Suspended.|Resume.|Last code: |Wait for the signal ready. | [s]|"
                              "Data from: |. model: |Date not valid. Current date: |Poll code |Checking the coordinator cofiguration.|"
                              "Can't enter command mode.|Check API+ mode.|Can't configure the modem.|Enabling API+ mode.|Mode |"
                              "Network reset").split("|"));

    //Suspended. Poll code  Resume.

    listDevCommand.append(QString("Stopping|Channel|MHz|Network ID|Power level|Type|Coordinator|Updated|ZigBee modem|GSM modem|"
                                  "Band|Operator|Compact GSM|GSM with EGPRS|UTRAN with HSDPA|UTRAN with HSUPA|UTRAN with HSDPA and HSUPA|"
                                  "Not registered|Registered, home network|Not registered, but ME is currently searching a new operator to register to|"
                                  "Registration denied|Unknown|Registered, roaming").split("|"));

    listDevCommandH.append(tr("Stopping|Channel|MHz|Network ID|Power level|Type|Coordinator|Updated|ZigBee modem|GSM modem|"
                              "Band|Operator|Compact GSM|GSM with EGPRS|UTRAN with HSDPA|UTRAN with HSUPA|UTRAN with HSDPA and HSUPA|"
                              "Not registered|Registered, home network|Not registered, but ME is currently searching a new operator to register to|"
                              "Registration denied|Unknown|Registered, roaming").split("|"));


    return machine2human(listDevCommand, listDevCommandH );
}
//-------------------------------------------------------------------------------------------------------------------------------------
QHash<QString, QString> ShowMessHelper::machine2human(const QStringList &machineList, const QStringList &human)
{
    QHash<QString, QString> h;
    for(int i = 0, iMax = machineList.size(), iMax2 = human.size(); i < iMax && i < iMax2; i++)
        h.insert(machineList.at(i), human.at(i));
    return h;
}
//-------------------------------------------------------------------------------------------------------------------------------------
QStringList ShowMessHelper::devCode2txt(const int &devCode)
{
    QString s, ext;
    switch(devCode){
    case DEV_POLL: s = tr("Poll Device"); ext = tr("Modes: meter poll, gate, storage and data transfer"); break;
    case DEV_STOR: s = tr("Storage Device"); ext = tr("Modes: storage and data transfer"); break;
    case DEV_GATE: s = tr("Gate Device"); ext = tr("Modes: gate"); break;
    case DEV_SVAHA: s = tr("Svaha Device"); ext = tr("Modes: switching"); break;

    case DEV_POLL_EMULATOR_L0: s = tr("The emulator of Poll Device (Level 0)"); ext = tr("Modes: poll, storage and data transfer (stripped down version of the Poll Device)"); break;
    case DEV_POLL_EMULATOR_L1: s = tr("The emulator of Poll Device (Level 1)"); ext = tr("Modes: poll, gateway (via protocol Matilda), storage and and data transfer (stripped down version of the Poll Device)"); break;
    case DEV_POLL_EMULATOR_L2: s = tr("The emulator of Poll Device (Level 2)"); ext = tr("Modes: poll, gateway (during service), storage and and data transfer (stripped down version of the Poll Device)"); break;
    }

    if(!s.isEmpty())
        s.prepend(tr("Device type: "));
    return QStringList() << s << ext;

}
//-------------------------------------------------------------------------------------------------------------------------------------
QDateTime ShowMessHelper::tableName2DateTime(const QString &tableName, const qint32 addDays, const bool useLocal)
{
    if(tableName.mid(4,1) != "_")
        return QDateTime( QDate::fromString(tableName.mid(4, 10), "yyyy_MM_dd"), QTime::fromString(tableName.right(5), "hh_mm"),  useLocal ? Qt::LocalTime : Qt::UTC ).addDays(addDays);
    else
        return QDateTime( QDate::fromString(tableName.left(10), "yyyy_MM_dd"), QTime::fromString(tableName.right(5), "hh_mm"), useLocal ? Qt::LocalTime : Qt::UTC ).addDays(addDays);
    //yyyy_MM_dd
}
//-------------------------------------------------------------------------------------------------------------------------------------
QDateTime ShowMessHelper::tableName2DateTime(const QString &tableName, const QString &dateFormat, const QString &timeFormat, const qint32 addDays)
{
    return QDateTime( QDate::fromString(tableName.left(dateFormat.length()), dateFormat), QTime::fromString(tableName.right(timeFormat.length()), timeFormat),  Qt::UTC ).addDays(addDays);
    //yyyy_MM_dd
}
//-------------------------------------------------------------------------------------------------------------------------------------
QDateTime ShowMessHelper::tableVal2DateTime(const QString &text, const QString &mask, const bool useLocal)
{
    //yyyy-MM-dd hh:mm:ss
    return QDateTime( QDate::fromString(text.left(10), "yyyy-MM-dd"), QTime::fromString( text.right(mask.length()), mask), useLocal ? Qt::LocalTime : Qt::UTC );
}
//-------------------------------------------------------------------------------------------------------------------------------------
QString ShowMessHelper::messFromCode(const int &messCode)
{
    QString mess;
    switch(messCode){
    case MESS_NO_TABLE: mess = tr("No table for this period("); break;
    case MESS_CORRUPTED_DATA: mess = addWithFontColorRed(tr("Corrupted data(")); break;
    case MESS_CORRUPTED_PACKET: mess = addWithFontColorRed(tr("Corrupted packet(")); break;
    case MESS_NO_ANSWER_FROM_DEVICE: mess = addWithFontColorRed(tr("No answer from device(")); break;
    case MESS_NO_DATA: mess = tr("Data not found("); break;
    case MESS_OPERATION_ABORTED: mess = tr("Operation aborted"); break;
    case MESS_CANT_CONNECT2DEV: mess = tr("Can't connect to device("); break;


    }
    return mess;
}
//-------------------------------------------------------------------------------------------------------------------------------------
QString ShowMessHelper::byte2humanRead(const qint64 &bytes)
{
    if(bytes > 10000000){
        return QString::number(bytes / 1000000) + tr("M");
    }else{
        if(bytes > 10000){
            return QString::number(bytes / 1000) + tr("K");
        }else{
            return QString::number(bytes);
        }
    }

}
//-------------------------------------------------------------------------------------------------------------------------------------
QString ShowMessHelper::addSpaceEvry(const QString &s, const int &evrySymb)
{
    if(evrySymb < 1 || s.isEmpty())
        return s;

    QStringList l;

    int iMax = s.length();
    int i = ( iMax % evrySymb);
    if(i != 0)
        l.append( s.mid(0, i ));

    for( ; i < iMax; i += evrySymb)
        l.append(s.mid(i, evrySymb));

    return l.join(" ");

}
//-------------------------------------------------------------------------------------------------------------------------------------
QString ShowMessHelper::str2hexStr(const QString &s)
{
    if(s.isEmpty())
        return "";

    bool ok;
    quint64 i = s.toULongLong(&ok);
    if(!ok)
        return "";

    return QString("0x %1").arg( addSpaceEvry(QString::number(i, 16), 2) );
}
//-------------------------------------------------------------------------------------------------------------------------------------
QString ShowMessHelper::prettyFileName4selFromDb(const QString &pName, const QDateTime &dtFrom, const QDateTime &dtTo, const bool &useLocalTime)
{
    QString s = useLocalTime ? dtTo.toLocalTime().toString("yyyy MM dd  hh mm") : dtTo.toString("yyyy MM dd  hh mm") ;
    QString abrev = timeZoneShortAbbreviation(  useLocalTime ? dtTo.toLocalTime() : dtTo.toUTC());
    if(!dtTo.isValid()){
        s = useLocalTime ? QDateTime::currentDateTime().toString("yyyy MM dd  hh mm") : QDateTime::currentDateTimeUtc().toString("yyyy MM dd  hh mm") ;
        s.append(tr(" L"));
        abrev = timeZoneShortAbbreviation(  useLocalTime ? QDateTime::currentDateTime().toLocalTime() : QDateTime::currentDateTimeUtc());
    }
    return tr("%1 Date from %2 to %3 %4")
            .arg(pName)
            .arg( useLocalTime ? dtFrom.toLocalTime().toString("yyyy MM dd  hh mm") : dtFrom.toUTC().toString("yyyy MM dd  hh mm") )
            .arg( s )
            .arg( abrev);

}
//-------------------------------------------------------------------------------------------------------------------------------------
QString ShowMessHelper::prettyFileName4selFromDb(const QVariantHash &h, const bool &useLocalTime)
{
    return prettyFileName4selFromDb(h.value("pName").toString(), h.value("dtFrom").toDateTime(), h.value("dtTo").toDateTime(), useLocalTime);
}
//-------------------------------------------------------------------------------------------------------------------------------------
QStringList ShowMessHelper::prettyTableShapka(const QVariantHash &h,  const QString &dateMask, const bool &useLocalTime)
{
    if(h.isEmpty() || dateMask.isEmpty())
        return QStringList();

    QDateTime dtTo = h.value("dtTo").toDateTime();
    QDateTime dtFrom = h.value("dtFrom").toDateTime();

    QString dtMask = dateMask + " hh:mm:ss";
    QString s = useLocalTime ? dtTo.toLocalTime().toString(dtMask) : dtTo.toUTC().toString(dtMask) ;
    if(!dtTo.isValid()){
        dtTo = QDateTime::currentDateTimeUtc();
        s = useLocalTime ? dtTo.toLocalTime().toString(dtMask) : dtTo.toUTC().toString(dtMask) ;
        s.append(tr(" L"));
    }

    QString tzAbrev = timeZoneShortAbbreviation( useLocalTime ? dtFrom.toLocalTime() : dtFrom.toUTC()) ;
    QString tzAbrev2 = timeZoneShortAbbreviation( useLocalTime ? dtTo.toLocalTime() : dtTo.toUTC()) ;
    QString sFrom = useLocalTime ? dtFrom.toLocalTime().toString(dtMask) : dtFrom.toString(dtMask);

    QString localDateMask = dateMask;
    localDateMask = localDateMask.replace(QString("yyyy"), tr("yyyy")).replace( QString("MM"), tr("mm") ).replace(QString("dd"), tr("dd")).toLower();

    QStringList l;
    l.append(tr("Profile: %1").arg(h.value("pName").toString()));
    l.append(tr("Date from: %1 %2").arg(sFrom).arg(tzAbrev));
    l.append(tr("Date to: %1 %2").arg(s).arg(tzAbrev2));
    l.append(tr("Date mask: %1").arg(localDateMask));
    return l;

}
//-------------------------------------------------------------------------------------------------------------------------------------
QString ShowMessHelper::matildaFileName(const QString &fileName)
{
    QString m = tr("Matilda");
    if(fileName.left(m.length()) == m)
        m.clear();
    else
        m.append(" ");
    return QString("%1%2 %3").arg(m).arg(fileName).arg(QDateTime::currentDateTime().toString( "yyyy MM dd  hh mm" ))   ;
}
//-------------------------------------------------------------------------------------------------------------------------------------
QString ShowMessHelper::timeZoneShortAbbreviation(const QDateTime &dt)
{    
    QString retVal = "";

#ifndef Q_OS_LINUX
    QString tzId = dt.timeZone().id();
    QFile file(dt.isDaylightTime() ? ":/IANA2abbreviationDST.tz" : ":/IANA2abbreviation.tz");
    if(file.open(QFile::ReadOnly)){
        QStringList l = QString(file.readAll()).split("\n");
        file.close();
        for(int i = 0, iMax = l.size(); i < iMax; i++){
            QStringList ll = l.at(i).split(";");
            if(ll.size() != 2 || ll.first().isEmpty())
                continue;
            if(ll.last() == tzId){
                retVal = ll.first();
                break;
            }
        }
    }
#else
    retVal = dt.timeZoneAbbreviation();
#endif

    if(retVal.isEmpty())
        retVal = dt.timeZone().displayName(dt, QTimeZone::OffsetName);

    return retVal;
}
//-------------------------------------------------------------------------------------------------------------------------------------
QString ShowMessHelper::eventCode2human(const int &event)
{
    QStringList list;
    list.append(tr("All"));
    list.append(tr("Other"));
    list.append(tr("Meter case open"));
    list.append(tr("Terminal box open"));
    list.append(tr("Bat. Low"));
    list.append(tr("EEPROM Error"));
    list.append(tr("Magnet"));
    list.append(tr("Hardware"));
    list.append(tr("Access"));
    list.append(tr("Relay"));
    list.append(tr("Internal error"));
    list.append(tr("Time corrected"));
    list.append(tr("Time need to correct"));

    list.append(tr("Unknown protocol"));
    list.append(tr("No event for this day"));
    list.append(tr("Meter done"));

    list.append(tr("Voltage"));
    list.append(tr("Changed configuration"));
    list.append(tr("Climate"));
    list.append(tr("Metering"));
    list.append(tr("DST state"));
    list.append(tr("On/Off"));
    list.append(tr("Parameters fault"));

    list.append(tr("Billing"));
    list.append(tr("Module open"));
    ////event4matilda  UC



    if(list.size() > event)
        return list.at(event);
    else{
        QString s = tr("System event");

        list.clear();
        list.append(tr("Need to correct time"));
        list.append(tr("Correct time: error"));
        list.append(tr("Correct time: done"));
        list.append(tr("Invalid argument"));

        int pos = event - 51;//ZBR_EVENT_DATETIME_NEED2CORR

        if(pos >= 0 && pos < list.size())
            s = list.at(pos);


        return s;
    }
}
//-------------------------------------------------------------------------------------------------------------------------------------
QHash<QString, QString> ShowMessHelper::meterEv2human()
{
    QStringList list = QString("meter done;No event;Time changed;Unknown event;Access unlocked.;"
                               "Access denied, key.;Opened;Closed;Magnet ON;Magnet OFF;"
                               "Time changed;CMD Relay: load on;CMD Realay: load off;Relay: load on;Relay: load off;"
                               "WDT: restart;RTC ready;RTC error;RTC bat. ok;RTC bat. fault;"
                               "Relay2: load on;Relay2: load off;Bad EEPROM;Bad FRAM;Relay: hard bad off;"
                               "Relay: hard on;Relay: hard bad on;Relay: hard off;Relay: load off (high load P);"
                               "Alternating magnet ON;Alternating magnet OFF").split(";", QString::SkipEmptyParts);
    QStringList listH = tr("meter done;No event;Time changed;Unknown event;Access unlocked.;"
                               "Access denied, key.;Opened;Closed;Magnet ON;Magnet OFF;"
                               "Time changed;CMD Relay: load on;CMD Realay: load off;Relay: load on;Relay: load off;"
                               "WDT: restart;RTC ready;RTC error;RTC bat. ok;RTC bat. fault;"
                               "Relay2: load on;Relay2: load off;Bad EEPROM;Bad FRAM;Relay: hard bad off;"
                               "Relay: hard on;Relay: hard bad on;Relay: hard off;Relay: load off (high load P);"
                               "Alternating magnet ON;Alternating magnet OFF").split(";", QString::SkipEmptyParts);
    QHash<QString, QString> h;
    for(int i = 0, iMax = list.size(), iMax2 = listH.size(); i < iMax && i < iMax2; i++)
        h.insert( QString("<%1>").arg(list.at(i)), QString("<%1>").arg(listH.at(i)));

    return h;
}
//-------------------------------------------------------------------------------------------------------------------------------------
QHash<QString, QString> ShowMessHelper::matildaEv2human()
{
    QStringList list = QString("Meter date;Meter new date").split(";", QString::SkipEmptyParts);
    QStringList listH = tr("Meter date;Meter new date").split(";", QString::SkipEmptyParts);

    list.append("Correct date: fail");
    listH.append(tr("Correct date: fail"));

    QHash<QString, QString> h;
    for(int i = 0, iMax = list.size(), iMax2 = listH.size(); i < iMax && i < iMax2; i++)
        h.insert( list.at(i), listH.at(i));

    return h;
}
//-------------------------------------------------------------------------------------------------------------------------------------
QString ShowMessHelper::translateString(QString inStr, const QHash<QString, QString> &hDev2human)
{
    QList<QString> lk = hDev2human.keys();
    for(int i = 0, iMax = lk.size(); i < iMax; i++){
        if(inStr.contains(lk.at(i))){
            inStr.replace(lk.at(i), hDev2human.value(lk.at(i), lk.at(i)));
        }
    }
    return inStr;
}
//-------------------------------------------------------------------------------------------------------------------------------------
QString ShowMessHelper::meterEvCode2humanV2(const int &ucmEventCode)
{
    QString localEvntCode;
    switch (ucmEventCode) {

    ///MTX3 Section
    case UCM_EV_ENRG_T1_FAULT      : localEvntCode = tr("Energy T%1 registers broken").arg("1")                                           ; break;  //0x01 //значення накоплюючих регістрів тарифу 1 втрачені
    case UCM_EV_ENRG_T2_FAULT      : localEvntCode = tr("Energy T%1 registers broken").arg("2")                                           ; break;  //0x02 //значення накоплюючих регістрів тарифу 2 втрачені
    case UCM_EV_ENRG_T3_FAULT      : localEvntCode = tr("Energy T%1 registers broken").arg("3")                                           ; break;  //0x03 //значення накоплюючих регістрів тарифу 3 втрачені
    case UCM_EV_ENRG_T4_FAULT      : localEvntCode = tr("Energy T%1 registers broken").arg("4")                                           ; break;  //0x04 //значення накоплюючих регістрів тарифу 4 втрачені
    case UCM_EV_ACCESS_LOCKED        : localEvntCode = tr("Access: Locked")                                                                 ; break;  //0x11  //доступ закритий до кінця доби із-за помилки ключа доступу
    case UCM_EV_ACCESS_UNLOCKED      : localEvntCode = tr("Access: Unlocked")                                                               ; break;  //0x12  //доступ відкритий ( була помилки ключа доступу)
    case UCM_EV_ERR_ACCESS           : localEvntCode = tr("Access: bad key")                                                                ; break;  //0x13 //Неправильний ключ
    case UCM_EV_CASE_OPEN            : localEvntCode = tr("Meter cover: %1, %2").arg(tr("opened")).arg(tr("wait for close"))                ; break;  //0x14 //Корпус лічильника відкритий
    case UCM_EV_CASE_CLOSE           : localEvntCode = tr("Meter cover: %1").arg(tr("closed"))                                              ; break;  //0x15 //Корпус лічильника закритий
    case UCM_EV_MAGNETIC_ON          : localEvntCode = tr("Magnetic field, %1").arg(tr("begining"))                                         ; break;  //0x16 //виявлено наявність впливу постійного магнітного поля
    case UCM_EV_MAGNETIC_OFF         : localEvntCode = tr("Magnetic field, %1").arg(tr("end"))                                              ; break;  //0x17 //виявлено закінчення впливу постійного магнітного поля
    case UCM_EV_CHANGE_ACCESS_KEY_0  : localEvntCode = tr("Changed access key L%1").arg("0")                                                ; break;  //0x20 //Ключ доступу рівня 0 змінено
    case UCM_EV_CHANGE_ACCESS_KEY_1  : localEvntCode = tr("Changed access key L%1").arg("1")                                                ; break;  //0x21 //Ключ доступу рівня 1 змінено
    case UCM_EV_CHANGE_ACCESS_KEY_2  : localEvntCode = tr("Changed access key L%1").arg("2")                                                ; break;  //0x22 //Ключ доступу рівня 2 змінено
    case UCM_EV_CHANGE_ACCESS_KEY_3  : localEvntCode = tr("Changed access key L%1").arg("3")                                                ; break;  //0x23 //Ключ доступу рівня 3 змінено
    case UCM_EV_CHANGE_PAR_LOCAL     : localEvntCode = tr("Configuration: changed %1").arg(tr("local"))                                     ; break;  //0x24 //параметри змінено локально
    case UCM_EV_CHANGE_PAR_REMOTE    : localEvntCode = tr("Configuration: changed %1").arg(tr("remote"))                                    ; break;  //0x25 //параметри змінено локально !!!???
    case UCM_EV_CMD_CHANGE_TIME      : localEvntCode = tr("Received command to change the %1").arg(tr("time"))                              ; break;  //0x26 //отримана команда зміни часу, час змінено
    case UCM_EV_CMD_RELAY_ON         : localEvntCode = tr("Received command to switch the relay %1").arg(tr("on"))                          ; break;  //0x27 //отримана команда увімкнення реле
    case UCM_EV_CMD_RELAY_OFF        : localEvntCode = tr("Received command to switch the relay %1").arg(tr("off"))                         ; break;  //0x28 //отримана команда вимкнення реле
    case UCM_EV_ENERGY_REG_OVERFLOW  : localEvntCode = tr("Energy register overflow")                                                       ; break;  //0x31 //переповнення накоплюючого регістру енергії
    case UCM_EV_CHANGE_TARIFF_TBL    : localEvntCode = tr("Changed the tariff table")                                                       ; break;  //0x32 //тарифний план змінено
    case UCM_EV_SET_TARIFF_TBL       : localEvntCode = tr("Set tariff table")                                                               ; break;  //0x33 //отриманий новий тарифний план
    case UCM_EV_SUMMER_TIME          : localEvntCode = tr("DST is active: %1").arg(tr("yes"))                                               ; break;  //0x34 //прехід на літній час
    case UCM_EV_WINTER_TIME          : localEvntCode = tr("DST is active: %1").arg(tr("no"))                                                ; break;  //0x35 //перехід на зимовий час
    case UCM_EV_RELAY_ON             : localEvntCode = tr("Relay: load %1").arg(tr("on"))                                                   ; break;  //0x36 //реле вимкнено
    case UCM_EV_RELAY_OFF            : localEvntCode = tr("Relay: load %1").arg(tr("off"))                                                  ; break;  //0x37 //реле умвімкнено
    case UCM_EV_RESTART              : localEvntCode = tr("Meter restart")                                                                  ; break;  //0x38 //рестарт ПЗ контролера
    case UCM_EV_WD_RESTART           : localEvntCode = tr("WDT: %1").arg(tr("Meter restart"))                                               ; break;  //0x39 //рестарт по Сторожовику

    case UCM_EV_VA_MAX_OK            : localEvntCode = tr("Voltage L%1: normal after %2").arg("1").arg(tr("overvoltage"))                   ; break;  //0x40 //Відновлення нормальної напруги L1 після підвищеної напруги
    case UCM_EV_VA_MAX_OVER          : localEvntCode = tr("Voltage L%1: %2, %3").arg("1").arg(tr("overvoltage")).arg(tr("wait for normal")) ; break;  //0x41 //напруга L1 вище порогу максимальної напруги
    case UCM_EV_VA_MIN_OK            : localEvntCode = tr("Voltage L%1: normal after %2").arg("1").arg(tr("undervoltage"))                  ; break;  //0x42 //Відновлення нормальної напруги L1 після пониженої напруги
    case UCM_EV_VA_MIN_UNDER         : localEvntCode = tr("Voltage L%1: %2, %3").arg("1").arg(tr("undervoltage")).arg(tr("wait for normal")); break;  //0x43 //Напруга L1 нище порогу мінімальної напруги

    case UCM_EV_VB_MAX_OK            : localEvntCode = tr("Voltage L%1: normal after %2").arg("2").arg(tr("overvoltage"))                   ; break;  //0x44 //Відновлення нормальної напруги L2 після підвищеної напруги
    case UCM_EV_VB_MAX_OVER          : localEvntCode = tr("Voltage L%1: %2, %3").arg("2").arg(tr("overvoltage")).arg(tr("wait for normal")) ; break;  //0x45 //напруга L2 вище порогу максимальної напруги
    case UCM_EV_VB_MIN_OK            : localEvntCode = tr("Voltage L%1: normal after %2").arg("2").arg(tr("undervoltage"))                  ; break;  //0x46 //Відновлення нормальної напруги L2 після пониженої напруги
    case UCM_EV_VB_MIN_UNDER         : localEvntCode = tr("Voltage L%1: %2, %3").arg("2").arg(tr("undervoltage")).arg(tr("wait for normal")); break;  //0x47 //Напруга L2 нище порогу мінімальної напруги

    case UCM_EV_VC_MAX_OK            : localEvntCode = tr("Voltage L%1: normal after %2").arg("3").arg(tr("overvoltage"))                   ; break;  //0x48 //Відновлення нормальної напруги L3 після підвищеної напруги
    case UCM_EV_VC_MAX_OVER          : localEvntCode = tr("Voltage L%1: %2, %3").arg("1").arg(tr("overvoltage")).arg(tr("wait for normal")) ; break;  //0x49 //напруга L3 вище порогу максимальної напруги
    case UCM_EV_VC_MIN_OK            : localEvntCode = tr("Voltage L%1: normal after %2").arg("3").arg(tr("undervoltage"))                  ; break;  //0x4A //Відновлення нормальної напруги L3 після пониженої напруги
    case UCM_EV_VC_MIN_UNDER         : localEvntCode = tr("Voltage L%1: %2, %3").arg("3").arg(tr("undervoltage")).arg(tr("wait for normal")); break;  //0x4B //Напруга L3 нище порогу мінімальної напруги

    case UCM_EV_F_MAX_OK             : localEvntCode = tr("Frequency: normal after %1").arg(tr("high"))                                     ; break;  //0x4C //Відновлення нормальної частоти після підвищеної
    case UCM_EV_F_MAX_OVER           : localEvntCode = tr("Frequency: %1, %2").arg(tr("over")).arg(tr("wait for normal"))                   ; break;  //0x4D //Частота вижче порогу нормальної
    case UCM_EV_F_MIN_OK             : localEvntCode = tr("Frequency: normal after %1").arg(tr("low"))                                      ; break;  //0x4E //Відновлення нормальної частоти після пониженої
    case UCM_EV_F_MIN_UNDER          : localEvntCode = tr("Frequency: %1, %2").arg(tr("under")).arg(tr("wait for normal"))                  ; break;  //0x4F //Частора нижче порогу нормальної

    case UCM_EV_T_MAX_OK             : localEvntCode = tr("Temperature: normal after %1").arg(tr("high"))                                   ; break;  //0x50 //Відновлення нормальної температури після підвищеної
    case UCM_EV_T_MAX_OVER           : localEvntCode = tr("Temperature: %1, %2").arg(tr("high")).arg(tr("wait for normal"))                 ; break;  //0x51 //Температура вище порогу максильної температури
    case UCM_EV_T_MIN_OK             : localEvntCode = tr("Temperature: normal after %1").arg(tr("low"))                                    ; break;  //0x52 //Відновлення нормальної температури після пониженої
    case UCM_EV_T_MIN_UNDER          : localEvntCode = tr("Temperature: %1, %2").arg(tr("low")).arg(tr("wait for normal"))                  ; break;  //0x53 //Температура нижче порогу нормальної

    case UCM_EV_IA_MAX_OK            : localEvntCode = tr("Current L%1: normal after %2").arg("1").arg(tr("high"))                          ; break;  //0x54 //Відновлення допустимого струму L1 після підвищеного
    case UCM_EV_IA_MAX_OVER          : localEvntCode = tr("Current L%1: %2, %3").arg("1").arg(tr("high")).arg(tr("wait for normal"))        ; break;  //0x55 //Струм L1 вище порогу допустимого
    case UCM_EV_IB_MAX_OK            : localEvntCode = tr("Current L%1: normal after %2").arg("2").arg(tr("high"))                          ; break;  //0x56 //Відновлення допустимого струму L2 після підвищеного
    case UCM_EV_IB_MAX_OVER          : localEvntCode = tr("Current L%1: %2, %3").arg("2").arg(tr("high")).arg(tr("wait for normal"))        ; break;  //0x57 //Струм L2 вище порогу допустимого
    case UCM_EV_IC_MAX_OK            : localEvntCode = tr("Current L%1: normal after %2").arg("3").arg(tr("high"))                          ; break;  //0x58 //Відновлення допустимого струму L3 після підвищеного
    case UCM_EV_IC_MAX_OVER          : localEvntCode = tr("Current L%1: %2, %3").arg("3").arg(tr("high")).arg(tr("wait for normal"))        ; break;  //0x59 //Струм L3 вище порогу допустимого

    case UCM_EV_PA_MAX_OK            : localEvntCode = tr("Power A+: normal after %1").arg(tr("high"))                                      ; break;  //0x5A //Відновлення допустимої активної споживаної потужності після підвищеної
    case UCM_EV_PA_MAX_OVER          : localEvntCode = tr("Power A+: %1, %2").arg(tr("high")).arg(tr("wait for normal"))                    ; break;  //0x5B //Активна споживана потужність вище максимальної
    case UCM_EV_PV_MAX_OK            : localEvntCode = tr("Power R+: normal after %1").arg(tr("high"))                                      ; break;  //0x5C //Відновлення допустимої реактивної споживаної потужності після підвищеної
    case UCM_EV_PV_MAX_OVER          : localEvntCode = tr("Power R+: %1, %2").arg(tr("high")).arg(tr("wait for normal"))                    ; break;  //0x5D //Реактивна спожива потужність вище максимальної

    case UCM_EV_IDIFF_OK             : localEvntCode = tr("Idiff: %1").arg(tr("ok"))                                                        ; break;  //0x5E //Відновлення допустимого дифференціального струму
    case UCM_EV_IDIFF_OVER           : localEvntCode = tr("Idiff: %1, %2").arg(tr("over")).arg(tr("wait for ok"))                           ; break;  //0x5F //Перевищення допустимого дифференціального струму
    case UCM_EV_CLOCK_OK             : localEvntCode = tr("Clock: %1").arg(tr("ok"))                                                        ; break;  //0x60 //Нормальний стан RTC відновлено
    case UCM_EV_CLOCK_FAULT          : localEvntCode = tr("Clock: %1, %2").arg(tr("error")).arg(tr("wait for ok"))                          ; break;  //0x61 //RTC не встановлені

    case UCM_EV_POWER_C_ON           : localEvntCode = tr("Power L%1: %2").arg("3").arg(tr("on"))                                           ; break;  //0x62 //Увімкнення напруги L3
    case UCM_EV_POWER_C_OFF          : localEvntCode = tr("Power L%1: %2").arg("3").arg(tr("off"))                                          ; break;  //0x63 //Вимкнення напруги L3
    case UCM_EV_POWER_B_ON           : localEvntCode = tr("Power L%1: %2").arg("2").arg(tr("on"))                                           ; break;  //0x64 //Увімкнення напруги Фази В
    case UCM_EV_POWER_B_OFF          : localEvntCode = tr("Power L%1: %2").arg("2").arg(tr("off"))                                          ; break;  //0x65 //Вимкнення напруги Фази В
    case UCM_EV_POWER_A_ON           : localEvntCode = tr("Power L%1: %2").arg("1").arg(tr("on"))                                           ; break;  //0x66 //Увімкнення напруги L1
    case UCM_EV_POWER_A_OFF          : localEvntCode = tr("Power L%1: %2").arg("1").arg(tr("off"))                                          ; break;  //0x67 //Вимкнення напруги L1

    case UCM_EV_BAT_OK               : localEvntCode = tr("RTC Bat.: %1").arg(tr("ok"))                                                     ; break;  //0x68 //Відновлення нормальної напруги батареї RTC
    case UCM_EV_BAT_FAULT            : localEvntCode = tr("RTC Bat.: %1").arg(tr("error"))                                                  ; break;  //0x69 //Понижена напруга батареї RTC
    case UCM_EV_CAL_OK               : localEvntCode = tr("Calibration: %1").arg(tr("parameters ok"))                                       ; break;  //0x6A //Параметри калібровки встановлені
    case UCM_EV_CAL_FAULT            : localEvntCode = tr("Calibration: %1").arg(tr("broken parameters"))                                   ; break;  //0x6B //Калібровочні параметри втрачені
    case UCM_EV_V_PAR_OK             : localEvntCode = tr("Vendor parameters %1").arg(tr("changed"))                                        ; break;
    case UCM_EV_V_PAR_FAULT          : localEvntCode = tr("Vendor parameters %1").arg(tr("broken"))                                         ; break;  //0x6D //Заводські параметри втрачено
    case UCM_EV_O_PAR_OK             : localEvntCode = tr("Operator parameters %1").arg(tr("changed"))                                      ; break;  //0x6E //Параметри встановлено
    case UCM_EV_O_PAR_FAULT          : localEvntCode = tr("Operator parameters %1").arg(tr("broken"))                                       ; break;  //0x6F //Параметри втрачено
    case UCM_EV_CHANGE_COR_TIME      : localEvntCode = tr("DST settings chaged")                                                            ; break;  //0x70 //Змінено параметри переходу на літній час
    case UCM_EV_CMD_RELAY_2_ON       : localEvntCode = tr("Received command to switch the relay #2 %1").arg(tr("on"))                       ; break;   //0x71 //Включення другого реле
    case UCM_EV_CMD_RELAY_2_OFF      : localEvntCode = tr("Received command to switch the relay #2 %1").arg(tr("off"))                      ; break;   //0x72 //Вимкнення другого реле
    case UCM_EV_CRSSZERO_ENT1       : localEvntCode = tr("Crosszero %1 T%2").arg("A+").arg("1")                                            ; break;  //0x73 //Перехід через 0 лічильника активної енергії по тарифу 1 при досягненні 1000000.00 кВт
    case UCM_EV_CRSSZERO_ENT2       : localEvntCode = tr("Crosszero %1 T%2").arg("A+").arg("2")                                            ; break;  //0x74 //Перехід через 0 лічильника активної енергії по тарифу 2 при досягненні 1000000.00 кВт
    case UCM_EV_CRSSZERO_ENT3       : localEvntCode = tr("Crosszero %1 T%2").arg("A+").arg("3")                                            ; break;  //0x75 //Перехід через 0 лічильника активної енергії по тарифу 3 при досягненні 1000000.00 кВт
    case UCM_EV_CRSSZERO_ENT4       : localEvntCode = tr("Crosszero %1 T%2").arg("A+").arg("4")                                            ; break;  //0x76 //Перехід через 0 лічильника активної енергії по тарифу 4 при досягненні 1000000.00 кВт
    case UCM_EV_CRSSZERO_VARi1      : localEvntCode = tr("Crosszero %1 T%2").arg("R+").arg("1")                                            ; break;  //0x77 //Перехід через 0 лічильника позитивної реактивної енергії по тарифу 1 при досягненні 1000000.00 кВар
    case UCM_EV_CRSSZERO_VARi2      : localEvntCode = tr("Crosszero %1 T%2").arg("R+").arg("2")                                            ; break;  //0x78 //Перехід через 0 лічильника позитивної реактивної енергії по тарифу 2 при досягненні 1000000.00 кВар
    case UCM_EV_CRSSZERO_VARi3      : localEvntCode = tr("Crosszero %1 T%2").arg("R+").arg("3")                                            ; break;   //0x79 //Перехід через 0 лічильника позитивної реактивної енергії по тарифу 3 при досягненні 1000000.00 кВар
    case UCM_EV_CRSSZERO_VARi4      : localEvntCode = tr("Crosszero %1 T%2").arg("R+").arg("4")                                            ; break;  //0x7A //Перехід через 0 лічильника позитивної реактивної енергії по тарифу 4 при досягненні 1000000.00 кВар
    case UCM_EV_CRSSZERO_VARe1      : localEvntCode = tr("Crosszero %1 T%2").arg("R-").arg("1")                                            ; break;  //0x7B //Перехід через 0 лічильника негативної реактивної енергії по тарифу 1 при досягненні 1000000.00 кВар
    case UCM_EV_CRSSZERO_VARe2      : localEvntCode = tr("Crosszero %1 T%2").arg("R-").arg("2")                                            ; break;  //0x7C //Перехід через 0 лічильника негативної реактивної енергії по тарифу 2 при досягненні 1000000.00 кВар
    case UCM_EV_CRSSZERO_VARe3      : localEvntCode = tr("Crosszero %1 T%2").arg("R-").arg("3")                                            ; break;  //0x7D //Перехід через 0 лічильника негативної реактивної енергії по тарифу 3 при досягненні 1000000.00 кВар
    case UCM_EV_CRSSZERO_VARe4      : localEvntCode = tr("Crosszero %1 T%2").arg("R-").arg("4")                                            ; break;  //0x7E //Перехід через 0 лічильника негативної реактивної енергії по тарифу 4 при досягненні 1000000.00 кВар
    case UCM_EV_CALFLAG_SET          : localEvntCode = tr("Calibration bit %1").arg(tr("set"))                                              ; break;  //0x7F //Калібровочний біт встановлено
    case UCM_EV_CALFLAG_RESET        : localEvntCode = tr("Calibration bit %1").arg(tr("reset"))                                            ; break;  //0x80 //Калібровочний біт скинено
    case UCM_EV_BAD_TEST_EEPROM      : localEvntCode = tr("Bad EEPROM")                                                                     ; break;   //0x81 //Тест EEPROM не пройшов
    case UCM_EV_BAD_TEST_FRAM        : localEvntCode = tr("Bad FRAM")                                                                       ; break;   //0x82 //Тест FRAM не пройшов
    case UCM_EV_SET_NEW_SALDO        : localEvntCode = tr("Received prepaid")                                                               ; break;  //0x83 //Отримана нова передоплата
    case UCM_EV_SALDO_PARAM_BAD      : localEvntCode = tr("Prepaid: broken parameters")                                                     ; break;  //0x84 //Параметри сальдо втрачені
    case UCM_EV_ACCPARAM_BAD         : localEvntCode = tr("Accumulation: %1").arg(tr("broken parameters"))                                  ; break;  //0x85 //Акумуляційні параметри втрачено
    case UCM_EV_ACCPARAM_EXT_BAD     : localEvntCode = tr("Accumulation ext: %1").arg(tr("broken parameters"))                              ; break;  //0x86 //Додаткові акумуляційні параметри втрачено
    case UCM_EV_CALC_PERIOD_BAD      : localEvntCode = tr("Calculation : %1").arg(tr("broken period"))                                      ; break;  //0x87 //Дані розрахункових періодів втрачено
    case UCM_EV_BLOCK_TARIF_BAD      : localEvntCode = tr("Tariff program: %1").arg(tr("broken"))                                           ; break;  //0x88 //Параметри блочних тарифів втрачено
    case UCM_EV_CALIBR_PARAM_BAD     : localEvntCode = tr("Сalibration: %1").arg(tr("broken parameters"))                                   ; break;  //0x89 //Значення калібровочних параметрів втрачено
    case UCM_EV_WINTER_SUMMER_BAD    : localEvntCode = tr("DST: %1").arg(tr("broken parameters"))                                           ; break;  //0x8A //Значення параметру переходу на зимовий/літній час втрачено
    case UCM_EV_OP_PARAM_BAD         : localEvntCode = tr("Operator settings: %1").arg(tr("broken parameters"))                             ; break;  //0x8B //Операторські параметри втрачено
    case UCM_EV_OP_PARAM_EXT_BAD     : localEvntCode = tr("Operator settings ext: %1").arg(tr("broken parameters"))                         ; break;  //0x8C //Операторські параметри 2 втрачено
    case UCM_EV_SALDO_EN_BAD         : localEvntCode = tr("Billing registers: %1").arg(tr("broken values"))                                 ; break;  //0x8D //Значення енергій для розрахунку сальдо втрачено
    case UCM_EV_TIME_CORRECT         : localEvntCode = tr("Time changed")                                                                   ; break;  //0x8E //Коррекція часу
    case UCM_EV_COEFF_TRANSF_CHANGE  : localEvntCode = tr("Transform coefficient changed")                                                  ; break;  //0x8F //Змінено коефіцієнти трансформації
    case UCM_EV_RELAY_HARD_BAD_OFF   : localEvntCode = tr("Relay: %1").arg(tr("turned off mechanically"))                                   ; break;   //0x90 //Реле вимкнено механічно
    case UCM_EV_RELAY_HARD_ON        : localEvntCode = tr("Relay: %1 after %2").arg("turned on").arg("turned off mechanically")             ; break;   //0x91 //Реле увімкнено після механічного впливу. Відновлено стан реле
    case UCM_EV_RELAY_HARD_BAD_ON    : localEvntCode = tr("Relay: %1").arg(tr("turned on mechanically"))                                    ; break;   //0x93 //Реле увімкнено механічно
    case UCM_EV_RELAY_HARD_OFF       : localEvntCode = tr("Relay: %1 after %2").arg("turned off").arg("turned on mechanically")             ; break;   //0x94 //Реле вимкнено після механічного впливу. Відновлено стан реле
    case UCM_EV_METER_TROUBLE        : localEvntCode = tr("Meter: internal error")                                                          ; break;   //0x95 //Збій лічильника
    case UCM_EV_CASE_KLEMA_OPEN      : localEvntCode = tr("Terminal box was %1, %2").arg(tr("opened")).arg(tr("wait for close"))            ; break;   //0x96 //Клемна коробка лічильника відкрита
    case UCM_EV_CASE_KLEMA_CLOSE     : localEvntCode = tr("Terminal box was %1").arg(tr("closed"))                                          ; break;   //0x97 //Клемна коробка лічильника закрита
    case UCM_EV_CHANGE_TARIFF_TBL_2  : localEvntCode = tr("Changed tariff table #%1").arg("2")                                              ; break;  //0x98 //Тарифний план 2 змінено
    case UCM_EV_CHANGE_TARIFF_TBL_3  : localEvntCode = tr("Changed tariff table #%1").arg("3")                                              ; break;  //0x99 //Тарифний план 3 змінено
    case UCM_EV_CASE_MODULE_OPEN     : localEvntCode = tr("Module case: %1").arg(tr("opened"))                                              ; break;  //0x9A //Відсік модуля лічильника відкрито
    case UCM_EV_CASE_MODULE_CLOSE    : localEvntCode = tr("Module case: %1").arg(tr("closed"))                                              ; break;  //0x9B //Відсік модуля лічильника закрито
    case UCM_EV_SET_SALDO_PARAM      : localEvntCode = tr("Billing settings changed")                                                       ; break;  //0x9C //Параметри сальдо встановлено
    case UCM_EV_POWER_OVER_RELAY_OFF : localEvntCode = tr("Relay load off, Pmax")                                                           ; break;   //0x9D //Викнення реле після перевищення активної потужності (Версія ПЗ >= 302.15.001)
    case UCM_EV_CHANGE_PARAM_CANAL1  : localEvntCode = tr("Load profile #%1, changed parameters").arg("1")                                  ; break;  //0x9E //Змінено параметр профіля навантаження 1 (Версія ПЗ >= 302.17.001)
    case UCM_EV_CHANGE_PARAM_CANAL2  : localEvntCode = tr("Load profile #%1, changed parameters").arg("2")                                  ; break;  //0x9F //Змінено парамтер профіля навантаження 2
    case UCM_EV_CHANGE_PARAM_CANAL3  : localEvntCode = tr("Load profile #%1, changed parameters").arg("3")                                  ; break;  //0xA0 //Змінено параметр профіля навантаження 3
    case UCM_EV_CHANGE_PARAM_CANAL4  : localEvntCode = tr("Load profile #%1, changed parameters").arg("4")                                  ; break;  //0xA1 //Змінено параметр профіля навантаження 4
    case UCM_EV_CHANGE_PARAM_CANAL5  : localEvntCode = tr("Load profile #%1, changed parameters").arg("5")                                  ; break;  //0xA2 //Змінено параметр профіля навантаження 5
    case UCM_EV_CHANGE_PARAM_CANAL6  : localEvntCode = tr("Load profile #%1, changed parameters").arg("6")                                  ; break;  //0xA3 //Змінено параметр профіля навантаження 6
    case UCM_EV_CRSSZERO_EXP_ENT1   : localEvntCode = tr("Crosszero %1 T%2").arg("A-").arg("1")                                            ; break;  //0xA4 //Перехід через 0 лічильника активної експортної енергії по тарифу 1 при досягненні 1000000.00 кВт
    case UCM_EV_CRSSZERO_EXP_ENT2   : localEvntCode = tr("Crosszero %1 T%2").arg("A-").arg("2")                                            ; break;  //0xA5 //Перехід через 0 лічильника активної експортної енергії по тарифу 2 при досягненні 1000000.00 кВт
    case UCM_EV_CRSSZERO_EXP_ENT3   : localEvntCode = tr("Crosszero %1 T%2").arg("A-").arg("3")                                            ; break;  //0xA6 //Перехід через 0 лічильника активної експортної енергії по тарифу 3 при досягненні 1000000.00 кВт
    case UCM_EV_CRSSZERO_EXP_ENT4   : localEvntCode = tr("Crosszero %1 T%2").arg("A-").arg("4")                                            ; break;  //0xA7 //Перехід через 0 лічильника активної експортної енергії по тарифу 4 при досягненні 1000000.00 кВт
    case UCM_EV_CRSSZERO_EXP_VARi1  : localEvntCode = tr("Crosszero %1 T%2").arg("R+").arg("1")                                            ; break;  //0xA8 //Перехід через 0 лічильника реактивної позитивної експортної енергії по тарифу 1 при досягненні 1000000.00 кВар
    case UCM_EV_CRSSZERO_EXP_VARi2  : localEvntCode = tr("Crosszero %1 T%2").arg("R+").arg("2")                                            ; break;  //0xA9 //Перехід через 0 лічильника реактивної позитивної експортної енергії по тарифу 2 при досягненні 1000000.00 кВар
    case UCM_EV_CRSSZERO_EXP_VARi3  : localEvntCode = tr("Crosszero %1 T%2").arg("R+").arg("3")                                            ; break;  //0xAA //Перехід через 0 лічильника реактивної позитивної експортної енергії по тарифу 3 при досягненні 1000000.00 кВар
    case UCM_EV_CRSSZERO_EXP_VARi4  : localEvntCode = tr("Crosszero %1 T%2").arg("R+").arg("4")                                            ; break;  //0xAB //Перехід через 0 лічильника реактивної позитивної експортної енергії по тарифу 4 при досягненні 1000000.00 кВар
    case UCM_EV_CRSSZERO_EXP_VARe1  : localEvntCode = tr("Crosszero %1 T%2").arg("R-").arg("1")                                            ; break;   //0xAC //Перехід через 0 лічильника реактивної негативної експортної енергії по тарифу 1 при досягненні 1000000.00 кВар
    case UCM_EV_CRSSZERO_EXP_VARe2  : localEvntCode = tr("Crosszero %1 T%2").arg("R-").arg("2")                                            ; break;  //0xAD //Перехід через 0 лічильника реактивної негативної експортної енергії по тарифу 2 при досягненні 1000000.00 кВар
    case UCM_EV_CRSSZERO_EXP_VARe3  : localEvntCode = tr("Crosszero %1 T%2").arg("R-").arg("3")                                            ; break;  //0xAE //Перехід через 0 лічильника реактивної негативної експортної енергії по тарифу 3 при досягненні 1000000.00 кВар
    case UCM_EV_CRSSZERO_EXP_VARe4  : localEvntCode = tr("Crosszero %1 T%2").arg("R-").arg("4")                                            ; break;  //0xAF //Перехід через 0 лічильника реактивної негативної експортної енергії по тарифу 4 при досягненні 1000000.00 кВар
    case UCM_EV_EM_MAGNETIC_ON       : localEvntCode = tr("AC magnetic field, %1").arg(tr("begining"))                                      ; break;   //0xB0 //Виявлено наявність впливу змінного магнітного поля
    case UCM_EV_EM_MAGNETIC_OFF      : localEvntCode = tr("AC magnetic field, %1").arg(tr("end"))                                           ; break;   //0xB1 //Виявлено закінчення впливу змінного магнітного поля

        ///MTX1 Section
    case UCM_EV_ENERGY_REG_FAULT        : localEvntCode = tr("Energy registers broken")                                                     ; break;//  0x101 // Значення накоплюючих регістрів втрачено

    case UCM_EV_4K_POWERSALDO_OK        : localEvntCode = tr("Billing, credit mode: load %1").arg(tr("normal"))                             ; break;//        0x150 // Перевещення потужності в режимі кредиту відсутнє
    case UCM_EV_4K_POWERSALDO_OVER      : localEvntCode = tr("Billing, credit mode: load %1").arg(tr("high"))                               ; break;//        0x151 // Перевищена потужність в режимі кредиту
    case UCM_EV_4K_POWER_GOOD_DIO       : localEvntCode = tr("No POWER_GOOD")                                                               ; break;//        0x17E //Відсутній сигнал POWER_GOOD

    case UCM_EV_4K_CURRENT_UNEQUIL_FAULT: localEvntCode = tr("Inequalities of currents: %1").arg(tr("begining"))                            ; break;//   0x1B2 // Виявлено нерівність струмів
    case UCM_EV_4K_CURRENT_UNEQUIL_OK   : localEvntCode = tr("Inequalities of currents: %1").arg(tr("end"))                                 ; break;//   0x1B3 // Нерівність струмів відсутня
    case UCM_EV_4K_BIPOLAR_POWER_FAULT  : localEvntCode = tr("Differential polarity in phase and neutral: %1").arg(tr("begining"))          ; break;//   0x1B4 // Виявлені різнополярні потужності у фазі та нейтралі
    case UCM_EV_4K_BIPOLAR_POWER_OK     : localEvntCode = tr("Differential polarity in phase and neutral: %1").arg(tr("end"))               ; break;//   0x1B5 // Різнополярні потужності у фазі та нейтралі відсутні
    case UCM_EV_4K_RESET_EM_FLAG        : localEvntCode = tr("Screen of electromagnetic effects: reset")                                    ; break;//   0x1B6 // Скинуто екран ел.магнітного впливу
    case UCM_EV_4K_RESET_MAGN_FLAG      : localEvntCode = tr("Screen of magnetic effects: reset")                                           ; break;//    0x1B7 // Скинуто екрат магнітного впливу


        ///Global section                         +0x200
        //BUS
    case UCM_EV_BUS_I2C_FAULT           : localEvntCode = tr("I2C bus broken")                                                              ; break;//   0x200 //


        //Meter STATE OK
    case UCM_EV_MSTATE_DIAGNOSTIC_OK    : localEvntCode = tr("Diagnostic: ok")                                                              ; break;//     0x300 //

        //clear data
    case UCM_EV_CLEAR_LOADPROFILEDATA   : localEvntCode = tr("Load profile: %1").arg(tr("clear data"))                                      ; break;//        0x500 //



    case UCM_EV_MSTATE_POWERON          : localEvntCode = tr("Meter: %1").arg(tr("power on"))                                               ; break; //             0x301 //
    case UCM_EV_MSTATE_POWEROFF         : localEvntCode = tr("Meter: %1").arg(tr("power off"))                                              ; break; //             0x302 //

        //Події що фіксуюють лише початок зміни стану
    case UCM_EV_VA_MIN_UNDER_SS         : localEvntCode = tr("Voltage L%1: %2").arg("1").arg(tr("undervoltage"))                            ; break; //             0x600 //Напруга L1 нище порогу мінімальної напруги, не очікую на нормальну
    case UCM_EV_VB_MIN_UNDER_SS         : localEvntCode = tr("Voltage L%1: %2").arg("1").arg(tr("undervoltage"))                            ; break; //             0x601 //Напруга L2 нище порогу мінімальної напруги, не очікую на нормальну
    case UCM_EV_VC_MIN_UNDER_SS         : localEvntCode = tr("Voltage L%1: %2").arg("1").arg(tr("undervoltage"))                            ; break; //             0x602 //Напруга L3 нище порогу мінімальної напруги, не очікую на нормальну

    case UCM_EV_VA_MAX_OVER_SS          : localEvntCode = tr("Voltage L%1: %2").arg("1").arg(tr("overvoltage"))                             ; break; //              0x603 //напруга L1 вище порогу максимальної напруги, не очікую на нормальну
    case UCM_EV_VB_MAX_OVER_SS          : localEvntCode = tr("Voltage L%1: %2").arg("1").arg(tr("overvoltage"))                             ; break; //             0x604 //напруга L2 вище порогу максимальної напруги, не очікую на нормальну
    case UCM_EV_VC_MAX_OVER_SS          : localEvntCode = tr("Voltage L%1: %2").arg("1").arg(tr("overvoltage"))                             ; break; //             0x605 //напруга L3 вище порогу максимальної напруги, не очікую на нормальну
    case UCM_EV_CASE_OPEN_SS            : localEvntCode = tr("Meter cover: %1").arg(tr("opened"))                                           ; break; //            0x606 //Корпус лічильника відкритий, не очікую на закриття
    case UCM_EV_CASE_KLEMA_OPEN_SS      : localEvntCode = tr("Terminal box was %1").arg(tr("opened"))                                       ; break; //         0x607 //Клемна коробка лічильника відкрита, не очікую на закриття


        //set new meter settings
    case UCM_EV_SET_PARAM               : localEvntCode = tr("Configuration changed")                                                       ; break; //                0x1000 // узагальнена подія змін параметрів лічильника, параметризація.




        ///[HELLO DLMS]

        ///STANDARD EVENT LOG
        //RESET AND SUPPLY DIRUPTIONS   +0x2000
    case UCM_EV_STANDARD_RESET_WTH_DATA_LOSS        : localEvntCode = tr("Reset with data loss")                                            ; break; //   0x2001
    case UCM_EV_STANDARD_RESET_WTHOUT_DATA_LOSS     : localEvntCode = tr("Reset without data loss")                                         ; break; //        0x2002
//    case UCM_EV_STANDARD_POWER_OUTAGES              : localEvntCode = UCM_EV_MSTATE_POWEROFF                                                   ; break; //           0x2003
    case UCM_EV_STANDARD_NO_CONNECTION_2_N          : localEvntCode = tr("No connection to neutral")                                        ; break; //           0x2004

        //CHANGE OF PARAMETERS
    case UCM_EV_STANDARD_CHNG_PRMTRS_REGISTER       : localEvntCode = tr("Changed parameters, register")                                    ; break; //          0x201E //30
    case UCM_EV_STANDARD_CHNG_PRMTRS_COMM_PORT      : localEvntCode = tr("Changed parameters, communication port")                          ; break; //          0x201F //31
        //case UCM_EV_STANDARD_CHNG_PASSWD_2_READ              UCM_EV_CHANGE_ACCESS_KEY_3// 0x2020 //32
        //case UCM_EV_STANDARD_CHNG_PASSWD_4_PRM               UCM_EV_CHANGE_ACCESS_KEY_2//  0x2021 //33
        //case UCM_EV_STANDARD_CHNG_PASSWD_4_FIRMWR            UCM_EV_CHANGE_ACCESS_KEY_1//  0x2022 //34

        //case UCM_EV_STANDARD_CHNG_TIME_OF_SSN_TRANSTN        UCM_EV_CHANGE_COR_TIME// 0x2024 //36
    case UCM_EV_STANDARD_CHNG_MIN_TIME_BTWN_INVOICN : localEvntCode = tr("Changed the minimum time between invoicig")                       ; break; //    0x2025 //37
    case UCM_EV_STANDARD_CHNG_PRD_4_LOADPROFILE     : localEvntCode = tr("Changed the period for the load profile")                         ; break; //      0x2026 //38
    case UCM_EV_STANDARD_CHNG_SYNC                  : localEvntCode = tr("Changed the synchronization")                                     ; break; //                0x2027 //39
    case UCM_EV_STANDARD_CHNG_PROGRAM_NAME          : localEvntCode = tr("Change program name")                                             ; break; //            0x2028 //40

    case UCM_EV_ACIN1_NO_VOLTAGE                    : localEvntCode = tr("ACIN1 state changed to on")                                       ; break; //    0x2032//50 - ACIN1 state changed to on
    case UCM_EV_ACIN1_VOLTAGE                       : localEvntCode = tr("ACIN1 state changed to off")                                      ; break; //    0x2033//51 - ACIN1 state changed to off
    case UCM_EV_ACIN2_NO_VOLTAGE                    : localEvntCode = tr("ACIN2 state changed to on")                                       ; break; //    0x2034//52 - ACIN2 state changed to on
    case UCM_EV_ACIN2_VOLTAGE                       : localEvntCode = tr("ACIN2 changed to off")                                            ; break; //    0x2035//53 - ACIN2 changed to off


        //CHANGING PARAMETERS OF PWR QUALITY SETT
    case UCM_EV_STANDARD_CHNG_DURTN_OF_VLTG_SGS_SWLS    : localEvntCode = tr("Changed duration of voltage sags and swells")                 ; break; //   0x205A //90
    case UCM_EV_STANDARD_CHNG_LIMIT_OF_PWR_OUTAGE       : localEvntCode = tr("Changed the limit of the power outage")                       ; break; //    0x205B //91
    case UCM_EV_STANDARD_CHNG_RATED_VOLTAGE             : localEvntCode = tr("Changing rated voltage")                                      ; break; //     0x205C //92
    case UCM_EV_STANDARD_CHNG_UPPR_LIMIT_PERMSSBL_VLTG  : localEvntCode = tr("Changing the upper limit of the permissible voltage")         ; break; //  0x205D //93
    case UCM_EV_STANDARD_CHNG_LOWR_LIMIT_PERMSSBL_VLTG  : localEvntCode = tr("Changeing the lower limit of the permissible voltage")        ; break; // 0x205E //94
    case UCM_EV_STANDARD_CHNG_LACK_OF_VTG               : localEvntCode = tr("Changing voltage value incicating lack of voltage (assumed 50%Un)"); break; //            0x205F //95

        // RESET
    case UCM_EV_STANDARD_PASSWRD_RESET                  : localEvntCode = tr("Password take the factory default settings")                  ; break; //           0x2060 //96
    case UCM_EV_STANDARD_ALL_2_FACTORY_SETT             : localEvntCode = tr("Parameters take the factory default settings and billing and consumption/energy export profiles are reset") ; break; //           0x2061 //97


        ///FIRMWARE UPDATE LOG
        //UPDATE
    case UCM_EV_FIRMWARE_CHNG_PROGRAM                   : localEvntCode = tr("Firmware - change")                 ; break; //            0x2501


        ///CLOCK SYNC LOG
        //CLOCK SYNC
    case UCM_EV_CLOCK_SYNC                              : localEvntCode = tr("Clock synchronization performed") ; break; //                  0x2601



        ///POWER QUALITY
        //VOLTAGE SAGS AND SWELLS



    case UCM_EV_PWR_QLT_VLTG_SAG_10_UN_L1           : localEvntCode = tr("Voltage sag of %1% Un L%2").arg("10").arg("1") ; break; //        0x2901
    case UCM_EV_PWR_QLT_VLTG_SAG_10_UN_L2           : localEvntCode = tr("Voltage sag of %1% Un L%2").arg("10").arg("2") ; break; //       0x2902
    case UCM_EV_PWR_QLT_VLTG_SAG_10_UN_L3           : localEvntCode = tr("Voltage sag of %1% Un L%2").arg("10").arg("3") ; break; //       0x2903
    case UCM_EV_PWR_QLT_VLTG_SAG_20_UN_L1           : localEvntCode = tr("Voltage sag of %1% Un L%2").arg("20").arg("1") ; break; //       0x2904
    case UCM_EV_PWR_QLT_VLTG_SAG_20_UN_L2           : localEvntCode = tr("Voltage sag of %1% Un L%2").arg("20").arg("2") ; break; //       0x2905
    case UCM_EV_PWR_QLT_VLTG_SAG_20_UN_L3           : localEvntCode = tr("Voltage sag of %1% Un L%2").arg("20").arg("3") ; break; //       0x2906
    case UCM_EV_PWR_QLT_VLTG_SAG_50_UN_L1           : localEvntCode = tr("Voltage sag of %1% Un L%2").arg("50").arg("1") ; break; //       0x2907
    case UCM_EV_PWR_QLT_VLTG_SAG_50_UN_L2           : localEvntCode = tr("Voltage sag of %1% Un L%2").arg("50").arg("2") ; break; //      0x2908
    case UCM_EV_PWR_QLT_VLTG_SAG_50_UN_L3           : localEvntCode = tr("Voltage sag of %1% Un L%2").arg("50").arg("3") ; break; //      0x2909
    case UCM_EV_PWR_QLT_VLTG_SWLL_10_UN_L1          : localEvntCode = tr("Voltage swell of %1% Un L%2").arg("10").arg("1") ; break; //     0x290A
    case UCM_EV_PWR_QLT_VLTG_SWLL_10_UN_L2          : localEvntCode = tr("Voltage swell of %1% Un L%2").arg("10").arg("2") ; break; //      0x290B
    case UCM_EV_PWR_QLT_VLTG_SWLL_10_UN_L3          : localEvntCode = tr("Voltage swell of %1% Un L%2").arg("10").arg("3") ; break; //   0x290C
    case UCM_EV_PWR_QLT_VLTG_RTRN_2_ACCPTBL_VALUE_L1: localEvntCode = tr("Voltage return to an acceptable value L%1").arg("1") ; break; //   0x290D
    case UCM_EV_PWR_QLT_VLTG_RTRN_2_ACCPTBL_VALUE_L2: localEvntCode = tr("Voltage return to an acceptable value L%1").arg("2") ; break; //   0x290E
    case UCM_EV_PWR_QLT_VLTG_RTRN_2_ACCPTBL_VALUE_L3: localEvntCode = tr("Voltage return to an acceptable value L%1").arg("3") ; break; //   0x290F


    case UCM_EV_IN_MAX_OVER                         : localEvntCode = tr("Over current in neutral, %1").arg(tr("wait for normal")); break;//  0x2917 //23 Over current in neutral
    case UCM_EV_IN_MAX_OK                           : localEvntCode = tr("Current IN returned to an acceptable value")          ; break;//                        0x291B //27 Current I1 returned to an acceptable value

    case UCM_EV_PWR_QLT_VLTG_VSS_RCURR_ON           : localEvntCode = tr("Revers current detected at least in one of phases, %1").arg(tr("wait for normal")); break;//           0x291E//30 Revers current detected at least in one of phases
    case UCM_EV_PWR_QLT_VLTG_VSS_RCURR_OFF          : localEvntCode = tr("No reverse current")                                  ; break;//           0x291F//31 No reverse current



        ///FRAUD DETECTION
        //Theft events register
        //case UCM_EV_THFT_OPENING_COVER                       UCM_EV_CASE_OPEN//  0x3101
        //case UCM_EV_THFT_CLOSING_COVER                       UCM_EV_CASE_CLOSE// 0x3102
        //case UCM_EV_THFT_DETECTON_MAGNETIC_FIELD             UCM_EV_MAGNETIC_ON// 0x3103
        //case UCM_EV_THFT_DISAPPRNCE_OF_MAGNETIC_FIELD        UCM_EV_MAGNETIC_OFF//    0x3104
    case UCM_EV_THFT_DTCTN_OF_PHASE_AND_ZERO_RPLC_BEG: localEvntCode = tr("Detection of phase and zero replacement: %1").arg(tr("begining"))    ; break; //    0x3105
    case UCM_EV_THFT_DTCTN_OF_PHASE_AND_ZERO_RPLC_END: localEvntCode = tr("Detection of phase and zero replacement: %1").arg(tr("end"))         ; break; //    0x3106
        //case UCM_EV_THFT_REMOVNG_COVER_OF_TRMNL_STRIP        UCM_EV_CASE_KLEMA_OPEN//  0x3107
        //case UCM_EV_THFT_CLOSING_COVER_OF_TRMNL_STRIP        UCM_EV_CASE_KLEMA_CLOSE//    0x3108
        //case UCM_EV_THFT_DIFF_CURRENT_DETECTION_BEG          UCM_EV_IDIFF_OVER//    0x3109
        //case UCM_EV_THFT_DIFF_CURRENT_DETECTION_END          UCM_EV_IDIFF_OK//    0x310A

    case UCM_EV_THFT_KOPEN_SUS_ON                   : localEvntCode = tr("Terminal cover opened in security suspend mode")                      ; break;//   0x3157//87 terminal cover opened in security suspend mode



        ///COMMUNICATION IFACE
        //COMMUNICATION EVENTS
    case UCM_EV_COMM_IFACE_PLC_PORT_END         : localEvntCode = tr("End communication PLC port")                                              ; break; //           0x3301
    case UCM_EV_COMM_IFACE_PLC_PORT_BEG         : localEvntCode = tr("Begin communication PLC port")                                            ; break; //          0x3302
    case UCM_EV_COMM_IFACE_OPT_PORT_END         : localEvntCode = tr("End communication Optical port")                                          ; break; //           0x3303
    case UCM_EV_COMM_IFACE_OPT_PORT_BEG         : localEvntCode = tr("Begin communication Optical port")                                        ; break; //          0x3304
    case UCM_EV_COMM_IFACE_ETH_PORT_END         : localEvntCode = tr("End communication Ethernet port")                                         ; break; //          0x3305
    case UCM_EV_COMM_IFACE_ETH_PORT_BEG         : localEvntCode = tr("Begin communication Ethernet port")                                       ; break; //          0x3306
    case UCM_EV_COMM_IFACE_3GPP_PORT_END        : localEvntCode = tr("End communication 3GPP port")                                             ; break; //         0x3307
    case UCM_EV_COMM_IFACE_3GPP_PORT_BEG        : localEvntCode = tr("Begin communication 3GPP port")                                           ; break; //         0x3308

default: qDebug() << "------------- meterEvCode2humanV2 0x" << QString::number(ucmEventCode, 16).toUpper();

    }
    return localEvntCode;
}
//-------------------------------------------------------------------------------------------------------------------------------------
