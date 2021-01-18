#include "convert2humanreadablefirefly.h"

#include <QTime>

Convert2humanReadableFirefly::Convert2humanReadableFirefly(QObject *parent) : QObject(parent)
{

}

QString Convert2humanReadableFirefly::scheduleMinutesToHourMinutesLine(const QString &fromTheLine)
{
    return scheduleMinutesToHourMinutes(fromTheLine).join(",");
}

QStringList Convert2humanReadableFirefly::scheduleMinutesToHourMinutes(const QString &fromTheLine)
{
    //minutes from MN to HH:MM

    const QStringList dpLine = fromTheLine.split(",");
    QStringList outDpLine;

    for(int m = 0, mMax = dpLine.size(); m < mMax; m++){
        QString s = dpLine.at(m);
        QStringList ll = s.split("-");
        bool ok;
        int val = ll.first().toInt(&ok);
        if(ok && val < 1440 && val >= 0){
            QTime time(0,0,0,0);
            time = time.addSecs(val * 60);
            ll.removeFirst();
            s = time.toString("hh:mm") + "-" + ll.join("-");
        }

        outDpLine.append(s);
    }
    return outDpLine;
}

QStringList Convert2humanReadableFirefly::scheduleHourMinutesToMinutes(const QString &fromTheLine)
{
    const QTime tStart(0,0,0,0);

    QString s = fromTheLine;
    //HH:MM-<power>,
    const QStringList ldp = s.split(",", QString::SkipEmptyParts);
    QStringList sdp;
    for(int m = 0, mMax = ldp.size(); m < mMax; m++){
        s = ldp.at(m);
        const QTime time = QTime::fromString(s.left(5), "hh:mm");
        if(time.isValid())
            s = QString::number(tStart.secsTo(time)/60) + s.mid(5);

        sdp.append(s);
    }
    return sdp;

}

QStringList Convert2humanReadableFirefly::scheduleHourMinutesToMinutesList(const QStringList &fromTheList)
{
    QStringList l;
    for(int i = 0, imax = fromTheList.size(); i < imax; i++)
        l.append(scheduleHourMinutesToMinutes(fromTheList.at(i)));
    return l;
}

