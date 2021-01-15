#include "integereditdlg.h"
#include "ui_integereditdlg.h"

IntegerEditDlg::IntegerEditDlg(const int &min, const int &max, const QString &text, const QString &specTxt, const int &mode, const int &tag, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IntegerEditDlg), myTag(tag)
{
    ui->setupUi(this);
    setWindowTitle(text);
    connect(ui->sbSunsetCorr, SIGNAL(valueChanged(int)), ui->hsSunsetMinute, SLOT(setValue(int)) );
    connect(ui->hsSunsetMinute, SIGNAL(valueChanged(int)), ui->sbSunsetCorr, SLOT(setValue(int)) );

    ui->sbSunsetCorr->setRange(min, max);
    ui->hsSunsetMinute->setRange(min, max);
    ui->label_13->setText(text);
    if(!specTxt.isEmpty())
        ui->sbSunsetCorr->setSpecialValueText(specTxt);


    modeselector = new InserModeSelector(mode, this);
    ui->vl4wdgt->addWidget(modeselector);

    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(deleteLater()));
}

IntegerEditDlg::~IntegerEditDlg()
{
    delete ui;
}

void IntegerEditDlg::setThisData(QVariant data)
{
    this->data = data;
}

void IntegerEditDlg::on_buttonBox_accepted()
{

    const int mode = modeselector->getInsertMode();

    const int val = ui->sbSunsetCorr->value();
    emit setInteger(val, mode, myTag);
    if(!data.isNull())
        emit setIntegerExt(val, mode, myTag, data);


    switch(mode){
    case 0: emit setInteger4selected(val, myTag); if(!data.isNull()) emit setInteger4selectedExt(val, myTag, data); break;
    case 1: emit setInteger4visible( val, myTag); if(!data.isNull()) emit setInteger4visibleExt( val, myTag, data);break;
    case 2: emit setInteger4all(     val, myTag); if(!data.isNull()) emit setInteger4allExt(     val, myTag, data);break;
    }

    deleteLater();
}
