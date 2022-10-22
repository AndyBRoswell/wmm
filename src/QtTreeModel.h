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

            /**
             * Insert a subnode for this node.
             * @param Position The position of insertion. The existing element at Position will be moved to (Position + RowCount).
             * @param Child The subnode to be inserted.
             * @return Whether the operation was succeeded.
             */
            bool InsertChild(lsize_t Position, Node* const Child);

            /**
             * Insert empty subnodes for this node.
             * @param Position The position of insertion. The existing element at Position will be moved to (Position + RowCount).
             * @param RowCount The number of subnodes.
             * @param ColumnCount The number of elements of each subnode.
             * @return Whether the operation was succeeded.
             */
            bool InsertChildren(lsize_t Position, lsize_t RowCount, lsize_t ColumnCount);

            /**
             * The functions for inserting and removing columns are used differently to those for inserting and removing child items,
             * because these are expected to be called on every item in the tree.
             * This is done by recursively calling this function on each child of the item.
             * ATTENTION: You MUSTN'T call this function at non-root nodes.
             * @param Position
             * @param ColumnCount The number of empty columns.
             * @return Whether the operation was succeeded.
             */
            bool InsertColumns(lsize_t Position, lsize_t ColumnCount);
            Node* Parent();

            /**
             * Remove child items at [Position, Position + Count)
             * @param Position
             * @param Count
             * @return Whether the operation was succeeded.
             */
            bool RemoveChildren(lsize_t Position, lsize_t Count);

            /**
             * The functions for inserting and removing columns are used differently to those for inserting and removing child items,
             * because these are expected to be called on every item in the tree.
             * This is done by recursively calling this function on each child of the item.
             * ATTENTION: You MUSTN'T call this function at non-root nodes.
             * @param Position
             * @param Count The number of columns to be deleted.
             * @return Whether the operation was succeeded.
             */
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
        ~QtTreeModel(); // This will cause all items to be recursively deleted.

        // Header:

        QVariant headerData(int Section, Qt::Orientation Orientation, int Role = Qt::DisplayRole) const override;
        bool setHeaderData(int Section, Qt::Orientation Orientation, const QVariant& Value, int Role = Qt::EditRole) override;

        // Basic functionality:

        /**
         * Obtain the index of a specific subnode of a node.
         * In this model, we only return model indices for child items,
         * if the parent index is invalid (corresponding to the root item) or if it has a 0 column number.
         * @param Row
         * @param Column
         * @param Parent
         * @return
         */
        QModelIndex index(lsize_t Row, lsize_t Column, const QModelIndex& Parent = QModelIndex()) const override;
        /**
         * Since each item contains information for an entire row of data,
         * we create a model index to uniquely identify it by calling createIndex() with the row and column numbers and a pointer to the item.
         * In the data() function, we will use item pointer & column number to access the data associated with the model index.
         * In this model, the row number is not needed to identify data.
         * Items without parents, including the root item, are handled by returning a null model index.
         * Otherwise, a model index is created and returned as in the index() function, with a suitable row number,
         * but with a 0 column number to be consistent with the scheme used in the index() implementation.
         * @param Index
         * @return
         */
        QModelIndex parent(const QModelIndex& Index) const override;

        /**
         * @param Parent The tree node whose children count is wanted.
         * @return The number of children it contains.
         */
        lsize_t rowCount(const QModelIndex& Parent = QModelIndex()) const override;

        /**
         * @param Parent The tree node whose number of elements (terms of data) is wanted.
         * @return The number of elements this tree node contains.
         */
        lsize_t columnCount(const QModelIndex& Parent = QModelIndex()) const override;

        // Fetch data dynamically:

//        bool hasChildren(const QModelIndex& Parent = QModelIndex()) const override;
//
//        bool canFetchMore(const QModelIndex& Parent) const override;
//        void fetchMore(const QModelIndex& Parent) override;

        QVariant data(const QModelIndex& Index, int Role = Qt::DisplayRole) const override;

        // Editable:

        bool setData(const QModelIndex& Index, const QVariant& Value, int Role = Qt::EditRole) override;

        /**
         * Get the properties (i.e., editable or not) of an item.
         * @param Index
         * @return Properties of the item.
         */
        Qt::ItemFlags flags(const QModelIndex& Index) const override;

        // Add data:

        bool insertRows(lsize_t Position, lsize_t ChildCount, const QModelIndex& Parent = QModelIndex()) override;
        bool insertColumns(lsize_t Position, lsize_t ColumnCount, const QModelIndex& Parent = QModelIndex()) override;

        // Remove data:

        bool removeRows(lsize_t Position, lsize_t ChildCount, const QModelIndex& Parent = QModelIndex()) override;
        bool removeColumns(lsize_t Position, lsize_t ColumnCount, const QModelIndex& Parent = QModelIndex()) override;

        // custom functions
        void FromJSON(const QByteArray& UTF8JSONString); // construct this tree model from JSON
    private:
        Node* GetItem(const QModelIndex& Index) const;
        Node* RootNode = nullptr;
    };
}

#endif // WRITING_MATERIALS_MANAGER_QTTREEMODEL_H
