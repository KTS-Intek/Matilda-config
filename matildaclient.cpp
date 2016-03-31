#include "matildaclient.h"
#include "moji_defy.h"
#include <QTimer>
#include <QtCore>
#include <QStringList>
#include <QHostAddress>

#include <QJsonObject>
#include <QVariantMap>

//################################################################################################
matildaclient::matildaclient(QObject *parent) : QTcpSocket(parent)
{
    connect(this, SIGNAL(readyRead()), SLOT(mReadyRead()) );
    connect(this, SIGNAL(disconnected()), SLOT(onDisconn()) );

    QTimer *timerWait4Answer = new QTimer;
    timerWait4Answer->setSingleShot(true);
    connect(this, SIGNAL(startWait4AnswerTimer(int)), timerWait4Answer, SLOT(start(int)) );
    connect(this, SIGNAL(stopWait4AnswerTimer()), timerWait4Answer, SLOT(stop()) );
    connect(timerWait4Answer, SIGNAL(timeout()), this, SLOT(onWaitTimerTimeOut()) );
}
//################################################################################################
void matildaclient::conn2thisDev(QString objN, QString login, QString passwd, QString add, quint16 port, int timeOut, bool add2list, bool allwCmprss)
{
    loginPasswd.clear();
    disconnect(this, SIGNAL(disconnected()), this, SLOT(onDisconn()) );
    iAmDone = false;

    if(isConnOpen())
        close();
//    waitForDisconnected();
    this->timeOut = 500;
    this->timeOutG = timeOut;

    timeOut -= 555;

    if(timeOut < 3000)
        timeOut = 3000;


//    dataStreamVersion = QDataStream::Qt_DefaultCompiledVersion;
    connectToHost(add,port);
    if(waitForConnected(timeOut)){
        connect(this, SIGNAL(disconnected()), this, SLOT(onDisconn()) );


        emit onConnectedStateChanged(true);
        loginPasswd.append(QCryptographicHash::hash(login.simplified().trimmed().toLocal8Bit(), QCryptographicHash::Sha3_256));
        loginPasswd.append(QCryptographicHash::hash( passwd.simplified().trimmed().toLocal8Bit(), QCryptographicHash::Sha3_256));
        loginPasswd.append("");
        allowCompress = allwCmprss;
        stopAll = false;
        if(add2list)
            emit addThisLogin2list(objN, login, passwd, add, port);
    }else{

        emit showMess(tr("Can't connect to device. Error: %1").arg(errorString()));
         close();
        return;
    }
}
//################################################################################################
void matildaclient::data2matilda(quint16 command, QVariantHash hash)
{    
    QJsonObject obj;
    foreach (QString key, hash.keys()) {
        obj.insert(key, hash.value(key).toString());
    }
    mWrite2SocketJSON(obj, command);
}
//################################################################################################
void matildaclient::closeConnection()
{
    onDisconn();
    emit showMess(tr("Done."));
}
//################################################################################################
void matildaclient::stopAllNow()
{
    stopAll = true;
}

