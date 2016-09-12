#ifndef MOJI_DEFY
#define MOJI_DEFY

#define PLG_BBB                         1//for Matilda, comment for Zbyrator

//dev type
#define DEV_POLL                        1// пристрій опитування лічильників. Режими: опитування, шлюз, зберігання і передача даних
#define DEV_STOR                        2// пристрій зберігання даних. Режими: зберігання і передача даних (дані можуть додаватись з іншого джерела, наприклад опитування лічильників ПЗ Збриратор )
#define DEV_GATE                        3// пристрій який працює в режимі шлюзу
#define DEV_SVAHA                       4// пристрій який комутує сокет активного клієнта матільди і сокет ПЗ конфігурації

#define DEV_POLL_EMULATOR_L0            20// емуляція пристрою опитування лічильників. Режими: опитування, зберігання і передача даних (урізана версія DEV_POLL)
#define DEV_POLL_EMULATOR_L1            21// емуляція пристрою опитування лічильників. Режими: опитування, шлюз (через протокол матільди), зберігання і передача даних (урізана версія DEV_POLL)
#define DEV_POLL_EMULATOR_L2            22// емуляція пристрою опитування лічильників. Режими: опитування, шлюз (в режимі сервісу), зберігання і передача даних (урізана версія DEV_POLL)

#define SETT_GPRS                       1
#define SETT_PEREDAVATOR                2
#define SETT_MATILDA                    3
#define SETT_ZIGBEE                     4
#define SETT_TCP                        5


#define SETT_MAX_UNCOMPRSS_PACkET_SIZE  500

#define SETT_MIN                        1
#define SETT_MAX                        3
#define SVAHA_MAX_MAC                   10

#define SETT_MAX_IDENTICAL_MESS_PER_SESION    30

#define SETT_DT_DIFF_MAX_SECS           90
#define SETT_DT_DIFF_MAX_ALLOW_CORR     1800//30 hv. 0 - disable, -1 - dozvil na vse

#define MATILDA_PROTOCOL_VERSION        1
#define MATILDA_BACKUP_VERSION          MATILDA_PROTOCOL_VERSION


//exit codes
#define APP_CODE_RESTART                23//RESTART IN ZAPUSKATOR SH
#define APP_CODE_FULL_UPGRADE           25//RESTART AND FULL UPGRADE
#define APP_CODE_UPGRADE                27//RESTART AND UPGRADE (ADD NEW PACKETS)
#define APP_CODE_REBOOT                 35//EXIT ZAPUSKATOR SH

#define FATAL_ERROR_MAX                 33


//Protocol Matilda v1
//#define COMMAND_SPLIT_SYMBOL            1
#define COMMAND_ZULU                            0
#define COMMAND_YOUR_ID_AND_MAC                 1

#define COMMAND_AUTHORIZE                       2

#define COMMAND_ACCESS_DENIED                   4
#define COMMAND_CONNECT_2_THIS_SERVICE          5
//#define COMMAND_LAST_OPER_RESULT                5
#define COMMAND_I_AM_A_ZOMBIE                     6
#define COMMAND_ERROR_CODE                      7

#define COMMAND_COMPRESSED_PACKET               8
#define COMMAND_COMPRESSED_STREAM               9
#define COMMAND_I_NEED_MORE_TIME                10
#define COMMAND_CONNECT_ME_2_THIS_ID_OR_MAC     11


#define COMMAND_ERROR_CODE_EXT                  13



//READ ROOT AND GUEST
#define COMMAND_READ_SYSTEM_SETTINGS            22
#define COMMAND_READ_TASK_INFO                  23
#define COMMAND_READ_ABOUT_PLG                  24
#define COMMAND_READ_DATE_SETT                  25
#define COMMAND_READ_GPRS_SETT                  26
#define COMMAND_READ_STATE                      27
#define COMMAND_READ_IFCONFIG                   28
#define COMMAND_READ_APP_LOG                    29
#define COMMAND_READ_POLL_SCHEDULE              30
#define COMMAND_READ_METER_LIST                 31

#define COMMAND_READ_DATABASE                   32

#define COMMAND_READ_DATABASE_GET_TABLES        33
#define COMMAND_READ_DATABASE_GET_VAL           34
#define COMMAND_READ_METER_LOGS                 35
#define COMMAND_READ_METER_LOGS_GET_TABLES      36
#define COMMAND_READ_METER_LOGS_GET_VAL         37

#define COMMAND_READ_METER_LIST_FRAMED          38

