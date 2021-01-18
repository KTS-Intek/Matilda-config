#ifndef RESCUESCHEDULESWDGT_H
#define RESCUESCHEDULESWDGT_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>

namespace Ui {
class RescueSchedulesWdgt;
}

class RescueSchedulesWdgt : public QWidget
{
    Q_OBJECT

public:
    explicit RescueSchedulesWdgt(QWidget *parent = nullptr);
    ~RescueSchedulesWdgt();

    QJsonObject getPageSettings();

public slots:
    void initPage();

    void clearPage();

    void setPageSett(QJsonObject json);


private:
    Ui::RescueSchedulesWdgt *ui;
};

#endif // RESCUESCHEDULESWDGT_H
