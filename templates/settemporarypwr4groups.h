#ifndef SETTEMPORARYPWR4GROUPS_H
#define SETTEMPORARYPWR4GROUPS_H

#include <QDialog>
#include <QSpinBox>
#include <QTimeEdit>
#include <QSlider>


namespace Ui {
class SetTemporaryPwr4groups;
}

class SetTemporaryPwr4groups : public QDialog
{
    Q_OBJECT

public:
    explicit SetTemporaryPwr4groups(const QString &datemask, const QString &timemask, const QStringList &groupslist, QWidget *parent = nullptr);
    ~SetTemporaryPwr4groups();

    QStringList groupslist;
    QString datemask;

    void connectSbAndTimeEdit(QTimeEdit *te, QSlider *sb);

    void connectSbAndSlider(QSlider *se, QSpinBox *sb);

signals:
    void setTempSchedule4theseGroups(QStringList groups, int power, int minutesfrom, int minutesto);

    void startTmrShowLbl();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void updateDateTimeLbls();



private:
    Ui::SetTemporaryPwr4groups *ui;
};

#endif // SETTEMPORARYPWR4GROUPS_H
