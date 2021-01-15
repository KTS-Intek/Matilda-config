#include "addminutesdialog.h"

AddMinutesDialog::AddMinutesDialog(const int &min, const int &max, const QString &text, const QString &specTxt, QWidget *parent) :
    IntegerEditDlg(min, max, text, specTxt, -1, 0, parent)
{
   connect(this, SIGNAL(setInteger4selected(int,int)), this, SIGNAL(setMinutes(int)));
}
