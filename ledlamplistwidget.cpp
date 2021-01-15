#include "ledlamplistwidget.h"
#include "ui_ledlamplistwidget.h"

#include <QDebug>


#include "definedpollcodes.h"

#include "templates/settemporarypwr4groups.h"
#include "templates/addminutesdialog.h"
#include "insertmeterdialog.h"

#include "moji_defy.h"


//--------------------------------------------------------------

LedLampListWidget::LedLampListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LedLampListWidget)
{
    ui->setupUi(this);
    setProtocolVersion(MATILDA_PROTOCOL_VERSION_V6);
    setCanWriteOperatorCommands(false);
    initPage();
}

//--------------------------------------------------------------

LedLampListWidget::~LedLampListWidget()
{
    delete ui;
}

//--------------------------------------------------------------

QString LedLampListWidget::map2jsonLine(const QVariantMap &map)
{
    return QJsonDocument(QJsonObject::fromVariantMap(map)).toJson(QJsonDocument::Compact);

}

//--------------------------------------------------------------

QVariantMap LedLampListWidget::getPower2groups(const QMap<int, int> &groupId2power)
{
    return getPower2groupsExt(groupId2power, true);
}

//--------------------------------------------------------------

QVariantMap LedLampListWidget::getPower2groupsExt(const QMap<int, int> &groupId2power, const bool &isPowerInPercents)
{
    const QList<int> lk = groupId2power.keys();
    QVariantMap map;

    const int multiplier = isPowerInPercents ? 254 : 100;

    for(int i = 0, imax = lk.size(); i < imax; i++){

        const QString grp = QString::number(lk.at(i));
        const QString pwr = QString::number( groupId2power.value(lk.at(i)) * multiplier / 100) ;//convert percents to 0-0xFE

        QStringList l = map.value(pwr).toStringList();
        l.append(grp);
        map.insert(pwr, l);
    }
    return map;
}

//--------------------------------------------------------------

QStringList LedLampListWidget::getKeysLedLampV2()
{
    //
    return QString("mdl|NI|grpw|pstrtw|pnaw|tnaw|crdnts|pll|strt|memo|type|msecexhngd|mseclanswr"
                   "|pwr|wtt|pstrt|pna|tna|grp|ampers|refpowerwt|prog|uptime|useuref|usens|uref").split("|");

    //|sync|jsnother - they are not used here
}

//--------------------------------------------------------------

int LedLampListWidget::getProtocolVersion()
{
    return protocolVersion;
}

//--------------------------------------------------------------

int LedLampListWidget::getMatches4ThisCellText(const int &col, const QString &celltxt, const int &limit)
{
    return getMatches4TheseCellsText(QList<int>() << col, QStringList() << celltxt, limit);

}

//--------------------------------------------------------------

int LedLampListWidget::getMatches4TheseCellsText(const QList<int> &cols, const QStringList &celltxtl, const int &limit)
{
    if(cols.size() < 0)
        return 0;

    int counter = 0;
    for(int i = 0, iMax = model->rowCount(), cmax = cols.size(); i < iMax && counter != limit; i++){

        QStringList l;
        for(int c = 0; c < cmax; c++)
            l.append(model->item(i, cols.at(c))->text());

        if(l == celltxtl)
            counter++;
    }
    return counter;
}

//--------------------------------------------------------------

QStringList LedLampListWidget::getSelectedRowsText(const int &col)
{
    QStringList list;
    const QModelIndexList l = ui->tvTable->selectionModel()->selectedRows(col);
    //    QStandardItemModel m = table->model();
    for(int i = 0, iMax = l.size(); i < iMax ; i++)
        list.append(l.at(i).data(Qt::DisplayRole).toString());

    return list;
}

//--------------------------------------------------------------

void LedLampListWidget::setModelHorizontalHeaderItems(const QStringList &header)
{
    model->clear();
    for(int i = 0, iMax = header.size(); i < iMax; i++)
        model->setHorizontalHeaderItem(i, new QStandardItem(header.at(i)));
}

//--------------------------------------------------------------

bool LedLampListWidget::getSelectedNI4lampsSmart(QString &s)
{
    const QStringList list = getSelectedRowsText( 1 );
    if(list.isEmpty()){
        emit showMessage(tr("There are no lamps("));
        return false;
    }

//    if(gHelper->managerEnDisBttn.pbWriteDis){
//        emit showMessage(tr("This action is not allowed("));
//        return false;
//    }
    s =  list.join(" ");
    return true;
}

//--------------------------------------------------------------

