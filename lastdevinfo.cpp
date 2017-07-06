#include "lastdevinfo.h"

LastDevInfo::LastDevInfo(QObject *parent) : QObject(parent)
{


    clearMatildaDev();
}

void LastDevInfo::clearMatildaDev()
{
    matildaDev.coordinatesIsDefault = true;

    QDate dStart = QDate(QDateTime::currentDateTime().date().year(), 1, 1);
    QTime tStart = QTime(0,0,0);
    int offset = QDateTime(dStart, tStart).offsetFromUtc();
    matildaDev.coordinates.setX((qreal)offset/(qreal)240.0 );
    matildaDev.coordinates.setY(-49.0);
    matildaDev.lastSerialNumber.clear();
}
