#include "mainwindow.h"

#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFileDialog>


#include "ui_mainwindow.h"
#include "matildaclient.h"
#include "moji_defy.h"
#include "peredavatordbgclient.h"
#include "energydialog.h"
#include "wait4answerdialog.h"
#include "insertmeterdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    modelDevOptions = new QStandardItemModel(this);
    modelAddMeter = new QStandardItemModel(0,7, this);

    modelProfile4DB = new QStandardItemModel(0,1,this);
    modelProfile4Hash = new QStandardItemModel(0,1,this);

    modelTarif4DB = new QStandardItemModel(0,1,this);
    modelPhVal4DB = new QStandardItemModel(0,1,this);

    modelDbData = new QStandardItemModel(0,7, this);
    modelTimeZone = new QStandardItemModel(this);
    modelDbDataEv = new QStandardItemModel(this);
    modelEvent4DB = new QStandardItemModel(0,1,this);

    modelPollStat = new QStandardItemModel(0,8, this);



    proxy_modelDevOptions = new MySortFilterProxyModel(this);
    proxy_modelAddMeter = new MySortFilterProxyModel(this);
    proxy_modelPhVal4DB = new MySortFilterProxyModel(this);
    proxy_modelDbData = new MySortFilterProxyModel(this);
    proxy_modelTimeZone = new MySortFilterProxyModel(this);
    proxy_modelDbDataEv = new MySortFilterProxyModel(this);
    proxy_modelPollStat = new MySortFilterProxyModel(this);




    proxy_modelDevOptions->setSourceModel(modelDevOptions);
    proxy_modelDevOptions->setDynamicSortFilter(true);
    connect(ui->leFilterDevOperation, SIGNAL(textChanged(QString)), proxy_modelDevOptions, SLOT(setNewFileterStr(QString)) );
//    filterMode.append(0);
    proxy_modelDevOptions->setFilterMode(getFilterList(0,1));
    ui->lvDevOperation->setModel(proxy_modelDevOptions);


    proxy_modelAddMeter->setSourceModel(modelAddMeter);
    proxy_modelAddMeter->setDynamicSortFilter(true);
    ui->tvAddMeterTable->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->leAddMeterFilter, SIGNAL(textChanged(QString)), proxy_modelAddMeter, SLOT(setNewFileterStr(QString)) );
    proxy_modelAddMeter->setFilterMode(getFilterList(1,3));
    ui->tvAddMeterTable->setModel(proxy_modelAddMeter);


    proxy_modelDbData->setSourceModel(modelDbData);
    proxy_modelDbData->setDynamicSortFilter(true);
    ui->tvMeterDataPollData->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->leMeterDataFIlter, SIGNAL(textChanged(QString)), proxy_modelDbData, SLOT(setNewFileterStr(QString)) );
    proxy_modelDbData->setFilterMode(getFilterList(1,3));
    ui->tvMeterDataPollData->setModel(proxy_modelDbData);


    proxy_modelTimeZone->setSourceModel(modelTimeZone);
    proxy_modelTimeZone->setDynamicSortFilter(true);
    ui->tvTz->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->leFltrTz, SIGNAL(textChanged(QString)), proxy_modelTimeZone, SLOT(setNewFileterStr(QString)) );
    proxy_modelTimeZone->setFilterMode(getFilterList(0,4));
    ui->tvTz->setModel(proxy_modelTimeZone);


    proxy_modelDbDataEv->setSourceModel(modelDbDataEv);
    proxy_modelDbDataEv->setDynamicSortFilter(true);
    ui->tvMeterDataPollData->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->leMeterDataFIlter_2, SIGNAL(textChanged(QString)), proxy_modelDbDataEv, SLOT(setNewFileterStr(QString)) );
    proxy_modelDbDataEv->setFilterMode(getFilterList(1,3));
    ui->tvMeterDataPollData_2->setModel(proxy_modelDbDataEv);


    proxy_modelPhVal4DB->setSourceModel(modelPhVal4DB);
    proxy_modelPhVal4DB->setDynamicSortFilter(true);
    ui->lvMeterDataPhVal->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->leAddMeterPhysicalVal, SIGNAL(textChanged(QString)), proxy_modelPhVal4DB, SLOT(setNewFileterStr(QString)) );
    proxy_modelPhVal4DB->setFilterMode(getFilterList(0,1));
    ui->lvMeterDataPhVal->setModel(proxy_modelPhVal4DB);

    proxy_modelPollStat->setSourceModel(modelPollStat);
    proxy_modelPollStat->setDynamicSortFilter(true);
    ui->tvPollStatistic->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->lePollStatistic, SIGNAL(textChanged(QString)), proxy_modelPollStat, SLOT(setNewFileterStr(QString)) );
    proxy_modelPollStat->setFilterMode(getFilterList(0,2));
    ui->tvPollStatistic->setModel(proxy_modelPollStat);


    ui->lvMeterDataProfile->setModel(modelProfile4DB);
    ui->lvDeleteTable->setModel(modelProfile4DB);
    ui->lvMeterDataTariff->setModel(modelTarif4DB);
    ui->lvMeterDataProfile_2->setModel(modelEvent4DB);
    ui->lvMeterDataProfile_3->setModel(modelProfile4Hash);




    ui->stackedWidget->setCurrentIndex(0);


    ui->label_4->hide();

    qDebug() << "\e[0;37m" << "utc start " ;

    allowDate2utc = false;
    dateInUtc = false;

    QTimer::singleShot(2000, this, SLOT(initializeMatilda()) );

}
//##########################################################################################
MainWindow::~MainWindow()
{
    delete ui;
}
//##########################################################################################
void MainWindow::initializeMatilda()
{
    QStringList l = tr("About Object,Date and time,State,Statistic of exchange,Network Interfaces,Meter Plugin (Activated),Application events,Zbyrator events,GPRS Settings,Administration,"
                       "Poll Settings,Poll schedule,Meter list,Database,Meter logs,Hash summ").split(",");

    QStringList m = QString("%1,0,0,0,0,0,0,0,%2,0,%3,%4,%5,0,0,0")
            .arg(COMMAND_WRITE_ABOUT_OBJECT)
            .arg(COMMAND_WRITE_GPRS_SETT)
            .arg(COMMAND_WRITE_POLL_SETT)
            .arg(COMMAND_WRITE_POLL_SCHEDULE)
            .arg(COMMAND_WRITE_METER_LIST_FRAMED)
            .split(",") ;

    QList<int> listInt;
    listInt << COMMAND_READ_ABOUT_OBJECT << COMMAND_READ_DATE_SETT << COMMAND_READ_STATE << COMMAND_READ_POLL_STATISTIC
            << COMMAND_READ_IFCONFIG << COMMAND_READ_ABOUT_PLG << COMMAND_READ_APP_LOG
            << COMMAND_READ_ZBR_LOG << COMMAND_READ_GPRS_SETT << 0 << COMMAND_READ_POLL_SETT
            << COMMAND_READ_POLL_SCHEDULE << COMMAND_READ_METER_LIST_FRAMED << COMMAND_READ_DATABASE << COMMAND_READ_METER_LOGS_GET_TABLES << COMMAND_READ_TABLE_HASH_SUMM;


    if(l.size() != m.size()){
        qDebug() << "size error " << l.size() << m.size();
        return;
    }

    for(int i = 0, iMax = l.size(); i < iMax; i++){
        QStandardItem *item = new QStandardItem(l.at(i));
        item->setData(m.at(i));
        item->setData(listInt.at(i), Qt::UserRole + 2);
        modelDevOptions->appendRow(item);
    }


    QDateTime dateTime = QDateTime::currentDateTime();

    Wait4AnswerDialog *dialog = new Wait4AnswerDialog(ui->pbLogOut->height(), this);
    connect(this, SIGNAL(showWaitMess(int)), dialog, SLOT(showAnimation(int)) );
    connect(this, SIGNAL(hideWaitMess()), dialog, SLOT(hideAnimation()) );
    connect(this, SIGNAL(uploadProgress(int,QString)), dialog, SLOT(uploadProgress(int,QString)) );
    connect(dialog, SIGNAL(noAnswerFromDev()), SLOT(noAnswerFromDev()) );


    matildaclient *client = new matildaclient;
    connect(this, SIGNAL(conn2thisDev(int,QString,QString,QString,quint16,int,bool,bool)), client, SLOT(conn2thisDev(int,QString,QString,QString,quint16,int,bool,bool)) );
    connect(this, SIGNAL(data2matilda(quint16,QVariantHash)), client, SLOT(data2matilda(quint16,QVariantHash)) );
    connect(this, SIGNAL(closeConnection()), client, SLOT(closeConnection()) );

    connect(client, SIGNAL(onErrorWrite()), dialog,SLOT(hideAnimation()), Qt::DirectConnection );
//    connect(client, SIGNAL(onConnectedStateChanged(bool)),dialog, SLOT(hideAnimation()), Qt::DirectConnection );
//    connect(client, SIGNAL(data2gui(quint16,QVariant)), dialog, SLOT(hideAnimation()) , Qt::DirectConnection);
    connect(client, SIGNAL(hideAnimation()), dialog, SLOT(hideAnimation()) , Qt::DirectConnection);

    connect(client, SIGNAL(authrizeAccess(int)),dialog, SLOT(hideAnimation()) , Qt::DirectConnection);
    connect(client, SIGNAL(showMess(QString)),dialog, SLOT(hideAnimation()), Qt::DirectConnection );
    connect(client, SIGNAL(uploadProgress(int,QString)), dialog, SLOT(uploadProgress(int,QString)), Qt::DirectConnection );
    connect(client, SIGNAL(startWait4AnswerTimer(int)), dialog, SLOT(resetCounter()), Qt::DirectConnection );



    connect(dialog, SIGNAL(noAnswerFromDev()), client, SLOT(stopAllNow()), Qt::DirectConnection );
    connect(dialog, SIGNAL(stopNow()), client, SLOT(stopAllNow()) );


    connect(dialog, SIGNAL(rejected()), client, SLOT(stopAllNow()), Qt::DirectConnection );


    connect(client, SIGNAL(onConnectedStateChanged(bool)), SLOT(onConnectedStateChanged(bool)) );
    connect(client, SIGNAL(data2gui(quint16,QVariantHash)), SLOT(data2gui(quint16,QVariantHash)) );
    connect(client, SIGNAL(onErrorWrite()), SLOT(onErrorWrite()) );
    connect(client, SIGNAL(showMess(QString)) , SLOT(showMess(QString)) );
    connect(client, SIGNAL(authrizeAccess(int)), SLOT(authrizeAccess(int)) );

    connect(client, SIGNAL(changeCounters(qint64,qint64,bool)), SLOT(changeCounters(qint64,qint64,bool)) );
    connect(client, SIGNAL(infoAboutObj(QString)), ui->pteAboutConnObj, SLOT(appendHtml(QString)) );

    ui->pbLogIn->setEnabled(true);

    byteSend = 0;
    byteReceiv = 0;
    byteSendNotComprssd = 0;
    byteReceivNotComprssd = 0;

    QThread *thread = new QThread(this);
    client->moveToThread(thread);
    thread->start();


    QLabel *lblPxmp;
    QLabel *lblTxt;

    lblPxmp = new QLabel(this);
    lblPxmp->setMaximumHeight(ui->statusBar->height() * 0.9);
    lblPxmp->setMaximumWidth(lblPxmp->height());
    lblPxmp->setScaledContents(true);
    lblPxmp->setPixmap(QPixmap(":/katynko/deviceisnotused.png"));


    lblTxt = new QLabel(this);
    lblTxt->setMaximumHeight(ui->statusBar->height() * 0.9);

    QLabel *connStat = new QLabel(this);
    connStat->setMaximumHeight(ui->statusBar->height() * 0.9);
    connStat->setToolTip(tr("Last R/W: last operation read/wrie to socket<br>"
                            "Total R/W: total received/sended bytes<br>"
                            "R/W (NC): uncompressed data (shows how much occupied uncompressed data)"));

    connect(this,SIGNAL(setSttsNewPixmap(QPixmap)), lblPxmp, SLOT(setPixmap(QPixmap)) );
    connect(this, SIGNAL(setSttsNewTxt(QString)), lblTxt, SLOT(setText(QString)) );
    connect(this, SIGNAL(setNewConnStat(QString)), connStat, SLOT(setText(QString)) );

    lastConnDevInfo = tr("Not connected.");

    ui->statusBar->addPermanentWidget(lblPxmp);
    ui->statusBar->addPermanentWidget(lblTxt, 11);
    ui->statusBar->addPermanentWidget(connStat, 11);
    emit setSttsNewPixmap(QPixmap(":/katynko/deviceisnotused.png"));
    emit setSttsNewTxt(lastConnDevInfo);

    ui->cbHashSumm->addItems(QString("Md4,Md5,Sha1,Sha224,Sha256,Sha384,Sha512,Sha3_224,Sha3_256,Sha3_384,Sha3_512").split(","));
    ui->cbHashSumm->setCurrentIndex(1);

    ui->cbHashSumm_2->addItems(QString("Md4,Md5,Sha1,Sha224,Sha256,Sha384,Sha512,Sha3_224,Sha3_256,Sha3_384,Sha3_512").split(","));
    ui->cbHashSumm_2->setCurrentIndex(1);


//    PeredavatorDbgClient *dbgClient = new PeredavatorDbgClient;
//    connect(this, SIGNAL(conn2thisDevDbg(QString,quint16,int)), dbgClient, SLOT(conn2thisDev(QString,quint16,int)) );

//    connect(this, SIGNAL(closeConnectionDbg()), dbgClient, SLOT(closeConnection()) );
//    connect(this, SIGNAL(conn2thisDev(QString,QString,QString,QString,quint16,int,bool)), dbgClient, SLOT(stopAllNow()), Qt::DirectConnection );
//    connect(this, SIGNAL(conn2thisDev(QString,QString,QString,QString,quint16,int,bool)), dbgClient, SLOT(closeConnection()) );

//    connect(ui->pbStopStream, SIGNAL(clicked(bool)), dbgClient, SLOT(stopAllNow()), Qt::DirectConnection );

//    connect(dbgClient, SIGNAL(appendPlainText(QString)), ui->pteSerialLog, SLOT(appendPlainText(QString)) );
//    connect(dbgClient, SIGNAL(showMess(QString)), this, SLOT(showMess(QString)) );
//    connect(dbgClient, SIGNAL(onConnectedStateChanged(bool)), this, SLOT(onConnectedStateChangedDbg(bool)) );
//    connect(dbgClient, SIGNAL(onCantConnect(bool)), ui->pbStartStream, SLOT(setEnabled(bool)) );
//    connect(dbgClient, SIGNAL(changeCounters(qint64,qint64,bool)), SLOT(changeCounters(qint64,qint64,bool)) );

//    QThread *thread2 = new QThread(this);
//    dbgClient->moveToThread(thread2);
//    thread2->start();


    QFont font;
#ifdef Q_OS_WIN
    font.fromString("Consolas,9,-1,5,50,0,0,0,0,0"  );
#else
    font.fromString( "Liberation Mono,9,-1,5,50,0,0,0,0,0" );
#endif
//    ui->pteSerialLog->setFont(font);
    ui->pteAboutObjectMemo->setFont(font);

    ui->pteIfconfig->setFont(font);
    ui->pteState->setFont(font);
    ui->pteAppLog->setFont(font);
    ui->pteMeterPlg->setFont(font);
    ui->pteMatildaTime->setFont(font);

    QSettings settings("kts-intek", "matilda");
    settings.beginGroup("meter");
    QVariantMap mpv = settings.value("mpv").toMap();
    settings.endGroup();

    if(mpv.isEmpty()){
        QStringList list = QString("A+;A+,R+;A+,R+,R-;A+,A-,R+,R-;A+,A-").split(";");
        while(!list.isEmpty()){
            QString s = list.takeFirst();

            mpv.insert(s, s);
        }
    }

    QList<QString> listK = mpv.keys();
//    qSort(listK);

    ui->cbAddMeterPhysicalVal->clear();
    for(int i = 0, iMax = listK.size(); i < iMax; i++){
        ui->cbAddMeterPhysicalVal->addItem(listK.at(i), mpv.value(listK.at(i)));
        ui->cbAddMeterPhysicalVal->setItemData(i, mpv.value(listK.at(i)) , Qt::ToolTipRole );

    }

    modelAddMeter->setHorizontalHeaderLabels(tr("Model,Serial Number,NI,Memo,Password,On/Off,Physical values").split(","));

    QStringList list;
    list.append(tr("Now"));
    list.append(tr("Day"));
    list.append(tr("Month"));
    list.append(tr("Power"));
    list.append(tr("Voltage"));

    QList<int> listData;
    listData.append(POLL_CODE_READ_CURRENT);
    listData.append(POLL_CODE_READ_END_DAY);
    listData.append(POLL_CODE_READ_END_MONTH);
    listData.append(POLL_CODE_READ_POWER);
    listData.append(POLL_CODE_READ_VOLTAGE);
    QStringList listKeys;
    listKeys.append(QString("A+,A-,R+,R-"));
    listKeys.append(QString("A+,A-,R+,R-"));
    listKeys.append(QString("A+,A-,R+,R-"));
    listKeys.append(QString("A+,A-,R+,R-"));
    listKeys.append(QString("UA,UB,UC,IA,IB,IC,PA,PB,PC,QA,QB,QC,cos_fA,cos_fB,cos_fC,F"));

    QStringList listKeys2;
    QString str = tr("Active import,Active export,Reactive import,Reactive export");
    for(int i = 0; i < 4; i++)
        listKeys2.append(str);

    str.clear();
    QStringList list2 = tr("Voltage,Current,Active Power,Reactive Power,cos φ").split(',');
    QStringList list3 = tr("A,B,C").split(',');
    for(int i = 0, iMax = list2.size(); i < iMax; i++){
        for(int j = 0; j < 3; j++)
            str.append(tr("Phs %1 %2,").arg(list3.at(j)).arg(list2.at(i)));
    }
    str.append(tr("Frequency"));
    listKeys2.append(str);


//    QList<QStandardItem*> listItem;
    ui->cbSttstOfExchngCode->addItem(tr("All"));
    for(int i = 0, iMax = list.size(); i < iMax; i++){
        ui->cbSttstOfExchngCode->addItem(QString::number(listData.at(i)));
        QStandardItem *item = new QStandardItem(list.at(i));
        item->setData(listData.at(i), Qt::UserRole + 1);
        item->setData(listKeys.at(i), Qt::UserRole + 2);
        item->setData(listKeys2.at(i), Qt::UserRole + 3);
        modelProfile4DB->appendRow(item);
    }

    for(int i = 0, iMax = list.size(); i < iMax; i++){
        QStandardItem *item = new QStandardItem(list.at(i));
        item->setData(listData.at(i), Qt::UserRole + 1);
        item->setData(listKeys.at(i), Qt::UserRole + 2);
        item->setData(listKeys2.at(i), Qt::UserRole + 3);
        modelProfile4Hash->appendRow(item);
    }

    ui->lvMeterDataProfile_3->setCurrentIndex(modelProfile4Hash->index(0,0));

    ui->cbSttstOfExchngCode->addItem(QString::number(POLL_CODE_METER_STATUS) );

    connect(ui->lvMeterDataProfile, SIGNAL(activated(QModelIndex)), this, SLOT(onLvMeterDataProfile_activated(QModelIndex)) );
    ui->lvMeterDataProfile->setCurrentIndex(modelProfile4DB->index(0,0));
    onLvMeterDataProfile_activated(ui->lvMeterDataProfile->currentIndex());

    list.clear();
    list.append(tr("Summ"));
    list.append(tr("Tariff 1"));
    list.append(tr("T"));
    list.append(tr("Power"));

    for(int i = 0; i < 5; i++){
         QStandardItem *item = new QStandardItem( (i == 0) ? tr("Summ") : tr("Tariff %1").arg(i));
         item->setCheckable(true);
         item->setCheckState( (i < 4) ? Qt::Checked : Qt::Unchecked);
         modelTarif4DB->appendRow(item);
    }

    list.clear();
    list.append(tr("All"));
    list.append(tr("Other"));
    list.append(tr("Meter open"));
    list.append(tr("Klema open"));
    list.append(tr("Bat. Low"));
    list.append(tr("EEPROM Error"));
    list.append(tr("Magnet"));
    list.append(tr("Hardware Error"));
    list.append(tr("Access"));
    list.append(tr("Relay"));
    list.append(tr("Internal error"));
    list.append(tr("Date corrected"));
    list.append(tr("Date need to correct"));

    list.append(tr("Unknown protocol"));

    listData.clear();
    listData.append(0);
    listData.append(ZBR_EVENT_OTHER);
    listData.append(ZBR_EVENT_METER_OPEN);
    listData.append(ZBR_EVENT_KLEMA_OPEN);
    listData.append(ZBR_EVENT_BAT_LOW);
    listData.append(ZBR_EVENT_EEPROM_ERROR);
    listData.append(ZBR_EVENT_MAGNET);
    listData.append(ZBR_EVENT_HARDWARE_ERROR);
    listData.append(ZBR_EVENT_ACCESS);
    listData.append(ZBR_EVENT_RELAY);
    listData.append(ZBR_EVENT_INTERNAL_ERROR);
    listData.append(ZBR_EVENT_DATETIME_CORRECTED);
    listData.append(ZBR_EVENT_DATETIME_NOT_CORRECT);

    listData.append(ZBR_EVENT_UNKNOWN_PROTOCOL);



    for(int i = 0, iMax = list.size(); i < iMax; i++){
        QStandardItem *item = new QStandardItem(list.at(i));
        item->setData(listData.at(i), Qt::UserRole + 1);
        modelEvent4DB->appendRow(item);
    }
    ui->lvMeterDataProfile->setCurrentIndex(modelProfile4DB->index(0,0));

    QTimer *niCheckTmr = new QTimer;
    niCheckTmr->setSingleShot(true);
    niCheckTmr->setInterval(100);

    connect(ui->leMeterDataOnlyThisNI, SIGNAL(textChanged(QString)), niCheckTmr, SLOT(start()) );
    connect(niCheckTmr, SIGNAL(timeout()), this, SLOT(onCheckDbNIfieldIsCorrect()) );

    ui->dteMeterDataFrom->setDateTime(QDateTime::fromString("15/01/2013 16:01","dd/MM/yyyy hh:mm"));
    ui->dteMeterDataTo->setDateTime(QDateTime::fromString("15/01/2013 16:30","dd/MM/yyyy hh:mm"));


    QList<QByteArray> lz = QTimeZone::availableTimeZoneIds();
    dateTime = QDateTime::currentDateTime();

    modelTimeZone->setHorizontalHeaderLabels(tr("Area,Region,UTC offset,Country,Has DST,Comment").split(','));

    while(!lz.isEmpty()){
        QList<QStandardItem*> listItem;
        QString z = lz.first();
        dateTime.setTimeZone( QTimeZone(lz.takeFirst()));

        QStandardItem *item = new QStandardItem(dateTime.timeZoneAbbreviation());
        item->setCheckable(true);
        listItem.append(item);
        listItem.append(new QStandardItem(z));
        listItem.append(new QStandardItem(emailUtcOffset(dateTime.offsetFromUtc())));
        listItem.append(new QStandardItem(QLocale::countryToString(dateTime.timeZone().country())));
        listItem.append(new QStandardItem( dateTime.timeZone().hasDaylightTime() ? tr("Yes") : tr("No") ));
        listItem.append(new QStandardItem(dateTime.timeZone().comment()));

        modelTimeZone->appendRow(listItem);





    }

    ui->tvTz->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tvTz->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);


    ui->tvAddMeterTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tvAddMeterTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    ui->tvMeterDataPollData->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tvMeterDataPollData->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);


    QTimer *validTmr = new QTimer(this);
    validTmr->setSingleShot(true);
    validTmr->setInterval(700);
    connect(ui->cbAddMeterModel, SIGNAL(currentIndexChanged(int)), validTmr, SLOT(start()) );
    connect(validTmr, SIGNAL(timeout()), this, SLOT(checkLineAddMeterNIandPasswd()) );


}

