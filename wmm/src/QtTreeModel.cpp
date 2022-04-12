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

/**
 * Insert empty subnodes at this node.
 * @param Position The position of insertion. The existing element at Position will be moved to (Position + RowCount).
 * @param RowCount The number of subnodes.
 * @param ColumnCount The number of elements of each subnode.
 * @return Whether the operation was succeeded.
 */
bool QtTreeModel::TreeItem::InsertChildren(const lsize_t Position, const lsize_t RowCount, const lsize_t ColumnCount) {
    if (Position < 0 || Position > ChildItem.size()) return false;
    const qsizetype OldSize = ChildItem.size();
    for (qsizetype i = 0; i < RowCount; ++i) {
        TreeItem* const Blank = new TreeItem(QList<QVariant>(ColumnCount), this);
        ChildItem.emplace_back(Blank);
    }
    for (qsizetype i = OldSize - 1; i >= Position; --i) std::swap(ChildItem[i], ChildItem[i + RowCount]);
    return true;
}

/**
 * Remove child items at [Position, Position + Count)
 * @param Position
 * @param Count
 * @return Whether the operation was succeeded.
 */
bool QtTreeModel::TreeItem::RemoveChildren(const lsize_t Position, const lsize_t Count) {
    if (Position < 0 || Position + Count > ChildItem.size()) return false;
    for (qsizetype i = Position; i < Position + Count; ++i) delete ChildItem[i];
    ChildItem.remove(Position, Count);
    return true;
}

/**
 * The functions for inserting and removing columns are used differently to those for inserting and removing child items,
 * because they are expected to be called on every item in the tree.
 * This is done by recursively calling this function on each child of the item.
 * You MUSTN'T call this function at non-root nodes.
 * @param Position
 * @param ColumnCount The number of empty columns.
 * @return Whether the operation was succeeded.
 */
bool QtTreeModel::TreeItem::InsertColumns(const lsize_t Position, const lsize_t ColumnCount) {
    if (Position < 0 || Position > ItemData.size()) return false;
    ItemData.insert(Position, ColumnCount, QVariant());
    for (TreeItem* CurrentChild: qAsConst(ChildItem)) CurrentChild->InsertColumns(Position, ColumnCount);
    return true;
}

/**
 * You MUSTN'T call this function at non-root nodes.
 * @param Position
 * @param Count The number of columns to be deleted.
 * @return Whether the operation was succeeded.
 */
bool QtTreeModel::TreeItem::RemoveColumns(lsize_t Position, lsize_t Count) {
    if (Position < 0 || Position + Count > ItemData.size()) return false;
    ItemData.remove(Position, Count);
    for (TreeItem* CurrentChild: qAsConst(ChildItem)) CurrentChild->RemoveColumns(Position, Count);
    return true;
}

/// QtTreeModel

QtTreeModel::QtTreeModel(QObject* Parent) : QAbstractItemModel(Parent) { RootItem = new TreeItem(); }

// This will cause all items to be recursively deleted.
QtTreeModel::~QtTreeModel() { delete RootItem; }

QVariant QtTreeModel::headerData(int Section, Qt::Orientation Orientation, int Role) const {
    // default header
    switch (Section) {
    case 0:return "Name/Index";
    case 1:return "Value";
    case 2:return "";
    }
}

bool QtTreeModel::setHeaderData(int Section, Qt::Orientation Orientation, const QVariant& Value, int Role) {
//    if (Role!=Qt::EditRole&&Orientation!=Qt::Horizontal) return false;
//    if (Value != headerData(Section, Orientation, Role)) {
//        emit headerDataChanged(Orientation, Section, Section);
//        return true;
//    }
    return false;
}

/**
 * Obtain the index of a specific subnode of a node.
 * In this model, we only return model indexes for child items
 * if the parent index is invalid (corresponding to the root item) or if it has a 0 column number.
 * @param Row
 * @param Column
 * @param Parent
 * @return
 */
QModelIndex QtTreeModel::index(lsize_t Row, lsize_t Column, const QModelIndex& Parent) const {
    if (Parent.isValid() && Parent.column() != 0) return {};
    TreeItem* ParentItem = GetItem(Parent);
    if (ParentItem == nullptr) return {};
    TreeItem* ChildItem = ParentItem->Child(Row);
    if (ChildItem != nullptr) return createIndex(Row, Column, ChildItem);
    return {};
}

/**
 * Since each item contains information for an entire row of data,
 * we create a model index to uniquely identify it by calling createIndex() with the row and column numbers and a pointer to the item.
 * In the data() function, we will use the item pointer and column number to access the data associated with the model index;
 * in this model, the row number is not needed to identify data.
 * Items without parents, including the root item, are handled by returning a null model index.
 * Otherwise, a model index is created and returned as in the index() function, with a suitable row number,
 * but with a 0 column number to be consistent with the scheme used in the index() implementation.
 * @param Index
 * @return
 */
