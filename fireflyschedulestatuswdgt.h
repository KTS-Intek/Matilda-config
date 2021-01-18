#ifndef FIREFLYSCHEDULESTATUSWDGT_H
#define FIREFLYSCHEDULESTATUSWDGT_H

#include <QWidget>
#include <QMenu>
#include <QStandardItemModel>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTableView>
#include "mysortfilterproxymodel.h"


namespace Ui {
class FireflyScheduleStatusWdgt;
}

class FireflyScheduleStatusWdgt : public QWidget
{
    Q_OBJECT

public:
    explicit FireflyScheduleStatusWdgt(QWidget *parent = nullptr);
    ~FireflyScheduleStatusWdgt();



    QStringList getValidFireflyScheduleStatus(const QString &line, const QString &prefix);

    QStringList getGroupPower(const QJsonArray &insline);

public slots:
    void clearPage();
    void setPageData(QJsonObject json);

private:
    Ui::FireflyScheduleStatusWdgt *ui;

    MySortFilterProxyModel *proxy_model;
    QStandardItemModel *model;
};

#endif // FIREFLYSCHEDULESTATUSWDGT_H
