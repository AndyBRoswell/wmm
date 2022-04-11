#include "QtTreeModel.h"

using namespace WritingMaterialsManager;
using lsize_t = QtTreeModel::lsize_t;

/// QtTreeModel::TreeItem

/**
 * Initially, each node has no children. They're added using the InsertChildren() function.
 * @param Data
 * @param Parent
 */
QtTreeModel::TreeItem::TreeItem(const QList<QVariant>& Data, QtTreeModel::TreeItem* Parent) : ItemData(Data), ParentItem(Parent) {}

// The destructor ensures that each child added to the item is deleted when the item itself is deleted.
QtTreeModel::TreeItem::~TreeItem() { qDeleteAll(ChildItem); }

QtTreeModel::TreeItem* QtTreeModel::TreeItem::Parent() { return ParentItem; }

/**
 * Returns a specific child from the internal list of children.
 * @param Number The number of the required child of this node.
 * @return
 */
QtTreeModel::TreeItem* QtTreeModel::TreeItem::Child(int Number) {
//    if (Number < 0 || Number >= ChildItem.size()) return nullptr;
    return ChildItem[Number];
}

lsize_t QtTreeModel::TreeItem::ChildCount() const { return ChildItem.count(); }

/**
 * The ChildNumber() function is used to determine the index of the child in its parent's list of children.
 * The root item has no parent item; for this item, we return 0 to be consistent with the other items.
 * @return
 */
lsize_t QtTreeModel::TreeItem::ChildNumber() const { return ParentItem != nullptr ? ParentItem->ChildItem.indexOf(this) : 0; }

/**
 * Returns the number of elements stored at this tree node.
 * @return
 */
lsize_t QtTreeModel::TreeItem::ColumnCount() const { return ItemData.count(); }

/**
 * Data is retrieved using the Data() function, which accesses the appropriate element in this tree node.
 * @param Column
 * @return
 */
QVariant QtTreeModel::TreeItem::Data(int Column) const {
//    if (Column < 0 || Column >= ItemData.size()) return {};
    return ItemData[Column];
}

/**
 * Data is set using the SetData() function, which only stores values in this tree node for valid list indexes, corresponding to column values in the model.
 * @param Column
 * @param Value
 * @return Whether the operation was succeeded.
 */
bool QtTreeModel::TreeItem::SetData(lsize_t Column, const QVariant& Value) {
    if (Column < 0 || Column >= ItemData.size()) return false;
    ItemData[Column] = Value;
    return true;
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
