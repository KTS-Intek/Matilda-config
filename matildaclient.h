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
    void data2gui(quint16,QJsonObject);
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

    void dataFromCoordinator(QByteArray);

    void onYouCanSelectDevice(QStringList);

    void add2pteLog(QString);

public slots:
    void conn2thisDev(int hashIndx, QString objN, QString login, QString passwd, QString add, quint16 port, int timeOut, bool add2list, bool allwCmprss, bool useMac, QString macAddr, bool useMacAddr2conn);

    void data2matilda(quint16 command, QJsonObject jobj);
    void closeConnection();

    void stopAllNow();
    void data2coordiantor(QByteArray writeArr);

    void setEmptyHsh(bool empty);



private slots:
    void decodeReadDataJSON(const QByteArray &dataArr);


    void mReadyRead();

    void mWrite2SocketJSON(QJsonObject jObj, const quint16 s_command);

    void onDisconn();

    void onWaitTimerTimeOut();
    void onDaOpened(bool isDaOpen);

    void add2pteLogP(QString s, bool isRead);



private:
    void getRightLeftDuzka(int &rightDuzka, int &leftDuzka, bool &wait4lapky, const QByteArray &arr);
    int indxOfRightDuzka(const int &lastRightDuzka, const QByteArray &arr);

    QString humanByteView(QString str);

    QString humanByteView(const int &val);

    QString hshSummName(const int &indx) const;
    QStringList getHshNames();

    bool messHshIsValid(const QJsonObject &jObj, QByteArray readArr);




private:
    bool isConnOpen();

    QDateTime dateTimeFromStr(const QString &str);
    bool block4activeClient;

    quint8 accessLevel;
     QList<QByteArray> loginPasswd;
     int lastHashSumm;

    bool iAmDone;
    int timeOut;
    int timeOutG;
    bool isSvahaService;

    bool stopAll, stopAfter, daOpened;
    bool allowCompress;

    bool emptyHsh;

};

#endif // MATILDACLIENT_H
