#include "langdialog.h"
#include "ui_langdialog.h"
#include "showmesshelper.h"
#include <QHash>
#include <QTimer>

LangDialog::LangDialog(QComboBox *cbx, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LangDialog)
{

    ui->setupUi(this);
    defLblTxt = ui->label->text();
//    ui->cbLang = cbx;
    if(cbx && cbx->count() > 0){
        for(int i = 0, iMax = cbx->count(); i < iMax; i++ ){
            ui->cbLang->addItem(cbx->itemIcon(i), cbx->itemText(i), cbx->itemData(i));
        }
        ui->cbLang->setCurrentIndex(cbx->currentIndex());
    }else{
        QTimer::singleShot(1, this, SLOT(reject()) );
    }
}

LangDialog::~LangDialog()
{
    delete ui;
}

void LangDialog::on_buttonBox_accepted()
{
    emit onLangSelected(ui->cbLang->currentData().toString());
    accept();
}

void LangDialog::on_cbLang_currentIndexChanged(int index)
{
    ui->label->setText( ShowMessHelper::machine2human(QString("uk;en").split(";"),
                        QString("Виберіть будь-ласка мову;Please select a language").split(";") ).value(ui->cbLang->itemData(index).toString(), defLblTxt) );

}
