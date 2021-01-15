#include "directaccesmatildaservice.h"
#include <QTimer>
#include <QHostAddress>
#include <QNetworkInterface>

//============================================================================================
DirectAccesMatildaService::DirectAccesMatildaService(QObject *parent) : QTcpServer(parent)
{
    clientCount = 0;
}
//============================================================================================
void DirectAccesMatildaService::startServerNow(qint8 mode, quint16 port)
{
    QHostAddress hAddr;
    switch(mode){
    case 1: hAddr = QHostAddress::LocalHost; break;
    case 2: hAddr = QHostAddress::LocalHostIPv6; break;
    default: hAddr = QHostAddress::Any; break;
    }

    if(isListening()){
        emit stopAllNow();
        this->close();
    }

    if(!listen(hAddr, port)){
        emit showMessage(tr("Can't start server. Error: %1").arg(errorString()));
    }
    QString str;
    switch(mode){
    case 1: str = QHostAddress(QHostAddress::LocalHost).toString(); break;
    case 2: str = QHostAddress(QHostAddress::LocalHostIPv6).toString(); break;
    default:{
        QStringList list;
        QList<QHostAddress> netIface = QNetworkInterface::allAddresses();
        for(int i = 0, iMax = netIface.size(); i < iMax; i++){
            if(netIface.at(i).isNull())
                continue;
            list.append( netIface.at(i).toString() );
        }
        str = list.join("\n");
        break;}

    }
    emit onDasStarted(str);

    updateStatus();
    lastState = isListening();
    emit onStateChanged(lastState);

}
//============================================================================================
void DirectAccesMatildaService::onSocketKickOff()
{
    clientCount--;
    if(clientCount < 0)
        clientCount = 0;
    updateStatus();
}
//============================================================================================
void DirectAccesMatildaService::updateStatus()
{
    if(isListening()){
        emit onStateChanged(tr("State: listening, Port: %1, Clients: %2").arg(serverPort()).arg(clientCount));
    }else{
        emit onStateChanged(tr("State: closed"));
    }
}
//============================================================================================
void DirectAccesMatildaService::stopServerNow()
{
    emit stopAllNow();
    close();
    updateStatus();
    if(lastState == isListening())
        return;
    lastState = isListening();
    emit onStateChanged(lastState);
    emit onDasStopped();
}
//============================================================================================
void DirectAccesMatildaService::onMatildaConnectedStateChanged(bool isConnUp)
{
    if(!isConnUp)
        stopServerNow();
}
//============================================================================================
void DirectAccesMatildaService::setDaForwardNI(QByteArray ni)
{
    modemNI = ni;
    emit setDaForwardNISign(ni);
}
//============================================================================================
void DirectAccesMatildaService::incomingConnection(qintptr socketDescr)
{
    DirectAccessSocket *socket = new DirectAccessSocket(this);
    if(!socket->setSocketDescriptor(socketDescr)){
        socket->deleteLater();
        return;
    }
    socket->setDaForwardNI(modemNI);
    clientCount++;
    connect(this, SIGNAL(dataFromCoordinator(QByteArray)), socket, SLOT(mWrite2Local(QByteArray)) );
    connect(socket, SIGNAL(data2coordiantor(QByteArray)), this, SIGNAL(data2coordiantor(QByteArray)) );
    connect(socket, SIGNAL(onSocketKickOff()), this, SLOT(onSocketKickOff()) );
    connect(this, SIGNAL(stopAllNow()), socket, SLOT(onDisconn()) );
    connect(this, SIGNAL(setDaForwardNISign(QByteArray)), socket, SLOT(setDaForwardNI(QByteArray)) );
    updateStatus();

}
//============================================================================================






DirectAccessSocket::DirectAccessSocket(QObject *parent) : QTcpSocket(parent)
{
    QTimer::singleShot(12 * 60 * 60 * 1000, this, SLOT(onDisconn()) );
    connect(this, SIGNAL(disconnected()), SLOT(onDisconn()) );
    connect(this, SIGNAL(readyRead()), this, SLOT(mReadyRead()) );
    isKickOff = false;
}
//============================================================================================
void DirectAccessSocket::mWrite2Local(QByteArray writeStr)
{
    if(state() == QAbstractSocket::ConnectedState){
        if(!modemNI.isEmpty() && (writeStr.left(modemNI.length()) == modemNI || writeStr.left(4) == "OK\r\n" || writeStr.left(7) == "ERROR\r\n")){
            if(writeStr.left(modemNI.length()) == modemNI){
                write( writeStr.mid(modemNI.length()));
            }else{
                write( (writeStr.left(4) == "OK\r\n") ? writeStr.mid(4) : writeStr.mid(7) );
            }
        }else
            write( writeStr );
    }else{
        if(state() != QAbstractSocket::ConnectingState)
            QTimer::singleShot(11, this, SLOT(onDisconn()) );
    }
}
//============================================================================================
void DirectAccessSocket::setDaForwardNI(QByteArray ni)
{
    modemNI = ni;
}
//============================================================================================
void DirectAccessSocket::mReadyRead()
{
    QByteArray readArr = readAll();
    if(waitForReadyRead(5))
        readArr.append(readAll());
    emit data2coordiantor(modemNI + readArr);
}
//============================================================================================
void DirectAccessSocket::onDisconn()
{
    if(!isKickOff){
        isKickOff = true;
        emit onSocketKickOff();
        close();
    }
    deleteLater();
}
//============================================================================================