QStringList LedLampListWidget::getSelectedNis(const int &maxsize)
{
    const QStringList l = getSelectedRowsText(1);//ni column
    const int ls = l.size();


    if(ls > maxsize){
        emit showMessage(tr("The maximum size is %1. The count of LCUs is %2.<br>%3 LCUs will be omitted").arg(maxsize).arg(ls).arg(ls-maxsize));
        return l.mid(0, maxsize);
    }
    return l;
}

//--------------------------------------------------------------

QStringList LedLampListWidget::getUniqGroupIds()
{
    const QStringList inl = getSelectedRowsText(2);
    QStringList out;
    for(int i = 0, imax = inl.size(); i < imax; i++){
        const QString grpid = inl.at(i).split(">").last();
        if(grpid.isEmpty())
            continue;
        if(out.contains(grpid))
            continue;
        out.append(grpid);
    }
    return out;
}



//--------------------------------------------------------------

QJsonObject LedLampListWidget::getPageSett(int &row, int &rowCount, const int &maxLen)
{
    QJsonObject jobj;
    if(row < 0)
        row = 0;
    rowCount = model->rowCount();

    if(row >= rowCount)
        return jobj;
// return QString("mdl NI grpw pstrtw pnaw tnaw crdnts pll strt memo type")

    const QStringList k = QString("mdl NI grpw pstrtw pnaw tnaw crdnts pll strt memo type").split(' ', QString::SkipEmptyParts);//only these keys can be written
    const QStringList ktable = getKeysLedLampV2();//all keys from table

    QVariantList listVarM;


    for(int j = 0, colMax = qMin(k.size(), model->columnCount()), cMax = ktable.size(), bytes = 50; row < rowCount; row++, j++){

        QMap<QString,QString> maponerow;
        for(int c = 0; c < cMax; c++)
            maponerow.insert(ktable.at(c), model->item(row, c)->text());

        if(maponerow.value("mdl") == "Auto")
            maponerow.insert("mdl", "");//it is Auto, so wipe the value

        QVariantList l;
        QStringList bytesl;
        for(int col = 0; col < colMax; col++){
            l.append(maponerow.value(k.at(col)));
            bytesl.append(maponerow.value(k.at(col)));
        }

        bytes += bytesl.join("___").size(); // "",
        if(bytes > maxLen){
            row--;
            break;
        }

        listVarM.append(QVariant(l));
    }

    jobj.insert("m", QJsonArray::fromVariantList(listVarM));
    row++;

    jobj.insert("i", (row >= rowCount) ? -1 : row);



    return jobj;
}

//--------------------------------------------------------------

QStringList LedLampListWidget::varList2strList(const QVariantList &list)
{
    QStringList l;
    for(int i = 0, iMax = list.size(); i < iMax; i++)
        l.append(list.at(i).toString());
    return l;
}

//--------------------------------------------------------------

void LedLampListWidget::initPage()
{

    model = new QStandardItemModel(ui->tvTable);
    proxy_model = new MySortFilterProxyModel(ui->tvTable);
    proxy_model->setSourceModel(model);
    proxy_model->setDynamicSortFilter(true);
    connect(ui->leFilter, SIGNAL(textChanged(QString)   ), proxy_model  , SLOT(setNewFileterStr(QString))   );

    ui->tvTable->setModel(proxy_model);

    editWdgt = new AddEditLampDlg(this);
    connect(editWdgt, &AddEditLampDlg::addLamp, this, &LedLampListWidget::addLamp);

}

//--------------------------------------------------------------

void LedLampListWidget::clearPage()
{
    setModelHorizontalHeaderItems(getKeysLedLampV2());
    editWdgt->clearPage();

}

//--------------------------------------------------------------

void LedLampListWidget::onReadingFinished()
{
    ui->tvTable->resizeColumnsToContents();


}

//--------------------------------------------------------------

