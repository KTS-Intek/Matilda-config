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
void matildaclient::conn2thisDev(int hashIndx, QString objN, QString login, QString passwd, QString add, quint16 port, int timeOut, bool add2list, bool allwCmprss, bool useMac, QString macAddr, bool useMacAddr2conn)
{
    daOpened = false;
    block4activeClient = true;

    Q_UNUSED(add2list);
    loginPasswd.clear();
    disconnect(this, SIGNAL(disconnected()), this, SLOT(onDisconn()) );
    iAmDone = false;
    accessLevel = 0;

    if(isConnOpen())
        close();
    //    waitForDisconnected();
    this->timeOut = 500;//read one block
    this->timeOutG = timeOut;

    timeOut -= 555;

    if(timeOut < 3000)
        timeOut = 3000;

    isSvahaService = useMac;
    //    dataStreamVersion = QDataStream::Qt_DefaultCompiledVersion;
    connectToHost(add,port);
    lastHashSumm = hashIndx;

    block4activeClient = false;

    if(waitForConnected((timeOut > 9000) ? 9000 : timeOut)){        


        connect(this, SIGNAL(disconnected()), this, SLOT(onDisconn()) );


        emit onConnectedStateChanged(true);
        loginPasswd.append(QCryptographicHash::hash(login.simplified().trimmed().toLocal8Bit(), QCryptographicHash::Sha3_256));
        loginPasswd.append(QCryptographicHash::hash( passwd.simplified().trimmed().toLocal8Bit(), QCryptographicHash::Sha3_256));
        loginPasswd.append("");
        allowCompress = allwCmprss;
        stopAll = false;

        if(useMac){

//            lastAdd2list = add2list;
            QJsonObject jObj;
            jObj.insert("version", MATILDA_PROTOCOL_VERSION);
            jObj.insert("useId", !useMacAddr2conn);
//
            //mode JSON and QDataStream
//            jObj.insert("QDS", QString::number(dataStreamVersion));//активація режиму QDataStream
            if(allwCmprss)
                jObj.insert("cmmprssn", "zlib");
            jObj.insert("remote", (useMacAddr2conn) ? macAddr : objN);
            stopAll = false;

             mWrite2SocketJSON(jObj, COMMAND_CONNECT_ME_2_THIS_ID_OR_MAC);


            return;
        }
    }else{

        emit showMess(tr("Can't connect to device. Error: %1").arg(errorString()));
        close();
        return;
    }
}
//################################################################################################
void matildaclient::data2matilda(quint16 command, QJsonObject jobj)
{    

    mWrite2SocketJSON(jobj, command);
}
//################################################################################################
void matildaclient::closeConnection()
{
    block4activeClient = false;

    onDisconn();
    emit showMess(tr("Done."));
}
//################################################################################################
void matildaclient::stopAllNow()
{
    if(stopAll)
        return;

    if(accessLevel < 1)
        QTimer::singleShot(1, this, SLOT( onDisconn()));

    stopAll = true;
}
//################################################################################################
void matildaclient::data2coordiantor(QByteArray writeArr)
{
    if(state() == QAbstractSocket::ConnectedState && daOpened){
        QJsonObject jobj;
        jobj.insert("d", QString(writeArr.toBase64(QByteArray::OmitTrailingEquals)));
        mWrite2SocketJSON(jobj, COMMAND_WRITE_DA_DATA_2_COORDINATOR);
    }
}

void matildaclient::setEmptyHsh(bool empty)
{
    this->emptyHsh = empty;
}

