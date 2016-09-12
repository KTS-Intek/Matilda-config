 #ifndef SCANIPDIALOG_H
#define SCANIPDIALOG_H

#include <QDialog>
#include <QtCore>
#include <QStandardItemModel>
#include "mysortfilterproxymodel.h"


namespace Ui {
class ScanIpDialog;
}

class ScanIpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScanIpDialog(int modeIp, QWidget *parent = 0);
    ~ScanIpDialog();

signals:
    void setThisObjName(QString);
    void setThisIp(QString);
    void setThisPort(int);
    void showMess(QString);

    void setThisMac(QString);
    void setThisMode(int);
    void setThisObjName_2(QString);


    void onClosd();

    ///kts connect
    void startUdpScanNow(quint16);
    void stopUdpScanNow();
    void stopCustomSslSocketNow();

public slots:
    void findedDevice(QString name, QString ipPOrt, QString mac);

    void startScanNow();

    void onScanStarted();
    void onScanStopped();
    void addProg();

private slots:

    void on_buttonBox_accepted();



    void on_lvDevHistory_doubleClicked(const QModelIndex &index);

private:
    Ui::ScanIpDialog *ui;
    void getThisRow(int row);

     QStandardItemModel *modelDevices;
     MySortFilterProxyModel *proxy_modelDevices;
     quint16 scanSessionID;


};

#endif // SCANIPDIALOG_H