//################################################################################################
void matildaclient::decodeReadDataJSON(const QByteArray &dataArr)
{
    QJsonParseError jErr;
    QJsonDocument jDoc = QJsonDocument::fromJson( dataArr, &jErr);

    QVariantHash hash = jDoc.object().toVariantHash();
//    qDebug() << hash;


    qint64 len = dataArr.length();
    qint64 lenUn = -1;
    quint16 command = hash.take("cmd").toUInt();

    bool checkHash = true;
    if(command == COMMAND_COMPRESSED_PACKET){
        checkHash = false;
        jDoc = QJsonDocument::fromJson( qUncompress( QByteArray::fromBase64( hash.value("zlib").toByteArray() )  ), &jErr);
        hash = jDoc.object().toVariantHash();
        lenUn = jDoc.toJson(QJsonDocument::Compact).length();
        command = hash.take("cmd").toUInt();

    }

    emit changeCounters(len, lenUn, true);

    if(command != COMMAND_READ_DATABASE && command != COMMAND_READ_DATABASE_GET_VAL  ){
    qDebug() << "decodeReadData" << command << stopAfter << stopAll;
    qDebug()  << jDoc.object();

    }

    if(checkHash){
        QByteArray myHash;
        QByteArray hshBase64;
        QString hshName;
        QJsonObject jObj = jDoc.object();

        if(hash.contains("Md5")){ //default hash sum
            hshName = "Md5";
            hshBase64 = QByteArray::fromBase64(jObj.take(hshName).toString().toLocal8Bit());
            jObj.insert(hshName, QString("0"));
            QJsonDocument jDoc2h(jObj);
            myHash = QCryptographicHash::hash(jDoc2h.toJson(QJsonDocument::Compact), QCryptographicHash::Md5);

        }else{
            hshName = getHshNames().at(lastHashSumm);
            hshBase64 = QByteArray::fromBase64(jObj.take(hshName).toString().toLocal8Bit());
            jObj.insert(hshName, QString("0"));
            QJsonDocument jDoc2h(jObj);
            myHash = QCryptographicHash::hash(jDoc2h.toJson(QJsonDocument::Compact), static_cast<QCryptographicHash::Algorithm>(lastHashSumm));
        }

        if(myHash != hshBase64){
            qDebug() << "if(myHash != hshBase64 " << myHash.toBase64() << hshBase64.toBase64();
            emit showMess(tr("Received uncorrect request"));
//            onDisconn();
            return;
        }
        hash.remove(hshName);


    }

    if(stopAll && command != COMMAND_AUTHORIZE && command != COMMAND_I_AM_ZOMBIE && command != COMMAND_I_NEED_MORE_TIME){
        return;
    }

    if(command != COMMAND_READ_DATABASE_GET_VAL && command != COMMAND_READ_DATABASE && command != COMMAND_READ_DATABASE_GET_TABLES && command != COMMAND_READ_METER_LIST_FRAMED
            && command != COMMAND_WRITE_METER_LIST_FRAMED && command != COMMAND_WRITE_DROP_TABLE  && command != COMMAND_WRITE_DROP_TABLE_GET_COUNT
            && command != COMMAND_READ_METER_LOGS_GET_TABLES && command != COMMAND_READ_METER_LOGS_GET_VAL && command != COMMAND_I_NEED_MORE_TIME
            && command != COMMAND_READ_METER_LOGS){
        emit hideAnimation();
    }

    switch(command){
    case COMMAND_ZULU:{
        if(hash.value("name").toString() == "Matilda" && hash.value("version").toString() == "1" && QDateTime::fromString(hash.value("UTC").toString(), "yyyy-MM-dd hh:mm:ss").isValid()){
            if(!hash.value("err").toString().isEmpty()){

                emit infoAboutObj(tr("DateTime: %1 UTC%2 <br> Version: %3 <br> message: %4")
                                  .arg(hash.value("UTC").toDateTime().addSecs(hash.value("UOFT").toInt()).toString("yyyy-MM-dd hh:mm:ss"))
                                  .arg( (hash.value("UOFT").toInt() < 0) ? QString::number(hash.value("UOFT").toInt()) : QString("+%1").arg(hash.value("UOFT").toInt()) )

                                  .arg( QString::number(hash.value("version").toInt() ))
                                  .arg(hash.value("message").toString())
                                  );

                qDebug() << hash.value("err").toString();

                emit showMess(hash.value("message").toString());
                  onDisconn();
                return;
            }else{

                emit infoAboutObj(tr("DateTime: %1 UTC%2 <br>SSL: %3 <br>Version: %4 <br> Black List: %5 <br> Counter: %6<br>Memo: %7")
                                  .arg(hash.value("UTC").toDateTime().addSecs(hash.value("UOFT").toInt()).toString("yyyy-MM-dd hh:mm:ss"))
                                  .arg( (hash.value("UOFT").toInt() < 0) ? QString::number(hash.value("UOFT").toInt()) : QString("+%1").arg(hash.value("UOFT").toInt()) )
                                  .arg( (hash.value("SSL").toInt() == 0) ? tr("Disabled") : tr("Enabled") )
                                  .arg( QString::number(hash.value("version").toInt() ))
                                  .arg(hash.value("BLC").toInt() )
                                  .arg( hash.value("CNTR").toInt() )
                                  .arg(hash.value("memo").toString())
                                  );

                if(hash.value("version").toInt() == MATILDA_PROTOCOL_VERSION){
//                    dataStreamVersion = hash.value("QDS").toInt();


                    QJsonObject jObj;

                    jObj.insert("version", hash.value("version").toInt());
                    jObj.insert("hsh", QString(QCryptographicHash::hash(loginPasswd.at(0) + "\n" + dataArr + "\n" + loginPasswd.at(1), QCryptographicHash::Sha3_256).toBase64()));
//
                    //mode JSON and QDataStream
//                    jObj.insert("QDS", QString::number(dataStreamVersion));//активація режиму QDataStream

                    if(allowCompress)
                        jObj.insert("cmprssn", "zlib");//дозволити стиснення
                    lastHashSumm = getHshNames().indexOf("Sha1");

                    stopAfter = false;
                    mWrite2SocketJSON(jObj, COMMAND_AUTHORIZE);
                    return;
                }

            }
        }


        emit infoAboutObj(tr("DateTime: %1 UTC%2 <br>SSL: %3 <br>Version: %4 <br> Black List: %5 <br> Counter: %6")
                          .arg(hash.value("UTC").toDateTime().addSecs(hash.value("UOFT").toInt()).toString("yyyy-MM-dd hh:mm:ss"))
                          .arg( (hash.value("UOFT").toInt() < 0) ? QString::number(hash.value("UOFT").toInt()) : QString("+%1").arg(hash.value("UOFT").toInt()) )
                          .arg( (hash.value("SSL").toInt() == 0) ? tr("Disabled") : tr("Enabled") )
                          .arg( QString::number(hash.value("version").toInt() ))
                          .arg(hash.value("BLC").toInt() )
                          .arg( hash.value("CNTR").toInt() )
                          );

        emit showMess(tr("Unknown device."));
        onDisconn();
        break;}

    case COMMAND_AUTHORIZE:{
        qDebug() << "access = " << hash;

        switch(hash.value("a").toUInt()){
        case MTD_USER_OPER:{
            emit authrizeAccess(hash.value("a").toUInt());
            emit data2gui(command, hash);
            emit showMess(tr("Hello, you only operator!"));
            break;}

        case MTD_USER_GUEST:{
            emit authrizeAccess(hash.value("a").toUInt());
            emit data2gui(command, hash);
            emit showMess(tr("Hello, you only guest!"));
            break;}

        default: {
            emit authrizeAccess(0);
            emit showMess(tr("Session closed."));
            onDisconn();
            break;}
        }
        return;}

    case COMMAND_I_NEED_MORE_TIME:{
        emit startWait4AnswerTimer(timeOutG);
        emit data2gui(command, hash);
        stopAfter = false;
        return;}

    case COMMAND_I_AM_ZOMBIE:{ qDebug() << "zombie killer" << peerAddress();   mWrite2SocketJSON(QJsonObject(), COMMAND_I_AM_ZOMBIE); return;}

    default:{ emit data2gui(command, hash); return;}

    }


}
//################################################################################################
void matildaclient::mReadyRead()
{

    emit startWait4AnswerTimer(timeOutG);

    QByteArray readarr = readAll();
    QTime time;
    time.start();
    while(readarr.right(1) != "}" && readarr.size() < MAX_PACKET_LEN && time.elapsed() < timeOutG){
        if(waitForReadyRead(timeOut)){
            readarr.append(readAll());
            waitForReadyRead(100);
            emit startWait4AnswerTimer(timeOutG);
            waitForReadyRead(100);
        }
    }

    emit startWait4AnswerTimer(timeOutG);


    emit stopWait4AnswerTimer();
    stopAfter = true;

    int lastIndx = 0;
    int duzkaIndx = readarr.indexOf("}");


    if(duzkaIndx < 1){
        emit changeCounters(readarr.length(), -1 , true);
        emit showMess("corrupted data.");
        qDebug()<< "readServer:"<< readarr;
        return ;
    }else{

        int len = readarr.length();
        while(duzkaIndx > 1 && lastIndx < len){
//            qDebug() << lastIndx << duzkaIndx << readarr.mid(lastIndx, duzkaIndx + 1) << len;

            decodeReadDataJSON(readarr.mid(lastIndx, duzkaIndx + 1));
            duzkaIndx = readarr.indexOf("}", lastIndx);
            lastIndx = duzkaIndx + 1;

        }
    }
    stopAll = stopAfter;
    qDebug() << stopAll << stopAfter;
}