void LedLampListWidget::setPageSett(const QJsonObject &jobj)
{

    if(jobj.contains("c"))
        lastColumnsSequence = jobj.value("c").toString().split("|");

    const QVariantList lVar = jobj.value("m").toArray().toVariantList();

    const QStringList k = getKeysLedLampV2();


    const int iMax = lVar.size();

    for(int i = 0, colMax = k.size(), devc = lastColumnsSequence.size(); i < iMax; i++){
        const QVariantList oneitem = lVar.at(i).toList();

        QMap<QString,QString> mapDev2gui;
        for(int c = 0, ccmax = oneitem.size(); c < ccmax && c < devc; c++)
            mapDev2gui.insert(lastColumnsSequence.at(c), oneitem.at(c).toString());




//        QStringList lh = varList2strList(lVar.at(i).toList());
        QList<QStandardItem*> m ;
        for(int c = 0; c < colMax ; c++){
            if(k.at(c).startsWith("msec")){
                bool ok;
                const qint64 msecsinceepoch = mapDev2gui.value(k.at(c)).toLongLong(&ok);
                if(ok){
                    //convert to human readable
                    mapDev2gui.insert(k.at(c), QDateTime::fromMSecsSinceEpoch(msecsinceepoch).toString("yyyy-MM-dd hh:mm:ss.zzz"));
                }
            }else{
                if(k.at(c) == "mdl")
                    mapDev2gui.insert(k.at(c), "Auto");
            }
            m.append(new QStandardItem(mapDev2gui.value(k.at(c))));// (col < lhMax) ? lh.at(col) : "---"));

        }


        model->appendRow(m);
    }

    if(jobj.value("i").toInt() < 1)
        onReadingFinished();


//    qDebug() << "LedLampListWidget " << model->rowCount() << rowF << lastIndx << iMax ;



}

//--------------------------------------------------------------

void LedLampListWidget::onActSelectedPollOnAddLedLampTable()
{
    const int pllcol = getKeysLedLampV2().indexOf("pll");

    foreach (QModelIndex i, ui->tvTable->selectionModel()->selectedRows()) {
        int row = proxy_model->mapToSource(i).row();
        model->item(row,pllcol)->setText("+");
    }
}

//--------------------------------------------------------------

void LedLampListWidget::onActSelectedPollOffAddLedLampTable()
{
    const int pllcol = getKeysLedLampV2().indexOf("pll");

    foreach (QModelIndex i, ui->tvTable->selectionModel()->selectedRows()) {
        int row = proxy_model->mapToSource(i).row();
        model->item(row,pllcol)->setText("-");
    }
}

//--------------------------------------------------------------

void LedLampListWidget::addLamp(QString lamp_ni, int grp_id, QString crdnts, QString memo, QString lampmodel, QVariant modelData, QString sbAddLampPstart, QString sbAddLampPna, QString sbAddLampTna, QString cbAddLampOnOffPoll, QString leAddLampStreet, bool quite)
{
    int niRow = -1;
    int rowCount = model->rowCount();


    QString messageStrr;
    if(lamp_ni.isEmpty() || lamp_ni.length() > 32)
        messageStrr.append(tr("The lamp NI is invalid<br>"));

    if(grp_id < 0 || grp_id > 127)
        messageStrr.append(tr("The Group ID is invalid<br>"));

//    if(!crdnts.isEmpty() && !GeoCoordinatesHelper::isCoordinatesValid(crdnts, crdnts))
//        messageStrr.append(tr("Coordinate is invalid<br>"));

    if(!messageStrr.isEmpty()){
        emit showMessage(tr("Couldn't add the lamp.<br>%1").arg(messageStrr));
        return;
    }

    for(int i = 0; i < rowCount; i++){
        //        TableHeaders::getColNamesLedLampV2();//   lamp_ni,grp_id,crdnts,pll,memo
        if(model->item(i, 1)->text() == lamp_ni){
            niRow = i;
            break;
        }
    }


    QList<QStandardItem*> li;
//    if(true){
//        QStandardItem *item = new QStandardItem(lampmodel);
//        item->setData(modelData);
//        li.append(item);
//    }

//
//    return QString("mdl|NI|grpw|pstrtw|pnaw|tnaw|crdnts|pll|strt|memo|type|msecexhngd|mseclanswr"
//                   "|pwr|wtt|pstrt|pna|tna|grp|ampers|refpowerwt|prog|uptime|useuref|usens|uref").split("|");

    QMap<QString,QString> mapDev2gui;
    mapDev2gui.insert("mdl", "Auto");

    mapDev2gui.insert("NI", lamp_ni);
    mapDev2gui.insert("grpw", QString::number(grp_id));

    mapDev2gui.insert("pstrtw", sbAddLampPstart);
    mapDev2gui.insert("pnaw", sbAddLampPna);
    mapDev2gui.insert("tnaw", sbAddLampTna);

    mapDev2gui.insert("crdnts", crdnts);
    mapDev2gui.insert("pll", cbAddLampOnOffPoll);
    mapDev2gui.insert("strt", leAddLampStreet);
    mapDev2gui.insert("memo", memo);

    const QStringList k = getKeysLedLampV2();
    for(int c = 0, colMax = k.size(); c < colMax ; c++){

        li.append(new QStandardItem(mapDev2gui.value(k.at(c))));// (col < lhMax) ? lh.at(col) : "---"));

    }


    if(niRow >= 0){
//        if(quite || true){
            model->removeRow(niRow);
            model->insertRow(niRow, li);

            if(proxy_model->mapFromSource(model->index(niRow,0)).isValid())
                ui->tvTable->selectRow(proxy_model->mapFromSource(model->index(niRow,0)).row());
//        }
        return;
    }

    model->appendRow(li);
    if(proxy_model->mapFromSource(model->index(rowCount,0)).isValid())
        ui->tvTable->selectRow(proxy_model->mapFromSource(model->index(rowCount,0)).row());

    onReadingFinished();
}

