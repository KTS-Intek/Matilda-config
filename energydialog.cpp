#include "energydialog.h"
#include "ui_energydialog.h"
#include "moji_defy.h"
#include <QMessageBox>
#include <QMenu>
#include <QAction>

//##########################################################################################
EnergyDialog::EnergyDialog(const QStringList &lName, const QStringList &lValue, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnergyDialog)
{
    ui->setupUi(this);

    ln = lName;
    ld = lValue;

    ui->splitter->setStretchFactor(0,0);
    ui->splitter->setStretchFactor(1,3);

    modelAvProf = new QStandardItemModel(0,0,this);

    modelProfiles = new QStandardItemModel(0,0,this);

    proxy_modelAvProf = new MySortFilterProxyModel(this);
    proxy_modelAvProf->setSourceModel(modelAvProf);
    proxy_modelAvProf->setDynamicSortFilter(true);
    ui->tvAvailable->setModel(proxy_modelAvProf);


    connect(ui->leFilterAvailable, SIGNAL(textChanged(QString)), proxy_modelAvProf, SLOT(setNewFileterStr(QString)) );


    ui->tvProfileEnrg->setModel(modelProfiles);


    for(int i = 0, iMax = lName.size(); i < iMax; i++){
        QList<QStandardItem*> l;
        l.append(new QStandardItem(lName.at(i).isEmpty() ? lValue.at(i) : lName.at(i) )) ;
        l.append(new QStandardItem( lValue.at(i) )) ;

        modelAvProf->appendRow(l);
    }

    QStringList l = QString("A+,A-,R+,R-").split(',');
    QStringList n = tr("Active import,Active export,Reactive import,Reactive export").split(',');


    QList<quint8> ll;
    ll << 0 << POLL_CODE_READ_CURRENT << POLL_CODE_READ_END_DAY << POLL_CODE_READ_END_MONTH << POLL_CODE_READ_POWER ;
    QStringList nn = tr("All,Now,End of Day,End of Month,Power,Voltage").split(',');

    for(int i = 0, iMax = ll.size(), jMax = l.size(); i < iMax; i++){
        QList<QStandardItem *> li;

        for(int j = 0; j < jMax; j++){
            QStandardItem *item = new QStandardItem(n.at(j))  ;
            item->setData(l.at(j));
            item->setCheckable(true);
            item->setCheckState(Qt::Unchecked);
            li.append(item);
        }


        modelProfiles->appendRow(li);

        QStandardItem *item = new QStandardItem(nn.at(i))  ;
        item->setData(ll.at(i));
        modelProfiles->setVerticalHeaderItem(i, item);

    }

    ui->tvProfileEnrg->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tvProfileEnrg->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);


    ui->tvAvailable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tvAvailable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    ui->tvAvailable->setContextMenuPolicy(Qt::CustomContextMenu);
    modelAvProf->setHorizontalHeaderLabels(tr("Name,Values").split(','));

}
//##########################################################################################
EnergyDialog::~EnergyDialog()
{
    delete ui;
}
//##########################################################################################
void EnergyDialog::on_pbAddNew_clicked()
{
    QString mess("");
    if(ui->leEnrgProfile->text().isEmpty())
        mess.append(tr("Energy not found<br>"));
    if(ui->leName->text().isEmpty())
        mess.append(tr("Name is empty<br>"));



    QString n = ui->leName->text().simplified().trimmed();
    QString d = ui->leEnrgProfile->text().simplified().trimmed();

    for(int i = 0, iMax = modelAvProf->rowCount(); i < iMax ; i++){
        if(modelAvProf->item(i,0)->text() == n && modelAvProf->item(i,1)->text() == d){
            mess.append(tr("Found duplicate<br>"));
            break;
        }
    }

    if(!mess.isEmpty()){
        QMessageBox::critical(this, "", mess);
        return;
    }

    QList<QStandardItem*> l;
    l.append(new QStandardItem( n) );
    l.append(new QStandardItem( d )) ;

    modelAvProf->insertRow(0, l);

    ui->tvAvailable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tvAvailable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}
