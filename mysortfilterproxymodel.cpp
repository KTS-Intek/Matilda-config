#include "mysortfilterproxymodel.h"
//#include <QDebug>

MySortFilterProxyModel::MySortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    filterMode.append(0);
    colMax = filterMode.size();
}
//========================================================================================================================================

void MySortFilterProxyModel::setFilterMinimumDate(const QDate &date)
{
    minDate = date;
    invalidateFilter();
}

//========================================================================================================================================

void MySortFilterProxyModel::setFilterMaximumDate(const QDate &date)
{
    maxDate = date;
    invalidateFilter();
}

//========================================================================================================================================

bool MySortFilterProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{

    bool retVal = false;
    for(int col = 0; col < colMax && !retVal; col++){
        QModelIndex index = sourceModel()->index(sourceRow, filterMode.at(col), sourceParent);
        retVal = retVal || sourceModel()->data(index).toString().contains(filterRegExp());
    }
    return retVal;
//    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
//    QModelIndex index1 = sourceModel()->index(sourceRow, 1, sourceParent);
//    QModelIndex index2 = sourceModel()->index(sourceRow, 1, sourceParent);

//     QDate invalid = QDate::fromString( sourceModel()->data(index2).toString().left(10), "yyyy.MM.dd");

     // qDebug() << "filterAcceptsRow " << invalid <<  sourceModel()->data(index2).toString();

//    return (sourceModel()->data(index0).toString().contains(filterRegExp())
//            || sourceModel()->data(index1).toString().contains(filterRegExp()))
//           && dateInRange(invalid)
//            );
}

//========================================================================================================================================

bool MySortFilterProxyModel::lessThan(const QModelIndex &left,
                                      const QModelIndex &right) const
{
    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);

    QString leftString = leftData.toString();
    QString rightString = rightData.toString();

    QDateTime invalid = QDateTime::fromString( leftString, "yyyy-MM-dd hh:mm");
//    qDebug() << "lessThan date is " << invalid;
    if (invalid.isValid()){ //leftData.type() == QVariant::DateTime) {
//        qDebug() << "date is valid " << left << right;
        return QDateTime::fromString( leftString, "yyyy-MM-dd hh:mm") < QDateTime::fromString( rightString, "yyyy-MM-dd hh:mm");
    } else {
        QRegExp *emailPattern = new QRegExp("([\\w\\.]*@[\\w\\.]*)");

//        QString leftString = leftData.toString();
        if(left.column() == 1 && emailPattern->indexIn(leftString) != -1)
            leftString = emailPattern->cap(1);

//        QString rightString = rightData.toString();
        if(right.column() == 1 && emailPattern->indexIn(rightString) != -1)
            rightString = emailPattern->cap(1);

        return QString::localeAwareCompare(leftString, rightString) < 0;
    }
}

//========================================================================================================================================

bool MySortFilterProxyModel::dateInRange(const QDate &date) const
{
    return (!minDate.isValid() || date >= minDate) && (!maxDate.isValid() || date <= maxDate);
}

//========================================================================================================================================
void MySortFilterProxyModel::setNewFileterStr(QString str)
{
    if(str.contains("'")){
        str.replace("'", " ");
//        return;
    }

    QRegExp regExp(str, Qt::CaseInsensitive, QRegExp::RegExp);
    this->setFilterRegExp(regExp);
//    mProxyModel->setFilterRegExp(regExp);
}
//========================================================================================================================================

void MySortFilterProxyModel::setFilterMode(QList<int> filterMode)
{
    this->filterMode = filterMode;
    colMax = filterMode.size();
}
//========================================================================================================================================
