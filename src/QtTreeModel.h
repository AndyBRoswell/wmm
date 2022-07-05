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
            explicit Node(const QList<QVariant>& Data = {}, Node* Parent = nullptr);
            ~Node();

            Node* Child(lsize_t Number);
            lsize_t ChildCount() const;
            lsize_t ColumnCount() const;
            QVariant Data(lsize_t Column) const;
            void PushBackChild(Node* const Child);
            bool InsertChild(lsize_t Position, Node* const Child);
            bool InsertChildren(lsize_t Position, lsize_t RowCount, lsize_t ColumnCount);
            bool InsertColumns(lsize_t Position, lsize_t ColumnCount);
            Node* Parent();
            bool RemoveChildren(lsize_t Position, lsize_t Count);
            bool RemoveColumns(lsize_t Position, lsize_t Count);
            void ReverseChild();
            lsize_t ChildNumber() const;
            bool SetData(lsize_t Column, const QVariant& Value);
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
