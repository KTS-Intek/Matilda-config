#include "insertmeterdialog.h"
#include "ui_insertmeterdialog.h"
#include "moji_defy.h"

InsertMeterDialog::InsertMeterDialog(QVariantList hm, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertMeterDialog)
{
    ui->setupUi(this);
    hashmeter = hm;
}

InsertMeterDialog::~InsertMeterDialog()
{
    delete ui;
}

void InsertMeterDialog::on_buttonBox_accepted()
{
    QJsonObject h;
    h.insert("m", QJsonArray::fromVariantList( hashmeter));
    int pos = ui->sbPosition->value();
    if(ui->rbAppend->isChecked())
        pos = 20000;
    else{
        if(ui->rbPrepend->isChecked())
            pos = 0;
    }
    h.insert("i", pos);

    //0 - ignore new meter, 1 - replace old meter, 2 - abort all
    int collision = 0;
    if(ui->rbCollsnInsrtNewVal->isChecked())
        collision = 1;
    else{
        if(ui->rbAbort->isChecked())
            collision = 2;
    }

    h.insert("c", collision);


    emit data2matilda(COMMAND_WRITE_METER_LIST_ONE_PART, h);
}
