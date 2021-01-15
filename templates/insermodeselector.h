#ifndef INSERMODESELECTOR_H
#define INSERMODESELECTOR_H

#include <QWidget>

namespace Ui {
class InserModeSelector;
}

class InserModeSelector : public QWidget
{
    Q_OBJECT

public:
    explicit InserModeSelector(const int &mode, QWidget *parent = nullptr);
    ~InserModeSelector();

    int getInsertMode();

private:
    Ui::InserModeSelector *ui;
};

#endif // INSERMODESELECTOR_H
