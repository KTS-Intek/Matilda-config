#ifndef ADDEDITLAMPDLG_H
#define ADDEDITLAMPDLG_H

#include <QDialog>
#include <QSpinBox>

namespace Ui {
class AddEditLampDlg;
}

class AddEditLampDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AddEditLampDlg(QWidget *parent = nullptr);
    ~AddEditLampDlg();

    QString getWriteVal(const QString &itmTxt);

    int getSbValue(QSpinBox *s);

    int getTna() ;

    int getPna();

    int getPstart();

signals:
    void addLamp(QString lamp_ni, int grp_id, QString crdnts, QString memo, QString model, QVariant modelData, QString sbAddLampPstart, QString sbAddLampPna, QString sbAddLampTna, QString cbAddLampOnOffPoll, QString leAddLampStreet, bool quite);

public slots:
    void clearPage();

    void addNewDevice();

    void setPageSett(const QString &cbxAddLampModel, const QString &leAddLampNI, const QString &cbAddLampPhysicalGroup, const QString &sbAddLampPstart, const QString &sbAddLampPna, const QString &sbAddLampTna, const QString &leAddLampCoordinates, const QString &cbAddLampOnOffPoll, const QString &leAddLampStreet,
                     const QString &leAddLampMemo);


    void showLater();

private slots:
    void on_pushButton_clicked();

    void on_pbAddLamp_clicked();


    void checkShowWarning();
private:
    Ui::AddEditLampDlg *ui;

    QString pgOpenedNI;

};

#endif // ADDEDITLAMPDLG_H
