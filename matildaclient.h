#ifndef MATILDACLIENT_H
#define MATILDACLIENT_H

#include <QTcpSocket>
#include <QObject>
#include <QTime>
#include <QJsonObject>

class matildaclient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit matildaclient(QObject *parent = 0);

signals:
    void onConnectedStateChanged(bool);
    void data2gui(quint16,QVariantHash);
    void onErrorWrite();
    void showMess(QString);
    void authrizeAccess(int); //0 - closed, 1 - root, 2 - operator, 3 - guest
    void addThisLogin2list(QString,QString,QString,QString,quint16);

    void infoAboutObj(QString);

    void startWait4AnswerTimer(int);
    void stopWait4AnswerTimer();
    void restartWaitCounter();
    void uploadProgress(int,QString);

    void hideAnimation();

    void changeCounters(qint64, qint64, bool );



public slots:
    void conn2thisDev(int hashIndx, QString login, QString passwd, QString add, quint16 port, int timeOut, bool add2list, bool allwCmprss);
    void data2matilda(quint16 command, QVariantHash hash);
    void closeConnection();

    void stopAllNow();


private slots:
    void decodeReadDataJSON(const QByteArray &dataArr);


    void mReadyRead();

    void mWrite2SocketJSON(QJsonObject jObj, const quint16 s_command);

    void onDisconn();

    void onWaitTimerTimeOut();




private:
    QString humanByteView(QString str);

    QString humanByteView(const int &val);

    QString hshSummName(const int &indx) const;
    QStringList getHshNames() const;

    bool messHshIsValid(QJsonObject jObj);




private:
    bool isConnOpen();

    QDateTime dateTimeFromStr(const QString &str);

    quint8 accessLevel;
     QList<QByteArray> loginPasswd;
     int lastHashSumm;

    bool iAmDone;
    int timeOut;
    int timeOutG;

    bool stopAll, stopAfter;
    bool allowCompress;


};

#endif // MATILDACLIENT_H
