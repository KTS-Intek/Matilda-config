#include "wait4answerdialog.h"
#include "ui_wait4answerdialog.h"
#include <QTimer>
#include <QtDebug>
Wait4AnswerDialog::Wait4AnswerDialog(int defH, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Wait4AnswerDialog)
{
    ui->setupUi(this);
    maxH = defH * 3;

    QTimer *timer = new QTimer;
    timer->setInterval(1000);

    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerTo()) );
    connect(this,SIGNAL(stopTimer()), timer,SLOT(stop()) );
    connect(this, SIGNAL(startTimr()), timer, SLOT(start()) );

    connect(this,SIGNAL(rejected()), timer, SLOT(stop()) );
}

Wait4AnswerDialog::~Wait4AnswerDialog()
{
    delete ui;
}

void Wait4AnswerDialog::showAnimation(int count)
{
    ui->progressBar->hide();
    ui->progressBar->setValue(0);
    rotateCounter = 0;
    emit stopTimer();
    defCount = count;
    counter = count;
    counter2 = 0;


    ui->lblWait->setText(tr("Wait %1 s. %2").arg(QString::number(counter) ).arg(rotateStr()));
    ui->lblRotate->setVisible(true);


    emit startTimr();
    this->exec();

}

void Wait4AnswerDialog::hideAnimation()
{
    emit stopTimer();
    if(isVisible())
        reject();
    else
        setVisible(false);
    QTimer::singleShot(5, this, SLOT(reject()) );
    QTimer::singleShot(5, this, SLOT(close()) );


}

void Wait4AnswerDialog::uploadProgress(int val, QString txt)
{
    if(val > 100)
        val = 100;

    emit stopTimer();
    counter++;


    if(!isVisible())
       this->exec();

    if(!ui->progressBar->isVisible())
        ui->progressBar->setVisible(true);
    ui->progressBar->setValue(val);
    ui->lblWait->setText(txt + rotateStr());
}

void Wait4AnswerDialog::resetCounter()
{
     counter = defCount;
}

void Wait4AnswerDialog::onTimerTo()
{
    counter2++;
    if(counter2 > 0){
        counter--;
        if(counter < 1){
            if(!isVisible())
                return;
            hideAnimation();
            emit noAnswerFromDev();
        }
        counter2 = 0;
        ui->lblWait->setText(tr("Wait %1 s. %2").arg(QString::number(counter) ).arg(rotateStr()));
    }
}

QString Wait4AnswerDialog::rotateStr()
{

    rotateCounter++;
    switch( rotateCounter ){
    case 1: return " / ";
    case 2: return " - ";
    case 3: return " \\ ";
    default: rotateCounter = 0;  return " | ";

    }
    return "";
}
