#ifndef INTEGEREDITDLG_H
#define INTEGEREDITDLG_H

#include <QDialog>
#include <QVariant>

#include "insermodeselector.h"

namespace Ui {
class IntegerEditDlg;
}

class IntegerEditDlg : public QDialog
{
    Q_OBJECT

public:
    explicit IntegerEditDlg(const int &min, const int &max, const QString &text, const QString &specTxt, const int &mode, const int &tag, QWidget *parent = nullptr);
    ~IntegerEditDlg();

    void setThisData(QVariant data);

signals:
    void setInteger(int value, int mode, int tag);

    void setInteger4selected(int value, int tag);

    void setInteger4visible(int value, int tag);

    void setInteger4all(int value, int tag);


    void setIntegerExt(int value, int mode, int tag, QVariant data);

    void setInteger4selectedExt(int value, int tag, QVariant data);

    void setInteger4visibleExt(int value, int tag, QVariant data);

    void setInteger4allExt(int value, int tag, QVariant data);




private slots:
    void on_buttonBox_accepted();

private:
    Ui::IntegerEditDlg *ui;
    InserModeSelector *modeselector;

    int myTag;
    QVariant data;
};

#endif // INTEGEREDITDLG_H
