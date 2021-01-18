#include "fireflyschedulestatuswdgt.h"
#include "ui_fireflyschedulestatuswdgt.h"

FireflyScheduleStatusWdgt::FireflyScheduleStatusWdgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FireflyScheduleStatusWdgt)
{
    ui->setupUi(this);
}

FireflyScheduleStatusWdgt::~FireflyScheduleStatusWdgt()
{
    delete ui;
}


QStringList FireflyScheduleStatusWdgt::getValidFireflyScheduleStatus(const QString &line, const QString &prefix)
{
    const QString lineext = line + "000000000000000";
//    FireflyV2ScheduleSett status;
    QStringList out;
    out.append(tr("%1 hasSpecialDay [0,1] - '%2'").arg(prefix).arg((lineext.mid(0,1).toInt())));
    out.append(tr("%1 hasDynamicSchedule [1,1] - '%2'").arg(prefix).arg((lineext.mid(1,1).toInt())));
    out.append(tr("%1 hasError [2,1] - '%2'").arg(prefix).arg((lineext.mid(2,1).toInt())));
    out.append(tr("%1 hasTempSchedule [3,1] - '%2'").arg(prefix).arg((lineext.mid(3,1).toInt())));
    out.append(tr("%1 isEmergencyActive [4,1] - '%2'").arg(prefix).arg((lineext.mid(4,1).toInt())));


    return out;
}

QStringList FireflyScheduleStatusWdgt::getGroupPower(const QJsonArray &insline)
{
    QStringList grp2pwr;
    for(int i = 0, imax = insline.size(); i < imax; i++){
        const QStringList onepwr = insline.at(i).toString().split(" ", QString::SkipEmptyParts);
        if(onepwr.isEmpty())
            continue;
        const QString pwr100 = onepwr.first();

        for(int j = 1, jmax = onepwr.size(); j < jmax; j++){
            grp2pwr.append(tr("Group %1 - %2 [%]").arg(onepwr.at(j)).arg(pwr100));
        }

    }
    return grp2pwr;
}

void FireflyScheduleStatusWdgt::clearPage()
{
    ui->plainTextEdit->clear();
}

void FireflyScheduleStatusWdgt::setPageData(QJsonObject json)
{

    clearPage();
    const qint64 ftmsec = qint64(json.value("ftmsec").toDouble());

    ui->plainTextEdit->appendPlainText(tr("Confirmed - it was sent to controllers, is waiting to change power %1 [isw2cpc]")
                                       .arg(json.value("isw2cpc").toBool()));
    ui->plainTextEdit->appendPlainText(getValidFireflyScheduleStatus(json.value("cnfs").toString(), tr("confirmed [cnfs]")).join("\n"));
    ui->plainTextEdit->appendPlainText(getGroupPower(json.value("cnfl").toArray()).join("\n"));
    ui->plainTextEdit->appendPlainText("cnfl\n");

    ui->plainTextEdit->appendPlainText(tr("Active - it was sent to controllers or it is going to be sent to controllers, is waiting to change power %1 [isw2cp]")
                                       .arg(json.value("isw2cp").toBool()));
    ui->plainTextEdit->appendPlainText(getValidFireflyScheduleStatus(json.value("acts").toString(), tr("active [acts]")).join("\n"));
    ui->plainTextEdit->appendPlainText(getGroupPower(json.value("actl").toArray()).join("\n"));
    ui->plainTextEdit->appendPlainText("actl\n");


    ui->plainTextEdit->appendPlainText(tr("Future - it will be sent to controllers, is waiting to change power %1 [isw2cpf]")
                                       .arg(json.value("isw2cpf").toBool()));
    ui->plainTextEdit->appendPlainText(getValidFireflyScheduleStatus(json.value("ftrs").toString(), tr("future [ftrs]")).join("\n"));
    ui->plainTextEdit->appendPlainText(getGroupPower(json.value("ftrl").toArray()).join("\n"));
    ui->plainTextEdit->appendPlainText("ftrl\n");

    ui->plainTextEdit->appendPlainText(tr("Future msec %1 %2 [ftmsec]")
                                       .arg(QString::number(ftmsec))
                                       .arg(QDateTime::fromMSecsSinceEpoch(ftmsec).toString("yyyy-MM-dd hh:mm:ss")));


//    if(json.contains("activs")){ //protocol v9 and higher
//        h.insert("dids", schedule.grpIdsDynamic);

//        h.insert("defl", schedule.defScheduleLine);
//        h.insert("eml", schedule.emergencyScheduleLine);
//        h.insert("regl", schedule.regularScheduleLine);
//        h.insert("specl", schedule.specialScheduleLine);
//        h.insert("templ", schedule.tempScheduleLine);

//    }

    //    //    rez.insert("isw2cp", state.schedulestates.isWaiting2changePower);


}