//################################################################################################
void matildaclient::mWrite2SocketJSON(QJsonObject jObj, const quint16 s_command)
{
    if(!isConnOpen())
        return;
    jObj.insert("cmd", s_command);
    stopAll = false;


    qint64 blSize = 0;
    QByteArray writeArr;
    if(blSize < 10){
        QJsonDocument jDoc2DST(jObj);
        writeArr = jDoc2DST.toJson(QJsonDocument::Compact);
        blSize = writeArr.length() ;
    }

    if(blSize < SETT_MAX_UNCOMPRSS_PACkET_SIZE || s_command == 0 || s_command == COMMAND_COMPRESSED_STREAM || !allowCompress){

        switch(lastHashSumm){
        case 0:{
            jObj.insert("Md4", QString("0"));
            QJsonDocument jDoc2DST(jObj);
            jObj.insert("Md4", QString(QCryptographicHash::hash(jDoc2DST.toJson(QJsonDocument::Compact), QCryptographicHash::Md4).toBase64() ));
            break;}
        case 2:{ jObj.insert("Sha1", QString("0")); QJsonDocument jDoc2DST(jObj); jObj.insert("Sha1", QString(QCryptographicHash::hash(jDoc2DST.toJson(QJsonDocument::Compact), QCryptographicHash::Sha1).toBase64())); break;}
        case 3:{ jObj.insert("Sha224", QString("0")); QJsonDocument jDoc2DST(jObj); jObj.insert("Sha224", QString(QCryptographicHash::hash(jDoc2DST.toJson(QJsonDocument::Compact), QCryptographicHash::Sha224).toBase64())); break;}
        case 4:{ jObj.insert("Sha256", QString("0")); QJsonDocument jDoc2DST(jObj); jObj.insert("Sha256", QString(QCryptographicHash::hash(jDoc2DST.toJson(QJsonDocument::Compact), QCryptographicHash::Sha256).toBase64())); break;}
        case 5:{ jObj.insert("Sha384", QString("0")); QJsonDocument jDoc2DST(jObj); jObj.insert("Sha384", QString(QCryptographicHash::hash(jDoc2DST.toJson(QJsonDocument::Compact), QCryptographicHash::Sha384).toBase64())); break;}
        case 6:{ jObj.insert("Sha512", QString("0")); QJsonDocument jDoc2DST(jObj); jObj.insert("Sha512", QString(QCryptographicHash::hash(jDoc2DST.toJson(QJsonDocument::Compact), QCryptographicHash::Sha512).toBase64())); break;}
        case 7:{ jObj.insert("Sha3_224", QString("0")); QJsonDocument jDoc2DST(jObj); jObj.insert("Sha3_224", QString(QCryptographicHash::hash(jDoc2DST.toJson(QJsonDocument::Compact), QCryptographicHash::Sha3_224).toBase64())); break;}
        case 8:{ jObj.insert("Sha3_256", QString("0")); QJsonDocument jDoc2DST(jObj); jObj.insert("Sha3_256", QString(QCryptographicHash::hash(jDoc2DST.toJson(QJsonDocument::Compact), QCryptographicHash::Sha3_256).toBase64())); break;}
        case 9:{ jObj.insert("Sha3_384", QString("0")); QJsonDocument jDoc2DST(jObj); jObj.insert("Sha3_384", QString(QCryptographicHash::hash(jDoc2DST.toJson(QJsonDocument::Compact), QCryptographicHash::Sha3_384).toBase64())); break;}
        case 10:{ jObj.insert("Sha3_512", QString("0")); QJsonDocument jDoc2DST(jObj); jObj.insert("Sha3_512", QString(QCryptographicHash::hash(jDoc2DST.toJson(QJsonDocument::Compact), QCryptographicHash::Sha3_512).toBase64())); break;}

        default:{
            jObj.insert("Md5", QString("0"));
            QJsonDocument jDoc2DST(jObj);
            jObj.insert("Md5", QString(QCryptographicHash::hash(jDoc2DST.toJson(QJsonDocument::Compact), QCryptographicHash::Md5).toBase64()));
            break;}

        }
        blSize = -1;
    }else{
        QList<QString> l = jObj.toVariantMap().keys();
        while(!l.isEmpty())
            jObj.remove(l.takeFirst());
        jObj.insert("cmd", QString::number(COMMAND_COMPRESSED_PACKET));
        jObj.insert("zlib", QString(qCompress(writeArr, 9).toBase64()));
    }

    QJsonDocument jDoc(jObj);
    qint64 len = write(jDoc.toJson(QJsonDocument::Compact));

    qDebug() << jObj;


    emit changeCounters(len, blSize , false);

    waitForBytesWritten(50);
    if(s_command != COMMAND_I_AM_ZOMBIE )
        emit startWait4AnswerTimer(timeOutG);
}

