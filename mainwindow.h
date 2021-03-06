#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QStandardItemModel>
#include "mysortfilterproxymodel.h"

#include "treemodel.h"
#include "lastdevinfo.h"

#include "ledlamplistwidget.h"
#include "rescuescheduleswdgt.h"
#include "fireflyschedulestatuswdgt.h"
#include "fireflygroupschedulewdgt.h"
#include "instantpower4lampswdgt.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QFont &font4log, QWidget *parent = 0);
    ~MainWindow();
    LastDevInfo *lDevInfo;
    int defFontSize;

    void loadMainSett();


signals:
//  void conn2thisDev(int ,QString, QString , QString , quint16 , int , bool, bool);
    void conn2thisDev(int, QString, QString, QString, QString, quint16, int, bool, bool, bool, QString, bool,bool);

    void data2matilda(quint16 , QJsonObject );
    void closeConnection();


    void setSttsNewPixmap(QPixmap);
    void setSttsNewTxt(QString);
    void setNewConnStat(QString);


    void conn2thisDevDbg(QString, quint16, int);
    void closeConnectionDbg();

    void uploadProgress(int,QString);
    void hideWaitMess();
    void showWaitMess(int);

    void kickThrd();



    void stopDaServer();
    void startDaServer(qint8,quint16); //mode , port

    void data2coordiantor(QByteArray);
    void dataFromCoordinator(QByteArray);

    void onDaServerStateS(bool);

    void setEmptyHsh(bool empty);

    //on dev type changed
    void clearLastDevTreeModel();


private slots:
    void initializeMatilda();


    void on_pbLogIn_clicked();

    void on_pbLogOut_clicked();

    void manualResizePollStat();

    void unCheckEvrDay();

    void onLvMeterDataProfile_activated(const QModelIndex &index);

    void onCheckDbNIfieldIsCorrect();


    void on_actionDevice_triggered();

    void on_actionHome_triggered();


    void on_pbRead_clicked();

    void on_pbWrite_clicked();

    void mWrite2RemoteDev(quint16 command, QJsonObject jobj = QJsonObject());

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pbDrestart_clicked();

    void on_pbReboot_clicked();

    void on_pbResetGsm_clicked();

    void on_pbResetEmbee_clicked();

    void on_pbAddMeter_clicked();

    void on_toolButton_5_clicked();

    void on_gbMeterDataFromTo_2_clicked(bool checked);

    void on_gbMeterDataYangerThan_2_clicked(bool checked);

    void on_gbMeterDataFromTo_clicked(bool checked);

    void on_gbMeterDataYangerThan_clicked(bool checked);

    void on_toolButton_9_clicked();

    void on_toolButton_10_clicked();

    void on_gbMeterDataFromTo_3_clicked(bool checked);

    void on_gbMeterDataYangerThan_3_clicked(bool checked);

    void on_pbReadHashSumm_clicked();

    void on_tvAddMeterTable_customContextMenuRequested(const QPoint &pos);

    void onActEditAddMeterTable();

    void onActWriteSeleted();

    void onActWriteSeletedOn();

    void onActWriteSeletedOff();

    void onActWriteSeletedDeletePart();

    void onActSelectedPollOnAddMeterTable();

    void onActSelectedPollOffAddMeterTable();

    void onActResetSortingAddMeterTable();

    void onActDeleteSelectedAddMeterTable();

    void on_tvAddMeterTable_doubleClicked(const QModelIndex &index);

    void checkLineAddMeterNIandPasswd();


    void on_pbDevCommands_clicked();

    void on_pbOpenCloseDA_clicked(bool checked);

    void on_tbAdd2SvahaService_2_clicked();

    void on_tbAddDayProfile_2_clicked();

    void on_tbAdd2SvahaService_clicked();

    void on_tbAddDayProfile_clicked();

    void on_toolButton_17_clicked();

    void on_toolButton_18_clicked();

    void on_toolButton_15_clicked();

    void on_toolButton_16_clicked();

    void on_tvSvahaService_2_clicked(const QModelIndex &index);

    void on_tvDayProfiles4svaha_2_clicked(const QModelIndex &index);

    void on_tvSvahaService_clicked(const QModelIndex &index);

    void on_tvDayProfiles4svaha_clicked(const QModelIndex &index);

    void on_toolButton_12_clicked();

    void on_pbReadUdp_clicked();

    void on_pbWriteUdp_clicked();

    void on_pbAddForward_clicked();

    void on_tvForward_clicked(const QModelIndex &index);

    void on_tbDelFromForwardTable_clicked();

    void on_pbWriteLocalTime_clicked();




    void on_toolButton_2_clicked();

    void on_toolButton_clicked();

    void on_toolButton_3_clicked();

    void on_actionLog_triggered();


    void on_toolButton_11_clicked();

    void on_toolButton_13_clicked();

    void on_toolButton_19_clicked();

    void on_toolButton_14_clicked();

    void on_pushButton_9_clicked();

    void on_pbLang_clicked();

    void on_actionOptions_triggered();

    void on_trDevOperation_clicked(const QModelIndex &index);

    void on_pbReadSn_clicked();

    void on_pbMeterDataShowHide_clicked(bool checked);

    void on_pbMeterDataShowHide_2_clicked(bool checked);

    void on_tbEvComment2txt_clicked(bool checked);

    void on_actionHelp_triggered();

    void on_cbChop_toggled(bool checked);

