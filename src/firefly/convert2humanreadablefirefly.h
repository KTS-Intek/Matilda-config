#ifndef CONVERT2HUMANREADABLEFIREFLY_H
#define CONVERT2HUMANREADABLEFIREFLY_H

#include <QObject>
#include <QStringList>



class Convert2humanReadableFirefly : public QObject
{
    Q_OBJECT
public:
    explicit Convert2humanReadableFirefly(QObject *parent = nullptr);

    static QString scheduleMinutesToHourMinutesLine(const QString &fromTheLine);

    static QStringList scheduleMinutesToHourMinutes(const QString &fromTheLine);

    static QStringList scheduleHourMinutesToMinutes(const QString &fromTheLine);

    static QStringList scheduleHourMinutesToMinutesList(const QStringList &fromTheList);

signals:

public slots:
};

#endif // CONVERT2HUMANREADABLEFIREFLY_H
