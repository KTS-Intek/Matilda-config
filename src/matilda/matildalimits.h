#ifndef MATILDALIMITS_H
#define MATILDALIMITS_H


#define MAX_RELAY_COUNT                     50
#define MAX_METER_COUNT                     5000
#define MAX_REQUEST_NI_IN_LINE              300
#define MAX_PRECISION                       9
#define MAX_INRVL_AGO                       100// Днів/Місяців назад
#define MAX_FF_DAYPROFILE                   1000

#define MAX_MSEC_TIME2OPEN_DA               100
#define MAX_TARIFF_COUNT                    5//0 - summ, 1 T1, 2 T2, 3 T3, 4 T4
#define MAX_DATALEN                         2800 //for reading from DB  2610 * k =~ 5875 byte
#define MAX_TABLE_READ                      450
#define MAX_PACKET_LEN_RECOMENDATION        1400
#define MAX_PACKET_LEN                      10000000

#define MAX_DATABASE_SIZE                   550000000 //~550M
#define MAX_DATABASE_RESERV_SIZE            5000000 //~5M
#define MAX_DATABASE_RESERV_SIZE_FREE_DISK  50000000 //~50M
#define MAX_DATABASE_SELECT_ROWS_ONE_TIME   1000

#define MAX_PACKET_SIZE_ONLY4DATA           5000000
#define MAX_GLYBYNA                         0x64
#define MAX_TIME_4_PPP_SUPERVISOR           3000
#define MAX_TIME_4_PPP_SUPERVISOR_HOURS     550


#define MAX_TIME_4_IFACE_SUPERVISOR           3000
#define MAX_TIME_4_IFACE_SUPERVISOR_HOURS     550

#define MAX_METER_READ_FROM_DB              100000
#define MAX_BADDATA_SIZE                    50000

#define MIN_AUTO_PACKET_LEN                 700

#define MAX_CLIENT_COUNT                     20


#define MAX_DIFF_DT_SECS  2400//1200

#define MAX_WATER_PROFILES      20
#define MAX_READ_PLAIN_TEXT_FILE        3000000

#endif // MATILDALIMITS_H
