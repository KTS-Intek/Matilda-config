#ifndef DEFINEDPOLLCODES_H
#define DEFINEDPOLLCODES_H

#define RELAY_LOAD_ON   1
#define RELAY_LOAD_OFF  2
#define RELAY_READ      3 ///READS ALL REALAYS

#define RELAY2_LOAD_ON  4
#define RELAY2_LOAD_OFF 5


#define RELAY_STATE_UNKN        0
#define RELAY_STATE_LOAD_ON     1  //main "relay_all_power_on" : secondary "relay_2_power_on"
#define RELAY_STATE_LOAD_OFF    2
#define RELAY_STATE_NOT_SUP     3
#define RELAY_STATE_OFF_PRSBTTN 4

//NI is not needed (ni from taks)
#define POLL_CODE_KILL_PRIORITY_TASKS       0
#define POLL_CODE_READ_MODEM_CONFIG         1
#define POLL_CODE_WRITE_MODEM_CONFIG        2
#define POLL_CODE_RESET_MODEM               3
#define POLL_CODE_FACTORY_SETTINGS          4
#define POLL_CODE_NODE_DISCOVERY            5

#define POLL_CODE_RESET_NETWORK             7
#define POLL_CODE_ZOMBIE_EXCHANGE           14

#define POLL_CODE_MODEM_OPEREATIONS_END     15//for all modems

//NI is needed for all that are under
#define POLL_CODE_QRS_EXT                   16
#define POLL_CODE_CCT                       17//check connection tool

#define POLL_CODE_MODEM_OPEREATIONS_END_EXT  22//it requires NI/NIs,for coordinators only


#define ZBRTR_OPEN_DIRECT_ACCESS            23


//BEGIN FIREFLY
#define POLL_CODE_FF_READ_LAMP              25

#define POLL_CODE_FF_RESET_EMB_IN_ALAMP     26//It resets settings of the modem

#define POLL_CODE_FF_WRITE_POWER_TO_LAMP    27 //роздає по лампам
#define POLL_CODE_FF_RESET_4_LCU            28 // POLL_CODE_FF_RESET_EMBMODEM         28//by AT command or hardware
#define POLL_CODE_FF_LED_VISUALISE          29
#define POLL_CODE_FF_WRITE_POWER_TO_GROUPS  30 //роздає по групам   POLL_CODE_FF_ WRITE_LAMP_POWER
#define POLL_CODE_FF_WRITE_LAMP_NETWRK      31
//#define POLL_CODE_FF_WRITE_LAMP_GROUP       32 //depreacted
#define POLL_CODE_FF_GET_LAMP_SENSORS       32

#define POLL_CODE_FF_WRITE_LAMP_CONFIG      33
#define POLL_CODE_FF_WRITE_LAMP_CONFIG_SMART 34 //у цьому випадку зчитується конф. ламп (Група, Поч Яск. Кін.Яск, Період.Неакт.), якщо один із цих параметрів відрізняється, то змінюю
#define POLL_CODE_FF_RELAY_CONFIG_SMART     35 //POLL_CODE_RELAY_OPERATIONS
//GOD'S COMMANDS
#define POLL_CODE_FF_READ_LAMP_MANUF        36
#define POLL_CODE_FF_WRITE_LAMP_CONF_MANUF  37
//END FIREFLY

#define POLL_CODE_SLEEP_2_METERS            38

#define POLL_CODE_READ_WTR_SLEEP            40
#define POLL_CODE_WRITE_WTR_SLEEP           41


#define POLL_CODE_READ_TARIFF               50
#define POLL_CODE_WRITE_TARIFF              51  //parametryzation - write tariff, date time and dst, display, relay sett and limits
#define POLL_CODE_READ_DATE_TIME_DST        52
#define POLL_CODE_WRITE_DATE_TIME           53 //only date and time



#define POLL_CODE_ADD_MANY                  57
#define POLL_CODE_WRITE_METER_ADDR          58
#define POLL_CODE_RELAY_OPERATIONS          59

#define POLL_CODE_READ_METER_LOGBOOK              60//POLL_CODE_METER_STATUS
#define POLL_CODE_READ_METER_STATE          80 //Relay, Temperature, Prepaid state and sett,

//since Matilda version 10 there is a JSN object can be inside F key for some extra values (harmonics for example)
#define POLL_CODE_READ_VOLTAGE              100
#define POLL_CODE_READ_POWER                120
//#define POLL_CODE_READ_TOTAL              140
#define POLL_CODE_READ_TOTAL                140

#define POLL_CODE_READ_END_DAY              160
#define POLL_CODE_READ_END_MONTH            180




#define POLL_CODE_WTR_METER_LOGBOOK          61 //Log book
#define POLL_CODE_WTR_METER_STATE           81 //Relay, Temperature, Prepaid state and sett,
#define POLL_CODE_WTR_INSTANT_VLS           101
#define POLL_CODE_WTR_PERIOD                121
#define POLL_CODE_WTR_TOTAL                 141
#define POLL_CODE_WTR_END_DAY               161
#define POLL_CODE_WTR_END_MONTH             181
#define POLL_CODE_WTR_ALL_SUPPORTED         201//не передається до плагіну, до плагіну має направитися лише список кодів опитування groupPollCode //QList<quint8> listOfPollCodes


#define POLL_CODE_EXCHANGE_HL8518           219 //it start exchange with HL8518 modem, other details are in the arguments

#define POLL_CODE_START_DEVICES_ZONE        220 //logs of the exchange with devices, It can be any device

#define POLL_CODE_DEVICE_EXCHANGE_LOGS      255 //logs of the exchange with devices, It can be any device


//#define POLL_CODE_READ_END_DAY              180
//#define POLL_CODE_READ_END_MONTH            220
//error code


//SUB JOURNALS
#define POLL_CODE_READ_METER_LOGBOOK_JRNL         61
#define POLL_CODE_MATILDA_EVNTS             70


#define POLL_CODE_ADD_4_REAL_VIEW           1000



//tariff program
#define TRFF_MAX_SEASON                     12
#define TRFF_MAX_DAYPROFILE                 100
#define TRFF_MAX_TARIFFNUMB                 5
#define TRFF_MAX_SPECDAY                    100

#define TRFF_MIN_SEASON                     0
#define TRFF_MIN_DAYPROFILE                 0
#define TRFF_MIN_TARIFFNUMB                 1
#define TRFF_MIN_SPECDAY                    0

//read%2 == 0
#define POLL_INTRVL_AGO                     11
#define POLL_UCON_TIMELIMIT_SECS            600

#define POLL_LONG_OK_CONF_DONCHG    0
#define POLL_LONG_OK_CONF_ENABLE    1
#define POLL_LONG_OK_CONF_DISABLE   2


#define THD_DEFAULT_LASTINDEX   31
#define THD_MAX_LASTINDEX   99

#endif // DEFINEDPOLLCODES_H