//##########################################################################################
void EnergyDialog::on_tvAvailable_doubleClicked(const QModelIndex &index)
{
    int row = proxy_modelAvProf->mapToSource(index).row();
    ui->leName->setText(modelAvProf->item(row, 0)->text());
    ui->leEnrgProfile->setText(modelAvProf->item(row, 1)->text());
    QStringList eAll = modelAvProf->item(row, 1)->text().split(',');

    QList<quint8> ll;
    ll << 0 << POLL_CODE_READ_CURRENT << POLL_CODE_READ_END_DAY << POLL_CODE_READ_END_MONTH << POLL_CODE_READ_POWER ;
    QStringList llStr;
    while (!ll.isEmpty()) {
        llStr.append(QString::number(ll.takeFirst()).rightJustified(3, '0') + ":");
    }

    for(int row = 0, rowMax = modelProfiles->rowCount(), colMax = modelProfiles->columnCount(); row < rowMax; row++){
        for(int col = 0; col < colMax; col++){
            modelProfiles->item(row,col)->setCheckState(Qt::Unchecked);
        }
    }


    QStringList e = QString("A+,A-,R+,R-").split(',');
    for(int i = 0, iMax = e.size(); i < iMax; i++){
        if(eAll.contains(e.at(i))){
            modelProfiles->item(0, i)->setCheckState(Qt::Checked);
            eAll.removeAll(e.at(i));
        }
    }

    if(eAll.isEmpty()){
        return;
    }


    for( int i = 0, iMax = llStr.size(), jMax = eAll.size(); i < iMax; i++){
        for(int j = 0; j < jMax; j++){
            if(eAll.at(j).left(4) == llStr.at(i)){
                int col = e.indexOf(eAll.at(j).mid(4));
                if(col < 0)
                    continue;
                modelProfiles->item(i,col)->setCheckState(Qt::Checked);
            }
        }
    }

}
//##########################################################################################
void EnergyDialog::on_tvProfileEnrg_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    QStringList enrg;

    bool fAllChecked = false;
    for(int row = 0, rowMax = modelProfiles->rowCount(), colMax = modelProfiles->columnCount(); row < rowMax; row++){
        int lastChCount = 0;
        QStringList enrgL;
        for(int col = 0; col < colMax; col++){
            if(modelProfiles->item(row,col)->checkState() == Qt::Checked){
                lastChCount++;
                if(row == 0)
                    enrgL.append(modelProfiles->item(row,col)->data().toString());
                else
                    enrgL.append(QString::number(modelProfiles->verticalHeaderItem(row)->data().toUInt()).rightJustified(3, '0') + ":" + modelProfiles->item(row,col)->data().toString());

            }
        }

        if(lastChCount == 4){
            if(row == 0)
                fAllChecked = true;
            else{
                if(fAllChecked)
                    continue;
            }
        }
        enrg.append(enrgL);

    }


    ui->leEnrgProfile->setText(enrg.join(','));

}
//##########################################################################################

