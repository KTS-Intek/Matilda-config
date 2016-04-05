#ifndef WAIT4ANSWERDIALOG_H
#define WAIT4ANSWERDIALOG_H

#include <QDialog>

namespace Ui {
class Wait4AnswerDialog;
}

class Wait4AnswerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Wait4AnswerDialog(int defH, QWidget *parent = 0);
    ~Wait4AnswerDialog();

signals:
    void stopTimer();
    void startTimr();
    void noAnswerFromDev();
    void stopNow();

public slots:
    void showAnimation(int count);
    void hideAnimation();
//    void uploadProgress(qreal val);
    void uploadProgress(int val, QString txt);

    void resetCounter();


private slots:
    void onTimerTo();



    void on_pushButton_clicked();

private:
    QString rotateStr();

    Ui::Wait4AnswerDialog *ui;
    int counter;
    int counter2;
    int maxH;
    int rotateCounter;

    int defCount;
};

#endif // WAIT4ANSWERDIALOG_H
