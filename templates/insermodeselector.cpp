#include "insermodeselector.h"
#include "ui_insermodeselector.h"

InserModeSelector::InserModeSelector(const int &mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InserModeSelector)
{
    ui->setupUi(this);

    setEnabled(true);

    ui->rbCurrentRow->setVisible(mode == 3);

    switch(mode){
    case 0: ui->rbSelected->setChecked(true); break;
    case 1: ui->rbVisible->setChecked(true); break;
    case 2: ui->rbAll->setChecked(true); break;
    case 3: ui->rbCurrentRow->setChecked(true); setEnabled(false); break;
    default: ui->rbSelected->setChecked(true); ui->groupBox->setEnabled(false); break;
    }
}

InserModeSelector::~InserModeSelector()
{
    delete ui;
}

int InserModeSelector::getInsertMode()
{
    int mode = 0;
    if(ui->rbVisible->isChecked())
        mode++;
    if(ui->rbAll->isChecked())
        mode+=2;

    if(ui->rbCurrentRow->isChecked())
        mode+=3;

    return mode;
}
