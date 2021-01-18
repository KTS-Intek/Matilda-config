#include "instantpower4lampswdgt.h"
#include "ui_instantpower4lampswdgt.h"

#include <QJsonArray>
#include "src/firefly/convert2humanreadablefirefly.h"

InstantPower4lampsWdgt::InstantPower4lampsWdgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstantPower4lampsWdgt)
{
    ui->setupUi(this);
    initPage();
}

InstantPower4lampsWdgt::~InstantPower4lampsWdgt()
{
    delete ui;
}

void InstantPower4lampsWdgt::setupObjectTv(QTableView *tv)
{
    tv->setAlternatingRowColors(true);
    tv->setSelectionBehavior(QAbstractItemView::SelectRows);
    tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tv->setSortingEnabled(true);
    tv->setCornerButtonEnabled(true);
    tv->setCornerWidget(new QWidget());
    tv->setContextMenuPolicy(Qt::CustomContextMenu);
}

QJsonObject InstantPower4lampsWdgt::getPageSettings()
{
    QJsonArray dtsl;
    QJsonArray sdp;

    QJsonObject json;
    for(int i = 0, imax = modelGroups->rowCount(); i < imax; i++){
        dtsl.append(QString("%1\t%2")
                    .arg(modelGroups->item(i,0)->text())
                    .arg(modelGroups->item(i,1)->text())
                    );
        sdp.append(QString("%1\t%2")
                   .arg(modelGroups->item(i,0)->text())
                   .arg(modelGroups->item(i,3)->text())
                   );
    }

    json.insert("dtsl", dtsl);
    json.insert("sdp", sdp);

    return json;
}

QStringList InstantPower4lampsWdgt::getHeaderGroups()
{
    return tr("Group ID,Date time source,Date time human,Power source,Power human").split(",");
}

void InstantPower4lampsWdgt::setModelHorizontalHeaderItems(const QStringList &header, QStandardItemModel *model)
{
    model->clear();
    for(int i = 0, iMax = header.size(); i < iMax; i++)
        model->setHorizontalHeaderItem(i, new QStandardItem(header.at(i)));
}

void InstantPower4lampsWdgt::initPage()
{

    modelGroups = new QStandardItemModel(this);
    proxyGroups = new MySortFilterProxyModel(this);
    proxyGroups->setSourceModel(modelGroups);
    ui->tableView->setModel(proxyGroups);
    setupObjectTv(ui->tableView);


    clearPage();
}

void InstantPower4lampsWdgt::clearPage()
{
    setModelHorizontalHeaderItems(getHeaderGroups(), modelGroups);
    proxyGroups->sort(-1);

}

void InstantPower4lampsWdgt::setPageSett(QJsonObject json)
{

    const QJsonArray dtsl = json.value("dtsl").toArray();
    const QJsonArray sdp = json.value("sdp").toArray();

    if(dtsl.size() != sdp.size()){
        emit showMessage(tr("Bad arrays"));
        return;

    }

    clearPage();
    QHash<QString,qint64> hashGrp2msec;

    for(int i = 0, imax = dtsl.size(); i < imax; i++){
        const QStringList grp2msec = dtsl.at(i).toString().split("\t");
        hashGrp2msec.insert(grp2msec.first(), grp2msec.last().toLongLong());


    }

    for(int i = 0, imax = sdp.size(); i < imax; i++){

        QList<QStandardItem*> li;
//        return tr("Group ID,Date time source,Date time human,Power source").split(",");

        const QStringList grp2profile = sdp.at(i).toString().split("\t");
        const QString grpid = grp2profile.first();

        li.append(new QStandardItem(grpid));
        li.append(new QStandardItem(QString::number(hashGrp2msec.value(grpid))));
        li.append(new QStandardItem(QDateTime::fromMSecsSinceEpoch(hashGrp2msec.value(grpid)).toString("yyyy-MM-dd hh:mm:ss")));
        li.append(new QStandardItem(grp2profile.last()));
        li.append(new QStandardItem(Convert2humanReadableFirefly::scheduleMinutesToHourMinutesLine(grp2profile.last())));

        modelGroups->appendRow(li);
    }

    //I dont know

}

void InstantPower4lampsWdgt::onEditProfiles(const QModelIndex &index)
{
    onEditProfilesRow(proxyGroups->mapToSource(index).row());

}

void InstantPower4lampsWdgt::onEditProfilesRow(const int &srcrow)
{

}

void InstantPower4lampsWdgt::on_tableView_doubleClicked(const QModelIndex &index)
{
    onEditProfiles(index);
}
