#include "scanipdialog.h"
#include "ui_scanipdialog.h"

#include "udpscanner.h"

//==================================================================================================================================
ScanIpDialog::ScanIpDialog(int modeIp, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScanIpDialog)
{
    ui->setupUi(this);

    ui->radioButton->setChecked(modeIp == 0);
    ui->radioButton_2->setChecked(modeIp == 1);

    modelDevices = new QStandardItemModel(0,3,this);
    proxy_modelDevices = new MySortFilterProxyModel(this);

    proxy_modelDevices->setSourceModel(modelDevices);
    proxy_modelDevices->setDynamicSortFilter(true);
    ui->lvDevHistory->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), proxy_modelDevices, SLOT(setNewFileterStr(QString)) );
    QList<int> filterMode;
    filterMode.append(0);
    filterMode.append(1);

    proxy_modelDevices->setFilterMode(filterMode);
    ui->lvDevHistory->setModel(proxy_modelDevices);


    UdpScanner *ktsScaner = new UdpScanner;
    connect(this, SIGNAL(startUdpScanNow(quint16)), ktsScaner, SLOT(startUdpScan(quint16)) );
    connect(this, SIGNAL(stopUdpScanNow()), ktsScaner, SLOT(stopAllNow()) );
    connect(ktsScaner, SIGNAL(findedDevice(QString,QString,QString)), this, SLOT(findedDevice(QString,QString,QString)) );

    connect(ktsScaner, SIGNAL(showMess(QString)), this, SIGNAL(showMess(QString)) );
    connect(this, SIGNAL(onClosd()), ktsScaner, SLOT(deleteLater()) );

    connect(ktsScaner, SIGNAL(scanStarted()), this, SLOT(onScanStarted()) );
    connect(ktsScaner, SIGNAL(scanFinished()), this, SLOT(onScanStopped()) );


    scanSessionID = 1;

    connect(ui->toolButton, SIGNAL(clicked(bool)), SLOT(startScanNow()) );

    QTimer *progss = new QTimer(this);
    progss->setInterval(333);
    progss->setSingleShot(false);
    connect(progss, SIGNAL(timeout()) , this, SLOT(addProg()) );

    connect(ktsScaner, SIGNAL(scanStarted()), progss, SLOT(start()) );
    connect(ktsScaner, SIGNAL(scanFinished()), progss, SLOT(stop()) );


    QThread *thread3 = new QThread;
    connect(this, SIGNAL(onClosd()), thread3, SLOT(quit()) );
    ktsScaner->moveToThread(thread3);
    thread3->start();

    QTimer::singleShot(50, this, SLOT(startScanNow()) );
}
//==================================================================================================================================
ScanIpDialog::~ScanIpDialog()
{
    emit stopUdpScanNow();
    emit onClosd();
    delete ui;
}
//==================================================================================================================================
void ScanIpDialog::findedDevice(QString name, QString ipPOrt, QString mac)
{
    QList<QStandardItem*> lItem;
    lItem.append(new QStandardItem(name));
    lItem.append(new QStandardItem(ipPOrt));
    lItem.append(new QStandardItem(mac));

    modelDevices->appendRow(lItem);
}

//==================================================================================================================================
void ScanIpDialog::startScanNow()
{
    ui->progressBar->setValue(0);
     scanSessionID++;
     modelDevices->clear();
     modelDevices->setHorizontalHeaderLabels(tr("Object ID;IP:Port;MAC").split(";"));
     emit startUdpScanNow(scanSessionID);

}
//==================================================================================================================================
void ScanIpDialog::onScanStarted()
{
    ui->progressBar->setValue(0);
}
//==================================================================================================================================
void ScanIpDialog::onScanStopped()
{
    ui->progressBar->setValue(100);
}
//==================================================================================================================================
void ScanIpDialog::addProg()
{
    if(ui->progressBar->value() < 98)
        ui->progressBar->setValue(ui->progressBar->value() + 1);
}
//==================================================================================================================================
void ScanIpDialog::on_buttonBox_accepted()
{
    emit stopUdpScanNow();
    if(!ui->lvDevHistory->currentIndex().isValid())
        return;

    getThisRow(ui->lvDevHistory->currentIndex().row());
}
//==================================================================================================================================
void ScanIpDialog::getThisRow(int row)
{

    if(ui->radioButton->isChecked()){
        emit setThisObjName(modelDevices->item(row,0)->text());

        QString oneLine = modelDevices->item(row,1)->text(); // ip:port if port == 9090  : login @ ip . IPv6 [ip]:port or [ip]

        if(oneLine.mid( oneLine.indexOf("[") ).contains("]")){ //IPv6
            if(oneLine.contains("]:")){
                emit setThisIp( oneLine.mid( oneLine.lastIndexOf("[") + 1, oneLine.lastIndexOf("]:") - oneLine.lastIndexOf("[") - 1 ) );
                emit setThisPort( oneLine.mid( oneLine.lastIndexOf("]:") + 2).toInt());
            }else{
                QString str = oneLine.mid( oneLine.lastIndexOf("[") + 1 );
                str.chop(1);
                emit setThisIp( str);
                emit setThisPort(9090);
            }
        }else{

            if(oneLine.contains(":")){
                emit setThisIp( oneLine.mid( 0, oneLine.lastIndexOf(":")  ) );
                emit setThisPort( oneLine.mid( oneLine.lastIndexOf(":") + 1).toInt());
            }else{
                emit setThisIp( oneLine);
                emit setThisPort(9090);
            }
        }
        emit setThisMode(0);
    }else{
        emit setThisObjName_2(modelDevices->item(row,0)->text());

        emit setThisMac(modelDevices->item(row,2)->text());
        emit setThisMode(1);

    }

}
//==================================================================================================================================
void ScanIpDialog::on_lvDevHistory_doubleClicked(const QModelIndex &index)
{
    emit stopUdpScanNow();

    getThisRow(proxy_modelDevices->mapToSource(index).row());
    accept();
}
//==================================================================================================================================
 
