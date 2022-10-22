#ifndef WRITING_MATERIALS_MANAGER_QTTREEMODEL_H
#define WRITING_MATERIALS_MANAGER_QTTREEMODEL_H

#include <QAbstractItemModel>

namespace WritingMaterialsManager {
    class QtTreeModel : public QAbstractItemModel {
    Q_OBJECT
    public:
        using lsize_t = int; // l -> local

        class Node {
        public:
            /**
             * Initially, each node has no children. They're added using the InsertChildren() function.
             * @param Data
             * @param Parent
             */
            explicit Node(const QList<QVariant>& Data = {}, Node* Parent = nullptr);
            ~Node(); // The destructor ensures that each child added to the item is deleted when the item itself is deleted.

            /**
             * Returns a specific child from the internal list of children.
             * @param Number The No. of the required child of this node.
             * @return
             */
            Node* Child(lsize_t Number);
            lsize_t ChildCount() const;

            /**
             * Returns the number of elements stored at this tree node.
             * @return
             */
            lsize_t ColumnCount() const;

            /**
             * Data is retrieved using this function, which accesses the appropriate element in this tree node.
             * @param Column
             * @return
             */
            QVariant Data(lsize_t Column) const;
            void PushBackChild(Node* const Child);
            bool InsertChild(lsize_t Position, Node* const Child);

            /**
             * Insert empty subnodes at this node.
             * @param Position The position of insertion. The existing element at Position will be moved to (Position + RowCount).
             * @param RowCount The number of subnodes.
             * @param ColumnCount The number of elements of each subnode.
             * @return Whether the operation was succeeded.
             */
            bool InsertChildren(lsize_t Position, lsize_t RowCount, lsize_t ColumnCount);
            bool InsertColumns(lsize_t Position, lsize_t ColumnCount);
            Node* Parent();
            bool RemoveChildren(lsize_t Position, lsize_t Count);
            bool RemoveColumns(lsize_t Position, lsize_t Count);
            void ReverseChild();

            /**
             * Determine the index of the child in its parent's children. (Element -> Array Index)
             * The root item has no parent item. For this item, we return 0 to be consistent with the other items.
             * @return
             */
            lsize_t ChildNumber() const;

            /**
             * Data is set by this function, which only stores values in this tree node for valid list indices, corresponding to column values in the model.
             * @param Column
             * @param Value
             * @return Whether the operation was succeeded.
             */
            bool SetData(lsize_t Column, const QVariant& Value);

            /*
            * Add data to the tail of the current row (i.e., node)
            */
            QVariant& PushBackData(const QVariant& Value);
        private:
            QList<Node*> SubNode;
            QList<QVariant> NodalData;
            Node* ParentNode;
        };

        explicit QtTreeModel(QObject* Parent = nullptr);
        ~QtTreeModel();

        // Header:

        QVariant headerData(int Section, Qt::Orientation Orientation, int Role = Qt::DisplayRole) const override;
        bool setHeaderData(int Section, Qt::Orientation Orientation, const QVariant& Value, int Role = Qt::EditRole) override;

        // Basic functionality:

        QModelIndex index(lsize_t Row, lsize_t Column, const QModelIndex& Parent = QModelIndex()) const override;
        QModelIndex parent(const QModelIndex& Index) const override;

        lsize_t rowCount(const QModelIndex& Parent = QModelIndex()) const override;
        lsize_t columnCount(const QModelIndex& Parent = QModelIndex()) const override;

        // Fetch data dynamically:

//        bool hasChildren(const QModelIndex& Parent = QModelIndex()) const override;
//
//        bool canFetchMore(const QModelIndex& Parent) const override;
//        void fetchMore(const QModelIndex& Parent) override;

        QVariant data(const QModelIndex& Index, int Role = Qt::DisplayRole) const override;

        // Editable:

        bool setData(const QModelIndex& Index, const QVariant& Value, int Role = Qt::EditRole) override;

        Qt::ItemFlags flags(const QModelIndex& Index) const override;

        // Add data:

        bool insertRows(lsize_t Position, lsize_t ChildCount, const QModelIndex& Parent = QModelIndex()) override;
        bool insertColumns(lsize_t Position, lsize_t ColumnCount, const QModelIndex& Parent = QModelIndex()) override;

        // Remove data:

        bool removeRows(lsize_t Position, lsize_t ChildCount, const QModelIndex& Parent = QModelIndex()) override;
        bool removeColumns(lsize_t Position, lsize_t ColumnCount, const QModelIndex& Parent = QModelIndex()) override;

        // custom functions
        void FromJSON(const QByteArray& UTF8JSONString);
    private:
        Node* GetItem(const QModelIndex& Index) const;
        Node* RootNode = nullptr;
    };
}

#endif // WRITING_MATERIALS_MANAGER_QTTREEMODEL_H