//--------------------------------------------------------------

void LedLampListWidget::showDlgSetupTempPower4schedule()
{
    SetTemporaryPwr4groups *d = new SetTemporaryPwr4groups("yyyy-MM-dd", "hh:mm:ss", getUniqGroupIds() ,this);
    connect(d, &SetTemporaryPwr4groups::setTempSchedule4theseGroups, this, &LedLampListWidget::setTempSchedule4theseGroups);
    d->exec();
    d->deleteLater();
}

//--------------------------------------------------------------

void LedLampListWidget::resetSelectedTempPower4schedule()
{
    sendCOMMAND_WRITE_ADD_DEL_TEMPSCHEDULE(QStringList(), QStringList(), getUniqGroupIds());

}

//--------------------------------------------------------------

void LedLampListWidget::resetAllTempPower4schedule()
{
    QStringList l;
    for(int i = 0; i < 256; i++)
        l.append(QString::number(i));
    sendCOMMAND_WRITE_ADD_DEL_TEMPSCHEDULE(QStringList(), QStringList(), l);
}

//--------------------------------------------------------------

void LedLampListWidget::setTempSchedule4theseGroups(QStringList groups, int power, int minutesfrom, int minutesto)
{
    const QDateTime currdt = QDateTime::currentDateTime();
    const QDateTime currdtzero = QDateTime(currdt.date(), QTime(0,0,0,0));

    const QDateTime dtfrom = currdtzero.addSecs(minutesfrom*60);
    const QDateTime dtto = (minutesfrom >= minutesto) ? currdtzero.addDays(1).addSecs(minutesto*60) : currdtzero.addSecs(minutesto*60);

    if(!dtfrom.isValid() || !dtto.isValid() || groups.isEmpty()){
        emit showMessage(tr("bad parameters"));
        return;
    }

    const int minutesfromvalid = currdtzero.secsTo(dtfrom)/60;
    const qint64 msecto = dtto.toMSecsSinceEpoch();

    QStringList sdp, dtsl;
    for(int i = 0, imax = groups.size(); i < imax; i++){
        sdp.append(QString("%1\t%2-%3").arg(groups.at(i)).arg(minutesfromvalid).arg(power));
        dtsl.append(QString("%1\t%2").arg(groups.at(i)).arg(msecto));
    }

    sendCOMMAND_WRITE_ADD_DEL_TEMPSCHEDULE(sdp, dtsl, QStringList());
}

//--------------------------------------------------------------

void LedLampListWidget::sendCOMMAND_WRITE_ADD_DEL_TEMPSCHEDULE(const QStringList &sdp, const QStringList &dtsl, const QStringList &idsr)
{
    QVariantHash h;
    h.insert("idsr", idsr);
    h.insert("sdp", sdp);
    h.insert("dtsl", dtsl);
    //    gHelper->mWrite2RemoteDevSlot(COMMAND_WRITE_ADD_DEL_TEMPSCHEDULE, h, this);
}

//--------------------------------------------------------------

void LedLampListWidget::setProtocolVersion(int version)
{
    protocolVersion = version;
}

void LedLampListWidget::setCanWriteOperatorCommands(bool canwrite)
{
    canWriteOperatorCommands = canwrite;
}

//--------------------------------------------------------------

void LedLampListWidget::setPower4selectedExt(int value, int tag, QVariant data)
{
    Q_UNUSED(tag);

    QVariantHash hash = data.toHash();

    QVariantMap map = hash.value("map").toMap();

    const int power = (value < 5) ? 0 : value;



    switch(hash.value("code").toInt()){
    case POLL_CODE_FF_WRITE_POWER_TO_GROUPS:{

        QStringList grps = map.value("-ns").toStringList();
        QMap<int, int> groupId2power;
        for(int i = 0, imax = grps.size(); i < imax; i++)
            groupId2power.insert(grps.at(i).toInt(), power);


        map.insert("-ns", QString("this_is_a_unique_ni_for_power2groups").split("\t")); // !empty qstringlist
        map.insert("-operation", map2jsonLine(getPower2groups(groupId2power)));
        break;}

    case POLL_CODE_FF_WRITE_POWER_TO_LAMP:{
        QVariantMap pwrmap ;
        pwrmap.insert("lastLampPower", power);
        map.insert("-operation", map2jsonLine(pwrmap));
        break;}
    }

    hash.insert("map", map);

//    gHelper->mWrite2RemoteDevSlot(COMMAND_WRITE_FIREFLY_START_POLL, hash, this);

}