QModelIndex QtTreeModel::parent(const QModelIndex& Index) const {
    if (Index.isValid() == false) return {};
    TreeItem* CurrentItem = GetItem(Index);
    TreeItem* ParentItem = CurrentItem != nullptr ? CurrentItem->Parent() : nullptr;
    if (ParentItem == RootItem || ParentItem == nullptr) return {};
    return createIndex(ParentItem->ChildNumber(), 0, ParentItem);
}

/**
 * @param Parent The tree node whose children count is wanted.
 * @return The number of children it contains.
 */
lsize_t QtTreeModel::rowCount(const QModelIndex& Parent) const {
    if (!Parent.isValid() && Parent.column() > 0) return 0;
//    if (!Parent.isValid()) return 0;
    const TreeItem* const ParentItem = GetItem(Parent);
    return ParentItem != nullptr ? ParentItem->ChildCount() : 0;
}

/**
 * @param Parent The tree node whose number of elements (terms of data) is wanted.
 * @return The number of elements this tree node contains.
 */
lsize_t QtTreeModel::columnCount(const QModelIndex& Parent) const {
//    return Parent.isValid() ? GetItem(Parent)->ColumnCount() : 0;
    // for the situation where column count is fixed:
    Q_UNUSED(Parent);
    return RootItem->ChildCount();
}

//bool QtTreeModel::hasChildren(const QModelIndex& Parent) const {
//    // FIXME: Implement me!
//}
//
//bool QtTreeModel::canFetchMore(const QModelIndex& Parent) const {
//    // FIXME: Implement me!
//    return false;
//}
//
//void QtTreeModel::fetchMore(const QModelIndex& Parent) {
//    // FIXME: Implement me!
//}

QVariant QtTreeModel::data(const QModelIndex& Index, int Role) const {
    if ((Index.isValid() == false) || (Role != Qt::DisplayRole && Role != Qt::EditRole)) return {};
    TreeItem* Item = GetItem(Index);
    return Item->Data(Index.column());
}

bool QtTreeModel::setData(const QModelIndex& Index, const QVariant& Value, int Role) {
    if (Role != Qt::EditRole) return false;
    TreeItem* Item = GetItem(Index);
    const bool Succeeded = Item->SetData(Index.column(), Value);
    if (Succeeded) { emit dataChanged(Index, Index, { Qt::DisplayRole, Qt::EditRole }); }
    return Succeeded;
}

Qt::ItemFlags QtTreeModel::flags(const QModelIndex& Index) const {
    if (!Index.isValid()) return Qt::NoItemFlags;
    return QAbstractItemModel::flags(Index) | Qt::ItemIsEditable;
}

bool QtTreeModel::insertRows(lsize_t Position, lsize_t ChildCount, const QModelIndex& Parent) {
    TreeItem* TargetItem = GetItem(Parent);
    if (TargetItem == nullptr) return false;
    beginInsertRows(Parent, Position, Position + ChildCount - 1);
//    const bool Succeeded = TargetItem->InsertChildren(Position, ChildCount, TargetItem->ColumnCount());
    const bool Succeeded = TargetItem->InsertChildren(Position, ChildCount, RootItem->ColumnCount());
    endInsertRows();
    return Succeeded;
}

bool QtTreeModel::insertColumns(lsize_t Position, lsize_t ColumnCount, const QModelIndex& Parent) {
    beginInsertColumns(Parent, Position, Position + ColumnCount - 1);
    const bool Succeeded = RootItem->InsertColumns(Position, ColumnCount);
    endInsertColumns();
    return Succeeded;
}

bool QtTreeModel::removeRows(lsize_t Position, lsize_t ChildCount, const QModelIndex& Parent) {
    TreeItem* TargetItem = GetItem(Parent);
    if (TargetItem == nullptr) return false;
    beginRemoveRows(Parent, Position, Position + ChildCount - 1);
    const bool Succeeded = TargetItem->RemoveChildren(Position, ChildCount);
    endRemoveRows();
    return Succeeded;
}

bool QtTreeModel::removeColumns(lsize_t Position, lsize_t ColumnCount, const QModelIndex& Parent) {
    beginRemoveColumns(Parent, Position, Position + ColumnCount - 1);
    const bool Succeeded = RootItem->RemoveColumns(Position, ColumnCount);
    if (RootItem->ChildCount() == 0) removeRows(0, rowCount());
    endRemoveColumns();
    return Succeeded;
}

QtTreeModel::TreeItem* QtTreeModel::GetItem(const QModelIndex& Index) const {
    if (Index.isValid()) {
        if (Index.internalPointer() != nullptr) return static_cast<TreeItem*>(Index.internalPointer());
    }
    return RootItem; // for a certain kind of consistency?
}
