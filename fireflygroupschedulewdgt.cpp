#include "fireflygroupschedulewdgt.h"
#include "ui_fireflygroupschedulewdgt.h"

FireFlyGroupScheduleWdgt::FireFlyGroupScheduleWdgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FireFlyGroupScheduleWdgt)
{
    ui->setupUi(this);
    initPage();
}

FireFlyGroupScheduleWdgt::~FireFlyGroupScheduleWdgt()
{
    delete ui;
}

void FireFlyGroupScheduleWdgt::setupObjectTv(QTableView *tv)
{
    tv->setAlternatingRowColors(true);
    tv->setSelectionBehavior(QAbstractItemView::SelectRows);
    tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tv->setSortingEnabled(true);
    tv->setCornerButtonEnabled(true);
    tv->setCornerWidget(new QWidget());
    tv->setContextMenuPolicy(Qt::CustomContextMenu);
}

QJsonObject FireFlyGroupScheduleWdgt::getPageSettings()
{
    //I dont know the format

    return lastjson;
}

QStringList FireFlyGroupScheduleWdgt::getHeaderProfiles()
{
    return tr("Profile name;Line").split(";");
}

QStringList FireFlyGroupScheduleWdgt::getHeaderGroups()
{
    return tr("Group;Season MDD;Memo;Sunday;Monday;Tuesday;Wednesday;Thursday;Friday;Saturday").split(";");
}

QStringList FireFlyGroupScheduleWdgt::getHeaderSpecialDays()
{
    return tr("Settings;Memo;groups").split(";");
}

void FireFlyGroupScheduleWdgt::addGroups(const QJsonArray &lcugrp)
{
    /*

     *gpr \t ssns(season names space separated) \t
s(season name0) \t <season days profiles names \t separated, 7 days>  \t <season memo>
s(season nameX) \t <season days profiles names \t separated, 7 days>  \t <season memo>

season name is MDD, where
DD - day of the month {01-31}
M - month number, {1-12},  1 - January, 12 - December

                {
                "m101":"радій",
                "m907":"радійзима",
                "s101":["asdf","asdf","asdf","asdf","asdf","asdf","asdf"],
                "s907":["офіс","офіс","офіс","офіс","офіс","офіс","офіс"],
                "ssns":"101 907"
                },
     *
     */

    for(int i = 0, imax = lcugrp.size(); i < imax; i++){
        const QStringList onegrpl = lcugrp.at(i).toString().split("\t");

        if(onegrpl.size() < 8){
            continue;//broken
        }

        const QString grpid = onegrpl.at(0);

        const QStringList ssns = onegrpl.at(1).split(" ", QString::SkipEmptyParts);

        if(ssns.isEmpty()){
            continue;//broken
        }

        for(int j = 2, jmax = onegrpl.size(), s = 0, smax = ssns.size(); s < smax; s++){



            const QString mdd = ssns.at(s);




            QStringList seasondayprofiles;
            for(int n = 0 ; n < 8 && j < jmax; j++, n++)
                seasondayprofiles.append(onegrpl.at(j));

            if(seasondayprofiles.size() != 8){
               continue;
            }

            if(seasondayprofiles.first() != mdd)
                continue;//something is wrong
            seasondayprofiles.removeFirst();//it is just for check

            QList<QStandardItem*> li;
            li.append(new QStandardItem(grpid));
            li.append(new QStandardItem(mdd));

            if(j < jmax){
                li.append(new QStandardItem(onegrpl.at(j)));//append memo
                j++;
            }

            for(int cc = 0, ccmax = seasondayprofiles.size(); cc < ccmax; cc++)
                li.append(new QStandardItem(seasondayprofiles.at(cc)));


            modelGroups->appendRow(li);

        }


    }


}

void FireFlyGroupScheduleWdgt::addDayProfiles(const QJsonArray &lcuprf)
{
    for(int i = 0, imax = lcuprf.size(); i < imax; i++){
        const QStringList oneprofile = lcuprf.at(i).toString().split("\t");
        if(oneprofile.size() < 2)
            continue;
        QList<QStandardItem*> li;
        for(int j = 0, jmax = oneprofile.size(); j < jmax; j++)
            li.append(new QStandardItem(oneprofile.at(j)));
        modelProfiles->appendRow(li);
    }
}