//################################################################################################
void matildaclient::onDisconn()
{
    qDebug() << "onDisconn";
    emit stopWait4AnswerTimer();
    if(!iAmDone){
        emit onConnectedStateChanged(false);
    }
    iAmDone = true;
    close();
}
//################################################################################################
void matildaclient::onWaitTimerTimeOut()
{

}
//################################################################################################
QString matildaclient::humanByteView(QString str)
{
    QString retVal = str.right(3);
    str.chop(3);

    while(!str.isEmpty()){
        retVal.prepend(str.right(3) + " ");
        str.chop(3);
    }
    return retVal;
}
//################################################################################################
QString matildaclient::humanByteView(const int &val)
{
    return humanByteView(QString::number(val));
}
//################################################################################################
QString matildaclient::hshSummName(const int &indx) const
{
    QStringList hashList = QString("Md4,Md5,Sha1,Sha224,Sha256,Sha384,Sha512,Sha3_224,Sha3_256,Sha3_384,Sha3_512").split(",");
    if(indx < 0 || indx >= hashList.size())
        return "Md5";

    return hashList.at(indx);
}
//################################################################################################
QStringList matildaclient::getHshNames() const
{
    return QString("Md4,Md5,Sha1,Sha224,Sha256,Sha384,Sha512,Sha3_224,Sha3_256,Sha3_384,Sha3_512").split(",");
}
//################################################################################################
bool matildaclient::isConnOpen()
{
    return (state() == QAbstractSocket::ConnectedState || state() == QAbstractSocket::ConnectingState);
}
//################################################################################################
QDateTime matildaclient::dateTimeFromStr(const QString &str)
{//ISO8601 strings ("YYYY-MM-DD HH:MM:SS").
    return QDateTime( QDate::fromString(str.left(10), "yyyy-MM-dd"), QTime::fromString(str.right(8), "hh:mm:ss"), Qt::UTC );
}
//################################################################################################