//--------------------------------------------------------------

void LedLampListWidget::on_pbAddLamp_clicked()
{
    editLampFromRow(-1);

}

//--------------------------------------------------------------

void LedLampListWidget::on_tvTable_doubleClicked(const QModelIndex &index)
{
    editLampFromRow(proxy_model->mapToSource(index).row());

}

//--------------------------------------------------------------

void LedLampListWidget::on_tvTable_customContextMenuRequested(const QPoint &pos)
{
    QTableView *tv = ui->tvTable;
    if(!tv)
        return;

//    const bool enbl = (proxy_model->rowCount() > 0);
    const int selRowCount = tv->selectionModel()->selectedRows().size();
    const bool hasSelItems = (selRowCount > 0 );

    const bool youHaveApower = canWriteOperatorCommands;

//    gHelper->updateLastDataType(CLBRD_LEDLAMP_LISTV2);
    QMenu *menu = new QMenu(tv);
    QAction *actEd = new QAction(tr("Edit") , menu);
    actEd->setIcon(QIcon(":/katynko/svg/document-edit.svg"));
    actEd->setEnabled(hasSelItems);
    connect(actEd, SIGNAL(triggered(bool)), this, SLOT(onActEditLedLampTable()) );
    menu->addAction(actEd);

//    menu->addAction(createSetFilterAct(menu, 1));

    menu->addSeparator();


    if(youHaveApower && hasSelItems && getProtocolVersion() >= MATILDA_PROTOCOL_VERSION_V4 ){

        QAction *actCheckState = new QAction(tr("Selected: read the state") , menu);
        actCheckState->setIcon(QIcon(":/katynko/svg/flag-green.svg"));
        connect(actCheckState, SIGNAL(triggered(bool)), this, SLOT(checkLampState4selected())) ;
//        actCheckState->setEnabled(!gHelper->managerEnDisBttn.pbWriteDis);
        menu->addAction(actCheckState);

        QAction *actBlym = new QAction(tr("Selected: activate the beacon mode"), menu);
        actBlym->setIcon(QIcon(":/katynko/svg4/lc_extrusionlightingfloater.svg"));
        connect(actBlym, SIGNAL(triggered(bool)), this, SLOT(blymBlym4selected()) );

//        actBlym->setEnabled(!gHelper->managerEnDisBttn.pbWriteDis);

        menu->addAction(actBlym);


        if(getProtocolVersion() >= MATILDA_PROTOCOL_VERSION_V6 ){
            menu->addMenu(getMenuSelected(menu, youHaveApower, hasSelItems));
            menu->addMenu(getMenuAdditionalCommand(menu, youHaveApower, hasSelItems));
            menu->addMenu(getMenuTempSchedule(menu, youHaveApower, hasSelItems));

        }



        menu->addSeparator();
    }

    QAction *actRs = new QAction(tr("Reset sorting") , menu);
    actRs->setIcon(QIcon(":/katynko/svg/view-refresh.svg"));
    connect(actRs, SIGNAL(triggered(bool)), this, SLOT(onResetSortingAct()) );
    menu->addAction(actRs);


    menu->addSeparator();


    QAction *actSpon = new QAction(tr("Selected: Poll On") , menu);
    actSpon->setIcon(QIcon(":/katynko/svg/draw-cross.svg"));
    actSpon->setEnabled(hasSelItems);
    connect(actSpon, SIGNAL(triggered(bool)), this, SLOT(onActSelectedPollOnAddLedLampTable()) );
    menu->addAction(actSpon);

    QAction *actSpoff = new QAction(tr("Selected: Poll Off") , menu);
    actSpoff->setIcon(QIcon(":/katynko/svg/dialog-cancel.svg"));
    actSpoff->setEnabled(hasSelItems);
    connect(actSpoff, SIGNAL(triggered(bool)), this, SLOT(onActSelectedPollOffAddLedLampTable()) );
    menu->addAction(actSpoff);

    menu->addSeparator();



    QAction *actDs = new QAction(tr("Delete selected") , menu);
    actDs->setIcon(QIcon(":/katynko/svg3/user-trash-full.svg"));
    actDs->setEnabled(hasSelItems);
    connect(actDs, SIGNAL(triggered(bool)), this, SLOT(onDeleteSelectedAct()) );
    //    connect(actDs, SIGNAL(triggered(bool)), this, SLOT(sayModelChangedLater()) );

    menu->addAction(actDs);




    menu->exec(tv->mapToGlobal(pos));
    menu->deleteLater();

}