//##########################################################################################
void MainWindow::onConnectedStateChanged(bool isConnected)
{
        ui->pbLogOut->setEnabled(isConnected);
        ui->actionDevice->setEnabled(ui->pbLogOut->isEnabled());
        if(!isConnected){
            emit closeConnectionDbg();
        }

        emit setSttsNewPixmap(isConnected ? QPixmap(":/katynko/deviceisready.png") : QPixmap(":/katynko/deviceisdisconnected.png"));
        emit setSttsNewTxt(lastConnDevInfo);

        qDebug() << "isConnected " << isConnected;
}
//##########################################################################################
void MainWindow::onConnectedStateChangedDbg(bool isConnected)
{
//    ui->pbStartStream->setEnabled(!isConnected);
//    ui->pbStopStream->setEnabled(isConnected);
}
//##########################################################################################
void MainWindow::data2gui(quint16 command, QVariantHash hash)
{
/*
    QList<int> listInt;
    listInt <<

#define COMMAND_READ_ABOUT_PLG                  24
#define COMMAND_READ_DATE_SETT                  25
#define COMMAND_READ_GPRS_SETT                  26
#define COMMAND_READ_STATE                      27
#define COMMAND_READ_IFCONFIG                   28
#define COMMAND_READ_APP_LOG                    29
#define COMMAND_READ_POLL_SCHEDULE              30


#define COMMAND_READ_DATABASE                   32

#define COMMAND_READ_DATABASE_GET_TABLES        33
#define COMMAND_READ_DATABASE_GET_VAL           34
COMMAND_READ_METER_LOGS_GET_TABLES;+


#define COMMAND_READ_METER_LOGS                  35

#define COMMAND_READ_METER_LOGS_GET_TABLES       36
#define COMMAND_READ_METER_LOGS_GET_VAL         37


#define COMMAND_READ_METER_LIST_FRAMED          38

#define COMMAND_READ_DATABASE_TABLES_PARSING    39
#define COMMAND_READ_ZBR_LOG                    40
#define COMMAND_READ_ABOUT_OBJECT               41
#define COMMAND_READ_POLL_SETT                  42
#define COMMAND_READ_POLL_STATISTIC             43

#define COMMAND_READ_TABLE_HASH_SUMM            44

#define COMMAND_READ_DATABASE_GET_VAL_EXT       45
#define COMMAND_READ_METER_LOGS_GET_VAL_EXT     46

#define COMMAND_READ_IP_FILTER_SETT             47

*/

    switch (command) {
    case COMMAND_AUTHORIZE:{



        ui->cbAddMeterModel->clear();
        QVariantList l = hash.value("b").toList();

        QVariantHash h;
        for(int i = 0, iMax = l.size(); i < iMax; i++){
            QStringList list = l.at(i).toString().split("\t");
            if(list.size() != 2)
                continue;

            h.insert(list.first(), list.last());
            qDebug() << list;
        }

        QList<QString> lKeys = h.keys();
        qSort(lKeys);

        lKeys.prepend("Auto");

        for(int i = 0, iMax = lKeys.size(); i < iMax; i++){
            ui->cbAddMeterModel->addItem(lKeys.at(i), h.value(lKeys.at(i), "^(.){32}$").toString());
        }
        ui->cbAddMeterModel->setCurrentIndex(0);
        break;  }

    case COMMAND_READ_ABOUT_PLG:{

        QList<QString> l = hash.keys();
        qSort(l);
        ui->pteMeterPlg->clear();
        ui->pteMeterPlg->appendPlainText(tr("#  Plugin\tCreate Date and Time\t\tInfo"));
        for(int i = 0, iMax = l.size(); i < iMax; i++){
            QStringList sl = varList2strList(hash.value(l.at(i)).toList());
            ui->pteMeterPlg->appendPlainText( QString("%1. %2\t%3")
                                              .arg(i + 1)
                                              .arg(l.at(i))
                                              .arg(sl.join("\t")));
        }
        break; }

    case COMMAND_READ_DATE_SETT:{

        QDateTime dateTime = QDateTime::currentDateTime();


        ui->dtLocal->setDateTime(dateTime);
        ui->dtLocalUTC->setDateTime(dateTime.toUTC());

        QTimeZone tz = QDateTime::currentDateTime().timeZone();

        ui->lblLocalTZ->setText(tr("%1, UTC offset %2, DST is %3")
                    .arg(tz.offsetData(ui->dtLocal->dateTime()).abbreviation)
                                .arg(emailUtcOffset(tz.offsetFromUtc(ui->dtLocal->dateTime())))
                                .arg( tz.isDaylightTime(ui->dtLocal->dateTime()) ? tr("On") : tr("Off"))
                                );

        QDateTime devTime = dateTimeFromStr(hash.value("dt").toString());

        if(devTime.isValid()){
            ui->lblLocalTZ->setText(tr("%1, Time difference: %2 s")
                                    .arg(ui->lblLocalTZ->text())
                                    .arg(dateTime.secsTo(devTime)));
        }
        dateTime.setTimeZone( QTimeZone(hash.value("tz").toByteArray()));

        ui->pteMatildaTime->appendPlainText(hash.value("timedatectl", tr("No data")).toString());
        if(hash.contains("tz") && hash.contains("uo") )
            ui->lblDeviceTz->setText(tr("Device time zone: %1 (%2, %3)")
                                 .arg(hash.value("tz").toString())
                                 .arg(dateTime.timeZoneAbbreviation())
                                 .arg(emailUtcOffset(hash.value("uo").toInt()))
                                 );
        else
            ui->lblDeviceTz->setText(tr("Device time zone: n/a"));

        QString tzStr = hash.value("tz").toString();

        for(int i = 0, iMax = modelTimeZone->rowCount(); i < iMax; i++){

            modelTimeZone->item(i,0)->setCheckState(Qt::Unchecked);
            if(!tzStr.isEmpty() && modelTimeZone->item(i,1)->text() == tzStr){
                modelTimeZone->item(i,0)->setCheckState(Qt::Checked);
                ui->leFltrTz->setText(tzStr);
                tzStr.clear();
            }
        }
        ui->pbChTz->setEnabled(true);

        ui->pbEnDisNtpSync->setEnabled(hash.contains("ntp-dead"));
        ui->pbDisableNtp->setEnabled(hash.contains("ntp-dead"));

        ui->pbWriteLocalTime->setEnabled(hash.value("ntp-dead").toInt() != 0);

        ui->pteNtpList->appendPlainText(hash.value("ntp-line").toString().split(" ", QString::SkipEmptyParts).join("\n"));
        ui->pteNtpList->setReadOnly(!hash.value("ntp-dead").toInt() != 0);

        break;}

    case COMMAND_READ_GPRS_SETT:{
        ui->leGPRS_Apn->setText(hash.value("apn").toString());
        ui->leGPRS_Apn_2->setText(hash.value("userName").toString());
        ui->leGPRS_Apn_3->setText(hash.value("password").toString());
        break;}

    case COMMAND_READ_STATE:{

        QList<QString> l = hash.keys();
        qSort(l);
        ui->pteState->clear();
        QStringList listVal;
        int maxKeyLen = 5;
//        ui->pteMeterPlg->appendPlainText(tr("#  Plugin\tCreate Date and Time\tInfo"));
        for(int i = 0, iMax = l.size(); i < iMax; i++){
            listVal.append(hash.value(l.at(i)).toString() );
            if(l.at(i).length() > maxKeyLen)
                maxKeyLen = l.at(i).length();
        }
        QString spaceStr("");
        spaceStr = QString("").leftJustified(maxKeyLen, ' ');
        for(int i = 0, iMax = l.size(); i < iMax; i++){
            QStringList list = listVal.at(i).split("\n");

            ui->pteState->appendPlainText(l.at(i).leftJustified(maxKeyLen, ' ') + "\t" + list.first() );
            for(int j = 1, jMax = list.size(); j < jMax; j++)
                ui->pteState->appendPlainText( spaceStr + "\t" + list.at(j) );
        }

        break;}

    case COMMAND_READ_IFCONFIG:{
        ui->pteIfconfig->clear();
        ui->pteIfconfig->appendPlainText(hash.value("s").toString());
        break;}

    case COMMAND_READ_APP_LOG:{
        ui->pteAppLog->clear();
        ui->pteAppLog->appendPlainText(hash.value("s").toString());
        break;}

    case COMMAND_READ_POLL_SCHEDULE:{

        QVariantList l = hash.value(QString::number(POLL_CODE_READ_CURRENT)).toList();
        while(l.size() < 5)
            l.append(0);

        ui->cbProfileEnableNow->setChecked(l.at(0).toBool());
        ui->sbProfileNow->setValue(l.at(1).toInt());
        ui->cbProfileNow->setCurrentIndex( comboIndxFromKftntAndIntrvl(l.at(4).toInt(), l.at(3).toInt()));


        l = hash.value(QString::number(POLL_CODE_READ_END_DAY)).toList();
        while(l.size() < 5)
            l.append(0);

        ui->cbProfileEnableDay->setChecked(l.at(0).toBool());
        ui->sbProfileDay->setValue(l.at(1).toInt() );
        int indx = l.at(3).toInt();
        indx--;
        if(indx < 0)
            indx = 0;
        else{
            if(indx > 4)
                indx = -1;
        }
        ui->cbProfileEOD->setCurrentIndex(indx);
        ui->sbGlbnDay->setValue(l.at(2).toInt());


        l = hash.value(QString::number(POLL_CODE_READ_END_MONTH)).toList();
        while(l.size() < 5)
            l.append(0);

        ui->cbProfileEnableMonth->setChecked(l.at(0).toBool());
        ui->sbProfileMonth->setValue(l.at(1).toUInt() );
        ui->sbGlbnMonth->setValue(l.at(2).toInt());
        indx = l.at(3).toUInt();
        indx--;
        if(indx < 0)
            indx = 0;
        else{
            if(indx > 3)
                indx = -1;
        }
        ui->cbProfileEOM->setCurrentIndex(indx);

        l = hash.value(QString::number(POLL_CODE_READ_POWER)).toList();
        while(l.size() < 5)
            l.append("0");

        ui->cbProfileEnablePower->setChecked(l.at(0).toBool());
        ui->cbProfilePower->setCurrentIndex( comboIndxFromKftntAndIntrvl(l.at(4).toUInt(), l.at(3).toUInt()));
        ui->sbGlbnPwr->setValue(l.at(2).toInt());


        ui->sbProfilePower->setValue(l.at(1).toInt());


        l = hash.value(QString::number(POLL_CODE_READ_VOLTAGE)).toList();
        while(l.size() < 5)
            l.append("0");

        ui->cbProfileEnableVoltage->setChecked(l.at(0).toBool());
        ui->cbProfileVoltage->setCurrentIndex( comboIndxFromKftntAndIntrvl(l.at(4).toUInt(), l.at(3).toUInt()));
        ui->sbProfileVoltage->setValue(l.at(1).toInt());
//        ui->sbGlbnVoltage->setValue(l.at(2).toInt());

        l = hash.value(QString::number(POLL_CODE_METER_STATUS)).toList();
        while(l.size() < 5)
            l.append(0);

        ui->cbProfileEnableMeterLog->setChecked(l.at(0).toInt() != 0 );
        ui->sbProfileMeterLog->setValue( l.at(1).toInt());
        ui->sbGlbnMeterLog->setValue(l.at(2).toInt());

        l = hash.value("dow").toList();//1 - mon, 2 - tue, 3 - wed
        unCheckEvrDay();
        bool checked = false;
        ui->cbMon->setChecked(checked);
        ui->cbTue->setChecked(checked);
        ui->cbWed->setChecked(checked);
        ui->cbThu->setChecked(checked);
        ui->cbFri->setChecked(checked);
        ui->cbSat->setChecked(checked);
        ui->cbSun->setChecked(checked);

        int evrDay = 0;

        for(int i = 0, iMax = l.size(); i < iMax && i < 7; i++){
            int day = l.at(i).toInt();

            evrDay += day;

            switch (day) {
            case 1: ui->cbMon->setChecked(true);  break;
            case 2: ui->cbTue->setChecked(true);  break;
            case 3: ui->cbWed->setChecked(true);  break;
            case 4: ui->cbThu->setChecked(true);  break;
            case 5: ui->cbFri->setChecked(true);  break;
            case 6: ui->cbSat->setChecked(true);  break;
            case 7: ui->cbSun->setChecked(true);  break;

            default: evrDay -= day;  break;
            }
        }

        if(evrDay == 28 && l.size() == 7)
            ui->cbEvrDay->setChecked(true);

        ui->teFrom->setTime(QTime::fromString(hash.value("tiFrom").toString(), "hh:mm"));
        ui->teTo->setTime(QTime::fromString(hash.value("tiTo").toString(), "hh:mm"));



        break;}

    case COMMAND_READ_DATABASE:{


        QStringList strTimeList = varList2strList(hash.value("d").toList()), strTimeStartIndx = varList2strList(hash.value("di").toList());


        bool ok;
        qint64 lastRowId = hash.value("lRwId").toLongLong(&ok);
        qDebug() << "lastRowId "  << hash.contains("lRwId") << lastRowId;

        qint64 tableCount = hash.value("t").toLongLong();

        bool itIsGeliks = hash.value("g", false).toBool();// versionName != "Geliks"; if(pollCode == 140 || pollCode == 100) //миттєві значення

        if(hash.contains("c")){
            QStringList columnList = varList2strList(hash.value("c").toList());

            if(columnList.isEmpty()){
                showMess(tr("Corrupted data."));
                return;
            }

            if(!itIsGeliks)
                columnList.prepend(tr("Date"));

            modelDbData->setHorizontalHeaderLabels(columnList);
        }
        QVariantList listVarData = hash.value("a").toList();

        qint64 lastTableRowId = hash.value("lTbRwId").toLongLong(&ok);
        qDebug() << "lastTableRowId "  << hash.contains("lTbRwId") << lastTableRowId;

        bool dateInUtc = itIsGeliks;

        QDateTime dt4data;

        this->dateInUtc = false;

        QString dateTimeMask = "yyyy-MM-dd hh:mm:ss";
        QString dtCell = "";
        if(!itIsGeliks && !listVarData.isEmpty()){//інакше буде показувати повідомлення "error dateTimeMask" якщо даних не буде

            QString strTime;
            if(!strTimeList.isEmpty())
                strTime = strTimeList.first();

            if(QDateTime::fromString(strTime, dateTimeMask).time().minute() != 59)
                dateInUtc = true;

            if(dateInUtc ){//4 power
                dtCell = tableName2DateTime(strTime, dateTimeMask.left(10), dateTimeMask.right(8)).toLocalTime().toString(dateTimeMask);
                dt4data = tableName2DateTime(strTime, dateTimeMask.left(10), dateTimeMask.right(8));
            }else{
                dtCell = strTime;
                dt4data = QDateTime::fromString(strTime, dateTimeMask);
            }
        }

        if(strTimeList.size() != strTimeStartIndx.size()){
            showMess(tr("Corrupted packet!"));
            qDebug() << strTimeList.size() << strTimeStartIndx.size() << strTimeList;
            qDebug() << strTimeStartIndx;
            return;
        }

        int columnListSize = modelDbData->columnCount() ;
        if(!itIsGeliks)
            columnListSize--;



        int meterSnIndx = itIsGeliks ? 1 : 0;

        int values = listVarData.size();

        for(int hashIndex = 0, maxIndex = values; hashIndex < maxIndex; hashIndex++){

            QList<QStandardItem*> listRowItem;

            if(!itIsGeliks){
                if(!strTimeStartIndx.isEmpty()){
                    if(strTimeStartIndx.first() == QString::number(hashIndex)){

                        QString strTime = strTimeList.takeFirst();
                        strTimeStartIndx.removeFirst();

                        if(dateInUtc ){//4 power
                            dtCell = tableName2DateTime(strTime, dateTimeMask.left(10), dateTimeMask.right(8)).toLocalTime().toString(dateTimeMask);
                            dt4data = tableName2DateTime(strTime, dateTimeMask.left(10), dateTimeMask.right(8));
                        }else{
                            dtCell = strTime;
                            dt4data = QDateTime::fromString(strTime, dateTimeMask);
                        }
                    }
                }



                QStandardItem *item = new QStandardItem(dtCell);
                item->setData(dt4data);
                listRowItem.append(item);
            }

            QString meterSN("");
            QStringList list = varList2strList(listVarData.at(hashIndex).toList());
            for(int i = 0; i < columnListSize; i++){
                QString str("");
                if(!list.isEmpty())
                    str = list.takeFirst();//.toString();

                if(i == meterSnIndx){
                    meterSN = str;
                }

                if(i == 0 && itIsGeliks){ //dateTime index

                    QDateTime dateTM = tableName2DateTime(str,"yyyy-MM-dd", "hh:mm:ss");

                    if(dateTM.isValid()){
                        str = dateTM.toLocalTime().toString(dateTimeMask);
                    }else{
                        str.prepend("UTC ");
                    }

                    QStandardItem *item = new QStandardItem(str);
                    item->setData(dateTM);
                    listRowItem.append(item);

                }else
                    listRowItem.append(new QStandardItem(str));
            }
            if(!listRowItem.isEmpty()){
                modelDbData->appendRow(listRowItem);
            }
        }


        if(lastTableRowId == 0 && lastRowId == 0 ){
            showMess(tr("Done."));
            return;
        }else{

            doneTables += tableCount;

            if(totalTables == 0)
                totalTables = 5000;

            if(doneTables >= totalTables)
                totalTables = doneTables ;

            int persentDone = ((doneTables * 100) / totalTables);
            if(persentDone < 1)
                persentDone = 1;
            else{
                if(persentDone > 99)
                    persentDone = 99;
            }
                emit uploadProgress(  persentDone, tr("Total count: %1 values.<br>Last Table ID: %2, Last Meter ID: %3.")
                                 .arg(doneTables)
                                     .arg(lastTableRowId).arg(lastRowId) );

            hash = hashMemoWrite.value(COMMAND_READ_DATABASE);

            hash.insert("lTbRwId", lastTableRowId);
            hash.insert("lRwId", lastRowId);

            mWrite2RemoteDev(COMMAND_READ_DATABASE, hash);

        }

        break; }

    case COMMAND_READ_DATABASE_GET_TABLES:{


        QStringList list = varList2strList(hash.value("t").toList());
        bool ok;
        qint64 lastTableRowId = hash.value("lRwId").toLongLong(&ok);

        qDebug() << "lastRowId " << lastTableRowId << hash.contains("lRwId") << lastTableRowId;

        lastTableList.append(list);

        if(lastTableList.isEmpty()){
            showMess(tr("No table."));

        }else{
            totalTables = lastTableList.size();

            if(list.isEmpty() || lastTableRowId == 0){
                doneTables = 0;
                blockDone = 0;
                emit uploadProgress( doneTables, tr("Total count: %1 tables, Downloaded: 0 tables").arg(totalTables) );
                hash = hashMemoWrite.value(COMMAND_READ_DATABASE_GET_VAL);

                hash.insert("table", lastTableList.first());
                hash.insert("gcl", true);
                lastTableRowId = 0;
                hash.insert("lRwId", lastTableRowId);
                mWrite2RemoteDev(COMMAND_READ_DATABASE_GET_VAL, hash);
            }else{
                emit uploadProgress( doneTables, tr("Selected: %1 tables").arg(totalTables) );
                hash = hashMemoWrite.value(COMMAND_READ_DATABASE_GET_TABLES);
                hash.insert("lRwId", lastTableRowId);
                mWrite2RemoteDev(COMMAND_READ_DATABASE_GET_TABLES, hash);
            }
        }
        break;}

    case COMMAND_READ_DATABASE_GET_VAL:{


        QString strTime = hash.value("d").toString();

        bool ok;
        qint64 lastRowId = hash.value("lRwId").toLongLong(&ok);
        qDebug() << "lastRowId "  << hash.contains("lRwId") << lastRowId;

        bool itIsGeliks = hash.value("g", false).toBool();//  140 || 100) //миттєві значення

        if(hash.contains("c")){
            QStringList columnList = varList2strList( hash.value("c").toList());

            if(columnList.isEmpty()){
                showMess(tr("Corrupted data."));
                return;
            }

            if(!itIsGeliks)
                columnList.prepend(tr("Date"));

            modelDbData->setHorizontalHeaderLabels(columnList);
        }

        bool dateInUtc = itIsGeliks;

        QDateTime dt4data;

        this->dateInUtc = false;

        QString dateTimeMask = "yyyy-MM-dd hh:mm:ss";
        QString dtCell = "";
        if(!itIsGeliks){

            if(QDateTime::fromString(strTime, dateTimeMask).time().minute() != 59)
                dateInUtc = true;

            if(dateInUtc ){//4 power
                dtCell = tableName2DateTime(strTime, dateTimeMask.left(10), dateTimeMask.right(8)).toLocalTime().toString(dateTimeMask);
                dt4data = tableName2DateTime(strTime, dateTimeMask.left(10), dateTimeMask.right(8));
            }else{
                dtCell = strTime;
                dt4data = QDateTime::fromString(strTime, dateTimeMask);
            }
        }

        int columnListSize = modelDbData->columnCount() ;
        if(!itIsGeliks)
            columnListSize--;


        QVariantList listVarData = hash.value("a").toList();

        int meterSnIndx = itIsGeliks ? 1 : 0;

        for(int hashIndex = 0, maxIndex = listVarData.size(); hashIndex < maxIndex; hashIndex++){

            QList<QStandardItem*> listRowItem;

            if(!itIsGeliks){
                QStandardItem *item = new QStandardItem(dtCell);
                item->setData(dt4data);
                listRowItem.append(item); //QDateTime::fromString(strTime, dateTimeMask).toString(dateTimeMask)));
            }

            QString meterSN("");
            QStringList list = varList2strList(listVarData.at(hashIndex).toList());// hash.value(QString::number(hashIndex)).toStringList();
            for(int i = 0; i < columnListSize; i++){
                QString str("");
                if(!list.isEmpty())
                    str = list.takeFirst();//.toString();

                if(i == meterSnIndx){
                    meterSN = str;
                }

                if(i == 0 && itIsGeliks){ //dateTime index

                    QDateTime dateTM = tableName2DateTime(str,"yyyy-MM-dd", "hh:mm:ss");// QDateTime::fromString(str,dateTimeMask);

                    if(dateTM.isValid()){
                        str = dateTM.toLocalTime().toString(dateTimeMask);
                    }else{
                        str.prepend("UTC ");
                    }

                    QStandardItem *item = new QStandardItem(str);
                    item->setData(dateTM);
                    listRowItem.append(item);

                }else
                    listRowItem.append(new QStandardItem(str));
            }
            if(!listRowItem.isEmpty()){
                modelDbData->appendRow(listRowItem);
            }
        }


        if(!lastTableList.isEmpty() && lastRowId == 0 ){
            lastTableList.removeFirst();
            doneTables++;
            blockDone = 0;
        }

        if(lastTableList.isEmpty()){
            showMess(tr("Done."));
        }else{
            blockDone++;

                emit uploadProgress( ((doneTables * 100) / totalTables) , tr("Total count: %1 tables, %2: %3 tables.<br>Block: %4, Last ID: %5.")
                                 .arg(totalTables).arg( listVarData.isEmpty() ? tr("Parsed") : tr("Downloaded"))
                                     .arg(doneTables).arg(blockDone).arg(lastRowId) );

            hash = hashMemoWrite.value(COMMAND_READ_DATABASE_GET_VAL);

            hash.insert("table", lastTableList.first());
            hash.insert("lRwId", lastRowId);

//            mWrite2RemoteDev(COMMAND_READ_DATABASE_GET_VAL, h);
            mWrite2RemoteDev(COMMAND_READ_DATABASE_GET_VAL, hash);

        }

        break; }

    case COMMAND_READ_METER_LOGS:{

        QString dateTimeMask, dateTimeFormat;


        bool ok;
        qint64 lastRowId = hash.value("lRwId").toLongLong(&ok);

        qDebug() << "lastRowId " << lastRowId << hash.contains("lRwId") << lastRowId;

        qint64 lastTableRowId = hash.value("lTbRwId").toLongLong(&ok);
        qDebug() << "lastRowId " << lastTableRowId << hash.contains("lTbRwId") << lastRowId;

        qint64 tableCount = hash.value("t").toLongLong();
        if(hash.contains("c")){
            QStringList columnList = varList2strList(hash.value("c").toList());

            if(columnList.isEmpty()){
                showMess(tr("Corrupted data."));
                return;
            }
            modelDbDataEv->setHorizontalHeaderLabels(columnList);

        }

        dateTimeFormat = "yyyy-MM-dd hh:mm:ss";
        dateTimeMask = "yyyy-MM-dd hh:mm:ss";

        int columnListSize = modelDbDataEv->columnCount() ;


        QVariantList listVarData = hash.value("a").toList();

        int meterSnIndx = 1;


        for(int hashIndex = 0, maxIndex = listVarData.size(); hashIndex < maxIndex; hashIndex++){

            QList<QStandardItem*> listRowItem;

            QString meterSN("");
            QStringList list = varList2strList(listVarData.at(hashIndex).toList());// h.value(QString::number(hashIndex)).toStringList();
            for(int i = 0; i < columnListSize; i++){
                QString str("");
                if(!list.isEmpty())
                    str = list.takeFirst();//.toString();

                if(i == meterSnIndx){
                    meterSN = str;
                }

                if(i == 0 ){ //dateTime index
                    QDateTime dateTM = tableName2DateTime(str,"yyyy-MM-dd", "hh:mm:ss");// QDateTime::fromString(str,dateTimeMask);

                    if(dateTM.isValid()){
                        str = dateTM.toLocalTime().toString(dateTimeFormat);
                    }else{
                        str.prepend("UTC ");
                    }
                }



                listRowItem.append(new QStandardItem(str));
            }
            if(!listRowItem.isEmpty()){
                modelDbDataEv->appendRow(listRowItem);
            }
        }


        if(lastTableRowId == 0 && lastRowId == 0 ){

            showMess(tr("Done."));
        }else{

            doneTables += tableCount;

            if(totalTables == 0)
                totalTables = 5000;

            if(doneTables > totalTables)
                totalTables = doneTables;

            int persentDone = ((doneTables * 100) / totalTables);
            if(persentDone < 1)
                persentDone = 1;
            else{
                if(persentDone > 99)
                    persentDone = 99;
            }
            emit uploadProgress(  persentDone, tr("Total count: %1 values.<br>Last Table ID: %2, Last Meter ID: %3.")
                                 .arg(doneTables)
                                     .arg(lastTableRowId).arg(lastRowId) );

            hash = hashMemoWrite.value(COMMAND_READ_METER_LOGS);//code listNI

            hash.insert("lTbRwId", lastTableRowId);
            hash.insert("lRwId", lastRowId);

            mWrite2RemoteDev(COMMAND_READ_METER_LOGS, hash);

        }

        break; }

    case COMMAND_READ_METER_LOGS_GET_TABLES:{

        QStringList list = varList2strList(hash.value("t").toList());
        lastTableList.append(list);

        bool ok;
        qint64 lastTableRowId = hash.value("lRwId").toLongLong(&ok);

        qDebug() << "lastRowId " << lastTableRowId << hash.contains("lRwId") << lastTableRowId;

        if(lastTableList.isEmpty()){
            showMess(tr("No table."));
        }else{

            totalTables = lastTableList.size();

            if(list.isEmpty() || lastTableRowId == 0){
                doneTables = 0;
                blockDone = 0;
                emit uploadProgress( doneTables, tr("Total count: %1 tables, Downloaded: 0 tables").arg(totalTables) );
                hash = hashMemoWrite.value(COMMAND_READ_METER_LOGS_GET_TABLES);

                hash.insert("gcl", true);

                hash.insert("table", lastTableList.first());
                hash.insert("lRwId", (qint64)0);
                mWrite2RemoteDev(COMMAND_READ_METER_LOGS_GET_VAL, hash);
            }else{
                emit uploadProgress( doneTables, tr("Selected: %1 tables").arg(totalTables) );
                hash = hashMemoWrite.value(COMMAND_READ_METER_LOGS_GET_TABLES);
                hash.insert("lRwId", lastTableRowId);
                mWrite2RemoteDev(COMMAND_READ_METER_LOGS_GET_TABLES, hash);

            }

        }


        break;}

    case COMMAND_READ_METER_LOGS_GET_VAL:{

        QString dateTimeMask, dateTimeFormat;


        bool ok;
        qint64 lastTableRowId = hash.value("lRwId").toLongLong(&ok);

        qDebug() << "lastRowId " << lastTableRowId << hash.contains("lRwId") << lastTableRowId;



        if(hash.contains("c")){
            QStringList columnList = varList2strList(hash.value("c").toList());

            if(columnList.isEmpty()){
                showMess(tr("Corrupted data."));
                return;
            }
            modelDbDataEv->setHorizontalHeaderLabels(columnList);

        }

        dateTimeFormat = "yyyy-MM-dd hh:mm:ss";
        dateTimeMask = "yyyy-MM-dd hh:mm:ss";

        int columnListSize = modelDbDataEv->columnCount() ;


        QVariantList listVarData = hash.value("a").toList();

        int meterSnIndx = 1;

        for(int hashIndex = 0, maxIndex = listVarData.size(); hashIndex < maxIndex; hashIndex++){

            QList<QStandardItem*> listRowItem;

            QString meterSN("");
            QStringList list = varList2strList(listVarData.at(hashIndex).toList());// h.value(QString::number(hashIndex)).toStringList();
            for(int i = 0; i < columnListSize; i++){
                QString str("");
                if(!list.isEmpty())
                    str = list.takeFirst();//.toString();

                if(i == meterSnIndx){
                    meterSN = str;
                }

                if(i == 0 ){ //dateTime index
                    QDateTime dateTM = tableName2DateTime(str,"yyyy-MM-dd", "hh:mm:ss");// QDateTime::fromString(str,dateTimeMask);

                    if(dateTM.isValid()){
                        str = dateTM.toLocalTime().toString(dateTimeFormat);
                    }else{
                        str.prepend("UTC ");
                    }
                }



                listRowItem.append(new QStandardItem(str));
            }
            if(!listRowItem.isEmpty()){
                modelDbDataEv->appendRow(listRowItem);
            }
        }


        if(!lastTableList.isEmpty() && lastTableRowId == 0 ){
            lastTableList.removeFirst();
            doneTables++;
            blockDone = 0;
        }

        if(lastTableList.isEmpty()){
            showMess(tr("Done."));
        }else{
            blockDone++;
            emit uploadProgress( ((doneTables * 100) / totalTables) , tr("Total count: %1 tables, %2: %3 tables.<br>Block: %4, Last RowID: %5.")
                                 .arg(totalTables).arg( listVarData.isEmpty() ? tr("Parsed") : tr("Downloaded"))
                                     .arg(doneTables).arg(blockDone).arg(lastTableRowId) );

            hash = hashMemoWrite.value(COMMAND_READ_METER_LOGS_GET_VAL);//code listNI

            hash.insert("table", lastTableList.first());
            hash.insert("lRwId", lastTableRowId);

            mWrite2RemoteDev(COMMAND_READ_METER_LOGS_GET_VAL, hash);

        }

        break; }

    case COMMAND_READ_METER_LIST_FRAMED:{

        bool ignoreEmptyList = hash.contains("t");
        if(ignoreEmptyList){
            modelAddMeter->setHorizontalHeaderLabels(tr("Model,Serial Number,NI,Memo,Password,On/Off,Physical values,Tariff Count,Meter Version").split(","));
            totalTables = hash.value("t").toInt();
            doneTables = 0;
        }

        QVariantList lVar = hash.value("m").toList();
        int lastIndx = hash.value("i").toInt();

        if((lVar.isEmpty() || !hash.contains("i")) && !ignoreEmptyList){
           showMess(tr("Corrupted data."));
            break;
        }

        QStringList k = QString("model,NI,SN,memo,passwd,on,politic,trff,vrsn").split(',');

        int iMax = lVar.size();
        int rowF = modelAddMeter->rowCount();

        for(int i = 0, colMax = k.size(); i < iMax; i++){
            QStringList lh = varList2strList(lVar.at(i).toList());
            QList<QStandardItem*> m ;


            for(int col = 0, lhMax = lh.size(); col < colMax; col++)
                m.append(new QStandardItem( (col < lhMax) ? lh.at(col) : "---"));
            modelAddMeter->appendRow(m);
        }

        qDebug() << "COMMAND_READ_METER_LIST_FRAMED " << modelAddMeter->rowCount() << rowF << lastIndx << iMax ;



        if(lastIndx >= 0){
            doneTables += iMax;
             blockDone = 0;
            emit uploadProgress( ((doneTables * 100) / totalTables), tr("Total count: %1 meters, Downloaded: %2 meters")
                                 .arg(totalTables).arg(doneTables) );

             hash.clear();
             hash.insert("i", lastIndx);
             hash.insert("max_len", ui->sbReadWriteMeterLen->value());
             hash.insert("cmprss", ui->cbCmprssMeterList->isChecked());
             mWrite2RemoteDev(COMMAND_READ_METER_LIST_FRAMED, hash);

        }else{
            ui->tvAddMeterTable->resizeColumnsToContents();
            showMess(tr("Done."));
        }


        break;}

    case COMMAND_READ_ZBR_LOG:{

        ui->pteZbyrLog->clear();
        ui->pteZbyrLog->appendPlainText(hash.value("s").toString());
        break;}

    case COMMAND_READ_ABOUT_OBJECT:{
        ui->pteAboutObjectMemo->clear();
        ui->leCoordinates->setText(hash.value("c").toString());
        ui->pteAboutObjectMemo->appendPlainText(hash.value("m").toString());
        ui->leLineMemo->setText(hash.value("l").toString());
        break;}

    case COMMAND_READ_POLL_SETT:{
        ui->sbMetrRetry->setValue(hash.value("mr").toInt());
        ui->sbWati4Poll->setValue(hash.value("pw").toInt());
        ui->cbHardAddrsn->setChecked(hash.value("ha").toBool());

        ui->cbW4E->setChecked(hash.value("w4e").toBool());
        ui->sbW4ERtrBf->setValue(hash.value("w4eRb").toInt());
        ui->sbW4ErA->setValue(hash.value("w4eRa").toInt());

        ui->cbTimeCorrection->setChecked(hash.value("tc").toBool());
        ui->sbMaxSecDiff->setValue(hash.value("td").toInt());

        break;}

    case COMMAND_READ_POLL_STATISTIC:{
        modelPollStat->clear();
        QVariantList listVar = hash.value("l2").toList();
        if(listVar.isEmpty())
            return;

        QStringList header = varList2strList(listVar.first().toList());
        if(header.size() < 7)
            break;


        modelPollStat->setHorizontalHeaderLabels(header);

        for(int i = 1, iMax = listVar.size(), colSize = modelPollStat->columnCount(); i < iMax; i++){
            QList<QStandardItem*> l;
            int j = 0;
            QStringList ll = varList2strList(listVar.at(i).toList());
            for(int jMax = ll.size(); j < jMax; j++ )
                l.append(new QStandardItem(ll.at(j)));
            for( ; j < colSize; j++)
                l.append(new QStandardItem("---"));
            modelPollStat->appendRow(l);
        }

        ui->tvPollStatistic->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tvPollStatistic->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        QTimer::singleShot(50, this, SLOT(manualResizePollStat()) );

        break;}

    case COMMAND_READ_TABLE_HASH_SUMM:{
        //????

        if(ui->pteHashSumm->toPlainText().isEmpty()){
            ui->pteHashSumm->appendPlainText(tr("Hash algoritm: %1\n   Table name   \t  Hash summ(base64)  \tHash summ (hex)  ").arg(hash.value("hsh").toString()));
        }
        qint64 lastTableRowId = hash.value("lRwId").toLongLong();

        QStringList listT = varList2strList(hash.value("lt").toList());
        QStringList listH = varList2strList(hash.value("lth").toList());

        if(listH.size() != listT.size()){
            showMess(tr("Corrupted data. Ignoring..."));
            break;
        }

        int addIndx = ui->pteHashSumm->toPlainText().split("\n").size();

        for(int i = 0, iMax = listH.size(); i < iMax; i++){
            ui->pteHashSumm->appendPlainText( QString("%1. %2\t%3\t%4").arg(i + addIndx ).arg(listT.at(i))
                                              .arg(listH.at(i)).arg(QString(QByteArray::fromBase64( listH.at(i).toLocal8Bit() ).toHex()))  );
        }

        if(hash.contains("lRwId")){
            ui->pteHashSumm_2->appendPlainText(listT.join("\n"));
        }

        if(lastTableRowId == 0)
            showMess(tr("Done"));
        else{
            hash = hashMemoWrite.value(COMMAND_READ_TABLE_HASH_SUMM);
            hash.insert("lRwId", lastTableRowId);
            mWrite2RemoteDev(COMMAND_READ_TABLE_HASH_SUMM, hash);

        }
        break;}



    case COMMAND_WRITE_METER_LIST_FRAMED:{

        int rowCount = modelAddMeter->rowCount();

        QStringList k = QString("model,NI,SN,memo,passwd,on,politic,trff").split(',');//,vrsn

        int row = hash.value("i").toInt();
        if(row >= rowCount){
            showMess(tr("Corrupted data."));
             break;
        }else{
            if(row < 0){
                showMess(tr("Done."));
                break;
            }
        }

        QVariantList listVarM;
        int maxLen = ui->sbReadWriteMeterLen->value();
        if(maxLen < 400)
            maxLen = MAX_PACKET_LEN_RECOMENDATION;

        for(int j = 0, colMax = k.size(), bytes = 50; row < rowCount; row++, j++){

            QStringList l;
            for(int col = 0; col < colMax; col++)
                l.append(modelAddMeter->item(row,col)->text());

            bytes += l.join("___").size(); // "",
            if(bytes > maxLen){
                row--;
                break;
            }

            listVarM.append(strList2Var(l));
        }

        hash.insert("m", listVarM);

        emit uploadProgress( ((row * 100) / rowCount), tr("Total count: %1 meters, Uploaded: %2 meters")
                             .arg(rowCount).arg(row) );
        row++;

        hash.insert("i", (row >= rowCount) ? -1 : row);

        mWrite2RemoteDev(COMMAND_WRITE_METER_LIST_FRAMED, hash);



        break;}

    case COMMAND_ERROR_CODE:{
        QString mess;
        switch(hash.value("e").toInt()){
        case ERR_DATABASE_CLOSED: mess = tr("Command %1. Database not opened.").arg(hash.value("lcmd").toInt()); break;
        case ERR_INCORRECT_REQUEST: mess = tr("Command %1. Incorrect request.").arg(hash.value("lcmd").toInt()); break;
        case ERR_INTERNAL_ERROR: mess = tr("Command %1. Internal error.").arg(hash.value("lcmd").toInt()); break;
        case ERR_NO_DATA: mess = tr("Command %1. Data not found.").arg(hash.value("lcmd").toInt()); break;



        case ERR_MAX_TABLE_COUNT : mess = tr("Command %1. Table count limit").arg(hash.value("lcmd").toInt()); break;
        case ERR_CORRUPTED_DATA : mess = tr("Command %1. Corrupted data").arg(hash.value("lcmd").toInt()); break;
        case ERR_DUPLICATE_NI : mess = tr("Command %1. Duplicating NI").arg(hash.value("lcmd").toInt()); break;
        case ERR_DUPLICATE_SN : mess = tr("Command %1. Duplicating SN").arg(hash.value("lcmd").toInt()); break;
        case ERR_DATE_NOT_VALID : mess = tr("Command %1. Date not valid").arg(hash.value("lcmd").toInt()); break;
        case ERR_COMMAND_NOT_ALLOWED : mess = tr("Command %1. Command not allowed").arg(hash.value("lcmd").toInt()); break;
        case ERR_ACCESS_DENIED : mess = tr("Command %1. Command not allowed").arg(hash.value("lcmd").toInt()); break;
        case ERR_RESOURCE_BUSY : mess = tr("Command %1. Command not allowed").arg(hash.value("lcmd").toInt()); break;


        case ERR_NO_ERROR : mess = tr("Command %1. Done!").arg(hash.value("lcmd").toInt()); break;
        default: mess = tr("Command %1. Unknown error. Error code: %2").arg(hash.value("lcmd").toInt()).arg(hash.value("e").toInt()); break;
        }
        showMess(mess);
        break;}

    case COMMAND_ERROR_CODE_EXT:{

        QString mess;
        switch(hash.value("e").toInt()){
        case ERR_DATABASE_CLOSED: mess = tr("Command %1. Database not opened.").arg(hash.value("lcmd").toInt()); break;
        case ERR_INCORRECT_REQUEST: mess = tr("Command %1. Incorrect request.").arg(hash.value("lcmd").toInt()); break;
        case ERR_INTERNAL_ERROR: mess = tr("Command %1. Internal error.").arg(hash.value("lcmd").toInt()); break;
        case ERR_NO_DATA: mess = tr("Command %1. Data not found.").arg(hash.value("lcmd").toInt()); break;



        case ERR_MAX_TABLE_COUNT : mess = tr("Command %1. Table count limit").arg(hash.value("lcmd").toInt()); break;
        case ERR_CORRUPTED_DATA : mess = tr("Command %1. Corrupted data").arg(hash.value("lcmd").toInt()); break;
        case ERR_DUPLICATE_NI : mess = tr("Command %1. Duplicating NI").arg(hash.value("lcmd").toInt()); break;
        case ERR_DUPLICATE_SN : mess = tr("Command %1. Duplicating SN").arg(hash.value("lcmd").toInt()); break;
        case ERR_DATE_NOT_VALID : mess = tr("Command %1. Date not valid").arg(hash.value("lcmd").toInt()); break;
        case ERR_COMMAND_NOT_ALLOWED : mess = tr("Command %1. Command not allowed").arg(hash.value("lcmd").toInt()); break;
        case ERR_ACCESS_DENIED : mess = tr("Command %1. Command not allowed").arg(hash.value("lcmd").toInt()); break;
        case ERR_RESOURCE_BUSY : mess = tr("Command %1. Command not allowed").arg(hash.value("lcmd").toInt()); break;


        case ERR_NO_ERROR : mess = tr("Command %1. Done!").arg(hash.value("lcmd").toInt()); break;
        default: mess = tr("Command %1. Unknown error. Error code: %2").arg(hash.value("lcmd").toInt()).arg(hash.value("e").toInt()); break;
        }

        mess.append(tr("<br>Device: %1").arg(hash.value("em").toString()));
        showMess(mess);
        break;}

    case COMMAND_READ_METER_LIST_HASH_SUMM:{
        showMess(tr("The meter list hash summ is %1 (base64)<br>%2(hex)<br>Algorith: %3").arg(hash.value("mhsh").toString())
                 .arg(QString(QByteArray::fromBase64(hash.value("mhsh").toByteArray()).toHex()))
                 .arg(hash.value("hsh").toString()));
        break;}

    case COMMAND_I_NEED_MORE_TIME:{

        switch(hash.value("lcmd").toInt()){
        case COMMAND_WRITE_DROP_TABLE:{

            qreal diff = hash.value("im").toInt() - doneTables;
            doneTables = hash.value("im").toInt();

            qreal speed = (diff / hash.value("et").toReal()) * (qreal)1000;

            emit uploadProgress( ((doneTables * 100) / totalTables), tr("Total count: %1 tables, Deleted: %2 tables.<br>Speed: %3 tables per secound.")
                                 .arg(totalTables).arg(doneTables).arg( QString::number(speed, 'f', 2) ) );

            break;}
        }

        return;}

    default:
        qDebug() << "data2gui unknown command " << command << hash;
        break;
    }
}
//##########################################################################################
void MainWindow::onErrorWrite()
{
    QMessageBox::critical(this, windowTitle(), tr("No answer from device."));
}
//##########################################################################################
void MainWindow::showMess(QString mess)
{
    emit hideWaitMess();
    QMessageBox::information(this, windowTitle(), mess);
    emit hideWaitMess();

}
//##########################################################################################
void MainWindow::authrizeAccess(int accessLevel)
{
    ui->pbRead->setEnabled(accessLevel > 0);
    ui->pbLogOut->setEnabled(accessLevel > 0);

    ui->pbWrite->setEnabled((accessLevel == 2 || accessLevel == 1));
    ui->groupBox_4->setEnabled((accessLevel == 2 || accessLevel == 1));

    ui->stackedWidget->setCurrentIndex((accessLevel > 0) ? 1 : 0);

//    ui->label_6->setPixmap(QPixmap((accessLevel == 2) ?  ":/katynko/kts-intek-logo-happy.png" : ":/katynko/kts-intek-logo.png"));
}
//##########################################################################################

