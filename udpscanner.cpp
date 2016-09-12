#include "udpscanner.h"
#define udp_version     1
//==================================================================================================================================
UdpScanner::UdpScanner(QObject *parent) : QObject(parent)
{
    groupAddress = QHostAddress("239.255.189.21");
    groupAddress2Android = QHostAddress("239.255.189.22");

    QString name;
    if(name.isEmpty()){
        QStringList fortunes = tr("Dnieper,Southern Buh,Dniester,Siversky Donets,Desna,Horyn,Inhulets,Psel,Sluch,Styr,Western Bug,Teteriv,Sula,Inhul,Vorskla,Ros,Oril,Udai,Vovcha,"
                              "Samara,Khorol,Prut,Prypiat,Uzh,Zbruch,Seret,Stryi,Seim,Aidar,Kalmius,Salhir,Tysa,Vysun,Oster,Luhan,Molochna,Snov,Stokhid,Turia,Oskil,Danube,Ubort,"
                              "Krynka,Smotrych,Hirsky Tikych,Velyka Vys,Murafa,Irpin,Tiasmyn,Bazavluk,Hnylyi Tikych,Ikva,Chychyklia,Tylihul,Derkul,Stvyha,Velykyi Kuyalnyk,UUU").split(",");
//        name = fortunes.at(qrand() % ((fortunes.size() + 1) - 1) )
        qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
        name = fortunes.at( qrand() % (fortunes.size() - 1) );
    }

    this->devName = name ;

    QString osMarker = "OS";

    osMarker = "Slave";


    udpServiceId = QString("%1%2%3%4%5%6%7%8%9")
            .arg(QDate::currentDate().year())
            .arg(QDate::currentDate().month())
            .arg(QDate::currentDate().day())
            .arg(QTime::currentTime().hour())
            .arg(QTime::currentTime().minute())
            .arg(QTime::currentTime().second())
            .arg(QTime::currentTime().msec())
            .arg(name)
            .arg(qrand() % ((999999 + 1) - 1) + 1);

    udpServiceId.prepend(osMarker);

    stopAll = true;

    udpSocketRead = new QUdpSocket(this);

    connect(udpSocketRead, SIGNAL(readyRead()), this, SLOT(mReadyRead()) );

    udpSocketWrite = new QUdpSocket(this);
    udpSocketWrite->setSocketOption(QAbstractSocket::MulticastTtlOption, 9);

    QTimer *writeTmr = new QTimer(this);
    writeTmr->setInterval(100);
    writeTmr->setSingleShot(true);
    connect(writeTmr, SIGNAL(timeout()), this, SLOT(mWriteDatagramku()) );
    connect(this, SIGNAL(startTmrs()), writeTmr, SLOT(start()) );
    connect(this, SIGNAL(stopPrevSession()), writeTmr, SLOT(stop()) );

    QTimer *killTmr = new QTimer(this);
    killTmr->setInterval(5555);
    killTmr->setSingleShot(true);
    connect(killTmr, SIGNAL(timeout()) ,this , SLOT(killUdpRead()) );
    connect(this, SIGNAL(startTmrs()), killTmr, SLOT(start()) );
    connect(this, SIGNAL(stopPrevSession()), killTmr, SLOT(stop()) );

}
//==================================================================================================================================
void UdpScanner::startUdpScan(quint16 scanSessionID)
{
    stopAll = true;
    emit stopPrevSession();
    listDev.clear();

    bool localIntrface = false;
    QStringList listIp;
    foreach (QNetworkInterface inrfc, QNetworkInterface::allInterfaces()) {

        if(inrfc.flags().testFlag(QNetworkInterface::IsUp) && !inrfc.flags().testFlag(QNetworkInterface::IsLoopBack)){
            foreach (QNetworkAddressEntry nEntry, inrfc.addressEntries()) {

                QHostAddress addr = nEntry.ip();

                if(addr.protocol() != QAbstractSocket::IPv4Protocol)
                    continue;

                if(!localIntrface){
                    localIntrface = true;
                }

                QString ip = addr.toString();

                if(ip.left(7).toLower() == "::ffff:" && ip.contains("."))
                    ip = ip.mid(7);


                listIp.append(ip);



            }
        }
    }

    if(!localIntrface){
        emit showMess(tr("Local inteface not found."));
        listDev.append("0");
        stopAllNow();
        return;
    }

    prepareReadSocket();

    this->scanSessionID = scanSessionID;
    qDebug() << "udp port " << udpSocketRead->localPort() << scanSessionID;

    if(!udpSocketRead->isOpen())
        qDebug() << "if(!udpSocketRead->isOpen()) " << udpSocketRead->errorString();

    QJsonObject jObj;
    jObj.insert("id", udpServiceId);
    jObj.insert("type", "ua.ktsintek.matilda");
    jObj.insert("version", udp_version);
    jObj.insert("name", devName);
    jObj.insert("udp_port", udpSocketRead->localPort());
    jObj.insert("ip", listIp.join("|"));

    QJsonDocument jDoc(jObj);
    QByteArray writArr = jDoc.toJson(QJsonDocument::Compact);

    emit scanStarted();
    stopAll = false;

    qDebug() << "startTime \"START\" " << QTime::currentTime().toString("hh:mm:ss.zzz") << writArr;
    this->wrtArr = writArr;
    mWriteDatagramku();
    qDebug() << "endTime " << QTime::currentTime().toString("hh:mm:ss.zzz");

    emit startTmrs();
}