//--------------------------------------------------------------

void LedLampListWidget::onActEditLedLampTable()
{
    on_tvTable_doubleClicked(ui->tvTable->currentIndex());
}

//--------------------------------------------------------------

void LedLampListWidget::editLampFromRow(int row)
{
    /*void setPageSett(const QString &cbxAddLampModel,
     * const QString &leAddLampNI,
     * const QString &cbAddLampPhysicalGroup,
     * const QString &sbAddLampPstart,
     * const QString &sbAddLampPna,
     * const QString &sbAddLampTna,
     * const QString &leAddLampCoordinates,
     * const QString &cbAddLampOnOffPoll,
     * const QString &leAddLampStreet,
                     const QString &leAddLampMemo);
                     */
    if(row >= 0){
        editWdgt->setPageSett(model->item(row, 0)->data().toString(), //mdl Auto
                              model->item(row, 1)->text(), //NI
                              model->item(row, 2)->text(), //grpw
                              model->item(row, 3)->text(), //pstw
                              model->item(row, 4)->text(),//pnaw
                              model->item(row, 5)->text(), //tna
                              model->item(row, 6)->text(),//crdn
                              model->item(row, 7)->text(), //pll
                              model->item(row, 8)->text(), //strt
                              model->item(row, 9)->text()); //memo

    }else{
        editWdgt->addNewDevice();
    }
    editWdgt->showLater();
}

//--------------------------------------------------------------

void LedLampListWidget::checkLampState4selected()
{
    if(getProtocolVersion() < MATILDA_PROTOCOL_VERSION_V6){
        //do not do anything
        return;
    }
//a normal format
    const QStringList nis = getSelectedRowsText(1);
    if(nis.length() > 0){
        QAction *a = qobject_cast<QAction *>(QObject::sender());
        if(a)
            tryToSendAdditionalCommand(QVariantList() << POLL_CODE_FF_WRITE_LAMP_CONFIG_SMART << nis, a->text());
    }
}

//--------------------------------------------------------------

void LedLampListWidget::blymBlym4selected()
{

    QString s;
    if(!getSelectedNI4lampsSmart(s))
        return;

//    gHelper->mWrite2RemoteDevSlot(COMMAND_WRITE_ACTVT_BEACON_MODE, s, this);
}


//--------------------------------------------------------------

void LedLampListWidget::onActWriteSeleted()
{
    const QStringList nis = getSelectedNis(50);
    if(nis.isEmpty())
        return;


    int rowCount = 0;
    int row = 0;
    const QVariantList meters =  getPageSett(row, rowCount, MAX_PACKET_LEN).toVariantMap().value("m").toList();
    QHash<QString, QVariantList> ni2sett;

    for(int i = 0, imax = meters.size(); i < imax; i++){
        const QVariantList onelcu = meters.at(i).toList();
        if(nis.contains(onelcu.at(1).toString()))
            ni2sett.insert(onelcu.at(1).toString(), onelcu);
    }

    QVariantList l;
    for(int i = 0, imax = nis.size(), j = 0; i < imax && j < 50; i++, j++){
        if(ni2sett.contains(nis.at(i))){
            l.append(ni2sett.value(nis.at(i)));
            j++;
        }
    }

    InsertMeterDialog *dialog = new InsertMeterDialog(COMMAND_WRITE_FIREFLY_INSERT_LAMPS, l, this);
    connect(dialog, SIGNAL(data2matilda(quint16,QJsonObject)), this, SIGNAL(mWrite2RemoteDev(quint16,QJsonObject))) ;
    dialog->exec();
    dialog->deleteLater();


}

//--------------------------------------------------------------

void LedLampListWidget::onActWriteSeletedOn()
{
    QStringList nis = getSelectedNis(MAX_METER_COUNT);
    if(nis.isEmpty())
        return;
    nis.prepend("2");//2 mode by ni ? 1 mode by SN
//    gHelper->mWrite2RemoteDevSlot(COMMAND_WRITE_FIREFLY_LIST_POLL_ON, nis.join(" "));
}

