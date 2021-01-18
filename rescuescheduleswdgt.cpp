#include "rescuescheduleswdgt.h"
#include "ui_rescuescheduleswdgt.h"
#include "src/firefly/convert2humanreadablefirefly.h"


RescueSchedulesWdgt::RescueSchedulesWdgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RescueSchedulesWdgt)
{
    ui->setupUi(this);
    initPage();
}

RescueSchedulesWdgt::~RescueSchedulesWdgt()
{
    delete ui;
}

QJsonObject RescueSchedulesWdgt::getPageSettings()
{
    QJsonObject json;
    json.insert("def", ui->lineEdit_2->text().simplified().trimmed());//remove non printable characters
    json.insert("em", QString("0-%1").arg(ui->spinBox->value())); //0-90  from 00:00 90%


    return json;
}

void RescueSchedulesWdgt::initPage()
{
    clearPage();
}

void RescueSchedulesWdgt::clearPage()
{
    ui->tabWidget->setCurrentIndex(0);

}

void RescueSchedulesWdgt::setPageSett(QJsonObject json)
{
    ui->lineEdit_2->setText(json.value("def").toString());//default source
    ui->spinBox->setValue(json.value("em").toString().split("-").last().toInt());

    ui->lineEdit->setText(Convert2humanReadableFirefly::scheduleMinutesToHourMinutesLine(ui->lineEdit_2->text()));
}
