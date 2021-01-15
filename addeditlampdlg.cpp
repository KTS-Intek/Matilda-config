#include "addeditlampdlg.h"
#include "ui_addeditlampdlg.h"
#include <QTimer>

AddEditLampDlg::AddEditLampDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEditLampDlg)
{
    ui->setupUi(this);

    QRegExp regni("^[0-9a-z]{32}$");
    QRegExpValidator *validatorni = new QRegExpValidator(regni, this);
    ui->leAddLampNI->setValidator(validatorni);

    ui->cbxAddLampModel->addItem("Auto");
    ui->rbAddLampGetModels->hide();


    ui->sbAddLampPna->setSpecialValueText(tr("Turn off"));
    ui->sbAddLampPstart->setSpecialValueText(tr("Turn off"));
    ui->sbAddLampTna->setSpecialValueText(tr("Minimum"));

    QTimer *t = new QTimer(this);
    t->setSingleShot(true);
    t->setInterval(111);
    connect(ui->sbAddLampPstart, SIGNAL(valueChanged(int)), t, SLOT(start()));
    connect(t, SIGNAL(timeout()), this, SLOT(checkShowWarning()));
    t->start();

}

AddEditLampDlg::~AddEditLampDlg()
{
    delete ui;
}

QString AddEditLampDlg::getWriteVal(const QString &itmTxt)
{
    //поточне значення > нове значення    - запланована зміна значення
    //поточне значення*                   - не введено необхідного значення, на задавати цього параметру лампі
    QString s = itmTxt;
    if(s.contains(">")){
        s = s.split(">", QString::SkipEmptyParts).last();
        s = s.remove(" ");
    }else{
        if(s.contains("*")){
            s = s.split("*").first();// .clear();
        }
    }
    if(s.isEmpty())
        s = itmTxt;
    return s;
}

int AddEditLampDlg::getSbValue(QSpinBox *s)
{
    return (s->value() <= s->minimum() ? 0 : s->value());

}

int AddEditLampDlg::getTna()
{
    return qMax(301, getSbValue(ui->sbAddLampTna));

}

int AddEditLampDlg::getPna()
{
    return getSbValue(ui->sbAddLampPna);

}

int AddEditLampDlg::getPstart()
{
    return getSbValue(ui->sbAddLampPstart);

}

void AddEditLampDlg::clearPage()
{
    ui->cbAddLampPhysicalGroup->setValue(0);
    ui->cbxAddLampModel->setCurrentIndex(0);
    ui->leAddLampCoordinates->clear();
    ui->leAddLampMemo->clear();
    ui->leAddLampNI->clear();
    ui->sbAddLampPna->setValue(ui->sbAddLampPna->minimum());
    ui->sbAddLampPstart->setValue(ui->sbAddLampPstart->minimum());
    ui->sbAddLampTna->setValue(ui->sbAddLampTna->minimum());
    ui->leAddLampStreet->clear();
    ui->cbAddLampOnOffPoll->setChecked(true);
}

void AddEditLampDlg::addNewDevice()
{
    ui->leAddLampNI->clear();

}

void AddEditLampDlg::setPageSett(const QString &cbxAddLampModel, const QString &leAddLampNI, const QString &cbAddLampPhysicalGroup, const QString &sbAddLampPstart, const QString &sbAddLampPna, const QString &sbAddLampTna, const QString &leAddLampCoordinates, const QString &cbAddLampOnOffPoll, const QString &leAddLampStreet, const QString &leAddLampMemo)
{
    const int p = ui->cbxAddLampModel->findData(cbxAddLampModel);
    pgOpenedNI = leAddLampNI;

    ui->cbxAddLampModel->setCurrentIndex((p < 0) ? 0 : p);

    ui->leAddLampNI->setText(leAddLampNI);
    ui->cbAddLampPhysicalGroup->setValue(getWriteVal(cbAddLampPhysicalGroup).toInt());

    ui->sbAddLampPstart->setValue(sbAddLampPstart.isEmpty() ? ui->sbAddLampPstart->minimum() : getWriteVal(sbAddLampPstart).toInt());
    ui->sbAddLampPna->setValue(sbAddLampPna.isEmpty() ? ui->sbAddLampPna->minimum() : getWriteVal(sbAddLampPna).toInt());
    ui->sbAddLampTna->setValue(sbAddLampTna.isEmpty() ? ui->sbAddLampTna->minimum() : getWriteVal(sbAddLampTna).toInt());

    ui->cbAddLampOnOffPoll->setChecked(cbAddLampOnOffPoll == "+");
    ui->leAddLampMemo->setText(leAddLampMemo);



    ui->leAddLampCoordinates->setText(leAddLampCoordinates);

    ui->leAddLampStreet->setText(leAddLampStreet);
}

void AddEditLampDlg::showLater()
{
    exec();
}

void AddEditLampDlg::on_pushButton_clicked()
{
    on_pbAddLamp_clicked();
    accept();

}

void AddEditLampDlg::on_pbAddLamp_clicked()
{
    emit addLamp(ui->leAddLampNI->text().simplified().trimmed(), ui->cbAddLampPhysicalGroup->value(), ui->leAddLampCoordinates->text().simplified().trimmed(),
                 ui->leAddLampMemo->text().simplified().trimmed(), ui->cbxAddLampModel->currentText().isEmpty() ? "Auto" : ui->cbxAddLampModel->currentText(),
                 ui->cbxAddLampModel->currentData(), QString::number(getPstart()), QString::number(getPna()), QString::number(getTna()),
                 ui->cbAddLampOnOffPoll->isChecked() ? "+" : "-", ui->leAddLampStreet->text(), (ui->leAddLampNI->text().simplified().trimmed() == pgOpenedNI) );


}

void AddEditLampDlg::checkShowWarning()
{
    if(ui->sbAddLampPstart->value() >= 0 && ui->sbAddLampPstart->value() < 50){
        ui->lblShowWarn->setText(tr("Some led power supplies can't start with power less than 50%"));
    }else{
        if(!ui->lblShowWarn->text().isEmpty())
            ui->lblShowWarn->clear();
    }
}