void FireFlyGroupScheduleWdgt::addSpecial(const QJsonArray &lcusp)
{
    /*
    "****042707***7119":
            {
                    "grp_idl":"4,5,6,7,121,255",
                    "memo":"",
                    "s121":"signal work days",
                    "s255":"asdf",
                    "s4":"my lamp profile",
                    "s5":"my lamp profile",
                    "s6":"Monday A",
                    "s7":"зворотній до сходу-заходу"
<sp name sett>\t<grp ids space separated>\t<memo>\t<grp id0 profile name>\t<grp idx profile name>

    },
     */

    for(int i = 0, imax = lcusp.size(); i < imax; i++){
        const QStringList onesp = lcusp.at(i).toString().split("\t");
        if(onesp.size() < 4){

            continue;

        }

        const QString spprofilename = onesp.at(0);
        const QStringList grp_idl = onesp.at(1).split(" ", QString::SkipEmptyParts);

        QList<QStandardItem*> li;
        li.append(new QStandardItem(spprofilename));
        li.append(new QStandardItem(onesp.at(2)));


        QStringList grp2profile;
        for(int j = 0, jmax = grp_idl.size(), s = 3, smax = onesp.size(); j < jmax && s < smax; j++, s++){
            grp2profile.append(QString("%1 - '%2'").arg(grp_idl.at(j)).arg(onesp.at(s)));
        }

        li.append(new QStandardItem(grp2profile.join(", ")));

        modelSpecial->appendRow(li);
    }
}

void FireFlyGroupScheduleWdgt::setModelHorizontalHeaderItems(const QStringList &header, QStandardItemModel *model)
{
    model->clear();
    for(int i = 0, iMax = header.size(); i < iMax; i++)
        model->setHorizontalHeaderItem(i, new QStandardItem(header.at(i)));
}

void FireFlyGroupScheduleWdgt::initPage()
{
    modelGroups = new QStandardItemModel(this);
    proxyGroups = new MySortFilterProxyModel(this);
    proxyGroups->setSourceModel(modelGroups);
    ui->tvSchedule->setModel(proxyGroups);
    setupObjectTv(ui->tvSchedule);


    modelProfiles = new QStandardItemModel(this);
    proxyProfiles = new MySortFilterProxyModel(this);
    proxyProfiles->setSourceModel(modelProfiles);
    ui->tvProfiles->setModel(proxyProfiles);
    setupObjectTv(ui->tvProfiles);

    modelSpecial = new QStandardItemModel(this);
    proxySpecial = new MySortFilterProxyModel(this);
    proxySpecial->setSourceModel(modelSpecial);
    ui->tvSpecial->setModel(proxySpecial);
    setupObjectTv(ui->tvSpecial);

    clearPage();

}

void FireFlyGroupScheduleWdgt::clearPage()
{
    setModelHorizontalHeaderItems(getHeaderGroups(), modelGroups);
    setModelHorizontalHeaderItems(getHeaderProfiles(), modelProfiles);
    setModelHorizontalHeaderItems(getHeaderSpecialDays(), modelSpecial);

    proxyGroups->sort(-1);
    proxyProfiles->sort(-1);
    proxyProfiles->sort(-1);
}

void FireFlyGroupScheduleWdgt::setPageSett(QJsonObject json)
{
    this->lastjson = json;

    clearPage();

    addGroups(json.value("lcugrp").toArray());
    addDayProfiles(json.value("lcuprf").toArray());
    addSpecial(json.value("lcusp").toArray());


    ui->tvProfiles->resizeColumnsToContents();
    ui->tvSchedule->resizeColumnsToContents();
    ui->tvSpecial->resizeColumnsToContents();

}

void FireFlyGroupScheduleWdgt::onEditProfiles(const QModelIndex &index)
{
    onEditProfilesRow(proxyProfiles->mapToSource(index).row());
}

void FireFlyGroupScheduleWdgt::onEditGroups(const QModelIndex &index)
{
    onEditGroupsRow(proxyGroups->mapToSource(index).row());
}

void FireFlyGroupScheduleWdgt::onEditSpecial(const QModelIndex &index)
{
    onEditSpecialRow(proxySpecial->mapToSource(index).row());
}

void FireFlyGroupScheduleWdgt::onEditProfilesRow(const int &srcrow)
{

}

void FireFlyGroupScheduleWdgt::onEditGroupsRow(const int &srcrow)
{

}

void FireFlyGroupScheduleWdgt::onEditSpecialRow(const int &srcrow)
{

}