void MainWindow::noAnswerFromDev()
{
    showMess(tr("No answer from device."));
}
//##########################################################################################
void MainWindow::changeCounters(qint64 addThisVal, qint64 addThisValNotCompr, bool rec)
{
    if(rec){
        byteReceiv += addThisVal;
        lastByteRec = addThisVal;
        if(addThisValNotCompr < 0){
            byteReceivNotComprssd += addThisVal;
            lastRecNC = addThisVal;
        }else{
            byteReceivNotComprssd += addThisValNotCompr;
            lastRecNC = addThisValNotCompr;
        }
    }else{
        byteSend += addThisVal;
        lastByteSend = addThisVal;
        if(addThisValNotCompr < 0){
            byteSendNotComprssd += addThisVal;
            lastSendNC = addThisVal;
        }else{
            byteSendNotComprssd += addThisValNotCompr;
            lastSendNC = addThisValNotCompr;
        }
    }

    QString svd = "-";
    if((byteReceivNotComprssd + byteSendNotComprssd) > 0){
        svd = QString::number(100 - (qreal)(byteReceiv + byteSend) * 100 / (qreal)(byteReceivNotComprssd + byteSendNotComprssd), 'f', 2);

    }
    svd.append("%");


    emit setNewConnStat(tr("Last R/W: %1/%2 bytes, R/W (NC): %3/%4; Total R/W: %5/%6, R/W (NC): %7/%8, Saving: %9")
                        .arg(humanByteView(lastByteRec))
                        .arg(humanByteView(lastByteSend))
                        .arg(humanByteView(lastRecNC))
                        .arg(humanByteView(lastSendNC))


                        .arg(humanByteView(byteReceiv))
                          .arg(humanByteView( byteSend))

                          .arg(humanByteView( byteReceivNotComprssd))
                          .arg(humanByteView( byteSendNotComprssd))
                          .arg(svd )
                        );
}
//##########################################################################################
void MainWindow::onPhysValChanged(QStringList n, QStringList l)
{
    QVariantMap mpv;
    ui->cbAddMeterPhysicalVal->clear();
    for(int i = 0, iMax = n.size(); i < iMax; i++){
        ui->cbAddMeterPhysicalVal->addItem(n.at(i), l.at(i));
        ui->cbAddMeterPhysicalVal->setItemData(i, l.at(i), Qt::ToolTipRole );

        mpv.insert(n.at(i), l.at(i));
    }
    if(!n.isEmpty())
        ui->cbAddMeterPhysicalVal->setCurrentIndex(0);
}
//##########################################################################################
void MainWindow::onCheckDbNIfieldIsCorrect()
{
    QString str = ui->leMeterDataOnlyThisNI->text().simplified().trimmed();
    if(!str.isEmpty()){
        if(str.contains(",") || str.contains("-")){
            if(listNiFromLine(str).size() > MAX_REQUEST_NI_IN_LINE){
                ui->leMeterDataOnlyThisNI->setStyleSheet("QLineEdit{background: qlineargradient(spread:reflect, x1:0.486513, y1:0.339, x2:1, y2:0.346, stop:0.572193 rgba(255, 0, 0, 193), stop:1 rgba(255, 255, 255, 255));}");
                return;
            }
        }

    }
    ui->leMeterDataOnlyThisNI->setStyleSheet("");

}
//##########################################################################################
QList<int> MainWindow::getFilterList(const int &startIndx, const int &count) const
{
    QList<int> l;
    for(int i = startIndx, iMax = startIndx + count; i < iMax; i++)
        l.append(i);
    return l;
}
//##########################################################################################
int MainWindow::comboIndxFromKftntAndIntrvl(const qint32 &kftnt, const qint32 &intrvl) const
{
    if(kftnt == 1)
        return 0;
    else{
        if(intrvl > 0 && intrvl < 5)
            return intrvl;
        else{
            switch(intrvl){
            case 6: return 5;
            case 8: return 6;
            case 12: return 7;
            case 24: return 8;
            case 48: return 9;
            case 72: return 10;

            }
        }
    }
    return -1;
}
//##########################################################################################
qint32 MainWindow::intrvalValFromComboIndx(const int &indx) const
{
    if(indx == 0)
        return 30;
    if(indx > 0 && indx < 5)
        return indx;
    switch (indx) {
    case 5: return 6;
    case 6: return 8;
    case 7: return 12;
    case 8: return 24;
    case 9: return 48;
    case 10: return 72;
    }
    return 333;
}
//##########################################################################################
QStringList MainWindow::listNiFromLine(const QString &aLine) const
{
    QStringList listNI;
    if(!aLine.isEmpty()){

            QStringList listComma ;
            // QStringList listNI;

            listComma = aLine.split(",");

            int niCounter = 0;
            foreach(QString strComma, listComma){

                if(strComma.indexOf('-') >= 0){
                    QStringList listDefis = strComma.split("-");

                    if(listDefis.size() > 1){
                        bool ok;
                        quint64 poch = listDefis.first().toInt(&ok, 10);

                        if(ok){
                            quint64 endNI=listDefis.last().toInt(&ok, 10);
                            if(ok){
                                if(poch <= endNI){
                                    for( ; poch <= endNI; poch++){
                                        listNI.append(QString::number(poch));
                                        niCounter++;
                                    }
                                }else{
                                    for( ; endNI <= poch; poch--){
                                        listNI.append(QString::number(poch));
                                        niCounter++;
                                    }
                                }

                            }else{
                                listNI.append(listDefis.first());
                                listNI.append(listDefis.last());
                                niCounter++;
                                niCounter++;
                            }
                        }else{
                            listNI.append(listDefis.first());
                            niCounter++;
                        }


                    }else{
                        listNI.append(strComma);
                        niCounter++;
                    }

                }else{
                    listNI.append(strComma);
                    niCounter++;
                }
                if(niCounter > MAX_REQUEST_NI_IN_LINE)
                    break;
            }
            listNI.removeDuplicates();



            for(int i = 0, iMax = listNI.size(); i < iMax; i++){
                if(listNI.at(i).isEmpty()){
                    listNI.removeAt(i);
                    i--;
                    iMax--;
                }
            }

//            listNI = listNI.mid(0,300);


    }
    return listNI;
}

