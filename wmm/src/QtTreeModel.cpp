#include "QtTreeModel.h"

using namespace WritingMaterialsManager;
using lsize_t = QtTreeModel::lsize_t;

/// QtTreeModel::TreeItem

QtTreeModel::TreeItem::TreeItem(const QList<QVariant>& Data, QtTreeModel::TreeItem* Parent) : ItemData(Data), ParentItem(Parent) {}

QtTreeModel::TreeItem::~TreeItem() { qDeleteAll(ChildItem); }

QtTreeModel::TreeItem* QtTreeModel::TreeItem::Parent() { return ParentItem; }

QtTreeModel::TreeItem* QtTreeModel::TreeItem::Child(int Number) {
//    if (Number < 0 || Number >= ChildItem.size()) return nullptr;
    return ChildItem[Number];
}



/// QtTreeModel

QtTreeModel::QtTreeModel(QObject* Parent)
    : QAbstractItemModel(Parent) {
}

QVariant QtTreeModel::headerData(int Section, Qt::Orientation Orientation, int Role) const {
    // FIXME: Implement me!
}

bool QtTreeModel::setHeaderData(int Section, Qt::Orientation Orientation, const QVariant& Value, int Role) {
    if (Value != headerData(Section, Orientation, Role)) {
        // FIXME: Implement me!
        emit headerDataChanged(Orientation, Section, Section);
        return true;
    }
    return false;
}

QModelIndex QtTreeModel::index(lsize_t Row, lsize_t Column, const QModelIndex& Parent) const {
    // FIXME: Implement me!
}

QModelIndex QtTreeModel::parent(const QModelIndex& Index) const {
    // FIXME: Implement me!
}

lsize_t QtTreeModel::rowCount(const QModelIndex& Parent) const {
    if (!Parent.isValid())
        return 0;

    // FIXME: Implement me!
}

lsize_t QtTreeModel::columnCount(const QModelIndex& Parent) const {
    if (!Parent.isValid())
        return 0;

    // FIXME: Implement me!
}

bool QtTreeModel::hasChildren(const QModelIndex& Parent) const {
    // FIXME: Implement me!
}

bool QtTreeModel::canFetchMore(const QModelIndex& Parent) const {
    // FIXME: Implement me!
    return false;
}

void QtTreeModel::fetchMore(const QModelIndex& Parent) {
    // FIXME: Implement me!
}

QVariant QtTreeModel::data(const QModelIndex& Index, int Role) const {
    if (!Index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}

bool QtTreeModel::setData(const QModelIndex& Index, const QVariant& Value, int Role) {
    if (data(Index, Role) != Value) {
        // FIXME: Implement me!
        emit dataChanged(Index, Index, { Role });
        return true;
    }
    return false;
}

Qt::ItemFlags QtTreeModel::flags(const QModelIndex& Index) const {
    if (!Index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(Index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

bool QtTreeModel::insertRows(lsize_t Row, lsize_t Count, const QModelIndex& Parent) {
    beginInsertRows(Parent, Row, Row + Count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool QtTreeModel::insertColumns(lsize_t Column, lsize_t Count, const QModelIndex& Parent) {
    beginInsertColumns(Parent, Column, Column + Count - 1);
    // FIXME: Implement me!
    endInsertColumns();
    return true;
}

bool QtTreeModel::removeRows(lsize_t Row, lsize_t Count, const QModelIndex& Parent) {
    beginRemoveRows(Parent, Row, Row + Count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

bool QtTreeModel::removeColumns(lsize_t Column, lsize_t Count, const QModelIndex& Parent) {
    beginRemoveColumns(Parent, Column, Column + Count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
    return true;
}
