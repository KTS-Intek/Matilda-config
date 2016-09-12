#include "peredavatordbgclient.h"

PeredavatorDbgClient::PeredavatorDbgClient(QObject *parent) : QTcpSocket(parent)
{
    connect(this, SIGNAL(readyRead()), SLOT(mReadyRead()) );
    connect(this, SIGNAL(disconnected()), SLOT(onDisconn()) );
}

void PeredavatorDbgClient::conn2thisDev(QString add, quint16 port, int timeOut)
{

    disconnect(this, SIGNAL(disconnected()), this, SLOT(onDisconn()) );

    if(isOpen())
        close();


    stopAll = false;
    connectToHost(add,port);
    if(waitForConnected(timeOut)){
        connect(this, SIGNAL(disconnected()), this, SLOT(onDisconn()) );
        emit onConnectedStateChanged(true);
    }else{
        emit showMess(tr("Can't connect to device. Error: %1").arg(errorString()));
         close();
         emit onCantConnect(true);

        return;
    }
}

void PeredavatorDbgClient::closeConnection()
{
    stopAll = true;
    onDisconn();
//    emit showMess(tr("Done."));
}


void PeredavatorDbgClient::stopAllNow()
{
    stopAll = true;
}

void PeredavatorDbgClient::mReadyRead()
{
    QByteArray readArr = readAll();

    while(!stopAll && waitForReadyRead(100)){
        readArr.append(readAll());
    }
    emit changeCounters(readArr.length(), -1, true);
    if(readArr.right(1) == "\n")
        readArr.chop(1);
    emit appendPlainText(readArr);
    if(stopAll)
        onDisconn();
}

void PeredavatorDbgClient::onDisconn()
{
    qDebug() << "onDisconn";
    emit onConnectedStateChanged(false);
    close();
}

