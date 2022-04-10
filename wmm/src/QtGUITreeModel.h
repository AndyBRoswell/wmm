#ifndef WRITING_ASSISTANT_QTGUITREEMODEL_H
#define WRITING_ASSISTANT_QTGUITREEMODEL_H

#include <QAbstractItemModel>

namespace WritingMaterialsManager {
    class QtGUITreeModel : public QAbstractItemModel {
    Q_OBJECT

    public:
        using lsize_t = int; // l -> local

        explicit QtGUITreeModel(QObject* const Parent = nullptr);
        ~QtGUITreeModel();

        [[nodiscard]] QVariant data(const QModelIndex& Index, int Role) const override;
        [[nodiscard]] Qt::ItemFlags flags(const QModelIndex& Index) const override;
        [[nodiscard]] QVariant headerData(int Section, Qt::Orientation Orientation, int Role = Qt::DisplayRole) const override;
        [[nodiscard]] QModelIndex index(lsize_t Row, lsize_t Column, const QModelIndex& ParentIndex = QModelIndex()) const override;
        [[nodiscard]] QModelIndex parent(const QModelIndex& Index) const override;
        [[nodiscard]] lsize_t rowCount(const QModelIndex& ParentIndex = QModelIndex()) const override;
        [[nodiscard]] lsize_t columnCount(const QModelIndex& ParentIndex = QModelIndex()) const override;
        void FromJSON(const QByteArray& JSONString);

        class Node {
        public:
            explicit Node(const QList<QVariant>& Data = {}, Node* const Parent = nullptr);
            ~Node();

            void AddChild(Node* const Child);
//            template<class... Args> constexpr Node*& AddChild(Args&&... args);
            [[nodiscard]] Node* Child(const lsize_t Row);
            [[nodiscard]] lsize_t ChildCount() const;
            [[nodiscard]] lsize_t RowCount() const { return ChildCount(); } // Same as ChildCount()
            [[nodiscard]] lsize_t ColumnCount() const;
            [[nodiscard]] lsize_t DataCount() const { return ColumnCount(); } // Same as ColumnCount()
            [[nodiscard]] QVariant Data(const lsize_t Column) const;
            [[nodiscard]] const QList<QVariant>& Data() const;
            [[nodiscard]] QList<QVariant>& Data();
            [[nodiscard]] lsize_t RowInParent() const;
            [[nodiscard]] lsize_t Row() const { return RowInParent(); } // Same as RowInParent();
            [[nodiscard]] lsize_t IndexInParent() const { return RowInParent(); } // Same as RowInParent()
            [[nodiscard]] Node* Parent();

            QList<QVariant> MData;

        private:
            QList<Node*> MChildItems;
            Node* MParentItem;
        };

    private:
        Node* Tree = nullptr; // also act as the root node of this tree
        Node*& RootNode = Tree;
    };
}

#endif //WRITING_ASSISTANT_QTGUITREEMODEL_H