//##########################################################################################
QString MainWindow::emailUtcOffset(const int &offst)
{
    QString h = QString::number(qAbs(offst)/3600).rightJustified(  2 , '0');
    QString m = QString::number( ( qAbs(offst)%3600)/60).rightJustified(2, '0');

    QString alfj = (offst > 0) ? "+" : "-";
    if(offst == 0)
        alfj = "±";
    return alfj + h + ":" + m;
}
//##########################################################################################
QVariantHash MainWindow::json2hash(const QByteArray &arrJson)
{
    QJsonParseError jErr;
    QJsonDocument jDoc = QJsonDocument::fromJson( arrJson, &jErr);
    return jDoc.object().toVariantHash();
}
//##########################################################################################
QDateTime MainWindow::tableName2DateTime(const QString &tableName, const qint32 addDays, const bool useLocal) const
{
    if(tableName.mid(4,1) != "_")
        return QDateTime( QDate::fromString(tableName.mid(4, 10), "yyyy_MM_dd"), QTime::fromString(tableName.right(5), "hh_mm"),  useLocal ? Qt::LocalTime : Qt::UTC ).addDays(addDays);
    else
        return QDateTime( QDate::fromString(tableName.left(10), "yyyy_MM_dd"), QTime::fromString(tableName.right(5), "hh_mm"), useLocal ? Qt::LocalTime : Qt::UTC ).addDays(addDays);
    //yyyy_MM_dd
}
//##########################################################################################
QDateTime MainWindow::tableName2DateTime(const QString &tableName, const QString &dateFormat, const QString &timeFormat, const qint32 addDays) const
{
    return QDateTime( QDate::fromString(tableName.left(dateFormat.length()), dateFormat), QTime::fromString(tableName.right(timeFormat.length()), timeFormat),  Qt::UTC ).addDays(addDays);
    //yyyy_MM_dd
}
//##########################################################################################
QDateTime MainWindow::tableVal2DateTime(const QString &text, const QString &mask, const bool useLocal) const
{
    //yyyy-MM-dd hh:mm:ss

       return QDateTime( QDate::fromString(text.left(10), "yyyy-MM-dd"), QTime::fromString( text.right(mask.length()), mask), useLocal ? Qt::LocalTime : Qt::UTC );

}
//##########################################################################################
QDateTime MainWindow::dateTimeFromStr(const QString &str)
{
    return QDateTime( QDate::fromString(str.left(10), "yyyy-MM-dd"), QTime::fromString(str.right(8), "hh:mm:ss"), Qt::UTC );
}
//##########################################################################################
QString MainWindow::humanByteView(QString str)
{
    QString retVal = str.right(3);
    str.chop(3);

    while(!str.isEmpty()){
        retVal.prepend(str.right(3) + " ");
        str.chop(3);
    }
    return retVal;

}
//##########################################################################################
QString MainWindow::humanByteView(const qint64 &val)
{
    return humanByteView(QString::number(val));
}
//##########################################################################################
bool MainWindow::connectionDown()
{
    if(!ui->pbLogOut->isEnabled()){
        showMess(tr("Device disconnected."));
        ui->actionDevice->setEnabled(ui->pbLogOut->isEnabled());
        ui->stackedWidget->setCurrentIndex(0);
        return true;
    }
    return false;

}
//##########################################################################################
QStringList MainWindow::varList2strList(const QVariantList &list)
{
    QStringList l;
    for(int i = 0, iMax = list.size(); i < iMax; i++)
        l.append(list.at(i).toString());
    return l;
}
//##########################################################################################
QVariantList MainWindow::strList2VarList(const QStringList &list)
{
    QVariantList l;
    for(int i = 0, iMax = list.size(); i < iMax; i++)
        l.append(list.at(i));
    return l;
}
//##########################################################################################
QVariant MainWindow::strList2Var(const QStringList &list)
{
    QVariantList l;
    for(int i = 0, iMax = list.size(); i < iMax; i++)
        l.append(list.at(i));
    return QVariant(l);
}
//##########################################################################################

