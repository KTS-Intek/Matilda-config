#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include <QPixmap>
//#include <QIcon>

class TreeItem
{
public:
    explicit TreeItem(const QStringList &columns, const QString &path2ico, const QVariantList &colData, const int defDataRole = Qt::UserRole, TreeItem *parentItem = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column, int role) const;
    QVariant data(int column) const;
    QString columnText(int column) const;
    QPixmap getImage() const;


    int row() const;
    TreeItem *parentItem();

    void setItemData(int column, QVariant data, int role = 0);
    void clear();


private:
    QList<TreeItem*> m_childItems;
    QHash<int, QHash<int,QVariant> > m_itemData;
    QPixmap pixmap;
//    QIcon m_icon;
    QStringList m_itemText;
    TreeItem *m_parentItem;
};

#endif // TREEITEM_H
