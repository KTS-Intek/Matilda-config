#ifndef SELECTDEVBYMACADDRDIALOG_H
#define SELECTDEVBYMACADDRDIALOG_H

#include <QDialog>
#include <QtCore>
#include <QStandardItemModel>
#include "mysortfilterproxymodel.h"


namespace Ui {
class SelectDevByMacAddrDialog;
}

class SelectDevByMacAddrDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectDevByMacAddrDialog(int hashIndx, bool allwC, QStringList listAddr, QString leObjectName_2, QString leLogin, QString lePasswd, QString leIp_2, int sbPort_2, int sbTimeOut_2, bool cbSave2KtsWallet,
                                       QWidget *parent = 0);

    ~SelectDevByMacAddrDialog();

signals:
    void conn2thisDev(int,QString,QString,QString,QString,quint16,int,bool,bool,bool,QString,bool);//bool useMac, QString macAddr, bool useMacAddr2conn

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_buttonBox_accepted();

private:
    void conn2this(const QModelIndex &index);
    Ui::SelectDevByMacAddrDialog *ui;

    QStandardItemModel *model;
    MySortFilterProxyModel *proxy_model;

     QString leObjectName_2;
     QString leLogin;
     QString lePasswd;
     QString leIp_2;
     int sbPort_2;
     int hashIndx;
     int sbTimeOut_2;

     bool allowComprss;
     bool cbSave2KtsWallet;
};

#endif // SELECTDEVBYMACADDRDIALOG_H
