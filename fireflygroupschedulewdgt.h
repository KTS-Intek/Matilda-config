#ifndef FIREFLYGROUPSCHEDULEWDGT_H
#define FIREFLYGROUPSCHEDULEWDGT_H

#include <QWidget>
#include <QMenu>
#include <QStandardItemModel>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTableView>
#include <QJsonArray>

#include "mysortfilterproxymodel.h"



namespace Ui {
class FireFlyGroupScheduleWdgt;
}

class FireFlyGroupScheduleWdgt : public QWidget
{
    Q_OBJECT

public:
    explicit FireFlyGroupScheduleWdgt(QWidget *parent = nullptr);
    ~FireFlyGroupScheduleWdgt();

     void setupObjectTv(QTableView *tv);

     QJsonObject getPageSettings();

     QStringList getHeaderProfiles();

     QStringList getHeaderGroups();

     QStringList getHeaderSpecialDays();

     void addGroups(const QJsonArray &lcugrp);

     void addDayProfiles(const QJsonArray &lcuprf);

     void addSpecial(const QJsonArray &lcusp);

     void setModelHorizontalHeaderItems(const QStringList &header, QStandardItemModel *model);

public slots:
    void initPage();

    void clearPage();

    void setPageSett(QJsonObject json);

    void onEditProfiles(const QModelIndex &index);

    void onEditGroups(const QModelIndex &index);

    void onEditSpecial(const QModelIndex &index);



    void onEditProfilesRow(const int &srcrow);

    void onEditGroupsRow(const int &srcrow);

    void onEditSpecialRow(const int &srcrow);



private:
    Ui::FireFlyGroupScheduleWdgt *ui;

    QStandardItemModel *modelProfiles;
    QStandardItemModel *modelGroups;
    QStandardItemModel *modelSpecial;

    MySortFilterProxyModel *proxyProfiles;
    MySortFilterProxyModel *proxyGroups;
    MySortFilterProxyModel *proxySpecial;


    QJsonObject lastjson;
};

#endif // FIREFLYGROUPSCHEDULEWDGT_H