#define COMMAND_READ_DATABASE_TABLES_PARSING    39
#define COMMAND_READ_ZBR_LOG                    40
#define COMMAND_READ_ABOUT_OBJECT               41
#define COMMAND_READ_POLL_SETT                  42
#define COMMAND_READ_POLL_STATISTIC             43

#define COMMAND_READ_TABLE_HASH_SUMM            44


#define COMMAND_READ_IP_FILTER_SETT             47

#define COMMAND_READ_METER_LIST_HASH_SUMM       48
#define COMMAND_READ_SERIAL_LOG                 49
#define COMMAND_READ_COMMANDS                   50


#define COMMAND_READ_DA_DATA_FROM_COORDINATOR   53
#define COMMAND_READ_DA_SERVICE_SETT            54
#define COMMAND_READ_PLUGIN_LOG_WARN            55
#define COMMAND_READ_PLUGIN_LOG_ERROR           56

#define COMMAND_READ_PEREDAVATOR_AC_SETT        57
#define COMMAND_READ_MATILDA_AC_SETT            58
#define COMMAND_READ_BACKUP_LIST                59
#define COMMAND_READ_UDP_BEACON                 60

#define COMMAND_READ_METER_LIST_HASH_SUMM_EXT   61
#define COMMAND_READ_METER_LIST_FRAMED_EXT      62

#define COMMAND_READ_ZIGBEE_SETT                63
#define COMMAND_READ_TCP_SETT                   64
#define COMMAND_READ_FRWRD_SETT                 65


//#define COMMAND_READ_MEMO_SN                36


//ROOT || OPERATOR WRITE
#define COMMAND_WRITE_FIRST_4_OPERATOR      40000
#define COMMAND_WRITE_POLL_SCHEDULE         40001
#define COMMAND_WRITE_METER_LIST            40002
#define COMMAND_WRITE_METER_LIST_FRAMED     40003
#define COMMAND_WRITE_DATE_SETT             40004
#define COMMAND_WRITE_RESET_MODEM           40005
#define COMMAND_WRITE_POLL_SETT             40006
#define COMMAND_WRITE_METER_LIST_ONE_PART   40007
#define COMMAND_WRITE_METER_LIST_POLL_ON    40008
#define COMMAND_WRITE_METER_LIST_POLL_OFF   40009
#define COMMAND_WRITE_METER_LIST_DEL_NI     40010
#define COMMAND_WRITE_COMMANDS              40011
#define COMMAND_WRITE_DA_SERVICE_SETT       40012
#define COMMAND_WRITE_PEREDAVATOR_AC_SETT   40013
#define COMMAND_WRITE_DA_OPEN_CLOSE         40014
#define COMMAND_WRITE_DA_DATA_2_COORDINATOR 40015
#define COMMAND_WRITE_FRWRD_SETT            40016





//ROOT: ONLY WRITE
#define COMMAND_WRITE_FIRST                 60000
#define COMMAND_WRITE_UPGRADE               60001

#define COMMAND_WRITE_GPRS_SETT             60003
#define COMMAND_WRITE_REBOOT                60004
#define COMMAND_WRITE_DAEMON_RESTART        60005
#define COMMAND_WRITE_FULL_UPGRADE          60006
#define COMMAND_WRITE_ROOT_LOGIN_PSWD       60007
#define COMMAND_WRITE_GUEST_LOGIN_PSWD      60008
#define COMMAND_WRITE_OPEARTOR_LOGIN_PSWD   60009
#define COMMAND_WRITE_DROP_TABLE_GET_COUNT  60010
#define COMMAND_WRITE_DROP_TABLE            60011
#define COMMAND_WRITE_DROP_TABLE_ALL        60012
#define COMMAND_WRITE_TIMEZONE              60013
#define COMMAND_WRITE_NTP_SETTINGS          60014
#define COMMAND_WRITE_ABOUT_OBJECT          60015
#define COMMAND_WRITE_IP_FILTER_SETT        60016
#define COMMAND_WRITE_ERASE_ALL_DATA        60017
#define COMMAND_WRITE_MATILDA_AC_SETT       60018
#define COMMAND_WRITE_DELETE_BACKUP_FILE    60019
#define COMMAND_WRITE_BACK_IN_TIME          60020
#define COMMAND_WRITE_CREATE_BACK_IN_TIME   60021
#define COMMAND_WRITE_UDP_BEACON            60022

#define COMMAND_WRITE_ZIGBEE_SETT           60023
#define COMMAND_WRITE_TCP_SETT              60024
#define COMMAND_WRITE_COMMAND2BASH          60025


#define COMMAND_GET_BACKUP_FILE             60419
#define COMMAND_PUSH_BACKUP_FILE_AND_APPLY  60420


