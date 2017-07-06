#ifndef LASTDEVINFO_H
#define LASTDEVINFO_H

#include <QObject>
#include <QtCore>

class LastDevInfo : public QObject
{
    Q_OBJECT
public:
    explicit LastDevInfo(QObject *parent = 0);

    struct DevInfo{
        QPointF coordinates;
        bool coordinatesIsDefault;

        QString lastSerialNumber;
        int devVersion;
        int devType;
        int protocolVersion;

        QVariantHash ABOUT_OBJECT;
        QVariantHash POLL_SETT;

    } matildaDev;

signals:

public slots:
    void clearMatildaDev();
};

#endif // LASTDEVINFO_H
