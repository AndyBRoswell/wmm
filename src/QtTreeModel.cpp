#include "QtTreeModel.h"

#include <stack>
#include <vector>

#include <QFlags>

#include "rapidjson/document.h"
#include "rapidjson/pointer.h"

namespace WritingMaterialsManager {
    using lsize_t = QtTreeModel::lsize_t;

/// class QtTreeModel::Node

/**
 * Initially, each node has no children. They're added using the InsertChildren() function.
 * @param Data
 * @param Parent
 */
    QtTreeModel::Node::Node(const QList<QVariant>& Data, QtTreeModel::Node* Parent) : NodalData(Data), ParentNode(Parent) {}

// The destructor ensures that each child added to the item is deleted when the item itself is deleted.
    QtTreeModel::Node::~Node() { qDeleteAll(SubNode); }

    QtTreeModel::Node* QtTreeModel::Node::Parent() { return ParentNode; }

/**
 * Returns a specific child from the internal list of children.
 * @param Number The number of the required child of this node.
 * @return
 */
    QtTreeModel::Node* QtTreeModel::Node::Child(int Number) {
//    if (Number < 0 || Number >= SubNode.size()) return nullptr;
        return SubNode[Number];
    }

    lsize_t QtTreeModel::Node::ChildCount() const { return SubNode.count(); }

/**
 * This function is used to determine the index of the child in its parent's children. (Element -> Array Index)
 * The root item has no parent item; for this item, we return 0 to be consistent with the other items.
 * @return
 */
    lsize_t QtTreeModel::Node::ChildNumber() const { return ParentNode != nullptr ? ParentNode->SubNode.indexOf(this) : 0; }

/**
 * Returns the number of elements stored at this tree node.
 * @return
 */
    lsize_t QtTreeModel::Node::ColumnCount() const { return NodalData.count(); }

/**
 * Data is retrieved using the Data() function, which accesses the appropriate element in this tree node.
 * @param Column
 * @return
 */
    QVariant QtTreeModel::Node::Data(int Column) const {
        if (Column < 0 || Column >= NodalData.size()) return {};
        return NodalData[Column];
    }

/**
 * Data is set using the SetData() function, which only stores values in this tree node for valid list indexes, corresponding to column values in the model.
 * @param Column
 * @param Value
 * @return Whether the operation was succeeded.
 */
    bool QtTreeModel::Node::SetData(lsize_t Column, const QVariant& Value) {
        if (Column < 0 || Column >= NodalData.size()) return false;
        NodalData[Column] = Value;
        return true;
    }

    QVariant& QtTreeModel::Node::PushBackData(const QVariant& Value) {
        return NodalData.emplace_back(Value);
    }

    void QtTreeModel::Node::PushBackChild(Node* const Child) {
        SubNode.emplace_back(Child);
    }