//bash commands
#define BASH_DMESG                          1
#define BASH_ROUTE                          2
#define BASH_DF                             3
#define BASH_SCREENFETCH                    4
#define BASH_TOP                            5
#define BASH_USB_DEVICES                    6
#define BASH_LSUSB                          7
#define BASH_LSBLK                          8
#define BASH_I2C_INFO                       9
#define BASH_LSMOD                          10
#define BASH_TAIL                           111
#define BASH_HEAD                           112
#define BASH_MTR                            113
#define BASH_FILE                           114
#define BASH_SYSTEMCTL_STATUS               115

//error codes
#define ERR_DATABASE_CLOSED                 1
#define ERR_NO_DATA                         2
#define ERR_INTERNAL_ERROR                  3
#define ERR_INCORRECT_REQUEST               4
#define ERR_MAX_TABLE_COUNT                 5
#define ERR_CORRUPTED_DATA                  6
#define ERR_DUPLICATE_NI                    7
#define ERR_DUPLICATE_SN                    8
#define ERR_DATE_NOT_VALID                  9
#define ERR_COMMAND_NOT_ALLOWED             10
#define ERR_ACCESS_DENIED                   11
#define ERR_RESOURCE_BUSY                   12
#define ERR_IP_BLOCKED                      13
#define ERR_DA_CLOSED                       14


#define ERR_NO_ERROR                        99





#define MAX_METER_COUNT                     5000
#define MAX_REQUEST_NI_IN_LINE              300
#define MAX_PRECISION                       9
#define MAX_INRVL_AGO                       100// Днів/Місяців назад

#define MAX_MSEC_TIME2OPEN_DA               100
#define MAX_TARIFF_COUNT                    5//0 - summ, 1 T1, 2 T2, 3 T3, 4 T4
#define MAX_DATALEN                         2800 //for reading from DB  2610 * k =~ 5875 byte
#define MAX_TABLE_READ                      450
#define MAX_PACKET_LEN_RECOMENDATION        1400
#define MAX_PACKET_LEN                      10000000

#define MAX_DATABASE_SIZE                   550000000 //~550M
#define MAX_DATABASE_RESERV_SIZE            5000000 //~5M
#define MAX_DATABASE_RESERV_SIZE_FREE_DISK  50000000 //~50M

#define MAX_PACKET_SIZE_ONLY4DATA           5000000
#define MAX_GLYBYNA                         0x64

#define MAX_METER_READ_FROM_DB              100000




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
#define POLL_CODE_READ_TARIFF               50
#define POLL_CODE_WRITE_TARIFF              51
#define POLL_CODE_READ_DATE_TIME_DST        52
#define POLL_CODE_WRITE_DATE_TIME_DST       53

#define POLL_CODE_WRITE_METER_ADDR          58
#define POLL_CODE_RELAY_OPERATIONS          59

#define POLL_CODE_METER_STATUS              60
#define POLL_CODE_READ_VOLTAGE              100
#define POLL_CODE_READ_POWER                120
#define POLL_CODE_READ_CURRENT              140

#ifdef PLG_BBB
#define POLL_CODE_READ_END_DAY              160
#define POLL_CODE_READ_END_MONTH            180
#else
#define POLL_CODE_READ_END_DAY              180
#define POLL_CODE_READ_END_MONTH            220
#endif
//error code


//SUB JOURNALS
#define POLL_CODE_METER_STATUS_JRNL         61
#define POLL_CODE_MATILDA_EVNTS             70




//protocol Matilda Extension v1
#define MTD_EXT_NAME_ALL                            0
#define MTD_EXT_NAME_ZBYRATOR                       1
#define MTD_EXT_NAME_PEREDAVATOR                    2
#define MTD_EXT_NAME_MATILDA                        3
#define MTD_EXT_NAME_MATILDA_PROC_MANAGER           4
#define MTD_EXT_NAME_MATILDA_SERVER                 5



#define MTD_EXT_GET_INFO                            1
#define MTD_EXT_GET_LOCATION                        2
#define MTD_EXT_INTERNAL_STATE                      3
#define MTD_EXT_GET_EVENTS                          4
#define MTD_EXT_CUSTOM_COMMAND_0                    5
#define MTD_EXT_CUSTOM_COMMAND_1                    6
#define MTD_EXT_CUSTOM_COMMAND_2                    7



#define MTD_EXT_OPERATION_HISTORY                   8
#define MTD_EXT_PING                                9
#define MTD_EXT_OPERATION_STATISTIC                 10
#define MTD_EXT_OPERATION_SERIAL_LOG                11
#define MTD_EXT_OPERATION_WARN_LOG                  12
#define MTD_EXT_OPERATION_ERROR_LOG                 13
#define MTD_EXT_PEREDAVATOR_AC_LOG                  14
#define MTD_EXT_MATILDA_AC_LOG                      15
#define MTD_EXT_PING_2_SERV                         16




