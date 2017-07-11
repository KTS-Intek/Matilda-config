#ifndef HELPFORM_H
#define HELPFORM_H

#include <QWidget>
#include <QWebEngineView>

//#include <QtWebKit>
//#include <QWebView>

#include <QTimer>


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

    QWebEngineView *myWeb;
    QTimer timerDec;
    QTimer timerInc;


};

#endif // HELPFORM_H
