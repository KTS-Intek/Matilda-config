#ifndef DEFINEDPOLLCODES_H
#define DEFINEDPOLLCODES_H

#define RELAY_LOAD_ON   1
#define RELAY_LOAD_OFF  2
#define RELAY_READ      3



#define POLL_CODE_KILL_PRIORITY_TASKS       0
#define POLL_CODE_RESET_NETWORK             7


#define ZBRTR_OPEN_DIRECT_ACCESS             23


#define POLL_CODE_READ_WTR_SLEEP            40
#define POLL_CODE_WRITE_WTR_SLEEP           41


#define POLL_CODE_READ_TARIFF               50
#define POLL_CODE_WRITE_TARIFF              51
#define POLL_CODE_READ_DATE_TIME_DST        52
#define POLL_CODE_WRITE_DATE_TIME           53



#define POLL_CODE_ADD_MANY                  57
#define POLL_CODE_WRITE_METER_ADDR          58
#define POLL_CODE_RELAY_OPERATIONS          59

#define POLL_CODE_READ_METER_LOGBOOK              60//POLL_CODE_METER_STATUS
#define POLL_CODE_READ_METER_STATE          80 //Relay, Temperature, Prepaid state and sett,

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




//#define POLL_CODE_READ_END_DAY              180
//#define POLL_CODE_READ_END_MONTH            220
//error code


//SUB JOURNALS
#define POLL_CODE_READ_METER_LOGBOOK_JRNL         61
#define POLL_CODE_MATILDA_EVNTS             70





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




#endif // DEFINEDPOLLCODES_H
