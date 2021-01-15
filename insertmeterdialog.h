#ifndef INSERTMETERDIALOG_H
#define INSERTMETERDIALOG_H

#include <QDialog>
#include <QtCore>

namespace Ui {
class InsertMeterDialog;
}

class InsertMeterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InsertMeterDialog(quint16 command, QVariantList hm, QWidget *parent = 0);
    ~InsertMeterDialog();


signals:
    void data2matilda(quint16 , QJsonObject );


private slots:
    void on_buttonBox_accepted();

private:
    Ui::InsertMeterDialog *ui;
    QVariantList hashmeter;
    quint16 wcommand;
};

#endif // INSERTMETERDIALOG_H
