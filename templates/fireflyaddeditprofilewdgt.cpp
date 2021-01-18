#include "fireflyaddeditprofilewdgt.h"
#include "ui_fireflyaddeditprofilewdgt.h"

FireflyAddEditProfileWdgt::FireflyAddEditProfileWdgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FireflyAddEditProfileWdgt)
{
    ui->setupUi(this);
}

FireflyAddEditProfileWdgt::~FireflyAddEditProfileWdgt()
{
    delete ui;
}

void FireflyAddEditProfileWdgt::on_pushButton_clicked()
{
    deleteLater();

}

void FireflyAddEditProfileWdgt::on_pbAddGroup_3_clicked()
{
//    const QString profileName = (ui->swSaveMode->currentIndex() == 0) ? ui->leLedProfileName->text().simplified().trimmed() : QString::number(ui->sbSaveAsGroupID->value());
//    QString messageStrr;
//    if(profileName.isEmpty())
//        messageStrr.append(tr("Profile name is empty(<br>"));

//    const int rowCount = model->rowCount();
//    if(rowCount <= 0)
//        messageStrr.append(tr("Profile settings are not found(<br>"));

//    if(!messageStrr.isEmpty()){
//        emit showMessage(messageStrr);
//        return;
//    }
//    QStringList profileLine;
//    for(int i = 0, iMax = model->rowCount(); i < iMax; i++)
//        profileLine.append(QString("%1-%2").arg(model->item(i, 0)->text()).arg(model->item(i,1)->text()));

//    emit addProfile(profileName, profileLine.join(","), (profileName == pgOpenedProfileName));

    on_pushButton_clicked();
}