    bool QtTreeModel::Node::InsertChild(lsize_t Position, Node* const Child) {
        if (Position < 0 || Position > SubNode.size()) return false;
        SubNode.insert(Position, Child);
        return true;
    }

/**
 * Insert empty subnodes at this node.
 * @param Position The position of insertion. The existing element at Position will be moved to (Position + RowCount).
 * @param RowCount The number of subnodes.
 * @param ColumnCount The number of elements of each subnode.
 * @return Whether the operation was succeeded.
 */
    bool QtTreeModel::Node::InsertChildren(const lsize_t Position, const lsize_t RowCount, const lsize_t ColumnCount) {
        if (Position < 0 || Position > SubNode.size()) return false;
        const qsizetype OldSize = SubNode.size();
        for (qsizetype i = 0; i < RowCount; ++i) {
            Node* const Blank = new Node(QList<QVariant>(ColumnCount), this);
            SubNode.emplace_back(Blank);
        }
        for (qsizetype i = OldSize - 1; i >= Position; --i) std::swap(SubNode[i], SubNode[i + RowCount]);
        return true;
    }

/**
 * Remove child items at [Position, Position + Count)
 * @param Position
 * @param Count
 * @return Whether the operation was succeeded.
 */
    bool QtTreeModel::Node::RemoveChildren(const lsize_t Position, const lsize_t Count) {
        if (Position < 0 || Position + Count > SubNode.size()) return false;
        for (qsizetype i = Position; i < Position + Count; ++i) delete SubNode[i];
        SubNode.remove(Position, Count);
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
    bool QtTreeModel::Node::InsertColumns(const lsize_t Position, const lsize_t ColumnCount) {
        if (Position < 0 || Position > NodalData.size()) return false;
        NodalData.insert(Position, ColumnCount, QVariant());
        for (Node* CurrentChild: qAsConst(SubNode)) CurrentChild->InsertColumns(Position, ColumnCount);
        return true;
    }

/**
 * You MUSTN'T call this function at non-root nodes.
 * @param Position
 * @param Count The number of columns to be deleted.
 * @return Whether the operation was succeeded.
 */
    bool QtTreeModel::Node::RemoveColumns(lsize_t Position, lsize_t Count) {
        if (Position < 0 || Position + Count > NodalData.size()) return false;
        NodalData.remove(Position, Count);
        for (Node* CurrentChild: qAsConst(SubNode)) CurrentChild->RemoveColumns(Position, Count);
        return true;
    }

    void QtTreeModel::Node::ReverseChild() { std::reverse(SubNode.begin(), SubNode.end()); }

/// class QtTreeModel

    QtTreeModel::QtTreeModel(QObject* Parent) : QAbstractItemModel(Parent), RootNode(new Node({ tr("Name/Index"), tr("Value") })) {}

// This will cause all items to be recursively deleted.
    QtTreeModel::~QtTreeModel() { delete RootNode; }

    QVariant QtTreeModel::headerData(int Section, Qt::Orientation Orientation, int Role) const {
        if (Orientation == Qt::Horizontal && Role == Qt::DisplayRole) { return RootNode->Data(Section); }
        return {};
    }

    bool QtTreeModel::setHeaderData(int Section, Qt::Orientation Orientation, const QVariant& Value, int Role) {
        if (Role != Qt::EditRole && Orientation != Qt::Horizontal) return false;
        const bool Succeeded = RootNode->SetData(Section, Value);
        if (Succeeded) { emit headerDataChanged(Orientation, Section, Section); }
        return Succeeded;
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
        Node* ParentItem = GetItem(Parent);
        if (ParentItem == nullptr) return {};
        Node* ChildItem = ParentItem->Child(Row);
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
        Node* CurrentItem = GetItem(Index);
        Node* ParentItem = CurrentItem != nullptr ? CurrentItem->Parent() : nullptr;
        if (ParentItem == RootNode || ParentItem == nullptr) return {};
        return createIndex(ParentItem->ChildNumber(), 0, ParentItem);
    }

/**
 * @param Parent The tree node whose children count is wanted.
 * @return The number of children it contains.
 */
    lsize_t QtTreeModel::rowCount(const QModelIndex& Parent) const {
        if (Parent.isValid() && Parent.column() > 0) return 0;
//    if (Parent.isValid()) return 0;
        const Node* const ParentItem = GetItem(Parent);
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
        return RootNode->ColumnCount();
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
        Node* Item = GetItem(Index);
        return Item->Data(Index.column());
    }

    bool QtTreeModel::setData(const QModelIndex& Index, const QVariant& Value, int Role) {
        if (Role != Qt::EditRole) return false;
        Node* Item = GetItem(Index);
//    const bool Succeeded = Item->SetData(Index.column(), Value);
//    if (Succeeded) { emit dataChanged(Index, Index, { Qt::DisplayRole, Qt::EditRole }); }
//    return Succeeded;
        return false;
    }

/**
 * Get the properties (i.e., editable) of an item.
 * @param Index
 * @return Properties of the item.
 */
    Qt::ItemFlags QtTreeModel::flags(const QModelIndex& Index) const {
        if (!Index.isValid()) return Qt::NoItemFlags;
//    return QAbstractItemModel::flags(Index);
//    return static_cast<QFlags<Qt::ItemFlag>>(QAbstractItemModel::flags(Index) | Qt::TextSelectableByMouse);
//    return static_cast<QFlags<Qt::ItemFlag>>(QAbstractItemModel::flags(Index) | Qt::TextSelectableByKeyboard);
//    return static_cast<QFlags<Qt::ItemFlag>>(QAbstractItemModel::flags(Index) | Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);
        return QAbstractItemModel::flags(Index) | Qt::ItemIsEditable;
    }

    bool QtTreeModel::insertRows(lsize_t Position, lsize_t ChildCount, const QModelIndex& Parent) {
        Node* TargetItem = GetItem(Parent);
        if (TargetItem == nullptr) return false;
        beginInsertRows(Parent, Position, Position + ChildCount - 1);
//    const bool Succeeded = TargetItem->InsertChildren(Position, ChildCount, TargetItem->ColumnCount());
        const bool Succeeded = TargetItem->InsertChildren(Position, ChildCount, RootNode->ColumnCount());
        endInsertRows();
        return Succeeded;
    }

    bool QtTreeModel::insertColumns(lsize_t Position, lsize_t ColumnCount, const QModelIndex& Parent) {
        beginInsertColumns(Parent, Position, Position + ColumnCount - 1);
        const bool Succeeded = RootNode->InsertColumns(Position, ColumnCount);
        endInsertColumns();
        return Succeeded;
    }

    bool QtTreeModel::removeRows(lsize_t Position, lsize_t ChildCount, const QModelIndex& Parent) {
        Node* TargetItem = GetItem(Parent);
        if (TargetItem == nullptr) return false;
        beginRemoveRows(Parent, Position, Position + ChildCount - 1);
        const bool Succeeded = TargetItem->RemoveChildren(Position, ChildCount);
        endRemoveRows();
        return Succeeded;
    }

    bool QtTreeModel::removeColumns(lsize_t Position, lsize_t ColumnCount, const QModelIndex& Parent) {
        beginRemoveColumns(Parent, Position, Position + ColumnCount - 1);
        const bool Succeeded = RootNode->RemoveColumns(Position, ColumnCount);
        if (RootNode->ChildCount() == 0) removeRows(0, rowCount());
        endRemoveColumns();
        return Succeeded;
    }

    QtTreeModel::Node* QtTreeModel::GetItem(const QModelIndex& Index) const {
        if (Index.isValid()) {
            if (Index.internalPointer() != nullptr) return static_cast<Node*>(Index.internalPointer());
        }
        return RootNode; // for a certain kind of consistency?
    }

    void QtTreeModel::FromJSON(const QByteArray& UTF8JSONString) {
        using namespace std;
        using namespace rapidjson;

        RootNode->RemoveChildren(0, RootNode->ChildCount());
        Node* const JSONRoot = new Node();
        RootNode->PushBackChild(JSONRoot);

        Document JSONDocument;
        JSONDocument.Parse(UTF8JSONString.constData());

        beginResetModel();
        stack<const Value*, vector<const Value*>> s;
        std::stack<Node*, std::vector<Node*>> t;
        s.emplace(Pointer("").Get(JSONDocument));
        JSONRoot->PushBackData("");
        t.emplace(JSONRoot);
        while (s.empty() == false) { // non-recursive DFS
            const Value* const ns = s.top();
            s.pop();
            Node* const nt = t.top();
            t.pop();
            switch (ns->GetType()) {
            case kNullType: nt->PushBackData(""); break;
            case kFalseType: case kTrueType: nt->PushBackData(ns->GetBool()); break;
            case kStringType: nt->PushBackData(ns->GetString()); break;
            case kNumberType:
                if (ns->IsUint64()) { nt->PushBackData(ns->GetUint64()); }
                else if (ns->IsInt64()) { nt->PushBackData(ns->GetInt64()); }
                else if (ns->IsDouble()) { nt->PushBackData(ns->GetDouble()); }
                break; // other number types are not supported.
            case kArrayType:
                if (ns->End() == ns->Begin()) break;
                for (Value::ConstValueIterator i = ns->End() - 1; i >= ns->Begin(); --i) {
                    s.emplace(&*i);
                    Node* const c = new Node({ i - ns->Begin() }, nt);
                    nt->PushBackChild(c);
                    t.emplace(c);
                }
                nt->ReverseChild();
                break;
            case kObjectType:
                if (ns->MemberEnd() == ns->MemberBegin()) break;
                for (Value::ConstMemberIterator i = ns->MemberEnd() - 1; i >= ns->MemberBegin(); --i) {
                    s.emplace(&i->value);
                    Node* const c = new Node({ i->name.GetString() }, nt);
                    nt->PushBackChild(c);
                    t.emplace(c);
                }
                nt->ReverseChild();
                break;
            }
        }
        endResetModel();
    }
}
