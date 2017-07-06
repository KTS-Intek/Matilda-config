#ifndef LANGDIALOG_H
#define LANGDIALOG_H

#include <QDialog>
#include <QComboBox>

namespace Ui {
class LangDialog;
}

class LangDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LangDialog(QComboBox *cbx, QWidget *parent = 0);
    ~LangDialog();

private slots:
    void on_buttonBox_accepted();


    void on_cbLang_currentIndexChanged(int index);

signals:
    void onLangSelected(QString lang);

private:
    Ui::LangDialog *ui;
    QString defLblTxt;
};

#endif // LANGDIALOG_H
