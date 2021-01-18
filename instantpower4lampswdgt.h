#ifndef INSTANTPOWER4LAMPSWDGT_H
#define INSTANTPOWER4LAMPSWDGT_H

#include <QWidget>
#include <QMenu>
#include <QStandardItemModel>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTableView>

#include "mysortfilterproxymodel.h"



namespace Ui {
class InstantPower4lampsWdgt;
}

class InstantPower4lampsWdgt : public QWidget
{
    Q_OBJECT

public:
    explicit InstantPower4lampsWdgt(QWidget *parent = nullptr);
    ~InstantPower4lampsWdgt();

    void setupObjectTv(QTableView *tv);
    QJsonObject getPageSettings();


    QStringList getHeaderGroups();

    void setModelHorizontalHeaderItems(const QStringList &header, QStandardItemModel *model);

signals:
    void showMessage(QString message);


public slots:
    void initPage();

    void clearPage();

    void setPageSett(QJsonObject json);

    void onEditProfiles(const QModelIndex &index);

    void onEditProfilesRow(const int &srcrow);


private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::InstantPower4lampsWdgt *ui;

    QStandardItemModel *modelGroups;
    MySortFilterProxyModel *proxyGroups;



};

#endif // INSTANTPOWER4LAMPSWDGT_H
