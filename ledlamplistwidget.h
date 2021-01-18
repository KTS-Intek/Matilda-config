#ifndef LEDLAMPLISTWIDGET_H
#define LEDLAMPLISTWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QStandardItemModel>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTableView>

#include "mysortfilterproxymodel.h"

#include "addeditlampdlg.h"


namespace Ui {
class LedLampListWidget;
}

class LedLampListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LedLampListWidget(QWidget *parent = nullptr);
    ~LedLampListWidget();


    int protocolVersion;

    bool canWriteOperatorCommands;
    void setupObjectTv(QTableView *tv);

    QString map2jsonLine(const QVariantMap &map);

    QVariantMap getPower2groups(const QMap<int, int> &groupId2power);
    QVariantMap getPower2groupsExt(const QMap<int, int> &groupId2power, const bool &isPowerInPercents);


    QStringList getKeysLedLampV2();

    int getProtocolVersion();

    int getMatches4ThisCellText(const int &col, const QString &celltxt, const int &limit);
    int getMatches4TheseCellsText(const QList<int> &cols, const QStringList &celltxtl, const int &limit);


    QStringList getSelectedRowsText( const int &col);

    void setModelHorizontalHeaderItems(const QStringList &header);

    bool getSelectedNI4lampsSmart(QString &s);
    QStringList getSelectedNis(const int &maxsize);


    QStringList getUniqGroupIds();

    //    QVariantHash getPageSett(bool &eraseAll);


    QJsonObject getPageSett(int &row, int &rowCount, const int &maxLen);


    QStringList varList2strList(const QVariantList &list);

signals:
    void showMessage(QString message);

    void mWrite2RemoteDev(quint16 command, QJsonObject jobj);


public slots:
    void initPage();

    void clearPage();

    void onReadingFinished();

    void setPageSett(const QJsonObject &jobj);


    void onActSelectedPollOnAddLedLampTable();

    void onActSelectedPollOffAddLedLampTable();

    void addLamp(QString lamp_ni, int grp_id, QString crdnts, QString memo, QString lampmodel, QVariant modelData, QString sbAddLampPstart, QString sbAddLampPna, QString sbAddLampTna, QString cbAddLampOnOffPoll, QString leAddLampStreet, bool quite);

    void showDlgSetupTempPower4schedule();
    void resetSelectedTempPower4schedule();
    void resetAllTempPower4schedule();

    void setTempSchedule4theseGroups(QStringList groups, int power, int minutesfrom, int minutesto);

    void sendCOMMAND_WRITE_ADD_DEL_TEMPSCHEDULE(const QStringList &sdp, const QStringList &dtsl, const QStringList &idsr);


    void setProtocolVersion(int version);
    void setCanWriteOperatorCommands(bool canwrite);

    void setPower4selectedExt(int value, int tag, QVariant data);


private slots:
    void on_pbAddLamp_clicked();

    void on_tvTable_doubleClicked(const QModelIndex &index);

    void on_tvTable_customContextMenuRequested(const QPoint &pos);

    void onActEditLedLampTable();

    void editLampFromRow(int row);

    void checkLampState4selected();

    void blymBlym4selected();



    void onActWriteSeleted();

    void onActWriteSeletedOn();

    void onActWriteSeletedOff();

    void onActWriteSeletedDeletePart();



    void tryToSendAdditionalCommand(const QVariantList &varl, const QString &text);

    void onDeleteSelectedAct();

    void onResetSortingAct();

private:
    Ui::LedLampListWidget *ui;

    AddEditLampDlg *editWdgt;
    MySortFilterProxyModel *proxy_model;
    QStandardItemModel *model;


    QMenu *getMenuSelected(QWidget *prnt, const bool &youHaveApower, const bool &hasSelItems);


    QMenu *getMenuTempSchedule(QWidget *prnt, const bool &youHaveApower, const bool &hasSelItems);

    QStringList lastColumnsSequence;

};

#endif // LEDLAMPLISTWIDGET_H