//################################################################################################
void matildaclient::decodeReadDataJSON(const QByteArray &dataArr)
{
    QJsonParseError jErr;
    QJsonDocument jDoc = QJsonDocument::fromJson( dataArr, &jErr);

    QJsonObject jobj = jDoc.object();

    qint64 len = dataArr.length();
    qint64 lenUn = -1;
    quint16 command = (quint16)jobj.take("cmd").toInt();

    if(command == COMMAND_COMPRESSED_PACKET){

        if(messHshIsValid(jDoc.object(), dataArr)){

            add2pteLogP( "\nUncompress: " + qUncompress( QByteArray::fromBase64( jobj.value("zlib").toString().toLocal8Bit() )  ), true);

            jDoc = QJsonDocument::fromJson( qUncompress( QByteArray::fromBase64( jobj.value("zlib").toString().toLocal8Bit() )  ), &jErr);//qUncompress( <quint32 not compressed data len><comprssed data> )
            jobj = jDoc.object();
            lenUn = jDoc.toJson(QJsonDocument::Compact).length();
            command = (quint16)jobj.take("cmd").toInt();

        }else{
            emit showMess(tr("Received uncorrect request"));
            return;
        }

    }

    emit changeCounters(len, lenUn, true);

    if(command < COMMAND_READ_DATABASE || command > COMMAND_READ_METER_LOGS_GET_VAL  ){
        qDebug() << "decodeReadData" << command << stopAfter << stopAll;
        qDebug()  << jDoc.object();

        qDebug()  << jDoc.toJson(QJsonDocument::Compact).toHex();
    }

    if(!messHshIsValid(jobj, dataArr)){
        emit showMess(tr("Received uncorrect request"));
        return;
    }


    if(stopAll && command != COMMAND_AUTHORIZE && command != COMMAND_I_AM_A_ZOMBIE && command != COMMAND_I_NEED_MORE_TIME && command != COMMAND_READ_DA_DATA_FROM_COORDINATOR ){

        return;
    }

    if(command != COMMAND_READ_DATABASE_GET_VAL && command != COMMAND_READ_DATABASE && command != COMMAND_READ_DATABASE_GET_TABLES && command != COMMAND_READ_METER_LIST_FRAMED
            && command != COMMAND_WRITE_METER_LIST_FRAMED && command != COMMAND_WRITE_DROP_TABLE  && command != COMMAND_WRITE_DROP_TABLE_GET_COUNT
            && command != COMMAND_READ_METER_LOGS_GET_TABLES && command != COMMAND_READ_METER_LOGS_GET_VAL && command != COMMAND_I_NEED_MORE_TIME
            && command != COMMAND_READ_METER_LOGS && command != COMMAND_READ_TABLE_HASH_SUMM && (accessLevel != 0 && command != COMMAND_ZULU ) && command != COMMAND_READ_DA_DATA_FROM_COORDINATOR){
        if(!(daOpened && command == COMMAND_WRITE_DA_OPEN_CLOSE))
            emit hideAnimation();
    }

    switch(command){
    case COMMAND_ZULU:{
        if(jobj.value("name").toString() == "Matilda" && jobj.value("version").toInt() == MATILDA_PROTOCOL_VERSION && QDateTime::fromString(jobj.value("UTC").toString(), "yyyy-MM-dd hh:mm:ss").isValid()){
            if(!jobj.value("err").toString().isEmpty()){

                emit infoAboutObj(tr("DateTime: %1 (UTC %2) <br> Version: %3 <br> message: %4")
                                  .arg( QDateTime( QDate::fromString( jobj.value("UTC").toString().left(10), "yyyy-MM-dd"), QTime::fromString(jobj.value("UTC").toString().right(8), "hh:mm:ss"  ), Qt::UTC ).addSecs(jobj.value("UOFT").toInt()).toString("yyyy-MM-dd hh:mm:ss"))
                                  .arg( QString("%1 %2").arg(jobj.value("UTC").toString()).arg((jobj.value("UOFT").toInt() < 0) ? QString::number(jobj.value("UOFT").toInt()) : QString("+%1").arg(jobj.value("UOFT").toInt()) ))

                                  .arg( jobj.value("version").toInt())
                                  .arg(jobj.value("message").toString())
                                  );

                qDebug() << jobj.value("err").toString();

                emit showMess(jobj.value("message").toString());
                onDisconn();
                return;
            }else{


                emit infoAboutObj(tr("DateTime: %1 (UTC %2) <br>Version: %3 <br> Black List: %4 <br> Counter: %5<br>Memo: %6")
                                  .arg( QDateTime( QDate::fromString( jobj.value("UTC").toString().left(10), "yyyy-MM-dd"), QTime::fromString(jobj.value("UTC").toString().right(8), "hh:mm:ss"  ), Qt::UTC ).addSecs(jobj.value("UOFT").toInt()).toString("yyyy-MM-dd hh:mm:ss"))

                                  .arg( QString("%1 %2").arg(jobj.value("UTC").toString()).arg((jobj.value("UOFT").toInt() < 0) ? QString::number(jobj.value("UOFT").toInt()) : QString("+%1").arg(jobj.value("UOFT").toInt()) ))

                                  .arg( jobj.value("version").toInt() )
                                  .arg(jobj.value("BLC").toInt() )
                                  .arg( jobj.value("CNTR").toInt() )
                                  .arg(jobj.value("memo").toString())
                                  );

                if(jobj.value("version").toInt() == MATILDA_PROTOCOL_VERSION){
                    QJsonObject jObj;

                    jObj.insert("version", jobj.value("version").toInt());
                    if(emptyHsh)
                        jObj.insert("hsh", "");//!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    else
                        jObj.insert("hsh", QString(QCryptographicHash::hash(loginPasswd.at(0) + "\n" + dataArr + "\n" + loginPasswd.at(1), QCryptographicHash::Sha3_256).toBase64(QByteArray::OmitTrailingEquals)));

                    jObj.insert("plg", true); //Передати інфо по плагінам (не обов’язкове значення, але потрібне для додавання лічильників)
                    if(allowCompress){
                        jObj.insert("cmprssn", QJsonArray::fromStringList(QString("zlib").split(" ")));
                    }

                    stopAfter = false;
                    mWrite2SocketJSON(jObj, COMMAND_AUTHORIZE);
                    return;
                }

            }
        }

        qDebug() << jobj.value("version").toInt() << jobj.value("version").toString() << QString::number(MATILDA_PROTOCOL_VERSION);

        emit infoAboutObj(tr("DateTime: %1 (UTC %2) <br>Version: %3 <br> Black List: %4 <br> Counter: %5")
                          .arg( QDateTime( QDate::fromString( jobj.value("UTC").toString().left(10), "yyyy-MM-dd"), QTime::fromString(jobj.value("UTC").toString().right(8), "hh:mm:ss"  ), Qt::UTC ).addSecs(jobj.value("UOFT").toInt()).toString("yyyy-MM-dd hh:mm:ss"))
                          .arg( QString("%1 %2").arg(jobj.value("UTC").toString()).arg((jobj.value("UOFT").toInt() < 0) ? QString::number(jobj.value("UOFT").toInt()) : QString("+%1").arg(jobj.value("UOFT").toInt()) ))
                          .arg( jobj.value("version").toInt() )
                          .arg(jobj.value("BLC").toInt() )
                          .arg( jobj.value("CNTR").toInt() )
                          );

        emit showMess(tr("Unknown device."));
        onDisconn();
        break;}

    case COMMAND_CONNECT_ME_2_THIS_ID_OR_MAC:{

        if(jobj.contains("sIp") && jobj.contains("sP")){
            block4activeClient = true;

            emit startWait4AnswerTimer(timeOutG);

            qDebug() << "close old connection " ;

            close();

            isSvahaService = false;
            connectToHost(jobj.value("sIp").toString(), jobj.value("sP").toInt());
            qDebug() << "openning new connection " << jobj.value("sIp").toString() << jobj.value("sP").toInt() << timeOut;


            if(waitForConnected(timeOut)){
                qDebug() << "bool isConnected " << block4activeClient;
                block4activeClient = false;
                stopAfter = false;
                return;

            }else{
                emit showMess(tr("Can't connect to device. Error: %1").arg(errorString()));
                 close();
                return;
            }
        }else{
            QVariantList l = jobj.value("l").toArray().toVariantList();
            QStringList list;
            for(int i = 0, iMax = l.size(); i < iMax; i++)
                list.append(l.at(i).toString());
            emit onYouCanSelectDevice(list);
        }
        break;}

    case COMMAND_AUTHORIZE:{
        qDebug() << "access = " << jobj;

        accessLevel = jobj.value("a").toInt();
        switch(jobj.value("a").toInt()){
        case MTD_USER_ADMIN:{
            emit authrizeAccess(jobj.value("a").toInt());
            emit data2gui(command, jobj);
            emit showMess(tr("Hello, you admin!"));
            break;}
        case MTD_USER_OPER:{
            emit authrizeAccess(jobj.value("a").toInt());
            emit data2gui(command, jobj);
            emit showMess(tr("Hello, you only operator!"));
            break;}

        case MTD_USER_GUEST:{
            emit authrizeAccess(jobj.value("a").toInt());
            emit data2gui(command, jobj);
            emit showMess(tr("Hello, you only guest!"));
            break;}

        default: {
            emit authrizeAccess(0);
            emit showMess(tr("Session closed."));
            onDisconn();
            break;}
        }
        return;}

    case COMMAND_WRITE_DA_OPEN_CLOSE:{
        //прямий доступ закрито
        daOpened = false;
        emit data2gui(command, jobj);
        return;}

    case COMMAND_READ_DA_DATA_FROM_COORDINATOR:{
        emit dataFromCoordinator( QByteArray::fromBase64(jobj.value("d").toString().toLocal8Bit()));
        return;}

    case COMMAND_I_NEED_MORE_TIME:{
        emit startWait4AnswerTimer(timeOutG);
        emit data2gui(command, jobj);
        stopAfter = false;
        return;}

    case COMMAND_I_AM_A_ZOMBIE:{ qDebug() << "zombie killer" << peerAddress();   mWrite2SocketJSON(QJsonObject(), COMMAND_I_AM_A_ZOMBIE); return;}

    default:{ jobj.remove(getHshNames().at(lastHashSumm)); emit data2gui(command, jobj); return;}

    }


}
//################################################################################################
void matildaclient::mReadyRead()
{

    emit startWait4AnswerTimer(timeOutG);
    qDebug() << "read " << QTime::currentTime().toString("hh:mm:ss.zzz") ;
    QByteArray readarr = readAll();

    QTime time;
    time.start();

    int razivDuzkaL = 0;
    int razivDuzkaR = 0;
    bool wait4lapky = false;
    getRightLeftDuzka(razivDuzkaR, razivDuzkaL, wait4lapky, readarr);

    while(  readarr.size() < MAX_PACKET_LEN && time.elapsed() < timeOutG){

        if(razivDuzkaL > 0 && razivDuzkaR > 0){
            if(razivDuzkaR == razivDuzkaL)
                break;
        }

        if(waitForReadyRead(timeOut)){
            
            QByteArray arr = readAll();
            getRightLeftDuzka(razivDuzkaR, razivDuzkaL, wait4lapky, arr);
            readarr.append(arr);

            waitForReadyRead(100);
            emit startWait4AnswerTimer(timeOutG);
            waitForReadyRead(100);

            arr = readAll();
            getRightLeftDuzka(razivDuzkaR, razivDuzkaL, wait4lapky, arr);
            readarr.append(arr);

        }
    }

    qDebug() << "read " << QTime::currentTime().toString("hh:mm:ss.zzz") << time.elapsed();

    emit startWait4AnswerTimer(timeOutG);


    emit stopWait4AnswerTimer();
    stopAfter = true;

    add2pteLogP(readarr, true);

    if(razivDuzkaR < 1){
        emit changeCounters(readarr.length(), -1 , true);
        emit showMess("corrupted data.");
        qDebug()<< "readServer:"<< readarr;
        return ;
    }else{

        int rIndx = 0, lIndx = 0;
        rIndx = indxOfRightDuzka(rIndx, readarr);
        rIndx++;
        for(int i = 0, iMax = 100; i < iMax; i++){

            decodeReadDataJSON(readarr.mid(lIndx, rIndx - lIndx ));
            lIndx = rIndx;
            rIndx = indxOfRightDuzka(rIndx, readarr);

            if(rIndx < 1){

                qDebug() << "rindx = " << rIndx  << readarr.length() << lIndx;
                break;
            }
            rIndx++;
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


    QByteArray writeArr ;
    if(true){
        QJsonDocument jDoc2DST(jObj);
        writeArr = jDoc2DST.toJson(QJsonDocument::Compact);
        writeArr.chop(1);// remove }
    }

    qDebug() << "writeArr0 " << writeArr;
    switch(lastHashSumm){
    case 0: { writeArr.append(", \"Md4\":\""      + QCryptographicHash::hash( writeArr + ", \"Md4\":\"0\"}"     , QCryptographicHash::Md4     ).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
    case 2: { writeArr.append(", \"Sha1\":\""     + QCryptographicHash::hash( writeArr + ", \"Sha1\":\"0\"}"    , QCryptographicHash::Sha1    ).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
    case 3: { writeArr.append(", \"Sha224\":\""   + QCryptographicHash::hash( writeArr + ", \"Sha224\":\"0\"}"  , QCryptographicHash::Sha224  ).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
    case 4: { writeArr.append(", \"Sha256\":\""   + QCryptographicHash::hash( writeArr + ", \"Sha256\":\"0\"}"  , QCryptographicHash::Sha256  ).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
    case 5: { writeArr.append(", \"Sha384\":\""   + QCryptographicHash::hash( writeArr + ", \"Sha384\":\"0\"}"  , QCryptographicHash::Sha384  ).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
    case 6: { writeArr.append(", \"Sha512\":\""   + QCryptographicHash::hash( writeArr + ", \"Sha512\":\"0\"}"  , QCryptographicHash::Sha512  ).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
    case 7: { writeArr.append(", \"Sha3_224\":\"" + QCryptographicHash::hash( writeArr + ", \"Sha3_224\":\"0\"}", QCryptographicHash::Sha3_224).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
    case 8: { writeArr.append(", \"Sha3_256\":\"" + QCryptographicHash::hash( writeArr + ", \"Sha3_256\":\"0\"}", QCryptographicHash::Sha3_256).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
    case 9: { writeArr.append(", \"Sha3_384\":\"" + QCryptographicHash::hash( writeArr + ", \"Sha3_384\":\"0\"}", QCryptographicHash::Sha3_384).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
    case 10:{ writeArr.append(", \"Sha3_512\":\"" + QCryptographicHash::hash( writeArr + ", \"Sha3_512\":\"0\"}", QCryptographicHash::Sha3_512).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
    default:{ writeArr.append(", \"Md5\":\""      + QCryptographicHash::hash( writeArr + ", \"Md5\":\"0\"}"     , QCryptographicHash::Md5     ).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
    }

    add2pteLogP(writeArr, false);

    qint64 blSize = writeArr.length();

    if(blSize >= SETT_MAX_UNCOMPRSS_PACkET_SIZE && allowCompress){

        if(true){

            QJsonObject jObjCpmrr;

            jObjCpmrr.insert("cmd", QString::number(COMMAND_COMPRESSED_PACKET));
            jObjCpmrr.insert("zlib", QString(qCompress(writeArr, 9).toBase64(QByteArray::OmitTrailingEquals)));

            QJsonDocument jDoc2DST(jObjCpmrr);
            writeArr = jDoc2DST.toJson(QJsonDocument::Compact);
            writeArr.chop(1);// remove }
        }

        qDebug() << "writeArr1 comprs " << writeArr;
        switch(lastHashSumm){
        case 0: { writeArr.append(", \"Md4\":\""      + QCryptographicHash::hash( writeArr + ", \"Md4\":\"0\"}"     , QCryptographicHash::Md4     ).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
        case 2: { writeArr.append(", \"Sha1\":\""     + QCryptographicHash::hash( writeArr + ", \"Sha1\":\"0\"}"    , QCryptographicHash::Sha1    ).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
        case 3: { writeArr.append(", \"Sha224\":\""   + QCryptographicHash::hash( writeArr + ", \"Sha224\":\"0\"}"  , QCryptographicHash::Sha224  ).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
        case 4: { writeArr.append(", \"Sha256\":\""   + QCryptographicHash::hash( writeArr + ", \"Sha256\":\"0\"}"  , QCryptographicHash::Sha256  ).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
        case 5: { writeArr.append(", \"Sha384\":\""   + QCryptographicHash::hash( writeArr + ", \"Sha384\":\"0\"}"  , QCryptographicHash::Sha384  ).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
        case 6: { writeArr.append(", \"Sha512\":\""   + QCryptographicHash::hash( writeArr + ", \"Sha512\":\"0\"}"  , QCryptographicHash::Sha512  ).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
        case 7: { writeArr.append(", \"Sha3_224\":\"" + QCryptographicHash::hash( writeArr + ", \"Sha3_224\":\"0\"}", QCryptographicHash::Sha3_224).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
        case 8: { writeArr.append(", \"Sha3_256\":\"" + QCryptographicHash::hash( writeArr + ", \"Sha3_256\":\"0\"}", QCryptographicHash::Sha3_256).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
        case 9: { writeArr.append(", \"Sha3_384\":\"" + QCryptographicHash::hash( writeArr + ", \"Sha3_384\":\"0\"}", QCryptographicHash::Sha3_384).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
        case 10:{ writeArr.append(", \"Sha3_512\":\"" + QCryptographicHash::hash( writeArr + ", \"Sha3_512\":\"0\"}", QCryptographicHash::Sha3_512).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
        default:{ writeArr.append(", \"Md5\":\""      + QCryptographicHash::hash( writeArr + ", \"Md5\":\"0\"}"     , QCryptographicHash::Md5     ).toBase64(QByteArray::OmitTrailingEquals) + "\"}" ); break;}
        }

        add2pteLogP( "\nCompress: " + writeArr, false);

    }else{
        blSize = -1;
    }

    qint64 len = write(writeArr);
    qDebug() << "write " << QTime::currentTime().toString("hh:mm:ss.zzz");



    emit changeCounters(len, blSize , false);

    waitForBytesWritten(50);
    if(s_command != COMMAND_I_AM_A_ZOMBIE )
        emit startWait4AnswerTimer(timeOutG);
}

//################################################################################################
void matildaclient::onDisconn()
{

    qDebug() << "onDisconn" << block4activeClient;

    if(block4activeClient){
        block4activeClient = false;
        return;
    }

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
void matildaclient::onDaOpened(bool isDaOpen)
{
    daOpened = isDaOpen;

}

void matildaclient::add2pteLogP(QString s, bool isRead)
{
    emit add2pteLog(QString("%1 %2 %3\n")
                    .arg(QTime::currentTime().toString("hh:mm:ss.zzz"))
                    .arg( isRead ? ">" : "<" )
                    .arg(s));
}
//################################################################################################
void matildaclient::getRightLeftDuzka(int &rightDuzka, int &leftDuzka, bool &wait4lapky, const QByteArray &arr)
{
    for(int i = 0, iMax = arr.length(); i < iMax; i++){
        if(wait4lapky){
            if(arr.mid(i,1) == "\"")
                wait4lapky = false;
        }else{
            if(arr.mid(i,1) == "\""){
                wait4lapky = true;
                continue;
            }

            if(arr.mid(i,1) == "}")
                rightDuzka++;
            else if(arr.mid(i,1) == "{")
                leftDuzka++;
        }
    }
}
//################################################################################################
int matildaclient::indxOfRightDuzka(const int &lastRightDuzka, const QByteArray &arr)
{
    bool wait4lapky = false;

    for(int i = lastRightDuzka, iMax = arr.length(); i < iMax; i++){
        if(wait4lapky){
            if(arr.mid(i,1) == "\"")
                wait4lapky = false;
        }else{
            if(arr.mid(i,1) == "\""){
                wait4lapky = true;
                continue;
            }

            if(arr.mid(i,1) == "}")
                return i;
        }
    }
    return -1;
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
QStringList matildaclient::getHshNames()
{
    return QString("Md4,Md5,Sha1,Sha224,Sha256,Sha384,Sha512,Sha3_224,Sha3_256,Sha3_384,Sha3_512").split(",");
}
//################################################################################################
bool matildaclient::messHshIsValid(const QJsonObject &jObj, QByteArray readArr)
{

    QString hshName;
    if(jObj.contains("Md5")){ //default hash sum
        hshName = "Md5";
    }else{
        if(lastHashSumm >=0 && lastHashSumm < getHshNames().size())
            hshName = getHshNames().at(lastHashSumm);
    }

    if(!hshName.isEmpty() && jObj.contains(hshName)){

        int startIndx = readArr.indexOf(QString("\"%1\":").arg(hshName));
        QByteArray hshBase64;
        if(startIndx > 0){
            startIndx += hshName.length() + 4;
            qDebug() << "hshName " << hshName << startIndx << readArr.mid(startIndx);

            int endIndx = readArr.indexOf("\"", startIndx + 1);
            qDebug() << "endIndx " << endIndx << readArr.mid(endIndx);

            hshBase64 = readArr.mid(startIndx , endIndx - startIndx);
            qDebug() << hshBase64;
            readArr = readArr.left(startIndx ) + "0" + readArr.mid(endIndx);
            qDebug() << readArr;

        }
        if(hshBase64.isEmpty())
            return false;

        hshBase64 = QByteArray::fromBase64(hshBase64);

        QByteArray myHash = QCryptographicHash::hash(readArr, static_cast<QCryptographicHash::Algorithm>(lastHashSumm));
        if(myHash == hshBase64){
            return true;
        }else{
            qDebug() << "if(myHash != hshBase64 " << myHash.toBase64(QByteArray::OmitTrailingEquals) << hshBase64.toBase64(QByteArray::OmitTrailingEquals);
            return false;
        }
    }

    return false;

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
