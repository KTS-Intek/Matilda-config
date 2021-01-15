#ifndef ADDMINUTESDIALOG_H
#define ADDMINUTESDIALOG_H


#include "integereditdlg.h"


class AddMinutesDialog : public IntegerEditDlg
{
    Q_OBJECT

public:
    explicit AddMinutesDialog(const int &min, const int &max, const QString &text, const QString &specTxt, QWidget *parent = nullptr);


signals:
    void setMinutes(int minutes);






};

#endif // ADDMINUTESDIALOG_H
