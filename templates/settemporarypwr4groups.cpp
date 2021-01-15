#include "settemporarypwr4groups.h"
#include "ui_settemporarypwr4groups.h"

#include <QTimer>
#include <QDateTime>


SetTemporaryPwr4groups::SetTemporaryPwr4groups(const QString &datemask, const QString &timemask, const QStringList &groupslist, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetTemporaryPwr4groups)
{
    ui->setupUi(this);

    setWindowTitle(tr("Temporary power"));
    ui->spinBox->setSpecialValueText(tr("Turn off"));

    this->groupslist = groupslist;
    ui->lblTempSchedule->setText(groupslist.join(", "));

    ui->timeEdit->setDisplayFormat(timemask);
    ui->timeEdit_2->setDisplayFormat(timemask);

    this->datemask = datemask;
    connectSbAndSlider(ui->hsPower, ui->spinBox);
    connectSbAndTimeEdit(ui->timeEdit, ui->hsTimeFrom);
    connectSbAndTimeEdit(ui->timeEdit_2, ui->hsTimeTo);

    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(onLinearChanged(int)));

    const QTime time = QTime::currentTime();
    ui->timeEdit->setTime(time);
    ui->timeEdit_2->setTime(time.addSecs(1800));

    QTimer *t = new QTimer(this);
    t->setSingleShot(true);
    t->setInterval(1111);
    connect(this, SIGNAL(startTmrShowLbl()), t, SLOT(start()));
    connect(t, SIGNAL(timeout()), this, SLOT(updateDateTimeLbls()));
    emit startTmrShowLbl();
    ui->lblDtFrom->hide();

    ui->label_2->hide();
    ui->timeEdit->hide();
    ui->hsTimeFrom->hide();

}

SetTemporaryPwr4groups::~SetTemporaryPwr4groups()
{
    delete ui;
}

void SetTemporaryPwr4groups::connectSbAndTimeEdit(QTimeEdit *te, QSlider *sb)
{
    te->setTime(QTime(0,0,0,0));
    sb->setValue(sb->minimum());
    connect(te, &QTimeEdit::timeChanged, [=](const QTime &time){
        sb->setValue(QTime(0,0,0,0).secsTo(time)/60);
    });

    connect(sb, &QSlider::valueChanged, [=](int value){
       te->setTime(QTime(0,0,0,0).addSecs(value*60));
    });
    connect(sb, SIGNAL(valueChanged(int)), this, SIGNAL(startTmrShowLbl()));

}

void SetTemporaryPwr4groups::connectSbAndSlider(QSlider *se, QSpinBox *sb)
{
    se->setMinimum(sb->minimum());
    se->setMaximum(sb->maximum());
    se->setValue(sb->value());


    connect(sb, SIGNAL(valueChanged(int)), se, SLOT(setValue(int)));
    connect(se, &QSlider::valueChanged, sb, &QSpinBox::setValue);
    connect(sb, SIGNAL(valueChanged(int)), this, SIGNAL(startTmrShowLbl()));


}

void SetTemporaryPwr4groups::on_pushButton_clicked()
{
    if(ui->hsTimeFrom->value() == ui->hsTimeTo->value())
        return;

    emit setTempSchedule4theseGroups(groupslist, (ui->spinBox->value() == ui->spinBox->minimum()) ? 0 : ui->spinBox->value(), ui->hsTimeFrom->value(), ui->hsTimeTo->value());
    accept();
}

void SetTemporaryPwr4groups::on_pushButton_2_clicked()
{
    reject();
}

void SetTemporaryPwr4groups::updateDateTimeLbls()
{
    const QDateTime currdt = QDateTime::currentDateTime();
    const QDateTime currdtzero = QDateTime(currdt.date(), QTime(0,0,0,0));



    const QDateTime dtfrom = currdtzero.addSecs(ui->hsTimeFrom->value()*60);
    const QDateTime dtto = (ui->hsTimeFrom->value() >= ui->hsTimeTo->value()) ? currdtzero.addDays(1).addSecs(ui->hsTimeTo->value()*60) : currdtzero.addSecs(ui->hsTimeTo->value()*60);

    if(!dtfrom.isValid() || !dtto.isValid()){
        ui->lblDtTo->setText(tr("bad"));
        ui->lblDtFrom->setText(tr("bad"));
        return;
    }

    ui->lblDtFrom->setText(dtfrom.toString(datemask));
    ui->lblDtTo->setText(dtto.toString(datemask));

}



