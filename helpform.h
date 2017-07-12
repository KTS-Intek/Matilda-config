#ifndef HELPFORM_H
#define HELPFORM_H

#include <QWidget>

//#include <QtWebKit>
//#include <QWebView>

#include <QTimer>
#include "webtextdocument.h"

namespace Ui {
class HelpForm;
}

class HelpForm : public QWidget
{
    Q_OBJECT

public:
    explicit HelpForm(QWidget *parent = 0);
    ~HelpForm();

private slots:
    void zoomDec();
    void zoomInc();
    void zoomNorm();
    void slotFinished(bool bOk);

    void zoomChanged();

    void loadSettings();
    void saveSettings();

    void on_goHome_clicked();

    void loadAvLang();

    void reloadLangSett();

    void on_toolButton_6_clicked();

    void on_toolButton_4_clicked();

    void on_pushButton_clicked();

    void on_leFindPteLog_2_returnPressed();

private:
    Ui::HelpForm *ui;

    WebTextDocument *myWeb;
    QTimer timerDec;
    QTimer timerInc;


};

#endif // HELPFORM_H
