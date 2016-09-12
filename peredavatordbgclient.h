#ifndef PEREDAVATORDBGCLIENT_H
#define PEREDAVATORDBGCLIENT_H


#include <QTcpSocket>
#include <QObject>

class PeredavatorDbgClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit PeredavatorDbgClient(QObject *parent = 0);

signals:
    void appendPlainText(QString);
    void showMess(QString);
    void onConnectedStateChanged(bool);
    void onCantConnect(bool);

    void changeCounters(qint64, qint64, bool );



public slots:
    void conn2thisDev(QString add, quint16 port, int timeOut);
    void closeConnection();

    void stopAllNow();

private slots:
    void mReadyRead();
      void onDisconn();

private:
    bool stopAll;
};

#endif // PEREDAVATORDBGCLIENT_H
