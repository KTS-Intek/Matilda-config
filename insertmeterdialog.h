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
    explicit InsertMeterDialog(QVariantList hm, QWidget *parent = 0);
    ~InsertMeterDialog();


signals:
    void data2matilda(quint16 , QVariantHash );


private slots:
    void on_buttonBox_accepted();

private:
    Ui::InsertMeterDialog *ui;
    QVariantList hashmeter;
};

#endif // INSERTMETERDIALOG_H