void MainWindow::on_pbLogIn_clicked()
{

    byteSend = 0;
    byteReceiv = 0;
    byteSendNotComprssd = 0;
    byteReceivNotComprssd = 0;

    ui->pteAboutConnObj->clear();
    ui->swDeviceOperations->setCurrentIndex( 0);
    ui->pbWrite->setVisible( false );
    ui->pbRead->setVisible(false);

    lastConnDevInfo = tr("IP: %1, Port: %2, Login: %3")
            .arg(ui->leIp->text().simplified().trimmed())
            .arg(ui->sbPort->value())
            .arg(ui->leLogin->text());

    emit setSttsNewPixmap( QPixmap(":/katynko/deviceisdisconnected.png"));
    emit setSttsNewTxt(lastConnDevInfo);

    emit conn2thisDev( ui->cbHashSumm->currentIndex(), ui->leLogin->text(), ui->lePasswd->text(), ui->leIp->text().simplified().trimmed(), ui->sbPort->value(),
                      ui->sbTimeOut->value() * 1000  ,
                      false, ui->cbZlib->isChecked());

    lastIpStr = ui->leIp->text().simplified().trimmed();
    lastTimeOutMS = ui->sbPort->value() * 1000;
    emit showWaitMess(ui->sbTimeOut->value());
}
//##########################################################################################
void MainWindow::on_pbLogOut_clicked()
{
    emit closeConnection();
    ui->stackedWidget->setCurrentIndex(0);
}
//##########################################################################################
void MainWindow::manualResizePollStat()
{
    ui->tvPollStatistic->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
}
//##########################################################################################

void MainWindow::unCheckEvrDay()
{
    ui->cbEvrDay->setChecked(false);
}
//##########################################################################################
void MainWindow::onLvMeterDataProfile_activated(const QModelIndex &index)
{
    switch (modelProfile4DB->itemData(index).value(Qt::UserRole + 1).toInt()) {
    case POLL_CODE_READ_CURRENT: ui->lvMeterDataTariff->setEnabled(true); break;
    case POLL_CODE_READ_END_DAY: ui->lvMeterDataTariff->setEnabled(true); break;
    case POLL_CODE_READ_END_MONTH: ui->lvMeterDataTariff->setEnabled(true); break;
//    case POLL_CODE_READ_POWER: ui->lvMeterDataTariff->setEnabled(true); break;
//    case POLL_CODE_READ_VOLTAGE: ui->lvMeterDataTariff->setEnabled(true); break;
    default: ui->lvMeterDataTariff->setEnabled(false);    break;
    }

    /*  item->setData(listData.at(i), Qt::UserRole + 1);
        item->setData(listKeys.at(i), Qt::UserRole + 2);
        item->setData(listKeys2.at(i), Qt::UserRole + 3);*/


    for(int i = 0, iMax = modelPhVal4DB->rowCount(); i < iMax; i++)
        hChecked.insert(modelPhVal4DB->item(i,0)->text(), modelPhVal4DB->item(i,0)->checkState());

    QStringList list = modelProfile4DB->itemData(index).value(Qt::UserRole + 2).toString().split(",");
    QStringList list2 = modelProfile4DB->itemData(index).value(Qt::UserRole + 3).toString().split(",");
    modelPhVal4DB->clear();
    for(int i = 0, iMax = list.size(); i < iMax; i++){
        QStandardItem *item = new QStandardItem(list2.at(i));
        item->setData(list.at(i));
        item->setCheckable(true);
        item->setCheckState( hChecked.value(list2.at(i),Qt::Checked));
        modelPhVal4DB->appendRow(item);
    }

}

