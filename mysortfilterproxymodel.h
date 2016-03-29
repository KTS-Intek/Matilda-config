#ifndef MYSORTFILTERPROXYMODEL_H
#define MYSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QDate>
#include <QTime>

class MySortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit MySortFilterProxyModel(QObject *parent = 0);

    QDate filterMinimumDate() const { return minDate; }
    void setFilterMinimumDate(const QDate &date);

    QDate filterMaximumDate() const { return maxDate; }
    void setFilterMaximumDate(const QDate &date);

    void setFilterMode(QList<int> filterMode);


protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

public slots:
    void setNewFileterStr(QString str);

private:
    bool dateInRange(const QDate &date) const;
    QList<int> filterMode;
    int colMax;
    QDate minDate;
    QDate maxDate;

};

#endif // MYSORTFILTERPROXYMODEL_H
