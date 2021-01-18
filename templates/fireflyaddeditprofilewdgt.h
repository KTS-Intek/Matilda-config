#ifndef FIREFLYADDEDITPROFILEWDGT_H
#define FIREFLYADDEDITPROFILEWDGT_H

#include <QWidget>

namespace Ui {
class FireflyAddEditProfileWdgt;
}

class FireflyAddEditProfileWdgt : public QWidget
{
    Q_OBJECT

public:
    explicit FireflyAddEditProfileWdgt(QWidget *parent = nullptr);
    ~FireflyAddEditProfileWdgt();

private slots:
    void on_pushButton_clicked();

    void on_pbAddGroup_3_clicked();

private:
    Ui::FireflyAddEditProfileWdgt *ui;
};

#endif // FIREFLYADDEDITPROFILEWDGT_H
