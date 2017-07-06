/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

/*
    treemodel.cpp

    Provides a simple tree model to show how to create and use hierarchical
    models.
*/

#include "treeitem.h"
#include "treemodel.h"

#include <QStringList>
#include <QDebug>
//-----------------------------------------------------------------------------------------------------------------------
TreeModel::TreeModel( QObject *parent)
    : QAbstractItemModel(parent)
{//    explicit TreeItem(const QStringList &columns, const QString &path2ico, const QVariantList &colData, const int defDataRole = 0, TreeItem *parentItem = 0);

    rootItem = new TreeItem(QStringList() << "rootItem", "",  QVariantList());
//    setupModelData(data.split(QString("\n")), rootItem);
}
//-----------------------------------------------------------------------------------------------------------------------
TreeModel::~TreeModel()
{
    delete rootItem;
}
//-----------------------------------------------------------------------------------------------------------------------
int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}
//-----------------------------------------------------------------------------------------------------------------------
void TreeModel::appendItem(const QString &text, const QVariant itemData, const QString path2ico)
{
    rootItem->appendChild(new TreeItem( QStringList() << text, path2ico, QVariantList() << itemData, Qt::UserRole, rootItem));
//     parents.last()->appendChild(new TreeItem(columnData, parents.last()));
}
//-----------------------------------------------------------------------------------------------------------------------
void TreeModel::appendItem(const QString &parentText, const QStringList &childList, const QStringList &path2icoLst, const QVariantList itemsData)
{
    TreeItem *item = new TreeItem( QStringList() << parentText,"",  QVariantList(), Qt::UserRole, rootItem);
    rootItem->appendChild(item);

    for(int i = 0, iMax = childList.size(), iMax2 = itemsData.size(), iMax3 = path2icoLst.size(); i < iMax; i++){
        QVariantList itemData = (i < iMax2) ? ( QVariantList() << itemsData.at(i)) : QVariantList();
        QString path2ico = (i < iMax3) ? path2icoLst.at(i) : QString("");
        item->appendChild( new TreeItem(QStringList() << childList.at(i), path2ico, itemData,  Qt::UserRole, item ) );
    }
}
//-----------------------------------------------------------------------------------------------------------------------
void TreeModel::clear()
{
//    for(int i = 0, iMax = rootItem->childCount(); i < iMax; i++)
//        rootItem->child(i)->clear();

    try{
        delete rootItem;
    }catch(...){
        qDebug() << "can't delete rootItem. Ignoring..." ;
    }
    rootItem = new TreeItem(QStringList()<< "rootItem", "",  QVariantList());

}
//-----------------------------------------------------------------------------------------------------------------------

void TreeModel::setDefSize4itms(QSize size)
{
    defSize4itms = size;
}
//-----------------------------------------------------------------------------------------------------------------------
QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    if (role == Qt::DisplayRole || role == Qt::DecorationRole || role == Qt::SizeHintRole){

        if(role == Qt::DisplayRole){
            return item->columnText(index.column());
        }else{
            if(role == Qt::DecorationRole)
                return item->getImage();
            else if(defSize4itms.isValid())
                return defSize4itms;
        }

    }
    return item->data(index.column(), role);// QVariant();


}
//-----------------------------------------------------------------------------------------------------------------------
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}
//-----------------------------------------------------------------------------------------------------------------------
QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}
//-----------------------------------------------------------------------------------------------------------------------
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
//-----------------------------------------------------------------------------------------------------------------------
QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}
//-----------------------------------------------------------------------------------------------------------------------
int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}
//-----------------------------------------------------------------------------------------------------------------------
void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
{
    QList<TreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            position++;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QList<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            parents.last()->appendChild(new TreeItem(columnStrings, "",  columnData, 0, parents.last()));
        }

        ++number;
    }
}
//-----------------------------------------------------------------------------------------------------------------------