//##########################################################################################

void MainWindow::on_actionDevice_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}
//##########################################################################################
void MainWindow::on_actionHome_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}

//##########################################################################################
void MainWindow::on_lvDevOperation_clicked(const QModelIndex &index)
{
    ui->swDeviceOperations->setCurrentIndex( proxy_modelDevOptions->mapToSource(index).row() + 1);
    readCommand = index.data(Qt::UserRole + 2).toInt();
    writeCommand = index.data(Qt::UserRole + 1).toInt() ;

    ui->pbWrite->setVisible( writeCommand > 0 );
    ui->pbRead->setVisible(readCommand > 0);
}
//##########################################################################################


void MainWindow::on_pbRead_clicked()
{
    if(connectionDown())
        return;

    if(readCommand < 1){
        ui->pbRead->setVisible(false);
        return;
    }
    QVariantHash hash;
    switch (readCommand) {
    case COMMAND_READ_ABOUT_OBJECT:  ui->pteAboutObjectMemo->clear(); ui->leCoordinates->clear(); ui->leLineMemo->clear(); break;

    case COMMAND_READ_ABOUT_PLG: ui->pteMeterPlg->clear(); break;
    case COMMAND_READ_DATE_SETT: {
        ui->dtLocal->clear();
        ui->dtLocalUTC->clear();
        ui->pteMatildaTime->clear();
        ui->lblLocalTZ->clear();
        ui->pbEnDisNtpSync->setEnabled(false);
        ui->pteNtpList->clear();
        ui->leFltrTz->clear();
        for(int i = 0, iMax = modelTimeZone->rowCount(); i < iMax; i++)
            modelTimeZone->item(i,0)->setCheckState(Qt::Unchecked);
        break;}
    case COMMAND_READ_GPRS_SETT: ui->leGPRS_Apn->clear(); ui->leGPRS_Apn_2->clear(); ui->leGPRS_Apn_3->clear(); break;
    case COMMAND_READ_STATE: ui->pteState->clear(); break;
//    case COMMAND_TASK_INFO: ui->pteRunningProc->clear(); break;
    case COMMAND_READ_IFCONFIG: ui->pteIfconfig->clear(); break;
//    case COMMAND_SYSTEM_SETTINGS: ui->pteSysInfo->clear(); break;
    case COMMAND_READ_APP_LOG:{ ui->pteAppLog->clear();  break;}
    case COMMAND_READ_ZBR_LOG: {ui->pteZbyrLog->clear(); break;}
    case COMMAND_READ_POLL_SCHEDULE:{
        ui->cbProfileEnableDay->setChecked(false);
        ui->cbProfileEnableMonth->setChecked(false);
        ui->cbProfileEnableNow->setChecked(false);
        ui->cbProfileEnablePower->setChecked(false);
        ui->cbProfileEnableVoltage->setChecked(false);
        ui->cbProfileEnableMeterLog->setChecked(false);

        ui->cbProfileNow->setCurrentIndex(-1);
        ui->cbProfilePower->setCurrentIndex(-1);
        ui->cbProfileVoltage->setCurrentIndex(-1);


        break;}

    case COMMAND_READ_METER_LIST_FRAMED:{
        modelAddMeter->clear();
        ui->leAddMeterFilter->clear();
        hash.insert("i", -1);
        hash.insert("max_len", ui->sbReadWriteMeterLen->value());
        hash.insert("cmprss", ui->cbCmprssMeterList->isChecked());
        break;}

    case COMMAND_READ_POLL_STATISTIC:{
        modelPollStat->clear();
        proxy_modelPollStat->sort(-1, Qt::AscendingOrder);

        if(ui->cbSttstOfExchngCode->currentIndex() > 0)
            hash.insert("c", ui->cbSttstOfExchngCode->currentText());

        if(!ui->leSttsctOfExchngFilter->text().simplified().trimmed().isEmpty()){
            hash.insert("n", ui->leSttsctOfExchngFilter->text().simplified().trimmed());
            hash.insert("m", ui->cbSttstcOfExchngMode->currentIndex());
        }
        break;}

    case COMMAND_READ_DATABASE:{

        if(ui->cbEnblTblBuff->isChecked()){
            readCommand = COMMAND_READ_DATABASE_GET_TABLES;
            QTimer::singleShot(10, this, SLOT(on_pbRead_clicked()));
            return;
        }

        modelDbData->clear();
        proxy_modelDbData->sort(-1, Qt::AscendingOrder);// setFilterKeyColumn(0);


        ui->leMeterDataFIlter->clear();

        QString mess;
        quint8 code = modelProfile4DB->itemData(ui->lvMeterDataProfile->currentIndex()).value(Qt::UserRole + 1).toUInt();

        hash.insert("code", code);
        hash.insert("cmprss", ui->cbCmprssDb->isChecked());
        hash.insert("max_len", ui->sbReadData->value());

        allowDate2utc = (code == POLL_CODE_READ_CURRENT || code == POLL_CODE_READ_VOLTAGE || code == POLL_CODE_READ_POWER);

        if(ui->gbMeterDataFromTo->isChecked()){

            hash.insert("FromDT", ui->dteMeterDataFrom->dateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss"));
            hash.insert("ToDT", ui->dteMeterDataTo->dateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss"));

            if(ui->dteMeterDataFrom->dateTime().secsTo(ui->dteMeterDataTo->dateTime()) < 1)
                mess.append(tr("The time interval is incorrect.<br>"));
        }else{
            QDateTime dtFrom = QDateTime::currentDateTimeUtc();
            QDateTime dtTo = dtFrom;
            dtTo.setTime(QTime::fromString(QString("%1:%2:00.000")
                                           .arg(dtTo.toString("hh"))
                                           .arg(dtTo.toString("mm")), "hh:mm:ss.zzz"));

            int coefficient = ui->cbMeterDataKftnt->currentIndex();
            int period = ui->sbMeterDataInterval->value();

            switch(coefficient){
            case 0:{  coefficient = 60; break;}
            case 1:{  coefficient = 60 * 60;  break;}
            case 2:{  coefficient = 60 * 60 * 24; break;}
            case 3:{
                dtFrom = dtTo.addMonths( (-1) * period);
                hash.insert("FromDT", dtFrom.toUTC().toString("yyyy-MM-dd hh:mm:ss"));
                break; }

            default:{
                mess.append(tr("The time interval is incorrect.<br>"));
                break; }
            }
            if(coefficient > 0 && period >= 0 && !hash.contains("FromDT")){
                dtFrom = dtTo.addSecs(period * coefficient * (-1));
                hash.insert("FromDT", dtFrom.toUTC().toString("yyyy-MM-dd hh:mm:ss"));

            }
            if(period < 0)
                mess.append(tr("Can't calculte date.<br>"));

        }

        if(code == POLL_CODE_READ_CURRENT || code == POLL_CODE_READ_END_DAY || code == POLL_CODE_READ_END_MONTH){
            QVariantList listTariff;
            for(int row = 0, rowMax = modelTarif4DB->rowCount(); row < rowMax; row++){
                if(modelTarif4DB->item(row)->checkState() == Qt::Checked)
                    listTariff.append(QString::number(row));
            }
            if(listTariff.isEmpty() )
                mess.append(tr("The tariff list is empty.<br>"));
            else
                hash.insert(QString("tarif"), QVariant(listTariff));
        }

        QStringList listEnrg;
        for(int row = 0, rowMax = proxy_modelPhVal4DB->rowCount(); row < rowMax; row++){
            int realRow = proxy_modelPhVal4DB->mapToSource( proxy_modelPhVal4DB->index(row,0) ).row();
            if(modelPhVal4DB->item(realRow)->checkState() == Qt::Checked)
                listEnrg.append(modelPhVal4DB->item(realRow)->data().toString());
        }
        if(listEnrg.isEmpty())
            mess.append(tr("The energy list is empty.<br>"));
        else
            hash.insert("enrg", strList2Var(listEnrg));


        hash.insert("msec", (ui->sbTimeOut->value() < 5000) ? ui->sbTimeOut->value() * 500 : 2500);
        hash.insert("lRwId", (qint64)0);
        hash.insert("lTbRwId", (qint64)0);


        if(listNiFromLine(ui->leMeterDataOnlyThisNI->text()).size() <= MAX_REQUEST_NI_IN_LINE) // ui->leMeterDataOnlyThisNI->styleSheet().isEmpty())
            hash.insert("ni", ui->leMeterDataOnlyThisNI->text().simplified().trimmed());
        else
            mess.append(tr("The NI filter: incorrect data.<br>"));

        if(mess.isEmpty()){
            lastTableList.clear();
            doneTables = 0;
            totalTables = 5000;
            this->hashMemoWrite.insert(COMMAND_READ_DATABASE, hash);
            hash.insert("gcl", true);
        }else{
            showMess(mess);
            return;
        }
        break;}

    case COMMAND_READ_DATABASE_GET_TABLES:{

        if(!ui->cbEnblTblBuff->isChecked()){
            readCommand = COMMAND_READ_DATABASE;
            QTimer::singleShot(10, this, SLOT(on_pbRead_clicked()));
            return;
        }


        modelDbData->clear();
        proxy_modelDbData->sort(-1, Qt::AscendingOrder);// setFilterKeyColumn(0);

        ui->leMeterDataFIlter->clear();

        QString mess;
        quint8 code = modelProfile4DB->itemData(ui->lvMeterDataProfile->currentIndex()).value(Qt::UserRole + 1).toUInt();

        hash.insert("code", code);
        hash.insert("len", ui->sbReadData->value());

        allowDate2utc = (code == POLL_CODE_READ_CURRENT || code == POLL_CODE_READ_VOLTAGE || code == POLL_CODE_READ_POWER);

        if(ui->gbMeterDataFromTo->isChecked()){

            hash.insert("FromDT", ui->dteMeterDataFrom->dateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss"));
            hash.insert("ToDT", ui->dteMeterDataTo->dateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss"));

            if(ui->dteMeterDataFrom->dateTime().secsTo(ui->dteMeterDataTo->dateTime()) < 1)
                mess.append(tr("The time interval is incorrect.<br>"));
        }else{
            QDateTime dtFrom = QDateTime::currentDateTimeUtc();
            QDateTime dtTo = dtFrom;
            dtTo.setTime(QTime::fromString(QString("%1:%2:00.000")
                                           .arg(dtTo.toString("hh"))
                                           .arg(dtTo.toString("mm")), "hh:mm:ss.zzz"));

            int coefficient = ui->cbMeterDataKftnt->currentIndex();
            int period = ui->sbMeterDataInterval->value();

            switch(coefficient){
            case 0:{  coefficient = 60; break;}
            case 1:{  coefficient = 60 * 60;  break;}
            case 2:{  coefficient = 60 * 60 * 24; break;}
            case 3:{
                dtFrom = dtTo.addMonths( (-1) * period);
                hash.insert("FromDT", dtFrom.toUTC().toString("yyyy-MM-dd hh:mm:ss"));
                break; }

            default:{
                mess.append(tr("The time interval is incorrect.<br>"));
                break; }
            }
            if(coefficient > 0 && period >= 0 && !hash.contains("FromDT")){
                dtFrom = dtTo.addSecs(period * coefficient * (-1));
                hash.insert("FromDT", dtFrom.toUTC().toString("yyyy-MM-dd hh:mm:ss"));

            }
            if(period < 0)
                mess.append(tr("Can't calculte date.<br>"));

        }

        if(code == POLL_CODE_READ_CURRENT || code == POLL_CODE_READ_END_DAY || code == POLL_CODE_READ_END_MONTH){
            QVariantList listTariff;
            for(int row = 0, rowMax = modelTarif4DB->rowCount(); row < rowMax; row++){
                if(modelTarif4DB->item(row)->checkState() == Qt::Checked)
                    listTariff.append(QString::number(row));
            }
            if(listTariff.isEmpty() )
                mess.append(tr("The tariff list is empty.<br>"));
            else
                hash.insert(QString("tarif"), QVariant(listTariff));
        }

        QStringList listEnrg;
        for(int row = 0, rowMax = proxy_modelPhVal4DB->rowCount(); row < rowMax; row++){
            int realRow = proxy_modelPhVal4DB->mapToSource( proxy_modelPhVal4DB->index(row,0) ).row();
            if(modelPhVal4DB->item(realRow)->checkState() == Qt::Checked)
                listEnrg.append(modelPhVal4DB->item(realRow)->data().toString());
        }
        if(listEnrg.isEmpty())
            mess.append(tr("The energy list is empty.<br>"));
        else
            hash.insert("enrg", strList2Var(listEnrg));


        hash.insert("msec", ui->sbTimeOut->value() * 500);
        hash.insert("lRwId", (qint64)0);

        if(listNiFromLine(ui->leMeterDataOnlyThisNI->text()).size() <= MAX_REQUEST_NI_IN_LINE) // ui->leMeterDataOnlyThisNI->styleSheet().isEmpty())
            hash.insert("ni", ui->leMeterDataOnlyThisNI->text().simplified().trimmed());
        else
            mess.append(tr("The NI filter: incorrect data.<br>"));

        if(mess.isEmpty()){
            lastTableList.clear();
            QVariantHash hashMemoWrite = hash;
            hashMemoWrite.remove("enrg");
            hashMemoWrite.remove("tarif");

            QVariantHash  hashMemoWrite2 = hash;
            this->hashMemoWrite.insert(COMMAND_READ_DATABASE_GET_TABLES, hashMemoWrite);

            hashMemoWrite2.insert("max_len", ui->sbReadData->value());
            hashMemoWrite2.insert("cmprss", ui->cbCmprssDb->isChecked());

            this->hashMemoWrite.insert(COMMAND_READ_DATABASE_GET_VAL, hashMemoWrite2);
        }else{
            showMess(mess);
            return;
        }
        break;}

    case COMMAND_READ_METER_LOGS:{
        readCommand = COMMAND_READ_METER_LOGS_GET_TABLES;
        QTimer::singleShot(10, this, SLOT(on_pbRead_clicked()));
        return;}

    case COMMAND_READ_METER_LOGS_GET_TABLES:{

        modelDbDataEv->clear();
        ui->leMeterDataFIlter_2->clear();

        QString mess;
        quint8 code = modelEvent4DB->itemData(ui->lvMeterDataProfile_2->currentIndex()).value(Qt::UserRole + 1).toUInt();

        hash.insert("code", code);
        hash.insert("max_len", ui->sbReadLenML->value());
        hash.insert("cmprss", ui->cbCmprssMeterLog->isChecked());
        if(ui->gbMeterDataFromTo_2->isChecked()){

            hash.insert("FromDT", ui->dteMeterDataFrom_2->dateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss"));
            hash.insert("ToDT", ui->dteMeterDataTo_2->dateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss"));

            if(ui->dteMeterDataFrom_2->dateTime().secsTo(ui->dteMeterDataTo_2->dateTime()) < 1)
                mess.append(tr("The time interval is incorrect.<br>"));
        }else{
            QDateTime dtTo = QDateTime::currentDateTime();
//            hash.insert("ToDT", dtTo);
            QDateTime dtFrom = dtTo;
            dtTo.setTime(QTime::fromString(QString("%1:%2:00.000")
                                           .arg(dtTo.toString("hh"))
                                           .arg(dtTo.toString("mm")), "hh:mm:ss.zzz"));

            int coefficient = ui->cbMeterDataKftnt_2->currentIndex();
            int period = ui->sbMeterDataInterval_2->value();

            switch(coefficient){
            case 0:{  coefficient = 60; break;}
            case 1:{  coefficient = 60 * 60;  break;}
            case 2:{  coefficient = 60 * 60 * 24; break;}
            case 3:{
                dtFrom = dtTo.addMonths( (-1) * period);
                hash.insert("FromDT", dtFrom.toUTC().toString("yyyy-MM-dd hh:mm:ss"));
                break; }

            default:{
                mess.append(tr("The time interval is incorrect.<br>"));
                break; }
            }
            if(coefficient > 0 && period >= 0 && !hash.contains("FromDT")){
                dtFrom = dtTo.addSecs(period * coefficient * (-1));
                hash.insert("FromDT", dtFrom.toUTC().toString("yyyy-MM-dd hh:mm:ss"));

            }
            if(period < 0)
                mess.append(tr("Can't calculte date.<br>"));

        }


        if(listNiFromLine(ui->leMeterDataOnlyThisNI_2->text()).size() <= MAX_REQUEST_NI_IN_LINE) // ui->leMeterDataOnlyThisNI->styleSheet().isEmpty())
            hash.insert("ni", ui->leMeterDataOnlyThisNI_2->text().simplified().trimmed());
        else
            mess.append(tr("The NI filter: incorrect data.<br>"));

         hash.insert("lRwId", (qint64)0);

        ui->tvMeterDataPollData_2->resizeColumnsToContents();
        if(mess.isEmpty()){
            lastTableList.clear();
            if(!ui->cbEnblTableBuffML->isChecked()){
                readCommand = COMMAND_READ_METER_LOGS;
                hash.insert("lTbRwId", (qint64)0);
                hash.insert("msec", ui->sbTimeOut->value() * 500);
                doneTables = 0;
                totalTables = 5000;
                this->hashMemoWrite.insert(COMMAND_READ_METER_LOGS, hash);
                hash.insert("gcl", true);
            }else{
                readCommand = COMMAND_READ_METER_LOGS_GET_TABLES;
                this->hashMemoWrite.insert(COMMAND_READ_METER_LOGS_GET_TABLES, hash);
                this->hashMemoWrite.insert(COMMAND_READ_METER_LOGS_GET_VAL, hash);
            }
        }else{
            showMess(mess);
            return;
        }
        break;}

    case COMMAND_READ_TABLE_HASH_SUMM:{
        //????
        ui->pteHashSumm->clear();
        QString mess;

        hash.insert("cmprss", ui->cbCmprssHsh->isChecked());
        hash.insert("hsh", ui->cbHashSumm_2->currentText());

        if(ui->tbwHashSumm->currentIndex() == 0){
            quint8 code = modelProfile4Hash->itemData(ui->lvMeterDataProfile_3->currentIndex()).value(Qt::UserRole + 1).toUInt();

            hash.insert("code", code);

            hash.insert("lRwId", 0);

            allowDate2utc = (code == POLL_CODE_READ_CURRENT || code == POLL_CODE_READ_VOLTAGE || code == POLL_CODE_READ_POWER);

            if(ui->gbMeterDataFromTo->isChecked()){

                hash.insert("FromDT", ui->dteMeterDataFrom_3->dateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss"));
                hash.insert("ToDT", ui->dteMeterDataTo_3->dateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss"));

                if(ui->dteMeterDataFrom->dateTime().secsTo(ui->dteMeterDataTo->dateTime()) < 1)
                    mess.append(tr("The time interval is incorrect.<br>"));
            }else{
                QDateTime dtFrom = QDateTime::currentDateTimeUtc();
                QDateTime dtTo = dtFrom;
                dtTo.setTime(QTime::fromString(QString("%1:%2:00.000")
                                               .arg(dtTo.toString("hh"))
                                               .arg(dtTo.toString("mm")), "hh:mm:ss.zzz"));

                int coefficient = ui->cbMeterDataKftnt_3->currentIndex();
                int period = ui->sbMeterDataInterval_3->value();

                switch(coefficient){
                case 0:{  coefficient = 60; break;}
                case 1:{  coefficient = 60 * 60;  break;}
                case 2:{  coefficient = 60 * 60 * 24; break;}
                case 3:{
                    dtFrom = dtTo.addMonths( (-1) * period);
                    hash.insert("FromDT", dtFrom.toUTC().toString("yyyy-MM-dd hh:mm:ss"));
                    break; }

                default:{
                    mess.append(tr("The time interval is incorrect.<br>"));
                    break; }
                }
                if(coefficient > 0 && period >= 0 && !hash.contains("FromDT")){
                    dtFrom = dtTo.addSecs(period * coefficient * (-1));
                    hash.insert("FromDT", dtFrom.toUTC().toString("yyyy-MM-dd hh:mm:ss"));

                }
                if(period < 0)
                    mess.append(tr("Can't calculte date.<br>"));
            }
        }else{
            QString str = ui->pteHashSumm_2->toPlainText().replace("\r","\n");
            QStringList list = str.split("\n", QString::SkipEmptyParts);
            if(list.isEmpty()){
                mess = tr("Table list is empty");
            }else{
                hash.insert("lt", list);
            }
        }
        hash.insert("msec", (ui->sbTimeOut->value() < 5500) ? ui->sbTimeOut->value() * 500 : 5500);

        if(!mess.isEmpty()){
            showMess(mess);
            return;
        }

        this->hashMemoWrite.insert(COMMAND_READ_TABLE_HASH_SUMM, hash);

        break;}

    default:
        break;
    }

    mWrite2RemoteDev(readCommand, hash);
    emit showWaitMess(ui->sbTimeOut->value());
}
//################################################################################################
void MainWindow::on_pbWrite_clicked()
{
    QVariantHash hash;
    switch(writeCommand){
    case COMMAND_WRITE_ABOUT_OBJECT:{
        hash.insert("c", ui->leCoordinates->text().simplified().trimmed());
        hash.insert("m", ui->pteAboutObjectMemo->toPlainText().trimmed());
        hash.insert("l", ui->leLineMemo->text().trimmed().left(1000));
        break;}
    case COMMAND_WRITE_GPRS_SETT: {

        if(ui->leGPRS_Apn->text().simplified().trimmed().isEmpty()){
            QTime time;
            time.start();

            while(time.elapsed() < 3000){
                if(QMessageBox::question(this , windowTitle(), tr("APN is empty. This will disable the GPRS modem!. Continue?"), QMessageBox::Yes|QMessageBox::No,QMessageBox::No) != QMessageBox::Yes)
                    return;
            }

        }

        hash.insert("apn", ui->leGPRS_Apn->text().simplified().trimmed());
        hash.insert("userName", ui->leGPRS_Apn_2->text().simplified().trimmed());
        hash.insert("password", ui->leGPRS_Apn_3->text().simplified().trimmed());

        break;}

    case COMMAND_WRITE_POLL_SCHEDULE:{

        QList<int> listInt;
        listInt << ui->sbProfileNow->value() << ui->sbProfileDay->value() << ui->sbProfileMonth->value() << ui->sbProfilePower->value() << ui->sbProfileVoltage->value() << ui->sbProfileMeterLog->value();
        QStringList list;
        for(int i = 0, iMax = listInt.size(); i < iMax; i++)
            list.append(QString::number(listInt.at(i)));

        if(list.removeDuplicates() > 0){
            showMess(tr("Priority: duplicate values.<br>Please fill the fields priorities unique values."));
            return;
        }

        //POLL_CODE_READ_CURRENT
        QVariantList l;
        l.append(ui->cbProfileEnableNow->isChecked());
        l.append(ui->sbProfileNow->value());
        l.append(ui->sbGlbnNow->value());
        l.append(intrvalValFromComboIndx(ui->cbProfileNow->currentIndex()));
        l.append( (ui->cbProfileNow->currentIndex() == 0) ? 1 : 2);
        hash.insert(QString::number(POLL_CODE_READ_CURRENT), l);


        //POLL_CODE_READ_END_DAY
        l.clear();
        l.append(ui->cbProfileEnableDay->isChecked());
        l.append(ui->sbProfileDay->value());
        l.append(ui->sbGlbnDay->value());
        l.append( ui->cbProfileEOD->currentIndex() + 1);//indx = 0, intrvl = indx + 1;
        l.append(3);//days

        hash.insert(QString::number(POLL_CODE_READ_END_DAY), l);

        //POLL_CODE_READ_END_MONTH
        l.clear();
        l.append(ui->cbProfileEnableMonth->isChecked());
        l.append(ui->sbProfileMonth->value());
        l.append(ui->sbGlbnMonth->value());
        l.append(ui->cbProfileEOM->currentIndex() + 1);//indx = 0, intrvl = indx + 1;
        l.append(4);//month

        hash.insert(QString::number(POLL_CODE_READ_END_MONTH), l);


        //POLL_CODE_READ_POWER
        l.clear();
        l.append(ui->cbProfileEnablePower->isChecked() );
        l.append(ui->sbProfilePower->value());
        l.append(ui->sbGlbnPwr->value());
        l.append(intrvalValFromComboIndx(ui->cbProfilePower->currentIndex()));
        l.append( (ui->cbProfilePower->currentIndex() == 0) ? 1 : 2);

        hash.insert(QString::number(POLL_CODE_READ_POWER), l);


        //POLL_CODE_READ_VOLTAGE
        l.clear();
        l.append( ui->cbProfileEnableVoltage->isChecked());
        l.append(ui->sbProfileVoltage->value());
        l.append("");
        l.append(intrvalValFromComboIndx(ui->cbProfileVoltage->currentIndex()));
        l.append((ui->cbProfileVoltage->currentIndex() == 0) ? 1 : 2);

        hash.insert(QString::number(POLL_CODE_READ_VOLTAGE), l);

        //POLL_CODE_METER_STATUS enable,prtt,glbn,intrvl,kftnt
        l.clear();
        l.append( ui->cbProfileEnableMeterLog->isChecked());
        l.append(ui->sbProfileMeterLog->value());
        l.append(ui->sbGlbnMeterLog->value());
        l.append(ui->cbProfileML->currentIndex() + 1);
        l.append(3);

        hash.insert(QString::number(POLL_CODE_METER_STATUS), l);

        l.clear();//1 - mon, 2 - tue

        if(ui->cbMon->isChecked())
            l.append(1);
        if( ui->cbTue->isChecked())
            l.append(2);
        if(ui->cbWed->isChecked())
            l.append(3);
        if(ui->cbThu->isChecked())
            l.append(4);
        if(ui->cbFri->isChecked())
            l.append(5);
        if(ui->cbSat->isChecked())
            l.append(6);
        if(ui->cbSun->isChecked())
            l.append(7);

        hash.insert("dow", l);

        if(l.isEmpty()){
            showMess(tr("Days of week is empty!"));
            return;
        }

        if(ui->teFrom->time() > ui->teTo->time()){
            showMess(tr("Time interval is not valid!"));
            return;
        }

        hash.insert("tiFrom", ui->teFrom->time().toString("hh:mm"));
        hash.insert("tiTo", ui->teTo->time().toString("hh:mm") );
        break;}

    case COMMAND_WRITE_METER_LIST_FRAMED:{

        int rowCount = modelAddMeter->rowCount();
        if(rowCount < 1){
            if(QMessageBox::question(this, windowTitle(), tr("You really want to clear the list of electric meters?"), QMessageBox::Yes|QMessageBox::No,QMessageBox::No) == QMessageBox::No)
                return;
        }

        hash.insert("t", rowCount);

        QVariantList lVar;
        QStringList k = QString("model,NI,SN,memo,passwd,on,politic,trff").split(',');//,vrsn

        int row = 0;
         int maxSize = ui->sbReadWriteMeterLen->value();
         if(maxSize < 300)
             maxSize = MAX_PACKET_LEN_RECOMENDATION;

        for(int j = 0, colMax = k.size(), bts = 50; row < rowCount; row++, j++){
            QStringList lOneMeter;
            for(int col = 0; col < colMax; col++)
                lOneMeter.append( modelAddMeter->item(row,col)->text());

            bts += lOneMeter.join("___").size();
            if(bts > maxSize){
                row--;
                break;
            }
            lVar.append(strList2Var(lOneMeter));
        }

        row++;
        hash.insert("m", lVar);
        hash.insert("i", (row >= rowCount) ? -1 : row);
        break;}

    case COMMAND_WRITE_POLL_SETT:{
        hash.insert("mr", ui->sbMetrRetry->value());
        hash.insert("pw", ui->sbWati4Poll->value());
        hash.insert("ha", ui->cbHardAddrsn->isChecked());

        hash.insert("w4e", ui->cbW4E->isChecked());
        hash.insert("w4eRb", ui->sbW4ERtrBf->value());
        hash.insert("w4eRa", ui->sbW4ErA->value());

        hash.insert("tc", ui->cbTimeCorrection->isChecked());
        hash.insert("td", ui->sbMaxSecDiff->value());

        hash.insert("pp", ui->cbPlgAndPl->isChecked());

        break;}

    }

   mWrite2RemoteDev(writeCommand, hash);

}
//################################################################################################
void MainWindow::mWrite2RemoteDev(quint16 command, QVariantHash hash)
{
    if(connectionDown())
        return;

    if(command < 1){
        ui->pbWrite->setVisible(false);
        return;
    }

    emit data2matilda(command, hash);

    emit showWaitMess(ui->sbTimeOut->value());
}
//##########################################################################################

void MainWindow::on_pushButton_2_clicked()
{
    QString slahDoPapky;
    if(slahDoPapky.isEmpty()){
#if QT_VERSION >= 0x050000
        slahDoPapky = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/";
#else
        slahDoPapky = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation) + "/";
#endif
    }


    QString fileName = QFileDialog::getSaveFileName(this, tr("Statistic of exchange"), QString("%1/Statistic of exchange %2.csv").arg(slahDoPapky).arg(QDateTime::currentDateTime().toString("yyyy MM dd  hh mm ss")),
                                 tr("CSV Files(*.csv"));

    if(!fileName.isEmpty()){
//        modelPollStat->horizontalHeaderItem()
        QFile file(fileName);
        if(file.open(QFile::WriteOnly|QFile::Text)){
            QTextStream stream(&file);
            stream << tr("Statistic of exchange;") << endl << QDateTime::currentDateTime().toString() << endl << endl;
            for(int col = 0, colMax = modelPollStat->columnCount(); col < colMax; col++){
                stream << modelPollStat->horizontalHeaderItem(col)->text() << ";";
            }
             stream << endl;
            for(int row = 0, rowMax = modelPollStat->rowCount(), colMax = modelPollStat->columnCount(); row < rowMax; row++){
                for(int col = 0; col < colMax; col++){
                    stream << modelPollStat->item(row,col)->text() << ";";
                }
                stream << endl;
            }
            file.close();
        }

    }

}
//##########################################################################################
void MainWindow::on_pushButton_clicked()
{
    proxy_modelPollStat->sort(-1, Qt::AscendingOrder);// setFilterKeyColumn(0);
}
//##########################################################################################

void MainWindow::on_pbDrestart_clicked()
{
    mWrite2RemoteDev(COMMAND_WRITE_DAEMON_RESTART);
}
//##########################################################################################

void MainWindow::on_pbReboot_clicked()
{
    mWrite2RemoteDev(COMMAND_WRITE_REBOOT);

}
//##########################################################################################
void MainWindow::on_pbResetGsm_clicked()
{
    QVariantHash h;
    h.insert("i", 2);
    mWrite2RemoteDev(COMMAND_WRITE_RESET_MODEM, h);

}
//##########################################################################################
void MainWindow::on_pbResetEmbee_clicked()
{
    QVariantHash h;
    h.insert("i", 1);
    mWrite2RemoteDev(COMMAND_WRITE_RESET_MODEM, h);
}
//##########################################################################################


void MainWindow::on_pbAddMeter_clicked()
{
    QString mess("");
    QStringList list;
    if(ui->cbAddMeterModel->currentIndex() < 0)
        mess.append(tr("Model: invalid index.<br>"));
    else
        list.append(ui->cbAddMeterModel->currentText());



    if(ui->leAddMeterNI->text().simplified().trimmed().isEmpty())
        mess.append(tr("NI is empty.<br>"));
    else
        list.append(ui->leAddMeterNI->text().simplified().trimmed());

     list.append(ui->leAddMeterSN->text().simplified().trimmed());


    list.append(ui->leAddMeterMemo->text().simplified().trimmed());
    list.append(ui->leAddMeterPasswd->text());
    list.append( ui->cbAddMeterOnOffPoll->isChecked() ? "+" : "-" );

    if(ui->cbAddMeterPhysicalVal->currentIndex() < 0)
        mess.append(tr("Physical values: invalid index.<br>"));
    else
        list.append(ui->cbAddMeterPhysicalVal->currentData().toString());
    list.append(QString::number( ui->sbAddMeterTariff->value()));
    list.append(""); //meter version

    if(modelAddMeter->rowCount() > MAX_METER_COUNT)
        mess.append(tr("MAX_METER_COUNT=%1.<br>").arg(MAX_METER_COUNT));

    if(mess.isEmpty()){
        int replaceIndx = -1;
        QString meterSN = ui->leAddMeterSN->text().simplified().trimmed();
        QString meterNI = ui->leAddMeterNI->text().simplified().trimmed();

        for(int row = 0, rowMax = modelAddMeter->rowCount(); row < rowMax; row++){
            if(!meterSN.isEmpty() && modelAddMeter->item(row, 1)->text() == meterSN){
                if(QMessageBox::question(this, windowTitle(), tr("The meter with SN: %1, already exists. Replace?")
                                         .arg(meterSN)
                                         , QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes){
                    replaceIndx = row;
                }else{
                    return;
                }
                break;
            }

            if(modelAddMeter->item(row, 2)->text() == meterNI){
                if(QMessageBox::question(this, windowTitle(), tr("The meter with NI: %1, already exists. Replace?")
                                         .arg(meterNI)
                                         , QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes){
                    replaceIndx = row;
                }
                break;
            }
        }
        QList<QStandardItem*> li;
        for(int i = 0, iMax = list.size(); i < iMax; i++)
            li.append(new QStandardItem(list.at(i)));

        if(replaceIndx < 0)
            modelAddMeter->appendRow(li);
        else{
            modelAddMeter->removeRow(replaceIndx);
            modelAddMeter->insertRow(replaceIndx, li);
        }

    }else{
        showMess(mess);
    }
}
//##########################################################################################
void MainWindow::on_toolButton_5_clicked()
{
    QStringList l,n;
    for(int i = 0, iMax = ui->cbAddMeterPhysicalVal->count(); i < iMax; i++){
        l.append(ui->cbAddMeterPhysicalVal->itemText(i));
        n.append(ui->cbAddMeterPhysicalVal->itemData(i).toString());
    }

    EnergyDialog *d = new EnergyDialog(l,n, this);
    connect(d, SIGNAL(onPhysValChanged(QStringList,QStringList)), this, SLOT(onPhysValChanged(QStringList,QStringList)) );
    d->exec();
    d->deleteLater();
}
//##########################################################################################

void MainWindow::on_gbMeterDataFromTo_2_clicked(bool checked)
{
    ui->gbMeterDataYangerThan_2->setChecked(!checked);
}
//##########################################################################################
void MainWindow::on_gbMeterDataYangerThan_2_clicked(bool checked)
{
    ui->gbMeterDataFromTo_2->setChecked(!checked);
}
//##########################################################################################

void MainWindow::on_gbMeterDataFromTo_clicked(bool checked)
{
    ui->gbMeterDataYangerThan->setChecked(!checked);
}
//##########################################################################################
void MainWindow::on_gbMeterDataYangerThan_clicked(bool checked)
{
    ui->gbMeterDataFromTo->setChecked(!checked);

}
//##########################################################################################

void MainWindow::on_toolButton_9_clicked()
{
    ui->dteMeterDataFrom->setDateTime(QDateTime::currentDateTime().addDays(-5));
    ui->dteMeterDataTo->setDateTime(QDateTime::currentDateTime());
}
//##########################################################################################
void MainWindow::on_toolButton_10_clicked()
{
    ui->dteMeterDataFrom_2->setDateTime(QDateTime::currentDateTime().addDays(-5));
    ui->dteMeterDataTo_2->setDateTime(QDateTime::currentDateTime());
}
//##########################################################################################

void MainWindow::on_gbMeterDataFromTo_3_clicked(bool checked)
{
    ui->gbMeterDataYangerThan_3->setChecked(!checked);
}
//##########################################################################################
void MainWindow::on_gbMeterDataYangerThan_3_clicked(bool checked)
{
    ui->gbMeterDataFromTo_3->setChecked(!checked);
}
//##########################################################################################

void MainWindow::on_pbReadHashSumm_clicked()
{
    QVariantHash hash;
    hash.insert("hsh", ui->cbHashSumm->currentText());
    mWrite2RemoteDev(COMMAND_READ_METER_LIST_HASH_SUMM, hash);
}
//##########################################################################################

void MainWindow::on_tvAddMeterTable_customContextMenuRequested(const QPoint &pos)
{
    bool enbl = (proxy_modelAddMeter->rowCount() > 0);

    QMenu *menu = new QMenu(ui->tvAddMeterTable);
    QAction *actEd = new QAction(tr("Edit") , menu);
    QAction *actSpon = new QAction(tr("Selected: poll on") , menu);
    QAction *actSpoff = new QAction(tr("Selected: poll off") , menu);


    QAction *actInsrt = new QAction(tr("Selected: insert to list") , menu);
    QAction *actInsrtOff = new QAction(tr("Selected: insert to list with Poll off") , menu);
    QAction *actInsrtOn = new QAction(tr("Selected: insert to list with Poll on") , menu);
    QAction *actInsrtDel = new QAction(tr("Selected: delete from list") , menu);

    QAction *actRs = new QAction(tr("Reset sorting") , menu);
    QAction *actDs = new QAction(tr("Delete selected") , menu);

    actEd->setEnabled(enbl);
    actSpon->setEnabled(enbl);
    actSpoff->setEnabled(enbl);
    actDs->setEnabled(enbl);

    actInsrt->setEnabled(enbl && ui->pbWrite->isEnabled());
    actInsrtOff->setEnabled(enbl && ui->pbWrite->isEnabled());
    actInsrtOn->setEnabled(enbl && ui->pbWrite->isEnabled());
    actInsrtDel->setEnabled(enbl && ui->pbWrite->isEnabled());



    connect(actInsrt, SIGNAL(triggered(bool)), SLOT(onActWriteSeleted()) );
    connect(actInsrtOff, SIGNAL(triggered(bool)), SLOT(onActWriteSeletedOff()) );
    connect(actInsrtOn, SIGNAL(triggered(bool)), SLOT(onActWriteSeletedOn()) );
    connect(actInsrtDel, SIGNAL(triggered(bool)), SLOT(onActWriteSeletedDeletePart()) );



    connect(actEd, SIGNAL(triggered(bool)), SLOT(onActEditAddMeterTable()) );
    connect(actSpon, SIGNAL(triggered(bool)), SLOT(onActSelectedPollOnAddMeterTable()) );

    connect(actSpoff, SIGNAL(triggered(bool)), SLOT(onActSelectedPollOffAddMeterTable()) );
    connect(actRs, SIGNAL(triggered(bool)), SLOT(onActResetSortingAddMeterTable()) );
    connect(actDs, SIGNAL(triggered(bool)), SLOT(onActDeleteSelectedAddMeterTable()) );

    menu->addAction(actEd);
    menu->addSeparator();

    menu->addAction(actSpon);
    menu->addAction(actSpoff);

    menu->addSeparator();

    menu->addAction(actInsrt);
    menu->addAction(actInsrtOn);
    menu->addAction(actInsrtOff);
    menu->addAction(actInsrtDel);

    menu->addSeparator();

//    menu->addAction(actRs);


    menu->addAction(actRs);
    menu->addSeparator();
    menu->addAction(actDs);

    menu->exec(ui->tvAddMeterTable->mapToGlobal(pos));
    menu->deleteLater();
}
//##########################################################################################
void MainWindow::onActEditAddMeterTable()
{
    on_tvAddMeterTable_doubleClicked(ui->tvAddMeterTable->currentIndex());
}
//##########################################################################################
void MainWindow::onActWriteSeleted()
{
    QList<int> lRows;
    foreach (QModelIndex i, ui->tvAddMeterTable->selectionModel()->selectedRows()) {
        lRows.append(proxy_modelAddMeter->mapToSource(i).row());
    }

    if(lRows.size() > 50){
        showMess(tr("Max size is 50. Meter count is %1").arg(lRows.size()));
    }else{


        QVariantList l;
        QStringList k = QString("model,NI,SN,memo,passwd,on,politic,trff").split(',');//,vrsn
        int colMax = k.size();
        while(!lRows.isEmpty()){
            int row = lRows.takeFirst();

            QStringList h;
            for(int col = 0; col < colMax; col++)
                h.append(modelAddMeter->item(row,col)->text());
            l.append(strList2Var(h));
        }

        InsertMeterDialog *dialog = new InsertMeterDialog(l, this);
        connect(dialog, SIGNAL(data2matilda(quint16,QVariantHash)), this, SLOT(mWrite2RemoteDev(quint16,QVariantHash))) ;
        dialog->exec();
        dialog->deleteLater();
    }

}
//##########################################################################################
void MainWindow::onActWriteSeletedOn()
{
    QList<int> lRows;
    foreach (QModelIndex i, ui->tvAddMeterTable->selectionModel()->selectedRows())
        lRows.append(proxy_modelAddMeter->mapToSource(i).row());

    int maxSize = MAX_METER_COUNT;

    if(lRows.size() > maxSize){
        showMess(tr("Max size is %1. Meter count is %2").arg(maxSize).arg(lRows.size()));
    }else{


        QStringList l;
//        QStringList k = QString("model,SN,NI,memo,passwd,on,politic,trff,vrsn").split(',');

        while(!lRows.isEmpty())
            l.append(modelAddMeter->item(lRows.takeFirst(),1)->text());

        if(l.isEmpty())
            return;

        QVariantHash h;
        h.insert("s", l);
        h.insert("m",2);//NI
        mWrite2RemoteDev(COMMAND_WRITE_METER_LIST_POLL_ON, h);
    }
}
//##########################################################################################
void MainWindow::onActWriteSeletedOff()
{
    QList<int> lRows;
    foreach (QModelIndex i, ui->tvAddMeterTable->selectionModel()->selectedRows())
        lRows.append(proxy_modelAddMeter->mapToSource(i).row());

    int maxSize = MAX_METER_COUNT;

    if(lRows.size() > maxSize){
        showMess(tr("Max size is %1. Meter count is %2").arg(maxSize).arg(lRows.size()));
    }else{


        QStringList l;
//        QStringList k = QString("model,SN,NI,memo,passwd,on,politic,trff,vrsn").split(',');

        while(!lRows.isEmpty())
            l.append(modelAddMeter->item(lRows.takeFirst(),1)->text());

        if(l.isEmpty())
            return;

        QVariantHash h;
        h.insert("s", l);
        h.insert("m", 2);//NI
        mWrite2RemoteDev(COMMAND_WRITE_METER_LIST_POLL_OFF, h);
    }
}
//##########################################################################################
void MainWindow::onActWriteSeletedDeletePart()
{
    QList<int> lRows;
    foreach (QModelIndex i, ui->tvAddMeterTable->selectionModel()->selectedRows())
        lRows.append(proxy_modelAddMeter->mapToSource(i).row());

    int maxSize = MAX_METER_COUNT;

    if(lRows.size() > maxSize){
        showMess(tr("Max size is %1. Meter count is %2").arg(maxSize).arg(lRows.size()));
    }else{

        if(QMessageBox::question(this, "", tr("R U really want to delete this meter from list?<br>This will be delete this meter directly from remote device.")) != QMessageBox::Yes)
                return;

        QStringList l;
//        QStringList k = QString("model,SN,NI,memo,passwd,on,politic,trff,vrsn").split(',');

        while(!lRows.isEmpty())
            l.append(modelAddMeter->item(lRows.takeFirst(),1)->text());

        if(l.isEmpty())
            return;

        QVariantHash h;
        h.insert("s", l);
        h.insert("m",2);
       mWrite2RemoteDev(COMMAND_WRITE_METER_LIST_DEL_NI, h);
    }
}

//##########################################################################################
void MainWindow::onActSelectedPollOnAddMeterTable()
{
    foreach (QModelIndex i, ui->tvAddMeterTable->selectionModel()->selectedRows()) {
        int row = proxy_modelAddMeter->mapToSource(i).row();
        modelAddMeter->item(row,5)->setText("+");
    }
}
//##########################################################################################
void MainWindow::onActSelectedPollOffAddMeterTable()
{
    foreach (QModelIndex i, ui->tvAddMeterTable->selectionModel()->selectedRows()) {
        int row = proxy_modelAddMeter->mapToSource(i).row();
        modelAddMeter->item(row,5)->setText("-");
    }
}
//##########################################################################################
void MainWindow::onActResetSortingAddMeterTable()
{
    proxy_modelAddMeter->sort(-1, Qt::AscendingOrder);// setFilterKeyColumn(0);

}
//##########################################################################################
void MainWindow::onActDeleteSelectedAddMeterTable()
{
    QList<int> l;
    foreach (QModelIndex i, ui->tvAddMeterTable->selectionModel()->selectedRows())
        l.append(proxy_modelAddMeter->mapToSource(i).row());

    qSort(l);

    while(!l.isEmpty())
        modelAddMeter->removeRow(l.takeLast());
}
//##########################################################################################



void MainWindow::on_tvAddMeterTable_doubleClicked(const QModelIndex &index)
{
    int row = proxy_modelAddMeter->mapToSource(index).row();
    int indx = ui->cbAddMeterModel->findText(modelAddMeter->item(row,0)->text());
    if( indx > 0)
        ui->cbAddMeterModel->setCurrentIndex(indx);
    else
        ui->cbAddMeterModel->setCurrentIndex(0);

    ui->leAddMeterSN->setText(modelAddMeter->item(row, 1)->text());
    ui->leAddMeterNI->setText(modelAddMeter->item(row, 2)->text());
    ui->leAddMeterMemo->setText(modelAddMeter->item(row,3)->text());

    ui->leAddMeterPasswd->setText(modelAddMeter->item(row,4)->text());
    ui->cbAddMeterOnOffPoll->setChecked(modelAddMeter->item(row,5)->text() == "+");


    ui->sbAddMeterTariff->setValue(modelAddMeter->item(row,7)->text().toInt());

    indx = ui->cbAddMeterPhysicalVal->findData(modelAddMeter->item(row,6)->text().simplified().trimmed());
    if(indx >= 0)
        ui->cbAddMeterPhysicalVal->setCurrentIndex(indx);
    else{
        if(!modelAddMeter->item(row,6)->text().simplified().trimmed().isEmpty()){
            int indx = ui->cbAddMeterPhysicalVal->count();
            ui->cbAddMeterPhysicalVal->addItem(modelAddMeter->item(row,6)->text().simplified().trimmed(), modelAddMeter->item(row,6)->text().simplified().trimmed() );
            ui->cbAddMeterPhysicalVal->setItemData(indx,modelAddMeter->item(row,6)->text().simplified().trimmed() , Qt::ToolTipRole );
            ui->cbAddMeterPhysicalVal->setCurrentIndex(indx);
        }
    }


}
//##########################################################################################
void MainWindow::checkLineAddMeterNIandPasswd()
{
    int indx = ui->cbAddMeterModel->currentIndex();
    QString niReg, passwdReg;

    if(indx >= 0){
        QString regStr = ui->cbAddMeterModel->itemData(indx).toString();
        if(regStr.split("^").length() == regStr.split("$").length() && regStr.split("$").length() == 3){
            niReg = regStr.split("$").first() + "$";
            passwdReg = "^" + regStr.split("^").last();
        }else{
            niReg = passwdReg = regStr;
        }
        qDebug() << "neRet " << niReg << passwdReg;

    }

    if(niReg.isEmpty())
        niReg = "^(.){32}$";//Any value

    if(passwdReg.isEmpty())
        niReg = "^(.){32}$";


    QRegExp regNI(niReg);
    QRegExpValidator *validatorNI = new QRegExpValidator(regNI, this);

    QString str = ui->leAddMeterNI->text();
    int iii = -1;

    while(validatorNI->validate(str, iii) == QRegExpValidator::Invalid){
        qDebug() << str << iii;
        str.chop(1);
    }

    ui->leAddMeterNI->setValidator(validatorNI);
    ui->leAddMeterNI->setText(str);


    QRegExp regPasswd(passwdReg);
    QRegExpValidator *validatorPasswd = new QRegExpValidator(regPasswd, this);

    str = ui->leAddMeterPasswd->text();
    while(validatorPasswd->validate(str, iii) == QRegExpValidator::Invalid){
        qDebug() << str << iii;
        str.chop(1);
    }

    ui->leAddMeterPasswd->setValidator(validatorPasswd);
    ui->leAddMeterPasswd->setText(str);
}
//##########################################################################################
