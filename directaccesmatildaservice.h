/*

 */

#ifndef DIRECTACCESMATILDASERVICE_H
#define DIRECTACCESMATILDASERVICE_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

class DirectAccesMatildaService : public QTcpServer
{
    Q_OBJECT
public:
    explicit DirectAccesMatildaService(QObject *parent = 0);

signals:
    void stopAllNow();
    void onStateChanged(bool);
    void onStateChanged(QString);//до лейби статусу

    void data2coordiantor(QByteArray);
    void dataFromCoordinator(QByteArray);
    void showMess(QString);

    void onDasStarted(QString);
    void onDasStopped();

    void setDaForwardNISign(QByteArray ni);

public slots:
    void startServerNow(qint8 mode, quint16 port);

    void onSocketKickOff();

    void updateStatus();

    void stopServerNow();

    void onMatildaConnectedStateChanged(bool isConnUp);

    void setDaForwardNI(QByteArray ni);


protected:
    void incomingConnection(qintptr socketDescr);

private:
    qint32 clientCount;
    bool lastState;
    QByteArray modemNI;

};


class DirectAccessSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit DirectAccessSocket(QObject *parent = 0);

public slots:
    void mWrite2Local(QByteArray writeStr);
    void setDaForwardNI(QByteArray ni);


signals:
    void data2coordiantor(QByteArray);
    void onSocketKickOff();

private slots:
    void mReadyRead();

    void onDisconn();

private:
    bool isKickOff;
    QByteArray modemNI;




};

#endif // DIRECTACCESMATILDASERVICE_H
