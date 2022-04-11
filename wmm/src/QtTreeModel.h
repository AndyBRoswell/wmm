#ifndef QTTREEMODEL_H
#define QTTREEMODEL_H

#include <QAbstractItemModel>

namespace WritingMaterialsManager {
    class QtTreeModel : public QAbstractItemModel {
    Q_OBJECT

    public:
        class TreeItem {
        public:
            explicit TreeItem(const QList<QVariant>& Data, TreeItem* Parent = nullptr);
            ~TreeItem();

            TreeItem* Child(int Number);
            int ChildCount() const;
            int ColumnCount() const;
            QVariant Data(int Column) const;
            bool InsertChildren(int Position, int Count, int Columns);
            bool InsertColumns(int Position, int Columns);
            TreeItem* Parent();
            bool RemoveChildren(int Position, int Count);
            bool RemoveColumns(int Position, int Columns);
            int ChildNumber() const;
            bool SetData(int Column, const QVariant& Value);

        private:
            QList<TreeItem*> ChildItems;
            QList<QVariant> ItemData;
            TreeItem* ParentItem;
        };

        explicit QtTreeModel(QObject* Parent = nullptr);

        // Header:
        QVariant headerData(int Section, Qt::Orientation Orientation, int Role = Qt::DisplayRole) const override;
        bool setHeaderData(int Section, Qt::Orientation Orientation, const QVariant& Value, int Role = Qt::EditRole) override;

        // Basic functionality:
        QModelIndex index(int Row, int Column, const QModelIndex& Parent = QModelIndex()) const override;
        QModelIndex parent(const QModelIndex& Index) const override;

        int rowCount(const QModelIndex& Parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& Parent = QModelIndex()) const override;

        // Fetch data dynamically:
        bool hasChildren(const QModelIndex& Parent = QModelIndex()) const override;

        bool canFetchMore(const QModelIndex& Parent) const override;
        void fetchMore(const QModelIndex& Parent) override;

        QVariant data(const QModelIndex& Index, int Role = Qt::DisplayRole) const override;

        // Editable:
        bool setData(const QModelIndex& Index, const QVariant& Value, int Role = Qt::EditRole) override;

        Qt::ItemFlags flags(const QModelIndex& Index) const override;

        // Add data:
        bool insertRows(int Row, int Count, const QModelIndex& Parent = QModelIndex()) override;
        bool insertColumns(int Column, int Count, const QModelIndex& Parent = QModelIndex()) override;

        // Remove data:
        bool removeRows(int Row, int Count, const QModelIndex& Parent = QModelIndex()) override;
        bool removeColumns(int Column, int Count, const QModelIndex& Parent = QModelIndex()) override;
    private:
    };
}

#endif // QTTREEMODEL_H