//==================================================================================================================================
void UdpScanner::stopAllNow()
{
    emit stopPrevSession();
    killUdpRead();
}
//==================================================================================================================================
void UdpScanner::mReadyRead()
{
    if(stopAll)
        return;

    while (udpSocketRead->hasPendingDatagrams()) {

        QByteArray datagram;
        datagram.resize(udpSocketRead->pendingDatagramSize());
        udpSocketRead->readDatagram(datagram.data(), datagram.size());

        QString readStr(datagram);
        if(!readStr.isEmpty()){

            QJsonParseError jErr;
            QJsonDocument jDoc = QJsonDocument::fromJson(readStr.toLocal8Bit(), &jErr);

            if(!jDoc.isNull()){
                if(jDoc.isObject()){
                    QJsonObject jObj = jDoc.object();
                    if(jObj.value("type").toString() == "ua.ktsintek.matilda"){
                        if(jObj["id"].toString().left(6) == "Master" && !listDev.contains(jObj["id"].toString())){
                            listDev.append(jObj["id"].toString());

                            QStringList listIp = (jObj.contains("tcp_ip")) ? jObj["tcp_ip"].toString().split("i") : QStringList();
//                            if(!jObj.contains("tcp_ip"))
//                                listIp.clear();
                            QStringList listPorts = jObj["tcp_port"].toString().split("i") ;//Matilda Peredavator DbgSerial

                            if(!jObj["name"].toString().isEmpty() && !listIp.isEmpty() && !listPorts.isEmpty() && jObj.contains("tcp_ip")){

                                for(int i = 0, iMax = listIp.size(); i < iMax; i++){
                                    QHostAddress addr(listIp.at(i));
                                    if(addr.protocol() == QAbstractSocket::IPv4Protocol)
                                        emit findedDevice(jObj.value("name").toString(),  QString("%1:%2").arg(listIp.at(i)).arg(listPorts.at(0)), "");
                                    else
                                        emit findedDevice(jObj.value("name").toString(),  QString("[%1]:%2").arg(listIp.at(i)).arg(listPorts.at(0)), "");
                                }

                                qDebug() << "finded dev " << jObj["name"].toString() << jObj["id"].toString() << listIp ;

                            }else{

                                listIp = jObj["tcp_mac_ip"].toString().split("i");
                                if(!jObj["name"].toString().isEmpty() && !listIp.isEmpty() && !listPorts.isEmpty()){

                                    for(int i = 0, iMax = listIp.size(); i < iMax; i++){
                                        QString mac = listIp.at(i).split("|").first();
                                        QString ip = listIp.at(i).split("|").last();

                                        QHostAddress addr(ip);
                                        if(addr.protocol() == QAbstractSocket::IPv4Protocol)
                                            emit findedDevice(jObj.value("name").toString(),  QString("%1:%2").arg(ip).arg(listPorts.at(0)), mac);
                                        else
                                            emit findedDevice(jObj.value("name").toString(),  QString("[%1]:%2").arg(ip).arg(listPorts.at(0)), mac);
                                    }

                                    qDebug() << "finded dev " << jObj["name"].toString() << jObj["id"].toString() << listIp ;

                                }

                            }

                        }

                    }
                }
            }
        }

        if(stopAll)
            return;

    }

}
//==================================================================================================================================
void UdpScanner::prepareReadSocket()
{
    if(udpSocketRead){
        udpSocketRead->deleteLater();
        udpSocketRead = new QUdpSocket(this);
        connect(udpSocketRead, SIGNAL(readyRead()), this, SLOT(mReadyRead()) );
    }else{
        udpSocketRead = new QUdpSocket(this);
        connect(udpSocketRead, SIGNAL(readyRead()), this, SLOT(mReadyRead()) );
    }


    for(quint16 port = 56835, i = 0; i < 50; i++, port++){
        if(udpSocketRead->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress))
            break;
        qDebug() << "Can't bind " << port << udpSocketRead->errorString();
    }
    if(!udpSocketRead->joinMulticastGroup(groupAddress2Android))
        qDebug() << "joinMulticastGroup error " << udpSocketRead->errorString();
}
//==================================================================================================================================
void UdpScanner::mWriteDatagramku()
{
    if(stopAll)
        return;

    qDebug() << "writArr "  << wrtArr;

    for(quint16 port = 56835, i = 0; i < 50; i++, port++){
         udpSocketWrite->writeDatagram(wrtArr, groupAddress, port);
//        udpSocketWrite->waitForBytesWritten(9);
        if(stopAll)
            return;
    }
}
//==================================================================================================================================
void UdpScanner::killUdpRead()
{
    stopAll = true;
    emit scanFinished();
    qDebug() << "scan finished " << listDev;

    if(listDev.isEmpty())
        emit showMess(tr("Device not found."));
}
//==================================================================================================================================
