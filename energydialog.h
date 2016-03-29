#ifndef ENERGYDIALOG_H
#define ENERGYDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "mysortfilterproxymodel.h"


namespace Ui {
class EnergyDialog;
}

class EnergyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EnergyDialog(const QStringList &lName, const QStringList &lValue, QWidget *parent = 0);
    ~EnergyDialog();


signals:
    void onPhysValChanged(QStringList,QStringList);

private slots:
    void on_pbAddNew_clicked();

    void on_tvAvailable_doubleClicked(const QModelIndex &index);

    void on_tvProfileEnrg_clicked(const QModelIndex &index);

    void on_tvAvailable_customContextMenuRequested(const QPoint &pos);

    void resetSortingTvPhysVal();
    void editTvPhysVal();
    void onRecoverPhysVal_clicked();

    void delSelectedPhysVal();

    void on_buttonBox_accepted();

private:
    Ui::EnergyDialog *ui;

    QStandardItemModel *modelAvProf;
    QStandardItemModel *modelProfiles;

    MySortFilterProxyModel *proxy_modelAvProf;

    QList<QVariant> lRecoverLvDevHistory;

    QStringList ln, ld;

};

#endif // ENERGYDIALOG_H