#define MTD_EXT_CUSTOM_COMMAND_3                    30
#define MTD_EXT_CUSTOM_COMMAND_4                    31
#define MTD_EXT_CUSTOM_COMMAND_5                    32
#define MTD_EXT_CUSTOM_COMMAND_6                    33
#define MTD_EXT_CUSTOM_COMMAND_7                    34
#define MTD_EXT_CUSTOM_COMMAND_8                    35
#define MTD_EXT_CUSTOM_COMMAND_9                    36
#define MTD_EXT_CUSTOM_COMMAND_10                   37
#define MTD_EXT_CUSTOM_COMMAND_11                   38
#define MTD_EXT_CUSTOM_COMMAND_12                   39
#define MTD_EXT_CUSTOM_COMMAND_13                   40
#define MTD_EXT_CUSTOM_COMMAND_14                   41



#define MTD_EXT_COMMAND_2_OTHER_APP                 90
#define MTD_EXT_COMMAND_KICK_COORDINATOR            91
#define MTD_EXT_COMMAND_RESTART_APP                 92
#define MTD_EXT_COMMAND_RELOAD_SETT                 93
#define MTD_EXT_COMMAND_RELOAD_DB                   94
#define MTD_EXT_COMMAND_KILL_PEREDAVATOR            95
#define MTD_EXT_COMMAND_PEREDAVATOR_IN_CONN         96
#define MTD_EXT_COMMAND_PEREDAVATOR_IN_IP_CONN      97
#define MTD_EXT_COMMAND_PEREDAVATOR_IN_IP_DISCONN   98


#define MTD_EXT_WRITE_STATE_AUTO_SEND          99


//event_code for meters
#define ZBR_EVENT_OTHER                     1
#define ZBR_EVENT_METER_OPEN                2//
#define ZBR_EVENT_KLEMA_OPEN                3
#define ZBR_EVENT_BAT_LOW                   4//RTC bat
#define ZBR_EVENT_EEPROM_ERROR              5//CE102v10
#define ZBR_EVENT_MAGNET                    6//magnetic field
#define ZBR_EVENT_HARDWARE_ERROR            7//i2c or etc.
#define ZBR_EVENT_ACCESS                    8//only if denied
#define ZBR_EVENT_RELAY                     9 //Load ON, Load Off
#define ZBR_EVENT_INTERNAL_ERROR            10
#define ZBR_EVENT_DATETIME_CORRECTED        11// Before: DT. After: DT or Error
#define ZBR_EVENT_DATETIME_NOT_CORRECT      12// Before: DT. After: DT or Error

#define ZBR_EVENT_UNKNOWN_PROTOCOL          13//protocol not supported or not ready to use
#define ZBR_EVENT_NO_EVNT_4_THIS_DATE       14//
#define ZBR_EVENT_READ_DONE                 15//


#define ZBR_EVENT_MAX_EVNT_CODE             56
#define ZBR_EVENT_DAY_DONE                  255//Ставиться в кінці таблиці в кінці дня по локальному часі (означа що за вказаний день в локальному часі всі дані зібрано, не може бути за поточний день)

//event4matilda
#define ZBR_EVENT_DATETIME_NEED2CORR        51// Before: DT. After: DT or Error
#define ZBR_EVENT_DATETIME_NOT_CORR         52// Before: DT. After: DT or Error
#define ZBR_EVENT_DATETIME_CORR_DONE        53// Before: DT. After: DT or Error
#define ZBR_EVENT_INVALID_ARGUMENT          54//

#define ZBR_EVENT_OTHER_4_MTDLD             55


//network speed level
#define NET_SPEED_VERY_LOW      3
#define NET_SPEED_LOW           11
#define NET_SPEED_NORMAL        30   //if speed > NET_SPEED_NORMAL : disable compressing
#define NET_SPEED_HIGH          55//300
#define NET_SPEED_VERY_HIGH     200//1500
#define NET_SPEED_UFS_1         800//1500


//#define MTD_EXT_

#define MTD_USER_ADMIN           1
#define MTD_USER_OPER           2
#define MTD_USER_GUEST           3


//стани клієнтів матільди та передаватора
#define AC_STATE_UNKNOWN        0
#define AC_STATE_READY          1
#define AC_STATE_CONNECTING     2
#define AC_STATE_CLOSED         3
#define AC_STATE_AUTHORIZATION  4



#endif // MOJI_DEFY

