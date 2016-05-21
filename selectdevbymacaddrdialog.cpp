#include "selectdevbymacaddrdialog.h"
#include "ui_selectdevbymacaddrdialog.h"
#include <QtCore>

SelectDevByMacAddrDialog::SelectDevByMacAddrDialog(int hashIndx, bool allwC, QStringList listAddr, QString leObjectName_2, QString leLogin, QString lePasswd, QString leIp_2, int sbPort_2, int sbTimeOut_2, bool cbSave2KtsWallet, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectDevByMacAddrDialog)
{
    ui->setupUi(this);

    model = new QStandardItemModel(0,2,this);
    proxy_model = new MySortFilterProxyModel(this);

    proxy_model->setSourceModel(model);
    proxy_model->setDynamicSortFilter(true);
//    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), proxy_model, SLOT(setNewFileterStr(QString)) );
    QList<int> filterMode;
    filterMode.append(0);
    filterMode.append(1);

    proxy_model->setFilterMode(filterMode);
    ui->tableView->setModel(proxy_model);

    model->setHorizontalHeaderItem(0, new QStandardItem(tr("MAC")));
    model->setHorizontalHeaderItem(1, new QStandardItem(tr("Object ID")));

    this->leObjectName_2 = leObjectName_2;
    this->leLogin = leLogin;
    this->lePasswd = lePasswd;
    this->leIp_2 = leIp_2;

    this->sbPort_2 = sbPort_2;
    this->sbTimeOut_2 = sbTimeOut_2;
    this->cbSave2KtsWallet = cbSave2KtsWallet;

    this->hashIndx = hashIndx;
    this->allowComprss = allwC;

    for(int i = 0, iMax = listAddr.size(); i < iMax; i++){
        QList<QStandardItem*> l;
        l.append(new QStandardItem(listAddr.at(i).left(listAddr.at(i).indexOf("@"))));
        l.append(new QStandardItem(listAddr.at(i).mid(listAddr.at(i).indexOf("@") + 1)));
        model->appendRow(l);
    }

}

SelectDevByMacAddrDialog::~SelectDevByMacAddrDialog()
{
    delete ui;
}



void SelectDevByMacAddrDialog::on_tableView_doubleClicked(const QModelIndex &index)
{
    conn2this(index);
    accept();
}

void SelectDevByMacAddrDialog::on_buttonBox_accepted()
{
    on_tableView_doubleClicked(ui->tableView->currentIndex());
}

void SelectDevByMacAddrDialog::conn2this(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    //    emit conn2thisDev(ui->leObjectName_2->text().simplified().trimmed(), ui->leLogin->text(), ui->lePasswd->text(), ui->leIp_2->text().simplified().trimmed(), ui->sbPort_2->value(),
    ////                      ui->sbTimeOut->value(),
    //                      ui->sbTimeOut_2->value() * 1000  ,
    //                      ui->cbSave2KtsWallet->isChecked(), true, ui->leObjectMac->text(), ui->rbUseMac->isChecked());

    emit conn2thisDev(hashIndx, leObjectName_2, leLogin,lePasswd, leIp_2,sbPort_2,sbTimeOut_2,cbSave2KtsWallet,allowComprss, true, model->item(proxy_model->mapToSource(index).row(), 0)->text(), true);
}