//--------------------------------------------------------------

void LedLampListWidget::onActWriteSeletedOff()
{
    QStringList nis = getSelectedNis(MAX_METER_COUNT);
    if(nis.isEmpty())
        return;
    nis.prepend("2");//2 mode by ni ? 1 mode by SN
//    gHelper->mWrite2RemoteDevSlot(COMMAND_WRITE_FIREFLY_LIST_POLL_OFF, nis.join(" "));
}

//--------------------------------------------------------------

void LedLampListWidget::onActWriteSeletedDeletePart()
{
    QStringList l = getSelectedRowsText(1);


    int maxSize = MAX_METER_COUNT;

    if(l.size() > maxSize){
        emit showMessage(tr("Max size is %1. LCUs count is %2").arg(maxSize).arg(l.size()));
    }else{


        if(l.isEmpty())
            return;

        l.removeDuplicates();
        l.prepend("2");//2 mode by ni ? 1 mode by SN

//        gHelper->questionWithTimeDlgSlot(tr("Delete LCUs from the remote device"),
//                                         tr("Do you really want to delete these LCUs from the remote device?<br>This will delete these LCUs directly from the remote device.<br>Would you like to continue?"),
//                                         3, COMMAND_WRITE_FIREFLY_REMOVE_NIS, l.join(" ") );
    }
}

//--------------------------------------------------------------

void LedLampListWidget::onActAdditionalCommands()
{
    QAction *a = qobject_cast<QAction *>(QObject::sender());
    if(a){
        tryToSendAdditionalCommand(a->data().toList(), a->text());
    }
}

//--------------------------------------------------------------

void LedLampListWidget::tryToSendAdditionalCommand(const QVariantList &varl, const QString &text)
{
    if(varl.isEmpty())
        return;//error
    QVariantHash hash;
    hash.insert("code", varl.at(0));

    if(varl.size() > 1){
        QVariantMap map;
        map.insert("-ns", varl.at(1).toStringList());
        hash.insert("map", map);

        switch(hash.value("code").toInt()){

        case POLL_CODE_FF_WRITE_POWER_TO_GROUPS :
        case POLL_CODE_FF_WRITE_POWER_TO_LAMP   :{

            AddMinutesDialog *d = new AddMinutesDialog( 4, 100, tr("Power [%]"), tr("Off"), this);
            connect(d, &AddMinutesDialog::setInteger4selectedExt, this, &LedLampListWidget::setPower4selectedExt);
            hash.insert("name", text);
            d->setThisData(hash);

            d->exec();
            return;}
        }

//        gHelper->mWrite2RemoteDevSlot(COMMAND_WRITE_FIREFLY_START_POLL, hash, this);


    }
}

//--------------------------------------------------------------

void LedLampListWidget::onDeleteSelectedAct()
{
    QTableView *tv = ui->tvTable;

    QList<int> l;
    const QModelIndexList li = tv->selectionModel()->selectedRows();
    for(int i = 0, iMax = li.size(); i < iMax; i++)
        l.append(proxy_model->mapToSource(li.at(i)).row());

    std::sort(l.begin(), l.end());

    while(!l.isEmpty())
        model->removeRow(l.takeLast());
}

//--------------------------------------------------------------

void LedLampListWidget::onResetSortingAct()
{
    proxy_model->sort(-1);

}

//--------------------------------------------------------------

QMenu *LedLampListWidget::getMenuSelected(QWidget *prnt, const bool &youHaveApower, const bool &hasSelItems)
{
    QMenu *menu = new QMenu(tr("Remote device (for selected only)"), prnt);
    //    menu->setIcon(QIcon(":/katynko/svg3/relay-load-command.svg"));


    QAction *actInsrt = new QAction(tr("Insert to the remote device") , menu);
    actInsrt->setEnabled(hasSelItems && youHaveApower);
    connect(actInsrt, SIGNAL(triggered(bool)), SLOT(onActWriteSeleted()) );
    menu->addAction(actInsrt);


    QAction *actInsrtOn = new QAction(tr("Insert to the remote device with Poll On") , menu);
    actInsrtOn->setEnabled(hasSelItems && youHaveApower);
    connect(actInsrtOn, SIGNAL(triggered(bool)), SLOT(onActWriteSeletedOn()) );
    menu->addAction(actInsrtOn);


    QAction *actInsrtOff = new QAction(tr("Insert to the remote list device Poll Off") , menu);
    actInsrtOff->setEnabled(hasSelItems && youHaveApower);
    connect(actInsrtOff, SIGNAL(triggered(bool)), SLOT(onActWriteSeletedOff()) );
    menu->addAction(actInsrtOff);



    QAction *actInsrtDel = new QAction(tr("Delete from the remote list") , menu);
    actInsrtDel->setEnabled(hasSelItems && youHaveApower);
    connect(actInsrtDel, SIGNAL(triggered(bool)), SLOT(onActWriteSeletedDeletePart()) );
    menu->addAction(actInsrtDel);

    return menu;
}

