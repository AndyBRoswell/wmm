#include "QtGUITreeModel.h"

#include <stack>
#include <vector>

#include "rapidjson/document.h"

using namespace WritingMaterialsManager;

using lsize_t = QtGUITreeModel::lsize_t;

/// QtGUITreeModel::Node

/**
 * This ctor is used to construct a new tree node and specify its parent node
 * @param Data Data to be stored at this node
 * @param Parent Parent node pointer
 */
QtGUITreeModel::Node::Node(const QList<QVariant>& Data, QtGUITreeModel::Node* const Parent) : MData(Data), MParentItem(Parent) {}

// Deletes all the nodes using the C++ delete operator.
QtGUITreeModel::Node::~Node() { qDeleteAll(MChildItems); }

// Add a sub node for this node
void QtGUITreeModel::Node::AddChild(QtGUITreeModel::Node* const Child) { MChildItems.append(Child); }

//template<class... Args> constexpr QtGUITreeModel::Node*& QtGUITreeModel::Node::AddChild(Args&& ... args) {
//    return MChildItems.emplace_back(new Node{{ std::forward<Args>(args)... }, this });
//}

/**
 * Get the direct sub node of this node
 * @param Row Get the pointer of the specified row (aka direct sub node) of this node
 * @return Pointer to the specified sub node
 */
QtGUITreeModel::Node* QtGUITreeModel::Node::Child(const lsize_t Row) {
    return (Row < 0 || Row >= MChildItems.size()) ? nullptr : MChildItems[Row];
}

// Get direct sub node count of this node
lsize_t QtGUITreeModel::Node::ChildCount() const { return MChildItems.size(); }

/**
 * Get the number of data items of this node
 * @return The number of terms of data stored at this node
 */
lsize_t QtGUITreeModel::Node::ColumnCount() const { return MData.count(); }

// Get the data stored at this node
QVariant QtGUITreeModel::Node::Data(const lsize_t Column) const {
//    return (Column < 0 || Column >= MData.size()) ? QVariant() : MData[Column];
    return MData[Column];
}

//QVariant& QtGUITreeModel::Node::Data(const lsize_t Column) {
////    if (Column < 0 || Column >= MData.size()) throw std::out_of_range("Column (Index) out of range ERROR when accessing nodal data.");
//    return MData[Column];
//}

const QList<QVariant>& QtGUITreeModel::Node::Data() const { return MData; }
QList<QVariant>& QtGUITreeModel::Node::Data() { return MData; }

// Get the position of this node in its parent node
lsize_t QtGUITreeModel::Node::RowInParent() const {
    return MParentItem != nullptr ? MParentItem->MChildItems.indexOf(this) : 0;
}

QtGUITreeModel::Node* QtGUITreeModel::Node::Parent() {
    return MParentItem;
}

/// QtGUITreeModel

/**
 * This ctor will make an empty tree: It has only 1 node and the data of this node is default-constructed (typically empty).
 * @param Parent
 */
QtGUITreeModel::QtGUITreeModel(QObject* const Parent) { Tree = new Node(); }

QtGUITreeModel::~QtGUITreeModel() {
    delete Tree; // This statement will cause the direct and indirect sub nodes deleted recursively. See Node::~Node().
}

/**
 * Models must implement an index() function to provide indexes for views and delegates to use when accessing data.
 * @param Row
 * @param Column
 * @param ParentIndex
 * @return
 */
QModelIndex QtGUITreeModel::index(lsize_t Row, lsize_t Column, const QModelIndex& ParentIndex) const {
    if (hasIndex(Row, Column, ParentIndex) == false) return {};
    Node* Parent, * This;
    if (ParentIndex.isValid() == false) Parent = RootNode;
    else Parent = static_cast<Node*>(ParentIndex.internalPointer()); // This node has a parent node
    This = Parent->Child(Row);
    if (This != nullptr) return createIndex(Row, Column, This); // The given parent node really has this child (There is a tree node in this position <r, c, p>)
    return {};
}

QModelIndex QtGUITreeModel::parent(const QModelIndex& Index) const {
    if (Index.isValid() == false) return {};
    Node* This = static_cast<Node*>(Index.internalPointer());
    Node* Parent = This->Parent();
    if (Parent == RootNode) return {};
    return createIndex(Parent->RowInParent(), 0, Parent);
}

lsize_t QtGUITreeModel::rowCount(const QModelIndex& ParentIndex) const {
    Node* Parent;
    if (ParentIndex.column() > 0) return 0;
    if (ParentIndex.isValid() == false) Parent = RootNode;
    else Parent = static_cast<Node*>(ParentIndex.internalPointer());
    return Parent->ChildCount();
}

lsize_t QtGUITreeModel::columnCount(const QModelIndex& ParentIndex) const {
    return (ParentIndex.isValid() == true) ? static_cast<Node*>(ParentIndex.internalPointer())->ColumnCount() : RootNode->ColumnCount();
}

QVariant QtGUITreeModel::data(const QModelIndex& Index, int Role) const {
    if (Index.isValid() == false) return {};
    if (Role != Qt::DisplayRole) return {}; // We only support the DisplayRole in this implementation, and we also return invalid QVariant objects for invalid model indexes.
    Node* N = static_cast<Node*>(Index.internalPointer());
    return N->Data(Index.column());
}

Qt::ItemFlags QtGUITreeModel::flags(const QModelIndex& Index) const {
    return (Index.isValid() == false) ? Qt::NoItemFlags : QAbstractItemModel::flags(Index);
}

QVariant QtGUITreeModel::headerData(int Section, Qt::Orientation Orientation, int Role) const {
    return (Orientation == Qt::Horizontal && Role == Qt::DisplayRole) ? RootNode->Data(Section) : QVariant();
}

void QtGUITreeModel::FromJSON(const QByteArray& JSONString) {
    using namespace std;
    using namespace rapidjson;

    if (Tree != nullptr) { delete Tree; } // delete the existing tree
    Tree = new Node();

    Document JSONDocument;
    JSONDocument.Parse(JSONString.constData());

    stack<const Value*, vector<const Value*>> s;
    stack<Node*, vector<Node*>> t;
    s.emplace(&JSONDocument);
    RootNode->MData.emplace_back("");
    t.emplace(RootNode);
    while (s.empty() == false) { // non-recursive DFS
        const Value* const ns = s.top();
        s.pop();
        Node* const nt = t.top();
        t.pop();
        switch (ns->GetType()) {
        case kNullType:
            nt->MData.emplace_back("");
            break;
        case kFalseType:
        case kTrueType:
            nt->MData.emplace_back(ns->GetBool());
            break;
        case kStringType:
            nt->MData.emplace_back(ns->GetString());
            break;
        case kNumberType:
            if (ns->IsUint64()) { nt->MData.emplace_back(ns->GetUint64()); }
            else { nt->MData.emplace_back(ns->GetInt64()); }
            break;
        case kArrayType:
            for (Value::ConstValueIterator i = ns->End() - 1; i >= ns->Begin(); --i) {
                s.emplace(&*i);
                Node* const c = new Node({ i - ns->Begin() }, nt);
                nt->AddChild(c);
                t.emplace(c);
            }
            break;
        case kObjectType:
            for (Value::ConstMemberIterator i = ns->MemberEnd() - 1; i >= ns->MemberBegin(); --i) {
                s.emplace(&i->value);
                Node* const c = new Node({ i->name.GetString() }, nt);
                nt->AddChild(c);
                t.emplace(c);
            }
            break;
        }
    }
}