void EnergyDialog::on_tvAvailable_customContextMenuRequested(const QPoint &pos)
{
    bool enbl = (modelAvProf->rowCount() > 0);

    QMenu *menu = new QMenu(ui->tvAvailable);
    QAction *actRs = new QAction(tr("Reset sorting") , menu);
    QAction *actRo = new QAction(tr("Edit") , menu);
    QAction *actFrankenshtein = new QAction(tr("Recover") , menu);

    QAction *actDel = new QAction(tr("Delete") , menu);

    actRo->setEnabled(enbl);
    actRs->setEnabled(enbl);
    actDel->setEnabled(enbl);
    if(modelAvProf->rowCount() < 2)
        actDel->setEnabled(false);
    actFrankenshtein->setEnabled(!lRecoverLvDevHistory.isEmpty());

    connect(actRs, SIGNAL(triggered(bool)), SLOT(resetSortingTvPhysVal()) );
    connect(actRo, SIGNAL(triggered(bool)), SLOT(editTvPhysVal()) );
    connect(actDel, SIGNAL(triggered(bool)), SLOT(delSelectedPhysVal()) );
    connect(actFrankenshtein, SIGNAL(triggered(bool)), SLOT(onRecoverPhysVal_clicked()) );

    menu->addAction(actRs);
    menu->addAction(actRo);
    menu->addAction(actFrankenshtein);
    menu->addSeparator();
    menu->addAction(actDel);

    menu->exec(ui->tvAvailable->mapToGlobal(pos));
     menu->deleteLater();

}
//##########################################################################################
void EnergyDialog::resetSortingTvPhysVal()
{
    proxy_modelAvProf->sort(-1, Qt::AscendingOrder);// setFilterKeyColumn(0);

}
//##########################################################################################
void EnergyDialog::editTvPhysVal()
{
    on_tvAvailable_doubleClicked(ui->tvAvailable->currentIndex());
}
//##########################################################################################

void EnergyDialog::onRecoverPhysVal_clicked()
{
    if(lRecoverLvDevHistory.isEmpty()){
        return;
    }
    QStringList l = lRecoverLvDevHistory.takeFirst().toStringList();

    if(l.size() != 3){
        return;
    }
    bool ok;
    qint64 lRow = l.at(0).toInt(&ok);
    qint64 rowC = modelAvProf->rowCount();
    if(ok && lRow >= rowC){
        ok = false;
    }else{
        lRow = rowC;
    }

    QList<QStandardItem*> li;
    li.append(new QStandardItem(l.at(1)));
    li.append(new QStandardItem(l.at(2)));

    if(ok)
        modelAvProf->insertRow(lRow,li);
    else
        modelAvProf->appendRow(li);

    modelAvProf->setHorizontalHeaderLabels(tr("Name,Values").split(','));
    ui->tvAvailable->selectRow( proxy_modelAvProf->mapFromSource(modelAvProf->index(lRow,0) ).row() );

    ui->tvAvailable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tvAvailable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}
//##########################################################################################
void EnergyDialog::delSelectedPhysVal()
{
    QList<int> delThisRow;
    foreach (QModelIndex i, ui->tvAvailable->selectionModel()->selectedRows()) {
        QStringList l; //size == 4 , 0 - rowIndx, 1 - n, 2 - d
        int row = proxy_modelAvProf->mapToSource(i).row();
        l.append(QString::number(row));
        l.append(modelAvProf->item(row, 0)->text());
        l.append(modelAvProf->item(row,1)->text());
        lRecoverLvDevHistory.prepend(l);
        delThisRow.append(row);
    }

    qSort(delThisRow);
    while(!delThisRow.isEmpty())
        modelAvProf->removeRow(delThisRow.takeLast());


    for(int i = 500, iMax = lRecoverLvDevHistory.size(); i < iMax; i++)
        lRecoverLvDevHistory.removeLast();
}
//##########################################################################################

void EnergyDialog::on_buttonBox_accepted()
{
    QStringList n,l;
    bool buloOnovlennya = false;
    for(int r = 0, rMax = modelAvProf->rowCount(), iMax = ln.size() ; r < rMax; r++){
        if(!buloOnovlennya){
            if(rMax == iMax){
                if( modelAvProf->item(r,0)->text() != ln.at(r) || modelAvProf->item(r,1)->text() != ld.at(r) )
                    buloOnovlennya = true;
            }else{
                buloOnovlennya = true;
            }
        }
        n.append(modelAvProf->item(r,0)->text());
        l.append(modelAvProf->item(r,1)->text());
    }
    if(!n.isEmpty() && !l.isEmpty())
        emit onPhysValChanged(n,l);
}
//##########################################################################################