//--------------------------------------------------------------


QMenu *LedLampListWidget::getMenuAdditionalCommand(QWidget *prnt, const bool &youHaveApower, const bool &hasSelItems)
{
    QMenu *menu = new QMenu(tr("Temporary operations (for selected only)"), prnt);


    QList<QAction*> actions;

    if(hasSelItems){
        const QStringList nis = getSelectedRowsText(1);
        if(nis.length() > 1){
            actions.append(new QAction(tr("Dimm the lamps '%1'").arg(nis.length()), menu));
            actions.last()->setData(QVariantList() << POLL_CODE_FF_WRITE_POWER_TO_LAMP << nis);
        }

        const QStringList grps = getUniqGroupIds();
        if(grps.length() > 1){
            actions.append(new QAction(tr("Dimm the groups '%1'").arg(grps.length()), menu));
            actions.last()->setData(QVariantList() << POLL_CODE_FF_WRITE_POWER_TO_GROUPS << grps);
        }


    }


    const int srcrow = proxy_model->mapToSource(ui->tvTable->currentIndex()).row();
    if(srcrow >= 0){
        const QString memo = model->item(srcrow, getKeysLedLampV2().indexOf("memo"))->text();


        QStringList nis = QStringList() << model->item(srcrow, 1)->text();

        actions.append(new QAction(tr("Dimm the lamp with NI '%1'%2").arg(nis.last()).arg(memo.isEmpty() ? QString() : tr(", '%1'").arg(memo)), menu));
        actions.last()->setData(QVariantList() << POLL_CODE_FF_WRITE_POWER_TO_LAMP << nis);


        QStringList grps = QStringList() << model->item(srcrow, 2)->text();//grpw

        const int matches = getMatches4ThisCellText(2, grps.last(), -1);

        actions.append(new QAction(tr("Dimm the group with ID '%1', %2").arg(grps.last()).arg((matches > 1) ? tr("'%1' lamps").arg(matches) : tr("'%1'").arg(memo)), menu));
        actions.last()->setData(QVariantList() << POLL_CODE_FF_WRITE_POWER_TO_GROUPS << grps);



    }

    actions.append(new QAction(tr("Cancel the lamp dimming"), menu));
    actions.last()->setData(QVariantList() << POLL_CODE_FF_WRITE_POWER_TO_LAMP);

    actions.append(new QAction(tr("Cancel the group dimming"), menu));
    actions.last()->setData(QVariantList() << POLL_CODE_FF_WRITE_POWER_TO_GROUPS);


    for(int i = 0, imax = actions.size(); i < imax; i++){
        connect(actions.at(i), SIGNAL(triggered(bool)), this, SLOT(onActAdditionalCommands()));
        actions.at(i)->setEnabled(youHaveApower);
        menu->addAction(actions.at(i));
    }

    return menu;
}

//--------------------------------------------------------------

QMenu *LedLampListWidget::getMenuTempSchedule(QWidget *prnt, const bool &youHaveApower, const bool &hasSelItems)
{
    QMenu *menu = new QMenu(tr("Temporary schedule"), prnt);

    QAction *actTempSchedule = new QAction(tr("Set temporary schedule for selected"), menu);
    connect(actTempSchedule, SIGNAL(triggered(bool)), this, SLOT(showDlgSetupTempPower4schedule()));
    actTempSchedule->setEnabled(youHaveApower && hasSelItems);
    menu->addAction(actTempSchedule);


    QAction *actTempSchedule2 = new QAction(tr("Reset temporary schedule for selected"), menu);
    connect(actTempSchedule2, SIGNAL(triggered(bool)), this, SLOT(resetSelectedTempPower4schedule()));
    actTempSchedule2->setEnabled(youHaveApower);
    menu->addAction(actTempSchedule2);

    QAction *actTempSchedule3 = new QAction(tr("Reset temporary schedule for all"), menu);
    connect(actTempSchedule3, SIGNAL(triggered(bool)), this, SLOT(resetAllTempPower4schedule()));
    actTempSchedule3->setEnabled(youHaveApower);
    menu->addAction(actTempSchedule3);

    return menu;
}

//--------------------------------------------------------------
