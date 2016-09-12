#ifndef UDPSCANNER_H
#define UDPSCANNER_H

#include <QObject>
#include <QtNetwork>
#include <QHostAddress>
#include <QUdpSocket>


class UdpScanner : public QObject
{
    Q_OBJECT
public:
    explicit UdpScanner(QObject *parent = 0);

signals:
    void stopPrevSession();
    void findedDevice(QString,QString,QString); //obj id , list ip, server port

    void showMess(QString);
    void scanStarted();
    void scanFinished();

    void startTmrs();

public slots:
    void startUdpScan(quint16 scanSessionID);
    void stopAllNow();



private slots:
    void mReadyRead();
    void prepareReadSocket();
    void mWriteDatagramku();
    void killUdpRead();

private:
    bool stopAll;
    quint16 scanSessionID;

    QString udpServiceId;
    QString devName;
    QHostAddress groupAddress;
    QHostAddress groupAddress2Android;

    QStringList listDev;
    QUdpSocket *udpSocketRead;
    QUdpSocket *udpSocketWrite;
    QByteArray wrtArr;

};

#endif // UDPSCANNER_H