public slots:
    void onConnectedStateChanged(bool isConnected);
    void onConnectedStateChangedDbg(bool isConnected);

    void data2gui(quint16 command, QJsonObject jobj);
    void onErrorWrite();
    void showMessage(QString message);
    void showMessSmpl(QString message);

    void authrizeAccess(int accessLevel); //0 - closed, 1 - root, 2 - operator, 3 - guest

    void noAnswerFromDev();

    void changeCounters(qint64 addThisVal, qint64 addThisValNotCompr, bool rec);

    void onPhysValChanged(QStringList n,QStringList l);

    void onDaServiceState(bool isListen);
    void onYouCanSelectDevice(QStringList listDev);

    void devTypeChanged(int devType, int version, QString sn);

    void onLangSelected(QString lang);

    void loadSettPageOptions();

    void setActiveProtocolVersion(int protocolVersion);

private:
    QList<int> getFilterList(const int &startIndx, const int &count) const;
    int comboIndxFromKftntAndIntrvl(const qint32 &kftnt, const qint32 &intrvl) const;
    qint32 intrvalValFromComboIndx(const int &indx) const;
    QStringList listNiFromLine(const QString &aLine) const;

    QString emailUtcOffset(const int &offst);

    QVariantHash json2hash(const QByteArray &arrJson);


    QDateTime tableName2DateTime(const QString &tableName, const qint32 addDays = 0, const bool useLocal = false) const;
    QDateTime tableName2DateTime(const QString &tableName, const QString &dateFormat, const QString &timeFormat, const qint32 addDays = 0) const;
    QDateTime tableVal2DateTime(const QString &text, const QString &mask, const bool useLocal = false) const;
    QDateTime dateTimeFromStr(const QString &str);

    QString humanByteView(QString str);

    QString humanByteView(const qint64 &val);
    bool connectionDown();

    QStringList varList2strList(const QVariantList &list);

    QVariantList strList2VarList(const QStringList &list);

    QVariant strList2Var(const QStringList &list);

    void createLanguageMenu();

    void loadLanguage(const QString& rLanguage);

     void saveMainSett();

     void savePageOptions();

     void onlvDevOperation_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;

    void createAddSmartLightingWidgets();

    QStandardItemModel *modelDevOptions;
    TreeModel *modelDevOptionsTree;
    QStandardItemModel *modelAddMeter;
    QStandardItemModel *modelProfile4DB;
    QStandardItemModel *modelTarif4DB;
    QStandardItemModel *modelPhVal4DB;
    QStandardItemModel *modelDbData;
    QStandardItemModel *modelTimeZone;
    QStandardItemModel *modelDbDataEv;
    QStandardItemModel *modelEvent4DB;
    QStandardItemModel *modelPollStat;
    QStandardItemModel *modelProfile4Hash;

    QStandardItemModel *modelSvahaList;
    QStandardItemModel *modelDayProfile4mac;

    QStandardItemModel *modelPeredavatorHost;
    QStandardItemModel *modelDayProfile4peredavator;
    QStandardItemModel *modelForward;


    MySortFilterProxyModel *proxy_modelForward ;
    MySortFilterProxyModel *proxy_modelDevOptions;
    MySortFilterProxyModel *proxy_modelAddMeter;
    MySortFilterProxyModel *proxy_modelPhVal4DB;
    MySortFilterProxyModel *proxy_modelDbData;
    MySortFilterProxyModel *proxy_modelTimeZone;
    MySortFilterProxyModel *proxy_modelDbDataEv;
    MySortFilterProxyModel *proxy_modelPollStat;

    QStringList lastTableList;
    int totalTables, doneTables, blockDone;
    QVariantHash lastSnMemoHash;
    QHash<QString, Qt::CheckState > hChecked;
    bool allowDate2utc;
    bool dateInUtc;


    quint16 readCommand;
    quint16 writeCommand;

    QString lastConnDevInfo;
    QHash<int, QVariantHash> hashMemoWrite;

    QString lastIpStr;
    int lastTimeOutMS;

    QList<QVariant> lRecoverLvDevHistory;

    qint64 byteSend, lastByteSend;
    qint64 byteReceiv, lastByteRec;
    qint64 byteSendNotComprssd, lastSendNC;
    qint64 byteReceivNotComprssd, lastRecNC;

    QTranslator translator;
    QString currLang;
    bool youAreRoot, youAreOper;
    int joingStts;


    LedLampListWidget *lcuwdgt;
    FireFlyGroupScheduleWdgt *lcuGroups;
    InstantPower4lampsWdgt *lcuTemporary;
    RescueSchedulesWdgt *lcuRescue;
    FireflyScheduleStatusWdgt *lcuStatus;


};

#endif // MAINWINDOW_H
